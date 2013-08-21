#pragma once

// Visual studio cannot compile gl.h from NDK
#ifndef _MSC_VER
#  include <GLES/gl.h>
#else
#  include <Windows.h>
#  include <gl/GL.h>
#endif
