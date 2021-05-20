#include <string.h>

#include "wren-bind.h"

void FooBar(WrenVM* vm) {
  double a = wrenGetSlotDouble(vm, 1);
  double b = wrenGetSlotDouble(vm, 2);
  wrenSetSlotDouble(vm, 0, a + b);
}

WrenForeignMethodFn FooModuleMethod(WrenVM* vm, const char* module,
    const char* className, bool isStatic, const char* signature) {
  if (strcmp(className, "Foo") == 0) {
    if (isStatic && strcmp(signature, "bar(_,_)") == 0) {
      return FooBar;
    }
  }

  return NULL;
}

WrenModule Foo() {
  WrenModule module;
  module.name = "Foo";
  module.code = "\n\
    class Foo {\
      foreign static bar(a, b)\
    }\
  ";
  module.methodFunction = FooModuleMethod;
  module.classFunction = NULL;
  return module;
}
