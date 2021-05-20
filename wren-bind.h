/**
 * wren-bind: Registry to easily bind multiple compiled Wren modules to a VM.
 *
 * LICENSE: MIT
 *
 * Copyright (c) 2021 Rob Loach (https://robloach.net)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef WREN_BIND_H__
#define WREN_BIND_H__

#include "wren.h"

typedef struct WrenModule {
  const char* name;
  WrenBindForeignMethodFn methodFunction;
  WrenBindForeignClassFn classFunction;
  const char* code;
} WrenModule;

bool wrenBind(WrenVM* vm, WrenModule* module);

#endif // WREN_BIND_H__

#ifdef WREN_BIND_IMPLEMENTATION
#ifndef WREN_BIND_IMPLEMENTATION_ONCE
#define WREN_BIND_IMPLEMENTATION_ONCE

#include <string.h>

#include "wren_primitive.h"

#ifndef WREN_BIND_MAX_MODULES
#define WREN_BIND_MAX_MODULES 20
#endif

WrenForeignMethodFn WrenBindForeignMethod(WrenVM* vm, const char* module,
    const char* className, bool isStatic, const char* signature) {
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
    const char* className) {
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

#endif // WREN_BIND_IMPLEMENTATION_ONCE
#endif // WREN_BIND_IMPLEMENTATION
