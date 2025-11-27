#include "debugglass/debugglass.h"

#include <chrono>
#include <iostream>
#include <thread>

namespace {
constexpr auto kDemoDuration = std::chrono::seconds(10);
}

int main() {
    DebugGlass monitor;

    DebugGlassOptions options;
    options.title = "DebugGlass Demo";

    if (!monitor.Run(options)) {
        std::cerr << "Failed to start DebugGlass" << std::endl;
        return 1;
    }

    std::cout << "Hello World" << std::endl;

    std::this_thread::sleep_for(kDemoDuration);

    monitor.Stop();
    return 0;
}
