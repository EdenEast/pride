
#pragma once

#if defined(PRIDE_INLINE_LIBRARY)
#    define PRIDE_FN_API inline
#    define PRIDE_CLASS_API
#elif defined(PRIDE_STATIC_LIRBARY)
#    define PRIDE_FN_API
#    define PRIDE_CLASS_API
#elif defined(PRIDE_EXPORT_LIBRARY)
#    define PRIDE_FN_API __declspec(dllexport)
#    define PRIDE_CLASS_API __declspec(dllexport)
#elif defined(PRIDE_IMPORT_LIBRARY)
#    define PRIDE_FN_API __declspec(dllimport)
#    define PRIDE_CLASS_API __declspec(dllimport)
#endif
