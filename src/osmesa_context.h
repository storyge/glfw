//========================================================================
// GLFW 3.3 OSMesa - www.glfw.org
//------------------------------------------------------------------------
// Copyright (c) 2016 Google Inc.
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================

#ifndef _glfw3_osmesa_context_h_
#define _glfw3_osmesa_context_h_

#define OSMESA_COLOR_INDEX GL_COLOR_INDEX
#define OSMESA_RGBA 0x1908
#define OSMESA_BGRA 0x1
#define OSMESA_ARGB 0x2
#define OSMESA_RGB 0x1907
#define OSMESA_BGR 0x4
#define OSMESA_RGB_565 0x5

#define OSMESA_ROW_LENGTH 0x10
#define OSMESA_Y_UP 0x11

#define OSMESA_WIDTH 0x20
#define OSMESA_HEIGHT 0x21
#define OSMESA_FORMAT 0x22
#define OSMESA_TYPE 0x23
#define OSMESA_MAX_WIDTH 0x24
#define OSMESA_MAX_HEIGHT 0x25

typedef void* OSMesaContext;
typedef void (*OSMESAproc)();

typedef OSMesaContext (* PFNOSMESACREATECONTEXTPROC)(GLenum,OSMesaContext);
typedef void (* PFNOSMESADESTROYCONTEXTPROC)(OSMesaContext);
typedef int (* PFNOSMESAMAKECURRENTPROC)(OSMesaContext,void*,int,int,int);
typedef OSMesaContext (* PFNOSMESAGETCURRENTCONTEXTPROC)(void);
typedef void (* PFNOSMESAPIXELSTOREPROC)(int,int);
typedef void (* PFNOSMESAGETINTEGERVPROC)(int,int*);
typedef int (* PFNOSMESAGETCOLORBUFFERPROC)(OSMesaContext,int*,int*,int*,void**);
typedef int (* PFNOSMESAGETDEPTHBUFFERPROC)(OSMesaContext,int*,int*,int*,void**);
typedef GLFWglproc (* PFNOSMESAGETPROCADDRESSPROC)(const char*);
#define OSMesaCreateContext _glfw.osmesa.CreateContext
#define OSMesaDestroyContext _glfw.osmesa.DestroyContext
#define OSMesaMakeCurrent _glfw.osmesa.MakeCurrent
#define OSMesaGetCurrentContext _glfw.osmesa.GetCurrentContext
#define OSMesaPixelStore _glfw.osmesa.PixelStore
#define OSMesaGetIntegerv _glfw.osmesa.GetIntegerv
#define OSMesaGetColorBuffer _glfw.osmesa.GetColorBuffer
#define OSMesaGetDepthBuffer _glfw.osmesa.GetDepthBuffer
#define OSMesaGetProcAddress _glfw.osmesa.GetProcAddress

#define _GLFW_PLATFORM_CONTEXT_STATE            _GLFWcontextOSMesa    osmesa
#define _GLFW_PLATFORM_LIBRARY_CONTEXT_STATE    _GLFWctxlibraryOSMesa osmesa


// OSMesa-specific per-context data
//
typedef struct _GLFWcontextOSMesa
{
   OSMesaContext       handle;
   int                 width;
   int                 height;
   void*               buffer;

} _GLFWcontextOSMesa;

// OSMesa-specific global data
//
typedef struct _GLFWctxlibraryOSMesa
{
    void*           handle;

    PFNOSMESACREATECONTEXTPROC       CreateContext;
    PFNOSMESADESTROYCONTEXTPROC      DestroyContext;
    PFNOSMESAMAKECURRENTPROC         MakeCurrent;
    PFNOSMESAGETCURRENTCONTEXTPROC   GetCurrentContext;
    PFNOSMESAPIXELSTOREPROC          PixelStore;
    PFNOSMESAGETINTEGERVPROC         GetIntegerv;
    PFNOSMESAGETCOLORBUFFERPROC      GetColorBuffer;
    PFNOSMESAGETDEPTHBUFFERPROC      GetDepthBuffer;
    PFNOSMESAGETPROCADDRESSPROC      GetProcAddress;

} _GLFWctxlibraryOSMesa;


GLFWbool _glfwInitOSMesa(void);
void _glfwTerminateOSMesa(void);
GLFWbool _glfwCreateContextOSMesa(_GLFWwindow* window,
                                  const _GLFWctxconfig* ctxconfig,
                                  const _GLFWfbconfig* fbconfig);

#endif // _glfw3_osmesa_context_h_
