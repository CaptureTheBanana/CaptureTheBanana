cmake_minimum_required(VERSION 3.1)

if(BUILD_EDITOR)
    # Find Qt5
    find_package(Qt5Widgets REQUIRED)
    include_directories(${Qt5Widgets_INCLUDES})
    add_definitions(${Qt5Widgets_DEFINITIONS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

    # Generated UI files
    set(EDITOR_MOC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/LayerSettingsDialog.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/LevelBackgroundSettingsDialog.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/LevelSettingsDialog.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/MainWindow.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/ResizeDialog.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/SetupBackgroundDialog.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/SetupDialog.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/TileDialog.hpp
    )

    set(EDITOR_UIS
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/MainWindow.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/LayerSettingsDialog.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/LevelBackgroundSettingsDialog.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/LevelSettingsDialog.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/ResizeDialog.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/SetupBackgroundDialog.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/SetupDialog.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/TileDialog.ui
    )

    set(CMAKE_AUTOMOC OFF)
    include_directories(${CMAKE_BINARY_DIR})
    qt5_wrap_cpp(MOC_SRCS ${EDITOR_MOC_FILES})
    qt5_wrap_ui(UI_HEADERS ${EDITOR_UIS})
endif()

# Setup sources
set(EDITOR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/Main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/SpawnInfo.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/entry/BackgroundListWidgetItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/entry/BackgroundListWidgetItem.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/LayerSettingsDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/LayerSettingsDialog.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/LevelBackgroundSettingsDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/LevelBackgroundSettingsDialog.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/LevelSettingsDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/LevelSettingsDialog.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/MainWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/MainWindow.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/ResizeDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/ResizeDialog.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/SetupBackgroundDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/SetupBackgroundDialog.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/SetupDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/SetupDialog.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/TileDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/gui/TileDialog.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/model/MainWindowModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/model/MainWindowModel.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/scene/GraphicsItems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/scene/GraphicsItems.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/scene/GraphicsScenes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/scene/GraphicsScenes.hpp
)

if(BUILD_EDITOR)
    # Build editor
    add_executable(ctb-editor ${EDITOR_SOURCES} ${MOC_SRCS} ${UI_HEADERS})
    target_link_libraries(ctb-editor Qt5::Widgets ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
    target_link_libraries(ctb-editor common-static parser-static)
    if(MSVC)
        set_target_properties(ctb-editor PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\"")
    else()
        target_link_libraries(ctb-editor m)
    endif()
    target_include_directories(ctb-editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

    #install(TARGETS ctb-editor DESTINATION bin COMPONENT CTB_EDITOR)
else()
    # Show editor source files
    add_custom_target(ctb-editor SOURCES ${EDITOR_SOURCES})
endif()
