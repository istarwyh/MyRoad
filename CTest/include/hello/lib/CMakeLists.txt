SET(LIBHELLO_SRC Hello.cpp)

#在Mac OS X上.dylib和.so之间的区别在于它们的编译方式。对于.so文件，使用-shared；对于.dylib，使用-dynamiclib
ADD_LIBRARY(IamHello_shared SHARED ${LIBHELLO_SRC})
SET_TARGET_PROPERTIES(IamHello_shared PROPERTIES OUTPUT_NAME "Hello")
SET_TARGET_PROPERTIES(IamHello_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)

ADD_LIBRARY(IamHello_static STATIC ${LIBHELLO_SRC})
SET_TARGET_PROPERTIES(IamHello_static PROPERTIES OUTPUT_NAME "Hello")
#cmake will clean libIamHello.a when building libIamHello.so, because cmake always try to clean the same name of a lib when it build a new lib.
SET_TARGET_PROPERTIES(IamHello_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

INSTALL(FILES Hello.h DESTINATION include/hello)
#dynamic lib:LIBRARY; static lib:ARCHIVE
INSTALL(TARGETS IamHello_shared IamHello_static LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)