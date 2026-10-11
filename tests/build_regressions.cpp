#include "streaming/streaming.h"
#include "filesystem/filestream.h"
#include "utility/observer.h"
#include "logging.h"
#include <fstream>
#include <stdexcept>

void require(bool ok) { if (!ok) { throw std::runtime_error("Regression check failed"); } }

int run_checks() {
    std::fputs("stream test\n", stderr);
    using namespace sge;
    streaming::iostream stream(0);
    require(bool(stream << uint32_t{42}));
    uint32_t value = 0;
    require(bool(stream >> value));
    require(value == 42);

    std::fputs("observer test\n", stderr);
    observer<int> event;
    int observed = 0;
    auto subscription = event.subscribe([&](int v) { observed = v; });
    event.broadcast(7);
    require(observed == 7);
    event.unsubscribe(subscription);
    event.broadcast(9);
    require(observed == 7);

    {
        std::fputs("file test\n", stderr);
        filesystem::ofstream file("regression-output.tmp");
        require(bool(file.write("hello", 5)));
        require(bool(file.flush()));
    }
    {
        std::ifstream file("regression-output.tmp", std::ios::binary);
        std::string text;
        std::getline(file, text);
        require(text == "hello");
        filesystem::ifstream input("regression-output.tmp");
    }
    std::filesystem::remove("regression-output.tmp");
    std::fputs("logger test\n", stderr);
    logging::logger log("regression");
    log.log_info("value {}", 42);
    return 0;
}

int main() { try { return run_checks(); } catch (const std::exception& e) { std::fprintf(stderr, "%s\n", e.what()); return 1; } }
