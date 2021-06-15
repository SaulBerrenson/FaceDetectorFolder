cmake_minimum_required(VERSION 3.3.1)
cmake_policy(VERSION 3.3.1)

project(FaceDetectorFolder)
set(CMAKE_CXX_STANDARD 17)

include(./cmake/windeploy.cmake)


set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(MSVC)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi /Ox /O2")

IF(WIN32)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows")
ENDIF(WIN32)


elseif (CMAKE_COMPILER_IS_GNUCXX OR MINGW OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-dev")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -static-libstdc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lrt -lpthread")
    if (NOT MINGW)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
    endif()
	
endif()


find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON) 
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Quick Core Concurrent Gui Qml Quick QuickControls2)

if (Qt5_FOUND)
    set(QT_LIBRARIES Qt5::Core Qt5::Concurrent Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Quick Qt5::QuickControls2)
endif()








file(GLOB_RECURSE HEADERS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE SOURCES_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE TargetQmls "${CMAKE_CURRENT_SOURCE_DIR}/rsc/*.qml")
file(GLOB_RECURSE TargetResources "${CMAKE_CURRENT_SOURCE_DIR}/rsc/*.qrc")



qt5_add_resources(RCC_SOURCES ${TargetResources})




if(MSVC) 
    add_executable(FaceDetectorFolder   
        WIN32
        ${HEADERS_FILES}
        ${SOURCES_FILES}
        ${RESOURCES}
        ${RCC_SOURCES}
        ${TargetQmls}
        ${TargetResources}
    )
else()
    add_executable(FaceDetectorFolder   
        ${HEADERS_FILES}
        ${SOURCES_FILES}
        ${RESOURCES}
        ${RCC_SOURCES}
        ${TargetQmls}
        ${TargetResources}
    )
endif()


target_link_libraries(FaceDetectorFolder 
Qt5::Quick
Qt5::Core
Qt5::Concurrent
Qt5::Gui
Qt5::Qml
Qt5::Quick
Qt5::QuickControls2
${OpenCV_LIBS}
)

message(STATUS "-----------------------------------------------------------")
message(STATUS "|-> QT: ${Qt5_VERSION} - [${QT_LIBRARIES}]")
message(STATUS "|-> OPENCV: ${OpenCV_VERSION} - [${OpenCV_LIB_COMPONENTS}]")
message(STATUS "-----------------------------------------------------------")



if(MSVC) 
    #Add post build for execution windeploy
    windeployqt(FaceDetectorFolder) 
endif()



