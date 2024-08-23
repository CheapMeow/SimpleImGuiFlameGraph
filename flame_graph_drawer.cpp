#include "flame_graph_drawer.h"

#include "imgui_internal.h"

void FlameGraphDrawer::Draw(const std::vector<ScopeTimeData>& scope_times,
                            int                               max_depth,
                            std::chrono::microseconds         global_start,
                            ImVec2                            graph_size)
{
    if (scope_times.size() == 0)
        return;

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;

    const auto   blockHeight = ImGui::GetTextLineHeight() + (style.FramePadding.y * 2);
    const ImVec2 label_size  = ImGui::CalcTextSize("Testing", NULL, true);
    if (graph_size.x == 0.0f)
        graph_size.x = ImGui::CalcItemWidth();
    if (graph_size.y == 0.0f)
        graph_size.y = label_size.y + (style.FramePadding.y * 3) + blockHeight * (max_depth + 1);

    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + graph_size);
    const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
    const ImRect total_bb(
        frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0));
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, 0, &frame_bb))
        return;

    ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

    const ImU32 col_base            = ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x77FFFFFF;
    const ImU32 col_hovered         = ImGui::GetColorU32(ImGuiCol_PlotHistogramHovered) & 0x77FFFFFF;
    const ImU32 col_outline_base    = ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x7FFFFFFF;
    const ImU32 col_outline_hovered = ImGui::GetColorU32(ImGuiCol_PlotHistogramHovered) & 0x7FFFFFFF;

    auto frame_time =
        scope_times[scope_times.size() - 1].start + scope_times[scope_times.size() - 1].duration - global_start;

    float inner_width = inner_bb.Max.x - inner_bb.Min.x;

    bool any_hovered = false;

    for (const auto& scope_time : scope_times)
    {
        auto start_time = scope_time.start - global_start;

        float start_x_percent = (double)start_time.count() / frame_time.count();
        float end_x_percent   = start_x_percent + (double)scope_time.duration.count() / frame_time.count();

        float bottom_height = blockHeight * (max_depth - scope_time.depth + 1) - style.FramePadding.y;

        auto pos0 = inner_bb.Min + ImVec2(start_x_percent * inner_width, bottom_height);
        auto pos1 = inner_bb.Min + ImVec2(end_x_percent * inner_width, bottom_height + blockHeight);

        bool v_hovered = false;
        if (ImGui::IsMouseHoveringRect(pos0, pos1))
        {
            ImGui::SetTooltip("%s: %8.4gms", scope_time.name.c_str(), (double)scope_time.duration.count() / 1000.0);
            v_hovered   = true;
            any_hovered = v_hovered;
        }

        window->DrawList->AddRectFilled(pos0, pos1, v_hovered ? col_hovered : col_base);
        window->DrawList->AddRect(pos0, pos1, v_hovered ? col_outline_hovered : col_outline_base);
        auto textSize   = ImGui::CalcTextSize(scope_time.name.c_str());
        auto boxSize    = (pos1 - pos0);
        auto textOffset = ImVec2(0.0f, 0.0f);
        if (textSize.x < boxSize.x)
        {
            textOffset = ImVec2(0.5f, 0.5f) * (boxSize - textSize);
            ImGui::RenderText(pos0 + textOffset, scope_time.name.c_str());
        }
    }

    if (!any_hovered && ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Total: %8.4g", frame_time.count() / 1000.0);
    }
}