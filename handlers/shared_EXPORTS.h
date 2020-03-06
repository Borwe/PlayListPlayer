#ifndef shared_EXPORTS_H
#define shared_EXPORTS_H

    #ifdef _WIN32
        #ifdef shared_EXPORTS
            #define SHARED_EXPORT __declspec(dllexport)
        #else
            #define SHARED_EXPORT __declspec(dllimport)
        #endif
    #endif

    #ifndef SHARED_EXPORT
        #define SHARED_EXPORT
    #endif

#endif
