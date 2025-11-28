#include "debugglass/widgets/message_monitor.h"

#include <imgui.h>

#include <utility>

namespace debugglass {
namespace {
constexpr float kHighlightWindowSeconds = 0.5f;
}

MessageMonitor::MessageMonitor(std::string label) : label_(std::move(label)) {}

void MessageMonitor::UpsertMessage(std::string id, std::string value) {
    auto now = std::chrono::steady_clock::now();
    std::lock_guard<std::mutex> lock(mutex_);
    auto found = index_by_id_.find(id);
    if (found == index_by_id_.end()) {
        Entry entry;
        entry.id = std::move(id);
        entry.value = std::move(value);
        entry.update_count = 1;
        entry.last_update = now;
        entries_.push_back(std::move(entry));
        index_by_id_[entries_.back().id] = entries_.size() - 1;
    } else {
        Entry& entry = entries_[found->second];
        entry.value = std::move(value);
        entry.update_count += 1;
        entry.last_update = now;
    }
}

void MessageMonitor::Render() const {
    struct Snapshot {
        std::string id;
        std::string value;
        uint64_t update_count;
        std::chrono::steady_clock::time_point last_update;
    };

    std::vector<Snapshot> snapshot;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        snapshot.reserve(entries_.size());
        for (const auto& entry : entries_) {
            Snapshot snapshot_entry;
            snapshot_entry.id = entry.id;
            snapshot_entry.value = entry.value;
            snapshot_entry.update_count = entry.update_count;
            snapshot_entry.last_update = entry.last_update;
            snapshot.push_back(std::move(snapshot_entry));
        }
    }

    if (snapshot.empty()) {
        ImGui::TextUnformatted("No messages received");
        return;
    }

    const std::string table_id = "MessageMonitor##" + label_;
    const ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                                  ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY |
                                  ImGuiTableFlags_Reorderable;
    if (ImGui::BeginTable(table_id.c_str(), 4, flags)) {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthStretch, 0.4f);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 0.4f);
        ImGui::TableSetupColumn("Updates", ImGuiTableColumnFlags_WidthFixed, 0.2f);
        ImGui::TableSetupColumn("Age (ms)", ImGuiTableColumnFlags_WidthFixed, 0.2f);
        ImGui::TableHeadersRow();

        const auto now = std::chrono::steady_clock::now();
        for (const auto& entry : snapshot) {
            const auto age = now - entry.last_update;
            const float age_seconds = std::chrono::duration_cast<std::chrono::duration<float>>(age).count();
            const bool highlight = age_seconds <= kHighlightWindowSeconds;

            if (highlight) {
                ImGui::TableNextRow(ImGuiTableRowFlags_None, 0.0f);
                const ImU32 bg_color = ImGui::GetColorU32(ImVec4(0.9f, 0.9f, 0.3f, 0.25f));
                ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, bg_color);
            } else {
                ImGui::TableNextRow();
            }

            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(entry.id.c_str());

            ImGui::TableSetColumnIndex(1);
            ImGui::TextUnformatted(entry.value.c_str());

            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%llu", static_cast<unsigned long long>(entry.update_count));

            ImGui::TableSetColumnIndex(3);
            const auto age_ms = std::chrono::duration_cast<std::chrono::milliseconds>(age).count();
            ImGui::Text("%lld", static_cast<long long>(age_ms));
        }
        ImGui::EndTable();
    }
}

}  // namespace debugglass
