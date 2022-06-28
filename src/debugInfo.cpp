#include "debugInfo.h"

DebugInfo::~DebugInfo()
{
    for (DebugInfo *debugInfo : child) {
        delete debugInfo;
    }
}
