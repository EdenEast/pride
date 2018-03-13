
#pragma once

#if defined(PRI_INLINE_LIBRARY)
    #define PRI_FN_API inline
    #define PRI_CLASS_API
#elif defined(PRI_STATIC_LIRBARY)
    #define PRI_FN_API
    #define PRI_CLASS_API
#elif defined(PRI_EXPORT_LIBRARY)
    #define PRI_FN_API __declspec(dllexport)
    #define PRI_CLASS_API __declspec(dllexport)
#elif defined(PRI_IMPORT_LIBRARY)
    #define PRI_FN_API __declspec(dllimport)
    #define PRI_CLASS_API __declspec(dllimport)
#endif
