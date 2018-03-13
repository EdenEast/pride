
#pragma once

#if defined(__PRETTY_FUNCTION__)
    #define PRI_FUNCTION __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
    #define PRI_FUNCTION __FUNCSIG__
#else
    #define PRI_FUNCTION __func__
#endif
