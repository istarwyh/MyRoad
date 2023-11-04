SET(SRC main.cpp)

#tell system how where to find .h file, equal to the '-I' in gcc
INCLUDE_DIRECTORIES(/usr/local/include/hello)
ADD_EXECUTABLE(CTest ${SRC})
#tell system where to find a third-party lib, equal to the '-L' in gcc
LINK_LIBRARIES(/usr/local/lib)
#tell system where to find a third-party lib, equal to the '-l' in gcc
TARGET_LINK_LIBRARIES(CTest IamHello)