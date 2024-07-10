/*
    glfm_loader.h - Single-header multi-platform OpenGL function loader

    ----------------------------------------------------------------------------
    USAGE
    ----------------------------------------------------------------------------
    1) Add the following lines in exactly one of your cpp files to compile the
       implementation:

           #define  GLFM_LOADER_IMPLEMENTATION
           #include "glfm_loader.h"

    2) In all other files in which you want to use OpenGL functions, simply 
       include this header file as follows:

           #include "glfm_loader.h"

    3) Call glfm_init_gl() before using any OpenGL function and after you have a
       valid OpenGL context.

    ----------------------------------------------------------------------------
    LICENSE
    ----------------------------------------------------------------------------
    This software is in the public domain. Where that dedication is not
    recognized, you are granted a perpetual, irrevocable license to copy,
    distribute, and modify this file as you see fit.

    ----------------------------------------------------------------------------
    ORIGINAL CODE
    https://github.com/ApoorvaJ/Papaya/blob/master/src/libpapaya/gl_lite.h
    ----------------------------------------------------------------------------
*/

#ifndef GLFM_LOADER_H
#define GLFM_LOADER_H

int glfm_init_gl();

#if defined(_WIN32)
#include <windows.h>
#define GLFM_GL_API WINAPI

#define GL_ARRAY_BUFFER                   0x8892 // Acquired from:
#define GL_ARRAY_BUFFER_BINDING           0x8894 // https://www.opengl.org/registry/api/GL/glext.h
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_COMPILE_STATUS                 0x8B81
#define GL_CURRENT_PROGRAM                0x8B8D
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_ELEMENT_ARRAY_BUFFER_BINDING   0x8895
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_FRAMEBUFFER                    0x8D40
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FUNC_ADD                       0x8006
#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C
#define GL_STATIC_DRAW                    0x88E4
#define GL_STREAM_DRAW                    0x88E0
#define GL_TEXTURE0                       0x84C0
#define GL_VERTEX_SHADER                  0x8B31
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_INFO_LOG_LENGTH                0x8B84

typedef char GLchar;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

#elif defined(__linux__)

#include <dlfcn.h>
#define GLFM_GL_API

#endif // __linux__

#include <GL/gl.h>

#define GLFM_GL_LIST(X) /* ret, name, params */ \
    X(GLenum,    CheckFramebufferStatus,  GLenum target) \
    X(GLint,     GetAttribLocation,       GLuint program, const GLchar *name) \
    X(GLint,     GetUniformLocation,      GLuint program, const GLchar *name) \
    X(GLuint,    CreateProgram,           void) \
    X(GLuint,    CreateShader,            GLenum type) \
    X(void,      ActiveTexture,           GLenum texture) \
    X(void,      AttachShader,            GLuint program, GLuint shader) \
    X(void,      BindAttribLocation,      GLuint program, GLuint index, const GLchar *name) \
    X(void,      BindBuffer,              GLenum target, GLuint buffer) \
    X(void,      BindFramebuffer,         GLenum target, GLuint framebuffer) \
    X(void,      BlendEquation,           GLenum mode) \
    X(void,      BlendFuncSeparate,       GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) \
    X(void,      BlendFuncSeparatei,      GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) \
    X(void,      BufferData,              GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage) \
    X(void,      BufferSubData,           GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data) \
    X(void,      ClearBufferfv,           GLenum buffer, GLint drawbuffer, const GLfloat * value) \
    X(void,      CompileShader,           GLuint shader) \
    X(void,      DeleteBuffers,           GLsizei n, const GLuint *buffers) \
    X(void,      DeleteFramebuffers,      GLsizei n, const GLuint *framebuffers) \
    X(void,      DeleteShader,            GLuint program) \
    X(void,      DrawBuffers,             GLsizei n, const GLenum *bufs) \
    X(void,      EnableVertexAttribArray, GLuint index) \
    X(void,      FramebufferTexture2D,    GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) \
    X(void,      GenBuffers,              GLsizei n, GLuint *buffers) \
    X(void,      GenFramebuffers,         GLsizei n, GLuint * framebuffers) \
    X(void,      GetShaderInfoLog,        GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) \
    X(void,      GetShaderiv,             GLuint shader, GLenum pname, GLint *params) \
    X(void,      LinkProgram,             GLuint program) \
    X(void,      ShaderSource,            GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length) \
    X(void,      Uniform1f,               GLint location, GLfloat v0) \
    X(void,      Uniform1i,               GLint location, GLint v0) \
    X(void,      Uniform2f,               GLint location, GLfloat v0, GLfloat v1) \
    X(void,      Uniform3f,               GLint location, GLfloat v0, GLfloat v1, GLfloat v2) \
    X(void,      Uniform4f,               GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) \
    X(void,      UniformMatrix4fv,        GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
    X(void,      UseProgram,              GLuint program) \
    X(void,      VertexAttribPointer,     GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer) \
    /* end */

#define GLFM_GL_DECL(ret, name, ...) typedef ret GLFM_GL_API name##proc(__VA_ARGS__); extern name##proc * gl##name;
#define GLFM_GL_DEF(ret, name, ...) name##proc * gl##name;

GLFM_GL_LIST(GLFM_GL_DECL)

#endif // GLFM_LOADER_H

// =============================================================================

#ifdef GLFM_LOADER_IMPLEMENTATION

#define GLFM_GL_PROC(ret, name, ...) \
    gl##name = (name##proc *)dlsym(libGL, "gl" #name); \
    if (!gl##name) { \
        printf("Function gl" #name " couldn't be loaded from libGL.so\n"); \
        return 0; \
    }

#define GLFM_WGL_PROC(ret, name, ...) \
    gl##name = (name##proc *)wglGetProcAddress("gl" #name); \
    if (!gl##name) { \
        puts("Function gl" #name " couldn't be loaded from opengl32.dll"); \
        return 0; \
    }

int glfm_init_gl()
{
#if defined(__linux__)

    void* libGL = dlopen("libGL.so", RTLD_LAZY);
    if (!libGL) {
        printf("ERROR: libGL.so couldn't be loaded\n");
        return 0;
    }

    GLFM_GL_LIST(GLFM_GL_PROC)

#elif defined(_WIN32)

    typedef PROC WINAPI wglGetProcAddressproc(LPCSTR lpszProc);
    HINSTANCE dll = LoadLibraryA("opengl32.dll");
    if (!dll) {
        puts("ERROR: opengl32.dll not found.");
        return 0;
    }
    wglGetProcAddressproc* wglGetProcAddress = (wglGetProcAddressproc*)GetProcAddress(dll, "wglGetProcAddress");

    GLFM_GL_LIST(GLFM_WGL_PROC)

#else
    #error "GL loading for this platform is not implemented yet."
#endif

    return 1;
}

GLFM_GL_LIST(GLFM_GL_DEF)

#endif // GLFM_LOADER_IMPLEMENTATION
