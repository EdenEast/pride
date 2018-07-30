
#pragma once

#include "../../config/detection/os.hpp"
#include "../../config/include/windows.hpp"

#include <thread>

#if defined(PRIDE_OS_WINDOWS)
    #include <io.h>
    #include <process.h>
#else
    #include <fcntl.h>
    #include <unistd.h>
#endif

namespace pride::log::internal
{
    inline size_t _thread_id()
    {
#if defined(PRIDE_OS_WINDOWS)
        return static_cast<size_t>(::GetCurrentThreadId());
#elif defined(PRIDE_OS_ANDROID)
        return static_cast<size_t>(syscall(__NR_gettid));
#elif defined(PRIDE_OS_MACOSX)
        uint64_t tid;
        pthread_threadid_np(nullptr, &tid);
        return static_cast<size_t>(tid);
#else
        return static_cast<size_t>(std::hash<std::thread::id>()(std::this_thread::get_id()));
#endif
    }

    inline size_t thread_id()
    {
        static thread_local const size_t tid = _thread_id();
        return tid;
    }
}
