#pragma once

#include "debugInfo.h"

namespace neith {
class Gui {
public:
    static void Init();
    static void Update();
    static void Clean();

    static void ShouldRenderGui(bool renderGui) { mRenderGui = renderGui; }

private:
    static unsigned int CreateTree(unsigned int entityID, unsigned int selectEntityID);
    static bool mRenderGui;
    static void DisplayEntityDetails(unsigned int entityID);
    static void DisplayDebugInfo(DebugInfo *debugInfo);
};
}  // namespace neith
