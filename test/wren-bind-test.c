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
  switch (errorType)
  {
    case WREN_ERROR_COMPILE:
    {
      printf("[%s line %d] [Error] %s\n", module, line, msg);
    } break;
    case WREN_ERROR_STACK_TRACE:
    {
      printf("[%s line %d] in %s\n", module, line, msg);
    } break;
    case WREN_ERROR_RUNTIME:
    {
      printf("[Runtime Error] %s\n", msg);
    } break;
  }
}

int main() {
  // Set up the 
  WrenConfiguration config;
  wrenInitConfiguration(&config);
  config.writeFn = &writeFn;
  config.errorFn = &errorFn;

  // Create the VM.
  WrenVM* vm = wrenNewVM(&config);

  // Create the Foo module and bind it.
  WrenModule foo = Foo();
  wrenBind(vm, &foo);

  const char* script = "\
    import \"Foo\" for Foo\n\
    System.print(\"wren-bind-test\")\n\
    System.print(Foo.bar(5, 8))\n\
  ";

  WrenInterpretResult result = wrenInterpret(vm, "main", script);

  switch (result) {
    case WREN_RESULT_COMPILE_ERROR: {
      printf("Compile Error!\n");
    } break;
    case WREN_RESULT_RUNTIME_ERROR: {
      printf("Runtime Error!\n");
    } break;
    case WREN_RESULT_SUCCESS: {
      printf("Success!\n");
    } break;
  }

  wrenFreeVM(vm);

}
