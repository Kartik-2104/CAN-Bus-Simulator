#pragma once
#include "ECU.h"
#include "CANAnalyzer.h"
#include <vector>

class CANBus {
private:
    std::vector<ECU*> nodes_;
    CANAnalyzer analyzer_;

    ECU* arbitrate(const std::vector<ECU*>& contenders) const;

public:
    void addNode(ECU* node);
    size_t nodeCount() const;
    CANAnalyzer& analyzer();

    void runCycle(bool showDetails);
    void demonstrateArbitration() const;
    void printECUStatus() const;
};
