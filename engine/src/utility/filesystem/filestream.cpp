#include "filesystem/filestream.h"
#include <fstream>
#include <stdexcept>
using namespace sge;
using namespace sge::filesystem;

ifstream::ifstream(const path_t& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) { throw std::runtime_error("Cannot open input file"); }
    const auto size = file.tellg();
    if (size < 0) { throw std::runtime_error("Cannot size input file"); }
    mBuffer.resize(static_cast<size_t>(size));
    file.seekg(0);
    if (!mBuffer.empty() && !file.read(reinterpret_cast<char*>(mBuffer.data()), size)) {
        throw std::runtime_error("Cannot read input file");
    }
    i = eastl::make_unique<streaming::istream>(&mBuffer);
}
ifstream::~ifstream() = default;

ofstream::ofstream(const path_t& path) {
    mFileHandle = _wfopen(path.c_str(), L"wb");
    if (!mFileHandle) { throw std::runtime_error("Cannot open output file"); }
}
ofstream::~ofstream() { if (mFileHandle) { fclose(mFileHandle); } }
void_r ofstream::write(const char* data, size_t size) {
    if ((!data && size) || !mFileHandle) { return std::unexpected(std::make_error_code(std::errc::invalid_argument)); }
    if (size && fwrite(data, 1, size, mFileHandle) != size) { return std::unexpected(std::make_error_code(std::errc::io_error)); }
    return {};
}
void_r ofstream::flush() {
    if (!mFileHandle || fflush(mFileHandle)) { return std::unexpected(std::make_error_code(std::errc::io_error)); }
    return {};
}
iofstream::iofstream(const path_t&) { throw std::logic_error("Bidirectional file streams are not implemented"); }
iofstream::~iofstream() = default;
void_r iofstream::flush() { return std::unexpected(std::make_error_code(std::errc::operation_not_supported)); }
