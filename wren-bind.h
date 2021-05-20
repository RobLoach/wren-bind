#ifndef WREN_BIND_H__
#define WREN_UTF8_WREN_BIND_H__H__

#include "wren.h"

#ifndef WREN_BIND_MAX_MODULES
#define WREN_BIND_MAX_MODULES 20
#endif

typedef struct WrenModule {
  const char* name;
  WrenBindForeignMethodFn methodFunction;
  WrenBindForeignClassFn classFunction;
  const char* code;
} WrenModule;

bool wrenBind(WrenVM* vm, WrenModule* module);

#endif // WREN_BIND_H__
