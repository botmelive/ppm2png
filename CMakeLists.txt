project(ppm2png)

include_directories("libs/" )

set(SRC_FILES 
ppm2png.cpp
libs/lodepng.cpp
)

add_executable(ppm2png ${SRC_FILES})
