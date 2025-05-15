#pragma once
#include <vector>
#include <cstdint>

// ====== 自动生成的C++结构体 ======
struct headData {
    uint32_t headClass;
    uint16_t version;
    uint32_t RSN;
    uint32_t TSN;
};

struct MessageHead {
    std::vector<MessageHead_Number_Messages> Number_Messages;
    uint16_t MessageType;
};

struct Train {
    std::vector<Train_Number_Trains> Number_Trains;
    uint8_t Train_direction;
    uint8_t Train_length;
    std::vector<Train_Number_Train_FrontTrains> Number_Train_FrontTrains;
    uint32_t Front_TrainId;
    union {
        struct {
            uint8_t Front_TrainEBStatus : 1;
            uint8_t Resversed : 6;
            uint8_t Front_TrainRunStatus : 1;
        };
        uint8_t _bits;
    };
};

struct Platform {
    std::vector<Platform_Number_Platforms> Number_Platforms;
    uint32_t PlatformId;
    union {
        struct {
            uint8_t Platform_Open_Status : 1;
            uint8_t Resversed : 7;
        };
        uint8_t _bits;
    };
};

struct Footer {
    uint32_t CRC;
};


// ====== 自动生成的序列化代码 ======
void Serialize(const headData& obj, std::vector<uint8_t>& buf) {
    buf.push_back((obj.headClass >> 24) & 0xFF);
    buf.push_back((obj.headClass >> 16) & 0xFF);
    buf.push_back((obj.headClass >> 8) & 0xFF);
    buf.push_back(obj.headClass & 0xFF);
    buf.push_back((obj.version >> 8) & 0xFF);
    buf.push_back(obj.version & 0xFF);
    buf.push_back((obj.RSN >> 24) & 0xFF);
    buf.push_back((obj.RSN >> 16) & 0xFF);
    buf.push_back((obj.RSN >> 8) & 0xFF);
    buf.push_back(obj.RSN & 0xFF);
    buf.push_back((obj.TSN >> 24) & 0xFF);
    buf.push_back((obj.TSN >> 16) & 0xFF);
    buf.push_back((obj.TSN >> 8) & 0xFF);
    buf.push_back(obj.TSN & 0xFF);
}

void Serialize(const MessageHead& obj, std::vector<uint8_t>& buf) {
    uint16_t Number_Messages_size = obj.Number_Messages.size();
    buf.push_back((Number_Messages_size >> 8) & 0xFF);
    buf.push_back(Number_Messages_size & 0xFF);
    for (const auto& item : obj.Number_Messages) Serialize(item, buf);
    buf.push_back((obj.MessageType >> 8) & 0xFF);
    buf.push_back(obj.MessageType & 0xFF);
}

void Serialize(const Train& obj, std::vector<uint8_t>& buf) {
    uint16_t Number_Trains_size = obj.Number_Trains.size();
    buf.push_back((Number_Trains_size >> 8) & 0xFF);
    buf.push_back(Number_Trains_size & 0xFF);
    for (const auto& item : obj.Number_Trains) Serialize(item, buf);
    buf.push_back(obj.Train_direction);
    buf.push_back(obj.Train_length);
    uint16_t Number_Train_FrontTrains_size = obj.Number_Train_FrontTrains.size();
    buf.push_back((Number_Train_FrontTrains_size >> 8) & 0xFF);
    buf.push_back(Number_Train_FrontTrains_size & 0xFF);
    for (const auto& item : obj.Number_Train_FrontTrains) Serialize(item, buf);
    buf.push_back((obj.Front_TrainId >> 24) & 0xFF);
    buf.push_back((obj.Front_TrainId >> 16) & 0xFF);
    buf.push_back((obj.Front_TrainId >> 8) & 0xFF);
    buf.push_back(obj.Front_TrainId & 0xFF);
    buf.push_back(obj._bits);
}

void Serialize(const Platform& obj, std::vector<uint8_t>& buf) {
    uint16_t Number_Platforms_size = obj.Number_Platforms.size();
    buf.push_back((Number_Platforms_size >> 8) & 0xFF);
    buf.push_back(Number_Platforms_size & 0xFF);
    for (const auto& item : obj.Number_Platforms) Serialize(item, buf);
    buf.push_back((obj.PlatformId >> 24) & 0xFF);
    buf.push_back((obj.PlatformId >> 16) & 0xFF);
    buf.push_back((obj.PlatformId >> 8) & 0xFF);
    buf.push_back(obj.PlatformId & 0xFF);
    buf.push_back(obj._bits);
}

void Serialize(const Footer& obj, std::vector<uint8_t>& buf) {
    buf.push_back((obj.CRC >> 24) & 0xFF);
    buf.push_back((obj.CRC >> 16) & 0xFF);
    buf.push_back((obj.CRC >> 8) & 0xFF);
    buf.push_back(obj.CRC & 0xFF);
}


// ====== 自动生成的反序列化代码 ======
void Deserialize(headData& obj, const uint8_t* buf, size_t& offset) {
    obj.headClass = (buf[offset] << 24) | (buf[offset+1] << 16) | (buf[offset+2] << 8) | buf[offset+3]; offset += 4;
    obj.version = (buf[offset] << 8) | buf[offset+1]; offset += 2;
    obj.RSN = (buf[offset] << 24) | (buf[offset+1] << 16) | (buf[offset+2] << 8) | buf[offset+3]; offset += 4;
    obj.TSN = (buf[offset] << 24) | (buf[offset+1] << 16) | (buf[offset+2] << 8) | buf[offset+3]; offset += 4;
}

void Deserialize(MessageHead& obj, const uint8_t* buf, size_t& offset) {
    uint16_t Number_Messages_size = (buf[offset] << 8) | buf[offset+1]; offset += 2;
    obj.Number_Messages.resize(Number_Messages_size);
    for (auto& item : obj.Number_Messages) Deserialize(item, buf, offset);
    obj.MessageType = (buf[offset] << 8) | buf[offset+1]; offset += 2;
}

void Deserialize(Train& obj, const uint8_t* buf, size_t& offset) {
    uint16_t Number_Trains_size = (buf[offset] << 8) | buf[offset+1]; offset += 2;
    obj.Number_Trains.resize(Number_Trains_size);
    for (auto& item : obj.Number_Trains) Deserialize(item, buf, offset);
    obj.Train_direction = buf[offset++];
    obj.Train_length = buf[offset++];
    uint16_t Number_Train_FrontTrains_size = (buf[offset] << 8) | buf[offset+1]; offset += 2;
    obj.Number_Train_FrontTrains.resize(Number_Train_FrontTrains_size);
    for (auto& item : obj.Number_Train_FrontTrains) Deserialize(item, buf, offset);
    obj.Front_TrainId = (buf[offset] << 24) | (buf[offset+1] << 16) | (buf[offset+2] << 8) | buf[offset+3]; offset += 4;
    obj._bits = buf[offset++];
}

void Deserialize(Platform& obj, const uint8_t* buf, size_t& offset) {
    uint16_t Number_Platforms_size = (buf[offset] << 8) | buf[offset+1]; offset += 2;
    obj.Number_Platforms.resize(Number_Platforms_size);
    for (auto& item : obj.Number_Platforms) Deserialize(item, buf, offset);
    obj.PlatformId = (buf[offset] << 24) | (buf[offset+1] << 16) | (buf[offset+2] << 8) | buf[offset+3]; offset += 4;
    obj._bits = buf[offset++];
}

void Deserialize(Footer& obj, const uint8_t* buf, size_t& offset) {
    obj.CRC = (buf[offset] << 24) | (buf[offset+1] << 16) | (buf[offset+2] << 8) | buf[offset+3]; offset += 4;
}


