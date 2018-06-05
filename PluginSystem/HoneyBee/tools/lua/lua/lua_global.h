#ifndef LUA_GLOBAL_H
#define LUA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LUA_LIBRARY)
#  define LUASHARED_EXPORT Q_DECL_EXPORT
#else
#  define LUASHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LUA_GLOBAL_H
