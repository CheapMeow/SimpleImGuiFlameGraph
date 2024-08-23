#pragma once

#include "scope_time_data.h"

#include <vector>

class TimerSingleton
{
public:
    TimerSingleton(const TimerSingleton&) = delete;
    TimerSingleton(TimerSingleton&&)      = delete;

    static TimerSingleton& Get()
    {
        static TimerSingleton instance;
        return instance;
    }

    void Push()
    {
        m_curr_depth++;
        if (m_curr_depth > m_max_depth)
            m_max_depth = m_curr_depth;
    }

    void Pop() { m_curr_depth--; }

    int GetCurrDepth() const { return m_curr_depth; }

    int GetMaxDepth() const { return m_max_depth; }

    std::chrono::microseconds GetGlobalStart() { return m_global_start; };

    void Upload(ScopeTimeData scope_time)
    {
        if (m_scope_times.size() == 0)
            m_global_start = scope_time.start;
        else
            m_global_start = m_global_start.count() > scope_time.start.count() ? scope_time.start : m_global_start;

        m_scope_times.push_back(scope_time);
    }

    const std::vector<ScopeTimeData>& GetScopeTimes() { return m_scope_times; }

    void Clear()
    {
        m_curr_depth = -1;
        m_max_depth  = -1;
        m_scope_times.clear();
    }

private:
    TimerSingleton() {}

    int m_curr_depth = -1;
    int m_max_depth  = -1;

    std::chrono::microseconds m_global_start;

    std::vector<ScopeTimeData> m_scope_times;
};

class Timer
{
public:
    Timer(const std::string& name, const std::string& filename)
        : m_name(name)
        , m_filename(filename)
        , m_stopped(false)
    {
        m_stopped = false;

        size_t pos = m_name.find(cdel_str);
        if (pos != std::string::npos)
        {
            m_name.erase(pos, cdel_str.length());
        }
        m_start_timepoint = std::chrono::steady_clock::now();

        TimerSingleton::Get().Push();
    }

    ~Timer()
    {
        if (!m_stopped)
            Stop();
    }

    void Stop()
    {
        auto end_timepoint = std::chrono::steady_clock::now();
        auto high_res_start =
            std::chrono::time_point_cast<std::chrono::microseconds>(m_start_timepoint).time_since_epoch();
        auto elapsed_time =
            std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint).time_since_epoch() -
            std::chrono::time_point_cast<std::chrono::microseconds>(m_start_timepoint).time_since_epoch();

        TimerSingleton::Get().Upload({m_name,
                                      m_filename,
                                      high_res_start,
                                      elapsed_time,
                                      TimerSingleton::Get().GetCurrDepth(),
                                      std::this_thread::get_id()});

        m_stopped = true;

        TimerSingleton::Get().Pop();
    }

private:
    std::string                                        m_name;
    std::string                                        m_filename;
    std::chrono::time_point<std::chrono::steady_clock> m_start_timepoint;
    bool                                               m_stopped;

    inline static const std::string cdel_str = "__cdecl ";
};

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || \
    defined(__ghs__)
#    define FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#    define FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#    define FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#    define FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#    define FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#    define FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#    define FUNC_SIG __func__
#else
#    define FUNC_SIG "FUNC_SIG unknown!"
#endif

#define SCOPE_TIMER_LINE(name, filename, line)      Timer timer##line(name, filename)
#define SCOPE_TIMER_PASTELINE(name, filename, line) SCOPE_TIMER_LINE(name, filename, line)
#define SCOPE_TIMER(name)                           SCOPE_TIMER_PASTELINE(name, __FILE__, __LINE__)
#define FUNCTION_TIMER()                            SCOPE_TIMER(FUNC_SIG)
