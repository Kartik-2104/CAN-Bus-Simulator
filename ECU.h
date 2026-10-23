#pragma once
#include "CANFrame.h"
#include <string>
#include <random>

class ECU {
protected:
    std::string name_;
    uint16_t canId_;
    std::mt19937 rng_;

    ECU(const std::string& name, uint16_t id);

public:
    virtual ~ECU() = default;
    virtual CANFrame generateFrame() = 0;
    virtual std::string status() const = 0;

    const std::string& name() const;
    uint16_t canId() const;
};

class EngineECU : public ECU {
private:
    int rpm_;
public:
    EngineECU();
    CANFrame generateFrame() override;
    std::string status() const override;
};

class ABSECU : public ECU {
private:
    int speed_;
public:
    ABSECU();
    CANFrame generateFrame() override;
    std::string status() const override;
};

class DashboardECU : public ECU {
private:
    int temperature_;
public:
    DashboardECU();
    CANFrame generateFrame() override;
    std::string status() const override;
};
