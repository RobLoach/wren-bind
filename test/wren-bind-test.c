#include <assert.h>
#include <stdio.h>

#include "wren.h"

#define WREN_BIND_IMPLEMENTATION
#include "wren-bind.h"

#include "modules/Foo.h"

void writeFn(WrenVM* vm, const char* text) {
  printf("%s", text);
}

void errorFn(WrenVM* vm, WrenErrorType errorType,
    const char* module, const int line,
    const char* msg) {
  switch (errorType) {
    case WREN_ERROR_COMPILE:
      printf("[%s line %d] [Error] %s\n", module, line, msg);
      break;
    case WREN_ERROR_STACK_TRACE:
      printf("[%s line %d] in %s\n", module, line, msg);
      break;
    case WREN_ERROR_RUNTIME:
      printf("[Runtime Error] %s\n", msg);
      break;
  }
}

void BarFoo(WrenVM* vm) {
  double a = wrenGetSlotDouble(vm, 1);
  double b = wrenGetSlotDouble(vm, 2);
  wrenSetSlotDouble(vm, 0, a + b);
}

WrenForeignMethodFn BarModuleMethod(WrenVM* vm, const char* module,
    const char* className, bool isStatic, const char* signature) {
  if (strcmp(className, "Bar") == 0) {
    if (isStatic && strcmp(signature, "foo(_,_)") == 0) {
      return BarFoo;
    }
  }

  return NULL;
}

int main() {
  // Set up the
  WrenConfiguration config;
  wrenInitConfiguration(&config);
  config.writeFn = &writeFn;
  config.errorFn = &errorFn;

  // Create the VM.
  WrenVM* vm = wrenNewVM(&config);

  // wrenBind()
  WrenModule foo = Foo();
  assert(wrenBind(vm, &foo));

  // wrenBindModule()
  const char* barCode = "class Bar {\n\
    foreign static foo(a,b)\n\
  }";
  assert(wrenBindModule(vm, "Bar", barCode, BarModuleMethod, NULL));

  const char* script = "\
    import \"Foo\" for Foo\n\
    import \"Bar\" for Bar\n\
    System.print(\"wren-bind-test\")\n\
    System.print(\"Expected: 13\")\n\
    System.print(Foo.bar(5, 8))\n\
    System.print(\"Expected: 23\")\n\
    System.print(Bar.foo(10, 13))\n\
  ";
  WrenInterpretResult result = wrenInterpret(vm, "main", script);
  assert(result == WREN_RESULT_SUCCESS);

  switch (result) {
    case WREN_RESULT_COMPILE_ERROR:
      printf("Compile Error!\n");
      break;
    case WREN_RESULT_RUNTIME_ERROR:
      printf("Runtime Error!\n");
      break;
    case WREN_RESULT_SUCCESS:
      printf("wren-bind-test: Pass\n");
      break;
  }

  wrenFreeVM(vm);

}
