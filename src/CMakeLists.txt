
set(SRC_FILES button.cpp ambientplayer.cpp playerthread.cpp randomtrigger.cpp randomunit.cpp preset_parser.cpp utils.cpp mainscreen.cpp mainlogic.cpp)

add_library(sdlgame_lib STATIC ${SRC_FILES})
target_link_libraries(sdlgame_lib spdlog)
target_include_directories(sdlgame_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${LIBXML2_INCLUDE_DIR} ${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR})
target_link_libraries(sdlgame_lib Threads::Threads ${CMAKE_THREAD_LIBS_INIT} ${SDL2_IMAGE_LIBRARY} ${SDL2_MIXER_LIBRARY} ${SDL2_LIBRARY} ${LIBXML2_LIBRARIES})# SDL2_mixer SDL2_image)

add_executable(sdlgame game.cpp)
target_link_libraries(sdlgame sdlgame_lib)
