#pragma once

namespace neith {

class CompManager {
public:
    template <typename T>
    static int GetCompID()
    {
        static int componentID = mCompCounter++;
        return componentID;
    }

private:
    static int mCompCounter;
};
}  // namespace neith
