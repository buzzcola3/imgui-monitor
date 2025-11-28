#include <array>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>

#include <imgui.h>

#include "debugglass/debugglass.h"

namespace {
constexpr auto kDemoDuration = std::chrono::seconds(20);
}

int main() {
    debugglass::DebugGlass monitor;

    auto& window = monitor.windows.add("Message Monitor");
    auto& tab = window.tabs.add("stream");
    tab.SetRenderCallback([]() {
        ImGui::TextUnformatted("IDs update in place as new samples arrive");
    });
    auto& message_monitor = tab.AddMessageMonitor("CAN Trace");

    debugglass::DebugGlassOptions options;
    options.title = "Message Monitor Demo";

    if (!monitor.Run(options)) {
        std::cerr << "Failed to start DebugGlass" << std::endl;
        return 1;
    }

    std::cout << "Message monitor demo running" << std::endl;

    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> distribution(0.0f, 100.0f);
    const std::array<const char*, 4> ids = {"ID_101", "ID_220", "ID_305", "ID_999"};

    auto end_time = std::chrono::steady_clock::now() + kDemoDuration;
    while (std::chrono::steady_clock::now() < end_time) {
        for (const char* id : ids) {
            float value = distribution(rng);
            message_monitor.UpsertMessage(id, value);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    monitor.Stop();
    return 0;
}
