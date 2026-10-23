#pragma once
#include "CANFrame.h"
#include <vector>

class CANAnalyzer {
private:
    std::vector<CANFrame> log_;

public:
    void record(const CANFrame& frame);
    const std::vector<CANFrame>& log() const;

    void printAll() const;
    void printById(unsigned int id) const;
    void printStatistics() const;
};
