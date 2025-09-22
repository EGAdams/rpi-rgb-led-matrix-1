#ifndef HISTORY_MONITOR_H
#define HISTORY_MONITOR_H

#include <memory>
#include <mutex>
#include <chrono>
#include "../History/History.h"

// External monitoring class to detect History corruption
class HistoryMonitor {
public:
    HistoryMonitor(History* history_to_monitor);
    ~HistoryMonitor();
    
    // Validate History object integrity
    bool validateHistory(const char* context);
    
    // Check if corruption has occurred
    bool isCorrupted() const { return _corruption_detected; }
    
    // Get corruption details
    std::string getCorruptionInfo() const { return _corruption_info; }
    
    // Reset monitoring state
    void reset();

private:
    History* _monitored_history;
    
    // Corruption detection
    bool _corruption_detected;
    std::string _corruption_info;
    std::chrono::steady_clock::time_point _last_validation;
    
    // Expected state tracking
    size_t _expected_logical_size;
    size_t _last_known_vector_size;
    size_t _last_known_vector_capacity;
    
    // Memory address tracking
    void* _history_address;
    void* _vector_address;
    
    // Thread safety
    mutable std::mutex _monitor_mutex;
    
    // Internal validation methods
    bool _validateMemoryLayout();
    bool _validateVectorIntegrity();
    bool _validateLogicalConsistency();
    void _recordCorruption(const std::string& details);
};

#endif // HISTORY_MONITOR_H