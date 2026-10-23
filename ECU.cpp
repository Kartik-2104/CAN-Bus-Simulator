#include "ECU.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

static std::vector<uint8_t> encode16(uint16_t value) {
    return {
        static_cast<uint8_t>((value >> 8) & 0xFF),
        static_cast<uint8_t>(value & 0xFF)
    };
}

ECU::ECU(const std::string& name, uint16_t id)
    : name_(name), canId_(id), rng_(std::random_device{}()) {}

const std::string& ECU::name() const { return name_; }
uint16_t ECU::canId() const { return canId_; }

EngineECU::EngineECU() : ECU("Engine ECU", 0x100), rpm_(2500) {}

CANFrame EngineECU::generateFrame() {
    std::uniform_int_distribution<int> change(-180, 180);
    rpm_ = std::clamp(rpm_ + change(rng_), 800, 6500);
    return CANFrame(canId_, encode16(static_cast<uint16_t>(rpm_)), name_);
}

std::string EngineECU::status() const {
    return "Engine RPM = " + std::to_string(rpm_) + " rpm";
}

ABSECU::ABSECU() : ECU("ABS ECU", 0x200), speed_(60) {}

CANFrame ABSECU::generateFrame() {
    std::uniform_int_distribution<int> change(-4, 4);
    speed_ = std::clamp(speed_ + change(rng_), 0, 180);
    return CANFrame(canId_, encode16(static_cast<uint16_t>(speed_)), name_);
}

std::string ABSECU::status() const {
    return "Vehicle Speed = " + std::to_string(speed_) + " km/h";
}

DashboardECU::DashboardECU() : ECU("Dashboard ECU", 0x080), temperature_(85) {}

CANFrame DashboardECU::generateFrame() {
    std::uniform_int_distribution<int> change(-2, 2);
    temperature_ = std::clamp(temperature_ + change(rng_), 70, 110);
    return CANFrame(canId_, encode16(static_cast<uint16_t>(temperature_)), name_);
}

std::string DashboardECU::status() const {
    return "Coolant Temperature = " + std::to_string(temperature_) + " C";
}
