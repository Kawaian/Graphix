#pragma once

#include <iostream>
#include <string>
#include <fstream>

#define LOGGER_ENABLED 1
#define SHUT_DOWN_ENABLED 1

#if LOGGER_ENABLED
#define LOG(x, msg) do{ std::cout << x << " : " << msg << "\n";}while(0)
#define LOG_INFO(x, msg, msg2) do{ std::cout << x << " : " << msg << " : " << msg2 << "\n";}while(0)
#define LOG_ERROR(x, msg) do{ std::cerr << x << " : " << msg << "\n";}while(0)
#define LOG_ERROR_INFO(x, msg, msg2) do{ std::cout << x << " : " << msg << " : " << msg2 <<  "\n";}while(0)
#define LOG_PAUSE() do{ std::cin.get();}while(0)

#else
#define LOG(x, msg)
#define LOG_INFO(x, msg, msg2)
#define LOG_ERROR(x, msg)
#define LOG_ERROR_INFO(x, msg, msg2)
#define LOG_PAUSE()
#endif


#if SHUT_DOWN_ENABLED
#define EXIT(status) do{ exit(status);}while(0)
#else
#define EXIT(status)
#endif







