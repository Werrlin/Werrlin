# LightProto 工具需求说明文档

## 一、工具目标

LightProto 工具用于**自动解析协议字段配置 CSV 文件**，并根据配置内容**自动生成 C++ 结构体定义代码**，以便协议数据的解析、封装和维护。

---

## 二、输入文件格式

### 1. 文件类型
- CSV（逗号分隔值），UTF-8 编码。

### 2. 文件结构
- **首行为表头**，包含：`blockName`、多列 `size|type|name`、`rule`、`note`。
- **每行为一个字段定义**，字段可属于不同 block（结构体）。
- **blockName**：结构体名称，若为空则继承上一行的 blockName。
- **size|type|name**：字段类型、特性（如 tree）、字段名，使用 `|` 分隔。
- **rule**：字段规则（如取值范围、映射关系等）。
- **note**：字段备注说明。
- 支持多级嵌套（如 tree 特性）。
- 支持注释行（以 `//` 开头，自动跳过）。

### 3. 示例
```csv
blockName,size|type|name,size|type|name,size|type|name,size|type|name,rule,note
headData,4Byte|headClass,,,,,协议头标识，区分不同类型的协议
,2Byte|version,,,,,协议版本
,4Byte|RSN,,,,,回复收到的包序号
,4Byte|TSN,,,,,发出的当前包序号
MessageHead,2Byte|tree|Number_Messages,,,,,message个数
,,2Byte|MessageType,,,"MessageType[0xff01]=>Train
MessageType[0xff02]=>Platform",根据messageType的值，关联对应blockName的数据结构
Train,,2Byte|tree|Number_Trains,,,"[0,10]",嵌套列车数量
,,,1Byte|Train_direction,,[0~2],"方向数据可能为：0,1,2"
,,,1Byte|Train_length,,>35,列车长度大于35
,,,2Byte|tree|Number_Train_FrontTrains,,"[0,10]",前车数量
,,,,4Byte|Front_TrainId,,前车编号
,,,,1bit|Front_TrainEBStatus,[0~4],紧急制动状态，取值范围0到4
,,,,6bit|Resversed,,预留字段
,,,,1bit|Front_TrainRunStatus,"[0,1,2,5,7,9]","取值范围0,1,2,5,7,9"
Platform,,2Byte|tree|Number_Platforms,,,,站台数量
,,,4Byte|PlatformId,,,站台Id
,,,1bit|Platform_Open_Status,,,站台开关状态
,,,7bit|Resversed,,,预留字段
Footer,4Byte|CRC,,,,,数据完整性校验
```

---

## 三、功能需求

### 1. CSV 解析
- 正确解析多级嵌套 block 结构（如 tree 特性）。
- 支持多列 `size|type|name`，自动识别有效字段。
- 支持空 blockName 自动继承上一个 block。
- 跳过注释行（以 `//` 开头）。

### 2. 字段类型映射
- 常见类型自动映射为 C++ 类型：
  - `1Byte` → `uint8_t`
  - `2Byte` → `uint16_t`
  - `4Byte` → `uint32_t`
  - `1bit` → `std::bitset<1>`
  - `tree` → 嵌套结构体（`std::vector<...>`）
- 支持自定义类型和大字段（如 `std::array<uint8_t, N>`）。

### 3. 结构体代码生成
- 每个 blockName 生成一个 C++ struct。
- 字段注释格式为多行，分别显示 size、rule、note，注释放在字段定义上方。
- 支持嵌套结构体（tree）。
- 自动生成必要的头文件引用（如 `<cstdint>`, `<vector>`, `<bitset>`, `<array>`）。

### 4. 动态消息块类型分派与循环解析
- 支持协议中**消息块数量可变**（如 `Number_Messages` 字段）。
- 支持**循环遍历每个消息块**，每个消息块包含一个 `MessageType` 字段。
- 工具需自动识别 `MessageType` 的取值与 blockName 的映射关系（如 `0xff01`→`Train`，`0xff02`→`Platform`）。
- 生成的结构体和注释应体现**根据 `MessageType` 动态分派对应 blockName 并依次解析**的机制。
- 解析流程示意：
  1. 读取 `Number_Messages`，如为2。
  2. 循环2次，每次读取 `MessageType`，根据其值选择并解析对应的 block（如 `Train` 或 `Platform`）。
  3. 支持多类型消息块混合解析。

### 5. 输出
- 生成 `.h` 头文件（如 `LightProtoStructs.h`），内容为所有结构体定义。

---

## 四、协议结构示例

### 协议结构体顺序
```
[HeadData(14B)] 
[MessageHead(4B)] 
[(Train数据块)|(Platform数据块)] 
[CRC校验(4B)]
```

### 动态消息块解析示意
- 读取 `Number_Messages`，假设为2。
- 第1次循环：`MessageType=0xff01`，解析 `Train`。
- 第2次循环：`MessageType=0xff02`，解析 `Platform`。
- 依次完成所有消息块的解析。

### 示例生成 C++ 结构体
```cpp
#pragma once
#include <cstdint>
#include <vector>
#include <bitset>
#include <array>

struct headData {
    // size: 4Byte
    // note: 协议头标识，区分不同类型的协议
    uint32_t headClass;
    // size: 2Byte
    // note: 协议版本
    uint16_t version;
    // size: 4Byte
    // note: 回复收到的包序号
    uint32_t RSN;
    // size: 4Byte
    // note: 发出的当前包序号
    uint32_t TSN;
};

struct MessageHead {
    // size: 2Byte
    // trait: tree
    // note: message个数
    std::vector<MessageBlock> Number_Messages;
};

struct MessageBlock {
    // size: 2Byte
    // rule: MessageType[0xff01]=>Train MessageType[0xff02]=>Platform
    // note: 根据messageType的值，关联对应blockName的数据结构
    uint16_t MessageType;
    // ...此处根据MessageType动态选择并解析Train或Platform结构体...
};

struct Train {
    // size: 2Byte
    // trait: tree
    // rule: [0,10]
    // note: 嵌套列车数量
    std::vector<Number_Trains> Number_Trains;
    // size: 1Byte
    // rule: [0~2]
    // note: 方向数据可能为：0,1,2
    uint8_t Train_direction;
    // ... 省略其余字段 ...
};

struct Platform {
    // size: 2Byte
    // trait: tree
    // note: 站台数量
    std::vector<Number_Platforms> Number_Platforms;
    // ... 省略其余字段 ...
};

struct Footer {
    // size: 4Byte
    // note: 数据完整性校验
    uint32_t CRC;
};
```

---

## 五、扩展需求（可选）

- 支持生成反序列化/序列化代码。
- 支持类型校验和默认值生成。
- 支持多语言结构体代码生成（如 C#/Java）。
- 提供命令行参数，指定输入/输出文件路径。

---

## 六、使用流程

1. 用户维护/编辑协议字段 CSV 文件。
2. 运行 LightProto 工具，自动生成最新的 C++ 结构体头文件。
3. 在项目中直接包含生成的头文件，进行协议数据处理。
4. 解析时，需根据 `Number_Messages` 循环读取每个消息块，并根据 `MessageType` 动态分派对应的数据结构进行解析。

---