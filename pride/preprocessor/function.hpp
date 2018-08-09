
#pragma once

#if defined(__PRETTY_FUNCTION__)
#    define PRIDE_FUNCTION __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#    define PRIDE_FUNCTION __FUNCSIG__
#else
#    define PRIDE_FUNCTION __func__
#endif
