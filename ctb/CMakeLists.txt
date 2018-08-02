cmake_minimum_required(VERSION 3.1)

set(CTB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ctb/Main.cpp
)

# Build CaptureTheBanana++
add_executable(CaptureTheBanana ${CTB_SOURCES})
target_link_libraries(CaptureTheBanana ${SDL2_LIBRARY} ${SDL2_IMG_LIBRARY} ${SDL2_MIXER_LIBRARY})
target_link_libraries(CaptureTheBanana ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
target_link_libraries(CaptureTheBanana common-static parser-static engine-static)
target_include_directories(CaptureTheBanana PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CONFIG_DIR}
)
#if(MSVC)
#    set_target_properties(CaptureTheBanana PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
#endif()

install(TARGETS CaptureTheBanana DESTINATION bin COMPONENT CTB)
