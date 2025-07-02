#include "filesystem/filestream.h"

using namespace eloo::filesystem;


ifstream::ifstream(const path_t& filePath) {
    i = eastl::make_unique<streaming::istream>();
    mFileHandle = fopen(filePath.string().c_str(), "rb");
}