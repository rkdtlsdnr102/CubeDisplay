#pragma once

#define LIB_PATH "C:/CLibrary/glfw-3.3.4.bin.WIN64"

#if _MSC_VER >= 1920
#define LIB_FOLDER_NAME "lib-vc2019"
#elif _MSC_VER >= 1910
#define LIB_FOLDER_NAME "lib-vc2017"
#endif

#pragma comment(lib,LIB_PATH "/" LIB_FOLDER_NAME "/glfw3.lib")