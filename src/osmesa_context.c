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

#include <stdlib.h>
#include <string.h>

#include "internal.h"


static void makeContextCurrentOSMesa(_GLFWwindow* window)
{
    if (window)
    {
        // Check to see if we need to allocate a new buffer
        if ((window->context.osmesa.buffer == NULL) ||
            (window->osmesa.width != window->context.osmesa.width) ||
            (window->osmesa.height != window->context.osmesa.height))
        {
            free(window->context.osmesa.buffer);

            // Allocate the new buffer (width * height * 1 byte per RGBA
            // channel)
            window->context.osmesa.buffer = (unsigned char *)
                malloc(window->osmesa.width * window->osmesa.height * 4);

            // Update the context size
            window->context.osmesa.width = window->osmesa.width;
            window->context.osmesa.height = window->osmesa.height;
        }

        // Make the context current
        if (!OSMesaMakeCurrent(window->context.osmesa.handle,
                            window->context.osmesa.buffer,
                            GL_UNSIGNED_BYTE,
                            window->osmesa.width, window->osmesa.height))
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "OSMesa: Failed to make context current");
            return;
        }
    }

    _glfwPlatformSetCurrentContext(window);
}

static GLFWglproc getProcAddressOSMesa(const char* procname)
{
    return (GLFWglproc) OSMesaGetProcAddress(procname);
}

static void destroyContextOSMesa(_GLFWwindow* window)
{
    if (window->context.osmesa.handle)
    {
        OSMesaDestroyContext(window->context.osmesa.handle);
        window->context.osmesa.handle = NULL;
    }

    if (window->context.osmesa.buffer)
    {
        free(window->context.osmesa.buffer);
        window->context.osmesa.width = 0;
        window->context.osmesa.height = 0;
    }
}

static void swapBuffersOSMesa(_GLFWwindow* window)
{
}

static void swapIntervalOSMesa(int interval)
{
}

static int extensionSupportedOSMesa(const char* extension)
{
    return GLFW_FALSE;
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

GLFWbool _glfwInitOSMesa(void)
{
    int i;
    const char* sonames[] =
    {
#if defined(_GLFW_WIN32)
        "libOSMesa.dll",
        "OSMesa.dll",
#elif defined(_GLFW_COCOA)
        "libOSMesa.dylib",
#else
        "libOSMesa.so.6",
#endif
        NULL
    };

    if (_glfw.osmesa.handle)
        return GLFW_TRUE;

    for (i = 0;  sonames[i];  i++)
    {
        _glfw.osmesa.handle = _glfw_dlopen(sonames[i]);
        if (_glfw.osmesa.handle)
            break;
    }

    if (!_glfw.osmesa.handle)
    {
        _glfwInputError(GLFW_API_UNAVAILABLE, "OSMesa: Library not found");
        return GLFW_FALSE;
    }

    _glfw.osmesa.CreateContext = (PFNOSMESACREATECONTEXTPROC)
        _glfw_dlsym(_glfw.osmesa.handle, "OSMesaCreateContext");
    _glfw.osmesa.DestroyContext = (PFNOSMESADESTROYCONTEXTPROC)
        _glfw_dlsym(_glfw.osmesa.handle, "OSMesaDestroyContext");
    _glfw.osmesa.MakeCurrent = (PFNOSMESAMAKECURRENTPROC)
        _glfw_dlsym(_glfw.osmesa.handle, "OSMesaMakeCurrent");
    _glfw.osmesa.GetCurrentContext = (PFNOSMESAGETCURRENTCONTEXTPROC)
        _glfw_dlsym(_glfw.osmesa.handle, "OSMesaGetCurrentContext");
    _glfw.osmesa.PixelStore = (PFNOSMESAPIXELSTOREPROC)
        _glfw_dlsym(_glfw.osmesa.handle, "OSMesaPixelStore");
    _glfw.osmesa.GetIntegerv = (PFNOSMESAGETINTEGERVPROC)
        _glfw_dlsym(_glfw.osmesa.handle, "OSMesaGetIntegerv");
    _glfw.osmesa.GetColorBuffer = (PFNOSMESAGETCOLORBUFFERPROC)
        _glfw_dlsym(_glfw.osmesa.handle, "OSMesaGetColorBuffer");
    _glfw.osmesa.GetDepthBuffer = (PFNOSMESAGETDEPTHBUFFERPROC)
        _glfw_dlsym(_glfw.osmesa.handle, "OSMesaGetDepthBuffer");
    _glfw.osmesa.GetProcAddress = (PFNOSMESAGETPROCADDRESSPROC)
        _glfw_dlsym(_glfw.osmesa.handle, "OSMesaGetProcAddress");

    if (!_glfw.osmesa.CreateContext ||
        !_glfw.osmesa.DestroyContext ||
        !_glfw.osmesa.MakeCurrent ||
        !_glfw.osmesa.GetCurrentContext ||
        !_glfw.osmesa.PixelStore ||
        !_glfw.osmesa.GetIntegerv ||
        !_glfw.osmesa.GetColorBuffer ||
        !_glfw.osmesa.GetDepthBuffer ||
        !_glfw.osmesa.GetProcAddress)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "OSMesa: Failed to load required entry points");

        _glfwTerminateOSMesa();
        return GLFW_FALSE;
    }

    return GLFW_TRUE;
}

void _glfwTerminateOSMesa(void)
{
    if (_glfw.osmesa.handle)
    {
        _glfw_dlclose(_glfw.osmesa.handle);
        _glfw.osmesa.handle = NULL;
    }
}

GLFWbool _glfwCreateContextOSMesa(_GLFWwindow* window,
                                  const _GLFWctxconfig* ctxconfig,
                                  const _GLFWfbconfig* fbconfig)
{
    OSMesaContext share;

    if (ctxconfig->share)
        share = ctxconfig->share->context.osmesa.handle;

    // Initialize the context
    window->context.osmesa.buffer = NULL;
    window->context.osmesa.width = 0;
    window->context.osmesa.height = 0;

    // Create to create an OSMesa context
    window->context.osmesa.handle = OSMesaCreateContext(OSMESA_RGBA, share);
    if (window->context.osmesa.handle == 0)
    {
        _glfwInputError(GLFW_VERSION_UNAVAILABLE,
                        "OSMesa: Failed to create context");
        return GLFW_FALSE;
    }

    // Set up the context API
    window->context.makeCurrent = makeContextCurrentOSMesa;
    window->context.swapBuffers = swapBuffersOSMesa;
    window->context.swapInterval = swapIntervalOSMesa;
    window->context.extensionSupported = extensionSupportedOSMesa;
    window->context.getProcAddress = getProcAddressOSMesa;
    window->context.destroy = destroyContextOSMesa;

    return GLFW_TRUE;
}


//////////////////////////////////////////////////////////////////////////
//////                        GLFW native API                       //////
//////////////////////////////////////////////////////////////////////////

