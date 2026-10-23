#pragma once
#include <vector>
#include <string>
#include <cstdint>

class CANFrame {
private:
    uint16_t id_;
    std::vector<uint8_t> data_;
    std::string sender_;

public:
    CANFrame(uint16_t id,
             const std::vector<uint8_t>& data,
             const std::string& sender);

    uint16_t id() const;
    uint8_t dlc() const;
    const std::vector<uint8_t>& data() const;
    const std::string& sender() const;

    std::string dataString() const;
    std::string binaryId() const;
    void print() const;
};
