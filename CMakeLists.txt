cmake_minimum_required(VERSION 3.6)

### MACRO for sort file inside a project in VS
MACRO(SOURCE_GROUP_BY_FOLDER target)
  SET(SOURCE_GROUP_DELIMITER "/")
  SET(last_dir "")
  SET(files "")
  FOREACH(file ${${TARGET}_SOURCES})
    file(RELATIVE_PATH relative_file "${PROJECT_SOURCE_DIR}" ${file})
    GET_FILENAME_COMPONENT(dir "${relative_file}" PATH)
    IF (NOT "${dir}" STREQUAL "${last_dir}")
      IF (files)
        SOURCE_GROUP("${last_dir}" FILES ${files})
      ENDIF (files)
      SET(files "")
    ENDIF (NOT "${dir}" STREQUAL "${last_dir}")
    SET(files ${files} ${file})
    SET(last_dir "${dir}")
  ENDFOREACH(file)
  IF (files)
    SOURCE_GROUP("${last_dir}" FILES ${files})
  ENDIF (files)
ENDMACRO(SOURCE_GROUP_BY_FOLDER)

set(TARGET rendererApp)

project(${TARGET})

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})

# Turn on the ability to create folders to organize projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Qt stuff
set(CMAKE_PREFIX_PATH "C:\\Qt\\5.6\\msvc2015_64")
set(CMAKE_AUTORCC ON)

# As compiled ui files are generated in the binary dir, tell CMake to always look for includes there
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# GLM and stb_image includes
include_directories(external/glm)
include_directories(external/stb_image)

# Qt library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)

# OpenGL library
#find_package(OpenGL REQUIRED)

# Glad
add_subdirectory(external/glad ${EXECUTABLE_OUTPUT_PATH}/external/glad)
include_directories(external/glad/include)
link_directories(${EXECUTABLE_OUTPUT_PATH}/external/glad)

# Compile *.ui files into ui_XXXX.h C++ headers
QT5_WRAP_CPP(UIS_MOC "${CMAKE_SOURCE_DIR}/src/ui/appWindow.h")
QT5_WRAP_UI(UIS_HDRS "${CMAKE_SOURCE_DIR}/src/ui/appWindow.ui")

# Specify the additional include directories for the project
include_directories(
  "src/"
  "src/ui/"
  "src/renderer/"
  "src/primitives/"
)

# Add the sources using GLOB utility
file(GLOB ${TARGET}_SOURCES 
  "src/*.cpp" 
  "src/ui/*.cpp"
  "src/renderer/*.cpp"
  "src/primitives/*.cpp"
)

add_executable(${TARGET} ${${TARGET}_SOURCES} ${UIS_MOC} ${UIS_HDRS})

SOURCE_GROUP_BY_FOLDER(${TARGET})

# TODO: this has been deprecated, should use target_link_libraries(... IMPORTED Qt::...)
#qt5_use_modules(${TARGET} Core Gui Widgets OpenGL)
qt5_use_modules(${TARGET} Core Gui Widgets)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET})
set_target_properties(${TARGET} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

target_link_libraries(
  ${TARGET}
  ${QT_LIBRARIES}
  #${OPENGL_LIBRARIES}
  glad
)
