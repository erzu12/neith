#include "save.h"

#include <fstream>
#include <string>

void Save::SaveScene(const char* path)
{
    std::ofstream outfile(path, std::ofstream::binary);

    outfile.close();
}
