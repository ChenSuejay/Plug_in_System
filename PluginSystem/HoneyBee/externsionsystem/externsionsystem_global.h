#ifndef EXTERNSIONSYSTEM_GLOBAL_H
#define EXTERNSIONSYSTEM_GLOBAL_H
#include <qglobal.h>
#if defined(EXTENSIONSYSTEM_LIBRARY)
#  define EXTENSIONSYSTEM_EXPORT Q_DECL_EXPORT
#else
#  define EXTENSIONSYSTEM_EXPORT Q_DECL_IMPORT
#endif

#endif // EXTERNSIONSYSTEM_GLOBAL_H
