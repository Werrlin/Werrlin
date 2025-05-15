#!/usr/bin/env python3

import csv
import os
import re
from collections import defaultdict

# 类型映射表
TYPE_MAP = {
    "1Byte":  ("uint8_t", 1),
    "2Byte":  ("uint16_t", 2),
    "4Byte":  ("uint32_t", 4),
    "bit":    ("bit", 1),
}

# 读取CSV并解析为结构体描述
def parse_csv(filename):
    structs = defaultdict(list)
    with open(filename, encoding="utf-8") as f:
        reader = csv.reader(f)
        header = next(reader)
        for row in reader:
            # 跳过空行
            if not any(row):
                continue
            # 解析blockName
            block = row[0].strip() if row[0].strip() else last_block
            if row[0].strip():
                last_block = row[0].strip()
            # 解析字段
            for i in range(1, 6):
                cell = row[i].strip()
                if cell:
                    parts = cell.split("|")
                    if len(parts) == 2:
                        size_type, name = parts
                        rule = row[5].strip() if len(row) > 5 else ""
                        note = row[6].strip() if len(row) > 6 else ""
                        structs[block].append({
                            "type": size_type,
                            "name": name,
                            "rule": rule,
                            "note": note
                        })
                    elif len(parts) == 3:
                        size_type, tree, name = parts
                        rule = row[5].strip() if len(row) > 5 else ""
                        note = row[6].strip() if len(row) > 6 else ""
                        structs[block].append({
                            "type": size_type,
                            "tree": tree,
                            "name": name,
                            "rule": rule,
                            "note": note
                        })
    return structs

# 生成C++结构体
def gen_structs(structs):
    code = ""
    for struct_name, fields in structs.items():
        code += f"struct {struct_name} {{\n"
        bitfields = []
        for field in fields:
            ftype = field["type"]
            fname = field["name"]
            if "bit" in ftype:
                bits = int(re.match(r"(\d+)bit", ftype).group(1))
                bitfields.append((fname, bits))
            else:
                if bitfields:
                    # 合并bitfields到一个uint8_t
                    code += "    union {\n        struct {\n"
                    for bname, bsize in bitfields:
                        code += f"            uint8_t {bname} : {bsize};\n"
                    code += "        };\n        uint8_t _bits;\n    };\n"
                    bitfields = []
                if "tree" in field.get("tree", ""):
                    # 嵌套vector
                    code += f"    std::vector<{struct_name}_{fname}> {fname};\n"
                else:
                    ctype = TYPE_MAP.get(ftype, (ftype, 0))[0]
                    code += f"    {ctype} {fname};\n"
        if bitfields:
            code += "    union {\n        struct {\n"
            for bname, bsize in bitfields:
                code += f"            uint8_t {bname} : {bsize};\n"
            code += "        };\n        uint8_t _bits;\n    };\n"
        code += "};\n\n"
    return code

# 生成序列化代码（大端序，bit合并）
def gen_serialize(structs):
    code = ""
    for struct_name, fields in structs.items():
        code += f"void Serialize(const {struct_name}& obj, std::vector<uint8_t>& buf) {{\n"
        bitfields = []
        for field in fields:
            ftype = field["type"]
            fname = field["name"]
            if "bit" in ftype:
                bits = int(re.match(r"(\d+)bit", ftype).group(1))
                bitfields.append((fname, bits))
            else:
                if bitfields:
                    code += "    buf.push_back(obj._bits);\n"
                    bitfields = []
                if "tree" in field.get("tree", ""):
                    # 变长vector，先写数量
                    code += f"    uint16_t {fname}_size = obj.{fname}.size();\n"
                    code += f"    buf.push_back(({fname}_size >> 8) & 0xFF);\n"
                    code += f"    buf.push_back({fname}_size & 0xFF);\n"
                    code += f"    for (const auto& item : obj.{fname}) Serialize(item, buf);\n"
                else:
                    ctype, clen = TYPE_MAP.get(ftype, (ftype, 0))
                    if clen == 1:
                        code += f"    buf.push_back(obj.{fname});\n"
                    elif clen == 2:
                        code += f"    buf.push_back((obj.{fname} >> 8) & 0xFF);\n"
                        code += f"    buf.push_back(obj.{fname} & 0xFF);\n"
                    elif clen == 4:
                        code += f"    buf.push_back((obj.{fname} >> 24) & 0xFF);\n"
                        code += f"    buf.push_back((obj.{fname} >> 16) & 0xFF);\n"
                        code += f"    buf.push_back((obj.{fname} >> 8) & 0xFF);\n"
                        code += f"    buf.push_back(obj.{fname} & 0xFF);\n"
        if bitfields:
            code += "    buf.push_back(obj._bits);\n"
        code += "}\n\n"
    return code

# 生成反序列化代码（大端序，bit合并）
def gen_deserialize(structs):
    code = ""
    for struct_name, fields in structs.items():
        code += f"void Deserialize({struct_name}& obj, const uint8_t* buf, size_t& offset) {{\n"
        bitfields = []
        for field in fields:
            ftype = field["type"]
            fname = field["name"]
            if "bit" in ftype:
                bits = int(re.match(r"(\d+)bit", ftype).group(1))
                bitfields.append((fname, bits))
            else:
                if bitfields:
                    code += "    obj._bits = buf[offset++];\n"
                    bitfields = []
                if "tree" in field.get("tree", ""):
                    code += f"    uint16_t {fname}_size = (buf[offset] << 8) | buf[offset+1]; offset += 2;\n"
                    code += f"    obj.{fname}.resize({fname}_size);\n"
                    code += f"    for (auto& item : obj.{fname}) Deserialize(item, buf, offset);\n"
                else:
                    ctype, clen = TYPE_MAP.get(ftype, (ftype, 0))
                    if clen == 1:
                        code += f"    obj.{fname} = buf[offset++];\n"
                    elif clen == 2:
                        code += f"    obj.{fname} = (buf[offset] << 8) | buf[offset+1]; offset += 2;\n"
                    elif clen == 4:
                        code += f"    obj.{fname} = (buf[offset] << 24) | (buf[offset+1] << 16) | (buf[offset+2] << 8) | buf[offset+3]; offset += 4;\n"
        if bitfields:
            code += "    obj._bits = buf[offset++];\n"
        code += "}\n\n"
    return code

# ...existing code...

def main():
    csv_path = os.path.join(os.path.dirname(__file__), "lightproto_config.csv")
    structs = parse_csv(csv_path)
    struct_code = "// ====== 自动生成的C++结构体 ======\n" + gen_structs(structs)
    serialize_code = "// ====== 自动生成的序列化代码 ======\n" + gen_serialize(structs)
    deserialize_code = "// ====== 自动生成的反序列化代码 ======\n" + gen_deserialize(structs)

    all_code = (
        "#pragma once\n"
        "#include <vector>\n"
        "#include <cstdint>\n\n"
        + struct_code + "\n"
        + serialize_code + "\n"
        + deserialize_code + "\n"
    )

    with open("protocol_generated.h", "w", encoding="utf-8") as f:
        f.write(all_code)

    print("代码已生成到 protocol_generated.h")

if __name__ == "__main__":
    main()