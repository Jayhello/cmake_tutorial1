cmake_minimum_required(VERSION 3.5)
project(demo2)

#----------------------------------------
message("hello world")
# set a cmake variable, and echo it
set(ECHO_HELLO "world")
message("hello: ${ECHO_HELLO}")

#----------------------------------------
# nested variable
set(XY_NAME "xiong yu")
set(XY_ADDRESS "china guangzhou")
set(PERSON "XY")

message("${${PERSON}_NAME} lives in ${${PERSON}_ADDRESS}")

#----------------------------------------
# flow control commands
if(WIN32)
    message("This is win32 platform")
else()
    message("This is not win32 platform")
endif()

#----------------------------------------
# foreach example
set(FOR_LIST demo1.cpp demo2.cpp demo3.cpp)
foreach (f ${FOR_LIST})
    message("now is file: " ${f})
endforeach ()
