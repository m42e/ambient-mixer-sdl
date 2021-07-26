add_executable(parsing parsing.cpp)
target_link_libraries(parsing sdlgame_lib gmock_main)
add_test(NAME parsing_test COMMAND parsing)
