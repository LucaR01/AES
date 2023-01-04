//
// Created by Luca on 07/12/2022.
//

#ifndef AES_OS_HPP
#define AES_OS_HPP

// Ottengo il sistema operativo in cui il codice viene eseguito.
#if defined(_WIN32) || defined(_WIN64)
#define PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
#define PLATFORM_MAC
#elif defined(__linux__)
#define PLATFORM_LINUX
#else
#define PLATFORM_LINUX
#endif

// In base al sistema operativo, chiamo la funzione per creare un breakpoint.
#if defined(PLATFORM_WINDOWS)
#define AES_BREAK __debugbreak();
#elif defined(PLATFORM_MAC)
#define AES_BREAK __builtin_debugtrap();
#elif defined(PLATFORM_LINUX)
#define AES_BREAK __builtin_trap();
#else
#define AES_BREAK __builtin_trap();
#endif

#endif //AES_OS_HPP
