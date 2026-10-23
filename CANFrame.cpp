#include "CANFrame.h"
#include <iostream>
#include <iomanip>
#include <sstream>

CANFrame::CANFrame(uint16_t id,
                   const std::vector<uint8_t>& data,
                   const std::string& sender)
    : id_(id), data_(data), sender_(sender) {}

uint16_t CANFrame::id() const { return id_; }
uint8_t CANFrame::dlc() const { return static_cast<uint8_t>(data_.size()); }
const std::vector<uint8_t>& CANFrame::data() const { return data_; }
const std::string& CANFrame::sender() const { return sender_; }

std::string CANFrame::dataString() const {
    std::ostringstream out;
    for (size_t i = 0; i < data_.size(); ++i) {
        if (i) out << ' ';
        out << std::uppercase << std::hex
            << std::setw(2) << std::setfill('0')
            << static_cast<int>(data_[i]);
    }
    return out.str();
}

std::string CANFrame::binaryId() const {
    std::string bits;
    for (int i = 10; i >= 0; --i)
        bits += ((id_ >> i) & 1) ? '1' : '0';
    return bits;
}

void CANFrame::print() const {
    std::cout << "ID: 0x"
              << std::uppercase << std::hex
              << std::setw(3) << std::setfill('0') << id_
              << std::dec << std::setfill(' ')
              << " | DLC: " << static_cast<int>(dlc())
              << " | DATA: " << dataString()
              << " | Sender: " << sender_ << '\n';
}
