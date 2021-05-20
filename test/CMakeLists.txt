add_executable(wren-bind-test
    wren-bind-test.c
    modules/Foo.c
)

target_link_libraries(wren-bind-test PUBLIC
    wren-bind
    wren
)

add_test(
  NAME wren-bind-test
  COMMAND wren-bind-test
)
