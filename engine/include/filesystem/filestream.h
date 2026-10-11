#pragma once

#include "streaming/streaming.h"
#include "filesystem/filesystem.h"

#include <EASTL/unique_ptr.h>

#include <cstdio>


namespace sge::filesystem {

    // ==============================================================================
    // Input (read) file stream

    struct ifstream {
    protected:
        streaming::data_vector mBuffer;
        eastl::unique_ptr<streaming::istream> i;
        FILE* mFileHandle = nullptr;

    public:
        ifstream(const path_t& filePath);
        ~ifstream();
        ifstream(const ifstream&) = delete;
        ifstream& operator=(const ifstream&) = delete;
    };


    // ==============================================================================
    // Output (write) file stream

    struct ofstream {
    protected:
        eastl::unique_ptr<streaming::ostream> o;
        FILE* mFileHandle = nullptr;

    public:
        ofstream(const path_t& filePath);
        ~ofstream();

        void_r write(const char* data, size_t size);
        void_r flush();
        ofstream(const ofstream&) = delete;
        ofstream& operator=(const ofstream&) = delete;
    };


    // ==============================================================================
    // Input (read) and output (write) file stream

    struct iofstream {
    protected:
        eastl::unique_ptr<streaming::iostream> io;
        FILE* mFileHandle = nullptr;

    public:
        iofstream(const path_t& filePath);
        ~iofstream();

        void_r flush();
        iofstream(const iofstream&) = delete;
        iofstream& operator=(const iofstream&) = delete;
    };
}
