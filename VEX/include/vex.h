#ifndef VEX_H_
#define VEX_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Prefer the system/SDK header `iq2_cpp.h` if it's available on the
// compiler include path (installed SDK). Otherwise fall back to the
// bundled development stub `iq2_cpp_stub.h` so the project builds
// and IntelliSense works for contributors who don't have the SDK.
#if defined(__has_include)
#  if __has_include(<iq2_cpp.h>)
#    include <iq2_cpp.h>
#  else
#    include "iq2_cpp_stub.h"
#  endif
#else
#  include "iq2_cpp_stub.h"
#endif



#endif