#include <string.h>

#include "wren.h"
#include "wren_primitive.h"

#include "wren-bind.h"

WrenForeignMethodFn WrenBindForeignMethod(WrenVM* vm, const char* module,
    const char* className, bool isStatic, const char* signature)
{
  WrenModule** modules = (WrenModule**)vm->config.userData;
  for (int i = 0; i < WREN_BIND_MAX_MODULES; i++) {
    WrenModule* currentModule = modules[i];
    if (currentModule != NULL) {
      if (strcmp(module, currentModule->name) == 0) {
        if (currentModule->methodFunction != NULL) {
          return currentModule->methodFunction(vm, module, className, isStatic, signature);
        }
      }
    }
  }

  return NULL;
}

WrenForeignClassMethods WrenBindForeignClass(WrenVM* vm, const char* module,
    const char* className)
{
  WrenModule** modules = (WrenModule**)vm->config.userData;
  for (int i = 0; i < WREN_BIND_MAX_MODULES; i++) {
    WrenModule* currentModule = modules[i];
    if (currentModule != NULL) {
      if (strcmp(module, currentModule->name) == 0) {
        if (currentModule->classFunction != NULL) {
          return currentModule->classFunction(vm, module, className);
        }
      }
    }
  }

  WrenForeignClassMethods empty;
  empty.allocate = NULL;
  empty.finalize = NULL;
  return empty;
}

bool wrenBind(WrenVM* vm, WrenModule* module) {
  if (!vm->config.userData) {
    vm->config.userData = malloc(sizeof(WrenModule*) * WREN_BIND_MAX_MODULES);

    // Make sure it's empty data.
    WrenModule** modules = (WrenModule**)vm->config.userData;
    for (int i = 0; i < WREN_BIND_MAX_MODULES; i++) {
      modules[i] = NULL;
    }
  }

  vm->config.bindForeignMethodFn = WrenBindForeignMethod;
  vm->config.bindForeignClassFn = WrenBindForeignClass;

  WrenModule** modules = (WrenModule**)vm->config.userData;
  for (int i = 0; i < WREN_BIND_MAX_MODULES; i++) {
    if (modules[i] == NULL) {
      modules[i] = (void*)module;
      wrenInterpret(vm, module->name, module->code);
      return true;
    }
  }

  return false;
}
