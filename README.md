# wren-bind

Provides a registry to easily bind multiple compiled Wren modules.

## Usage

1. Define your modules with the `WrenModule` struct...

    ``` c
    #include "wren-bind.h"

    void FooBar(WrenVM* vm) {
      double a = wrenGetSlotDouble(vm, 1);
      double b = wrenGetSlotDouble(vm, 2);
      wrenSetSlotDouble(vm, 0, a + b);
    }

    WrenForeignMethodFn FooModuleMethod(WrenVM* vm, const char* module,
        const char* className, bool isStatic, const char* signature)
    {
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
    ```

2. Register the module to the VM using the `wrenBind()` method...
    ``` c
    WrenVM* vm = wrenNewVM(&config);

    WrenModule foo = Foo();
    wrenBind(vm, &foo);
    ```

3. Once bound, you'll be able to use the Foo module from Wren...
    ``` js
    import "Foo" for Foo
    System.print(Foo.bar(5, 8))
    ```

## Notes

- Overrides `vm.bindForeignMethodFn` to manage multiple modules for foreign methods
- Overrides `vm.bindForeignClassFn` to manage multiple modules for classes
- Stores module data within `vm.config.userData`

## License

*wren-bind* is licensed under the [MIT License](LICENSE).
