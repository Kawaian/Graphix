#pragma once

#if defined(_WIN32) || defined(_WIN64)
      #define SPIKY_OS_WINDOWS

      #if defined(_WIN64)
            #define SPIKY_64_BIT
      #else
            #define SPIKY_32_BIT
      #endif
#elif defined(__APPLE__) && defined(__MACH)
      #define SPIKY_OS_APPLE
#elif defined(__unix__)
      #define SPIKY_OS_UNIX

      #if defined(__linux__)
            #define SPIKY_OS_LINUX
      #elif defined(__FreeBSD) || defined(__FreeBSD_kernel)
            #define SPIKY_OS_FREEBSD
      #else
            #warning This unix version is not supported : it may cause problems !
      #endif
#endif

#ifdef WINAPI
#define __GPX_APIENTRY WINAPI 
#else
#define __GPX_APIENTRY 
#endif

#if defined(__GNUC__)
      #if defined(__x86_64__) || defined(__ppc64__)
            #define SPIKY_64_BIT
      #else
            #define SPIKY_32_BIT
      #endif
#endif