#include "CANAnalyzer.h"
#include <iostream>
#include <iomanip>
#include <map>

void CANAnalyzer::record(const CANFrame& frame) {
    log_.push_back(frame);
}

const std::vector<CANFrame>& CANAnalyzer::log() const {
    return log_;
}

void CANAnalyzer::printAll() const {
    if (log_.empty()) {
        std::cout << "\nNo frames logged yet.\n";
        return;
    }

    std::cout << "\n\033[1;36mCAN TRAFFIC LOG\033[0m\n";
    std::cout << "---------------------------------------------------------------\n";
    std::cout << " #   ID       DLC   DATA              SENDER\n";
    std::cout << "---------------------------------------------------------------\n";

    int index = 1;
    for (const auto& frame : log_) {
        std::cout << std::setw(3) << index++ << "  "
                  << "0x" << std::uppercase << std::hex
                  << std::setw(3) << std::setfill('0') << frame.id()
                  << std::dec << std::setfill(' ') << "    "
                  << std::setw(2) << static_cast<int>(frame.dlc()) << "    "
                  << std::setw(17) << std::left << frame.dataString()
                  << std::right << "  "
                  << frame.sender() << '\n';
    }
    std::cout << "---------------------------------------------------------------\n";
}

void CANAnalyzer::printById(unsigned int id) const {
    int count = 0;
    std::cout << "\n\033[1;36mFILTER: ID = 0x"
              << std::uppercase << std::hex << id << std::dec
              << "\033[0m\n";

    for (const auto& frame : log_) {
        if (frame.id() == id) {
            frame.print();
            ++count;
        }
    }

    if (count == 0)
        std::cout << "No matching frames.\n";
    else
        std::cout << "Matching frames: " << count << '\n';
}

void CANAnalyzer::printStatistics() const {
    std::map<uint16_t, int> counts;

    for (const auto& frame : log_)
        counts[frame.id()]++;

    std::cout << "\n\033[1;36mBUS STATISTICS\033[0m\n";
    std::cout << "Total frames: " << log_.size() << '\n';

    for (const auto& [id, count] : counts) {
        std::cout << "  0x"
                  << std::uppercase << std::hex
                  << std::setw(3) << std::setfill('0') << id
                  << std::dec << std::setfill(' ')
                  << " : " << count << " frame(s)\n";
    }
}
