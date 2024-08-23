#pragma once

#include "scope_time_data.h"

#include <imgui.h>
#include <vector>

class FlameGraphDrawer
{
public:
    static void Draw(const std::vector<ScopeTimeData>& scope_times,
                     int                               max_depth,
                     std::chrono::microseconds         global_start,
                     ImVec2                            graph_size = ImVec2(0, 0));
};