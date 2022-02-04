#pragma once

namespace neith {

    class CompManager {
        static int mCompCounter;
public:
        template <typename T>
        static int GetCompID()
            {
          static int componentID = mCompCounter++;
          return componentID;
        }
    };
}
