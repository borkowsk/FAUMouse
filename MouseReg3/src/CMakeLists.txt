# This file is a part of MouseReg3
# ---------------------------------------------------
# @author Wojciech Borkowski <wborkowsk@gmail.com>
# @footnote Technical mail: <wborkowski@uw.edu.pl>
# @copyright 2020
# @version 0.03
#
#  See CURRENT licence file!
#

cmake_minimum_required(VERSION 2.8)

project(MouseReg3)

set( SRCPATH "./") #Directory where this CMakeList.txt is put!

FIND_PACKAGE( Threads REQUIRED)
FIND_PACKAGE( Boost 1.58 COMPONENTS system program_options filesystem regex locale REQUIRED )
SET(Boost_USE_MULTITHREADED ON)

cmake_policy(SET CMP0015 NEW)
link_directories("${SRCPATH}/../fasada-core/")

include_directories(
                    "${SRCPATH}"
                    "${SRCPATH}/../fasada-core/lib/fasada/"
                    "${SRCPATH}/../fasada-core/lib/fasada/tree/"
                    "${SRCPATH}/../fasada-core/lib/facebookspec/"
                    "${SRCPATH}/../fasada-core/tools/"
                     ${Boost_INCLUDE_DIR}
                    )

#How to make version.h from GIT versioning?
#See: https://gist.github.com/rkitover/3fe24bbf8f3a0e32a19259aca384680a

add_executable( regmouse3
    #"${SRCPATH}../fasada-core/lib/fasada/http/URLparser.cpp" #For DEBUG only
    "${SRCPATH}/regmouse3_main.cpp"
                )

if(NOT DEFINED DISTRIB_ID )
    set(DISTRIB_ID "UBUNTU" )
endif(NOT DEFINED DISTRIB_ID)

if(NOT DEFINED DISTRIB_RELEASE )
    set(DISTRIB_RELEASE 0.03 )
endif(NOT DEFINED DISTRIB_RELEASE)

add_definitions( "-DFAUMOUSEVERSION=\"${FAUMOUSEVERSION}\"" 
                 "-DDISTRIB_ID=${DISTRIB_ID}"  
                 "-DDISTRIB_RELEASE=${DISTRIB_RELEASE}" 
                )

target_compile_options( regmouse3      
                        PRIVATE "-L\"${SRCPATH}/../fasada-core/\"" 
                        "${CMAKE_CXX_FLAGS}" "-std=c++11" 
                )

target_link_libraries(regmouse3  LINK_PUBLIC
    fasada
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    magic
    rt
    )

