#include "glfm.h"

#if defined __linux__ || defined _WIN32 || (defined __APPLE__ && TARGET_OS_OSX)

#include "glfm_internal.h"

// #if defined(GLFW_EXPOSE_NATIVE_WIN32) || defined(GLFW_EXPOSE_NATIVE_WGL)
// #elif defined(GLFW_EXPOSE_NATIVE_COCOA) || defined(GLFW_EXPOSE_NATIVE_NSGL)
// #elif defined(GLFW_EXPOSE_NATIVE_X11) || defined(GLFW_EXPOSE_NATIVE_GLX)
// #elif defined(GLFW_EXPOSE_NATIVE_WAYLAND)
// #if defined(GLFW_EXPOSE_NATIVE_EGL)
// #if defined(GLFW_EXPOSE_NATIVE_OSMESA)

#if USE_GLFW3
#define GLAD_GL_IMPLEMENTATION
#include "glad.h"
#define GLFW_INCLUDE_NONE
// select backend
#ifdef __linux__
#define _GLFW_X11 // also _GLFW_WAYLAND or _GLFW_OSMESA
#elif defined _WIN32
#define _GLFW_WIN32
#else
#define _GLFW_COCOA
#endif
#define _GLFW_IMPLEMENTATION
#include "glfw3.h"
#define glfm__InitGL() gladLoadGL(glfwGetProcAddress)
#else
#define GLFM_LOADER_IMPLEMENTATION
#include "glfm_gl.h"
#define TIGR_IMPLEMENTATION
#define TIGR_DO_NOT_PRESERVE_WINDOW_POSITION
#include "tigr.h"
#define GLFWwindow Tigr
typedef struct tigrExtend {
    Tigr *self;
    void *other;

    int oldw, oldh;
    void (*on_resize)(GLFWwindow* window, int width, int height);
} tigrExtend;
#define glfwInit() (1)
#define glfwTerminate() (0)
#define glfwSwapBuffers(window) tigrUpdate(window)
#define glfwGetTime() tigrTime()
#define glfwPollEvents()
#define glfwMakeContextCurrent(window) 
#define glfm__InitGL() glfm_init_gl()
#define glfwSetWindowUserPointer(window, display) (((tigrExtend*)((window)->userdata))->other = (display))
#define glfwGetWindowUserPointer(window) (((tigrExtend*)((window)->userdata))->other)
#define glfwSetFramebufferSizeCallback(window, on_resize_cb) (((tigrExtend*)((window)->userdata))->on_resize = (on_resize_cb))
#define glfwGetFramebufferSize(window, width, height) (*(width) = (window)->w, *(height) = (window)->h)

GLFWwindow *glfwCreateWindow(int width, int height, const char *title, void *null1, void *null2) {
    Tigr *win = tigrWindow(width, height, title, 0|TIGR_AUTO);
    if( !win ) return NULL;

    tigrExtend *ext = (tigrExtend*)calloc(1, sizeof(tigrExtend)); // @leak
    win->userdata = ext;
    return win;
}
void glfwDestroyWindow(GLFWwindow *window) {
    free(window->userdata);
    tigrFree(window);
}

int glfwWindowShouldClose(GLFWwindow *window) {
    if(tigrClosed(window)) return 1;
    tigrBeginOpenGL(window);

    tigrExtend *ext = (tigrExtend*)window->userdata;

    if ( (window->w^ext->oldw) || (window->h^ext->oldh) ) {
        ext->oldw = window->w;
        ext->oldh = window->h;

        if ( ext->on_resize )
        ext->on_resize(window, window->w, window->h);
    }

    return 0;
}

#endif

#include <stdlib.h>

typedef struct GLFMPlatformData {
    GLFWwindow *window;
    int width;
    int height;
    double scale;

    GLFMDisplay display;
    GLFMRenderingAPI renderingAPI;
} GLFMPlatformData;

static void glfm__displayChromeUpdated(GLFMDisplay *display) {}
static void glfm__sensorFuncUpdated(GLFMDisplay *display) {}

GLFMRenderingAPI         glfmGetRenderingAPI(const GLFMDisplay *display) { return GLFMRenderingAPIOpenGLES2; }
bool                     glfmIsMetalSupported(const GLFMDisplay *display) { return false; }

GLFMInterfaceOrientation glfmGetInterfaceOrientation(const GLFMDisplay *display) { return GLFMInterfaceOrientationLandscapeLeft; }
void   glfmSetSupportedInterfaceOrientation(GLFMDisplay *display, GLFMInterfaceOrientation supportedOrientations) {}

bool   glfmIsSensorAvailable(const GLFMDisplay *display, GLFMSensor sensor) { return false; }
void   glfmPerformHapticFeedback(GLFMDisplay *display, GLFMHapticFeedbackStyle style) {}

void   glfmGetDisplayChromeInsets(const GLFMDisplay *display, double *top, double *right, double *bottom, double *left) { *top = *right = *bottom = *left = 0; }
void   glfmGetDisplaySize(const GLFMDisplay *display, int *width, int *height) { glfwGetFramebufferSize((GLFWwindow*)display->userData, width, height); }
double glfmGetDisplayScale(const GLFMDisplay *display) { return ((GLFMPlatformData*)display->platformData)->scale; }

bool   glfmSetClipboardText(GLFMDisplay *display, const char *string) { return false; }
void   glfmRequestClipboardText(GLFMDisplay *display, GLFMClipboardTextFunc clipboardTextFunc) { if(clipboardTextFunc) clipboardTextFunc(display, NULL); }

bool   glfmHasVirtualKeyboard(const GLFMDisplay *display) { return false; }
void   glfmSetKeyboardVisible(GLFMDisplay *display, bool visible) {}
bool   glfmIsKeyboardVisible(const GLFMDisplay *display) { return false; }

double glfmGetTime(void) { return glfwGetTime(); }
void   glfmSwapBuffers(GLFMDisplay *display) { glfwSwapBuffers((GLFWwindow*)display->userData); }

static void on_resize(GLFWwindow* window, int width, int height) {
    GLFMDisplay* display = (GLFMDisplay*)glfwGetWindowUserPointer(window);
    if (display->surfaceResizedFunc) {
        display->surfaceResizedFunc(display, width, height);
    }
}



int main(int argc, char** argv) {
    int fbw, fbh;

    if (!glfwInit())
        exit(EXIT_FAILURE);

    GLFWwindow *win = glfwCreateWindow(640, 480, "", NULL, NULL);
    if (!win) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    GLFMPlatformData p = {0};
    p.window = win;
    p.display.platformData = &p;
    glfmMain(&p.display);
    p.display.userData = win;

    glfwSetWindowUserPointer(win, &p.display);
    glfwSetFramebufferSizeCallback(win, on_resize);
    glfwGetFramebufferSize(win, &fbw, &fbh);
    p.scale = fbw / 640.0;
    if (p.display.surfaceCreatedFunc) {
        p.display.surfaceCreatedFunc(&p.display, fbw, fbh);
    }
    if (p.display.surfaceRefreshFunc) {
        p.display.surfaceRefreshFunc(&p.display);
    }

    glfwMakeContextCurrent(win);
    glfm__InitGL();

    while (glfwWindowShouldClose(win) == 0) {
        if (p.display.renderFunc) {
            p.display.renderFunc(&p.display);
        }
        glfwPollEvents();
    }

    glfwDestroyWindow(win);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
#endif
