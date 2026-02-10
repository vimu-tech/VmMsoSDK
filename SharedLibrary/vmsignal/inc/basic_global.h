#ifndef BAISC_GLOBAL_H
#define BAISC_GLOBAL_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define Q_DECL_EXPORT __declspec(dllexport)
#  define Q_DECL_IMPORT __declspec(dllimport)

#define Q_DECL_CALL __stdcall
//#define Q_DECL_CALL __cdecl //用来控制函数是否重命名，cdecl不需要重命名

#else
#  define Q_DECL_EXPORT     __attribute__((visibility("default")))
#  define Q_DECL_IMPORT     __attribute__((visibility("default")))

#define Q_DECL_CALL

#endif

#endif // 
