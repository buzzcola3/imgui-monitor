#pragma once

#include <atomic>
#include <chrono>
#include <string>
#include <thread>

struct MonitorOptions {
    int width = 640;
    int height = 480;
    std::string title = "ImGui Monitor";
    std::chrono::milliseconds frame_time{16};
};

class ImGuiMonitor {
public:
    ImGuiMonitor() = default;
    ~ImGuiMonitor();

    ImGuiMonitor(const ImGuiMonitor&) = delete;
    ImGuiMonitor& operator=(const ImGuiMonitor&) = delete;

    bool Run(const MonitorOptions& options = MonitorOptions{});
    void Stop();
    bool IsRunning() const;

private:
    void ThreadMain(MonitorOptions options);

    std::thread worker_;
    std::atomic<bool> running_{false};
    std::atomic<bool> stop_requested_{false};
};
