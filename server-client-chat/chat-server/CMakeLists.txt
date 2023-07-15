# CMakeList.txt : CMake project for chat-server, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

#define rpclib relative location
set(RPCLIB_PATH "${CMAKE_SOURCE_DIR}/../../rpclib/")

#add rpclib include directory
include_directories("${RPCLIB_PATH}/rpclib/include/")

project ("chat-server")

# Add source to this project's executable.
add_executable (chat-server "chat-server.cpp" "chat-server.h")

#add_library(rpc STATIC IMPORTED GLOBAL)
#set_property(TARGET chat-server APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
#set_target_properties(chat-server PROPERTIES
#        IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
#        IMPORTED_LOCATION_DEBUG "${RPCLIB_PATH}/rpclib/build/Debug/rpc.lib")

target_link_libraries(chat-server debug "${RPCLIB_PATH}/rpclib/build/Debug/rpc.lib")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET chat-server PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
