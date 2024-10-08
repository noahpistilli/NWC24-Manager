cmake_minimum_required(VERSION 3.27)
project(WiiConnect24_Downloader)

include_directories(${LIBOGCDIR}/include)
include_directories(${DEVKITPRO}/portlibs/wii/include)
include_directories(${DEVKITPRO}/portlibs/ppc/include)
link_directories(${DEVKITPRO}/portlibs/wii/lib)
link_directories(${DEVKITPRO}/portlibs/ppc/lib)
link_directories(${DEVKITPRO}/libogc/lib/wii)

set(MACHDEP "-DGEKKO -mrvl -mcpu=750 -meabi -mhard-float")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ASM_FLAGS "-x assembler-with-cpp")
set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-unused-function -O2 ${CMAKE_CXX_FLAGS} ${MACHDEP}")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-function -O2 ${CMAKE_CXX_FLAGS} ${MACHDEP} -fdiagnostics-color")
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

add_executable(WiiConnect24_Downloader main.cpp
        tui.cpp
        tui.h
        nwc24dl.cpp
        nwc24dl.h
        utils.cpp
        utils.h
        download.cpp
        download.h
        select.cpp
        select.h
        stats.cpp
        stats.h
        nwc24.h
        nwc24.cpp
)

target_link_libraries(WiiConnect24_Downloader wiiuse bte fat ogc m patcher)