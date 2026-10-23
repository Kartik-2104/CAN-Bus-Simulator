#include "CANBus.h"
#include <iostream>
#include <iomanip>
#include <bitset>
#include <thread>
#include <chrono>

void CANBus::addNode(ECU* node) {
    nodes_.push_back(node);
}

size_t CANBus::nodeCount() const {
    return nodes_.size();
}

CANAnalyzer& CANBus::analyzer() {
    return analyzer_;
}

ECU* CANBus::arbitrate(const std::vector<ECU*>& contenders) const {
    if (contenders.empty()) return nullptr;
    ECU* winner = contenders[0];

    for (size_t i = 1; i < contenders.size(); ++i) {
        if (contenders[i]->canId() < winner->canId())
            winner = contenders[i];
    }
    return winner;
}

void CANBus::runCycle(bool showDetails) {
    std::vector<CANFrame> frames;

    for (ECU* node : nodes_)
        frames.push_back(node->generateFrame());

    std::vector<ECU*> contenders;
    for (ECU* node : nodes_)
        contenders.push_back(node);

    ECU* winner = arbitrate(contenders);
    if (!winner) return;

    CANFrame* winningFrame = nullptr;
    for (auto& frame : frames) {
        if (frame.sender() == winner->name()) {
            winningFrame = &frame;
            break;
        }
    }

    if (showDetails) {
        std::cout << "\n\033[1;33mARBITRATION\033[0m\n";
        for (const auto& frame : frames) {
            std::cout << "  " << frame.sender()
                      << " -> ID 0x"
                      << std::uppercase << std::hex
                      << std::setw(3) << std::setfill('0')
                      << frame.id() << std::dec << std::setfill(' ')
                      << " (" << frame.binaryId() << ")\n";
        }

        std::cout << "  Winner: \033[1;32m" << winner->name()
                  << " (lowest ID = highest priority)\033[0m\n";

        if (winningFrame) {
            std::cout << "\n\033[1;32mTRANSMITTING FRAME\033[0m\n";
            winningFrame->print();
        }
    }

    if (winningFrame)
        analyzer_.record(*winningFrame);
}

void CANBus::demonstrateArbitration() const {
    std::cout << "\n\033[1;33mBIT-BY-BIT CAN ARBITRATION DEMO\033[0m\n\n";

    uint16_t idA = 0x100;
    uint16_t idB = 0x200;

    std::bitset<11> a(idA);
    std::bitset<11> b(idB);

    std::cout << "Node A ID: 0x100 -> " << a << '\n';
    std::cout << "Node B ID: 0x200 -> " << b << "\n\n";

    std::cout << "Bit    A    B    BUS\n";
    std::cout << "-------------------\n";

    for (int i = 10; i >= 0; --i) {
        int bitA = a[i];
        int bitB = b[i];
        int bus = bitA & bitB; // dominant 0 wins

        std::cout << std::setw(3) << (11 - i)
                  << "    " << bitA
                  << "    " << bitB
                  << "     " << bus << '\n';

        if (bitA != bitB) {
            if (bitA == 1)
                std::cout << "\nNode A transmitted 1 but observed 0.\n"
                          << "Node A LOST arbitration.\n"
                          << "Winner: Node B (0x200)\n";
            else
                std::cout << "\nNode B transmitted 1 but observed 0.\n"
                          << "Node B LOST arbitration.\n"
                          << "Winner: Node A (0x100)\n";
            return;
        }
    }
}

void CANBus::printECUStatus() const {
    std::cout << "\n\033[1;36mECU STATUS\033[0m\n";
    std::cout << "------------------------------------------------\n";
    for (const ECU* node : nodes_)
        std::cout << "  " << node->status() << '\n';
    std::cout << "------------------------------------------------\n";
}
