#pragma once

#include "OSConfig.h"

#define GLOBAL          static
#define INTERNAL        static
#define LOCAL_PERSIST   static

namespace gpx
{
      using s8 =  signed char;
      using u8 =  unsigned char;
      using s16 = signed short;
      using u16 = unsigned short;
      using s32 = signed int;
      using u32 = unsigned int;
#ifdef _MSC_VER
      using s64 = signed   __int64;
      using u64 = unsigned __int64;
#else
      using s64 = signed   long long;
      using u64 = unsigned long long;
#endif

#ifdef SPIKY_32_BIT
      using usize = u64;
      using ssize = u64;
#else
      using usize = u32;
      using ssize = u32;
#endif
	  using f32 = float;
	  using f64 = double;
}
