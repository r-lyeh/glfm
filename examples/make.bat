@echo off

if "%1"=="tidy" (
	del *.ilk
	del *.pdb
	del *.obj
	del *.exe
	del *.exp
	del *.lib
	exit /b
)

set OPTIONS=..\src\glfm_desktop.c -I ..\include -I deps -D_USE_MATH_DEFINES /Zi /nologo

cl compass.c %OPTIONS% %*
cl heightmap.c %OPTIONS% %*
cl shader_toy.c %OPTIONS% %*
cl test_pattern.c test_pattern_renderer_gles2.c %OPTIONS% %*
cl touch.c %OPTIONS% %*
cl triangle.c %OPTIONS% %*
cl typing.c %OPTIONS% %*
