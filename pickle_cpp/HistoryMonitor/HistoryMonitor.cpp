#include "HistoryMonitor.h"
#include <iostream>
#include <sstream>

HistoryMonitor::HistoryMonitor(History* history_to_monitor) 
    : _monitored_history(history_to_monitor), 
      _corruption_detected(false),
      _expected_logical_size(0),
      _last_known_vector_size(0),
      _last_known_vector_capacity(0) {
    
    std::lock_guard<std::mutex> lock(_monitor_mutex);
    
    std::cout << "*** MONITOR: HistoryMonitor created for History at: " << history_to_monitor << std::endl;
    
    _history_address = static_cast<void*>(history_to_monitor);
    _vector_address = nullptr; // We'll try to get this later
    _last_validation = std::chrono::steady_clock::now();
    
    // Initial validation to establish baseline
    if (history_to_monitor) {
        try {
            int initial_size = history_to_monitor->size();
            _expected_logical_size = initial_size;
            std::cout << "*** MONITOR: Initial logical size: " << initial_size << std::endl;
        } catch (...) {
            std::cout << "*** MONITOR: WARNING - Cannot get initial size, History may already be corrupted!" << std::endl;
        }
    }
}

HistoryMonitor::~HistoryMonitor() {
    std::cout << "*** MONITOR: HistoryMonitor destructor - Final corruption status: " 
              << (_corruption_detected ? "CORRUPTED" : "CLEAN") << std::endl;
}

bool HistoryMonitor::validateHistory(const char* context) {
    std::lock_guard<std::mutex> lock(_monitor_mutex);
    
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - _last_validation).count();
    
    std::cout << "*** MONITOR: Validation at [" << context << "] after " << elapsed << "ms" << std::endl;
    
    if (!_monitored_history) {
        _recordCorruption("History pointer is null!");
        return false;
    }
    
    // Check if the History object's memory address is still valid
    if (static_cast<void*>(_monitored_history) != _history_address) {
        std::stringstream ss;
        ss << "History address changed from " << _history_address 
           << " to " << static_cast<void*>(_monitored_history);
        _recordCorruption(ss.str());
        return false;
    }
    
    // Memory layout validation
    if (!_validateMemoryLayout()) {
        _recordCorruption("Memory layout validation failed");
        return false;
    }
    
    // Vector integrity validation
    if (!_validateVectorIntegrity()) {
        _recordCorruption("Vector integrity validation failed");
        return false;
    }
    
    // Logical consistency validation
    if (!_validateLogicalConsistency()) {
        _recordCorruption("Logical consistency validation failed");
        return false;
    }
    
    _last_validation = now;
    std::cout << "*** MONITOR: Validation PASSED at [" << context << "]" << std::endl;
    return true;
}

bool HistoryMonitor::_validateMemoryLayout() {
    // Basic memory address sanity checks
    
    // Check if History pointer is in reasonable memory range (not obviously corrupted)
    uintptr_t addr = reinterpret_cast<uintptr_t>(_monitored_history);
    
    // Check for obviously bad addresses (null, very low, very high)
    if (addr < 0x1000 || addr > 0x7FFFFFFFFFFF) {
        std::cout << "*** MONITOR: Bad memory address: " << std::hex << addr << std::dec << std::endl;
        return false;
    }
    
    // Check alignment (should be word-aligned for C++ objects)
    if (addr % sizeof(void*) != 0) {
        std::cout << "*** MONITOR: Bad memory alignment: " << std::hex << addr << std::dec << std::endl;
        return false;
    }
    
    return true;
}

bool HistoryMonitor::_validateVectorIntegrity() {
    try {
        // Try to safely access the History object's size method
        int logical_size = _monitored_history->size();
        
        std::cout << "*** MONITOR: Current logical size: " << logical_size << std::endl;
        
        // Check for reasonable size values
        if (logical_size < 0 || logical_size > 10000) {  // Reasonable upper bound
            std::stringstream ss;
            ss << "Logical size out of bounds: " << logical_size;
            std::cout << "*** MONITOR: " << ss.str() << std::endl;
            return false;
        }
        
        // Update our tracking
        _expected_logical_size = logical_size;
        
        return true;
        
    } catch (const std::exception& e) {
        std::cout << "*** MONITOR: Exception during vector validation: " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cout << "*** MONITOR: Unknown exception during vector validation" << std::endl;
        return false;
    }
}

bool HistoryMonitor::_validateLogicalConsistency() {
    // For now, just return true - we'll expand this as needed
    return true;
}

void HistoryMonitor::_recordCorruption(const std::string& details) {
    _corruption_detected = true;
    _corruption_info = details;
    
    std::cout << "*** MONITOR: CORRUPTION DETECTED: " << details << std::endl;
    std::cout << "*** MONITOR: History address: " << _history_address << std::endl;
    std::cout << "*** MONITOR: Expected logical size: " << _expected_logical_size << std::endl;
}

void HistoryMonitor::reset() {
    std::lock_guard<std::mutex> lock(_monitor_mutex);
    _corruption_detected = false;
    _corruption_info.clear();
    _last_validation = std::chrono::steady_clock::now();
}