#pragma once

#include "streaming/streaming.h"
#include "filesystem/filesystem.h"

#include <EASTL/unique_ptr.h>

#include <cstdio>


namespace eloo::filesystem {

    // ==============================================================================
    // Input (read) file stream

    struct ifstream {
    protected:
        eastl::unique_ptr<streaming::istream> i;
        FILE* mFileHandle = nullptr;

    public:
        ifstream(const path_t& filePath);
        ~ifstream();
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

        virtual void_r flush() = 0;
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

        virtual void_r flush() = 0;
    };
}