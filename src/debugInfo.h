#pragma once

#include <string>
#include <vector>

#include "debugInfo.h"

class DebugInfo {
public:
    DebugInfo(std::string message) { this->message = message; }
    ~DebugInfo();

    std::vector<DebugInfo *> child;
    std::string message;
};
