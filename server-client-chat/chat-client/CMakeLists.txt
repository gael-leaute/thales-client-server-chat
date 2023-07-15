# CMakeList.txt : CMake project for chat-client, include source and define
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

project ("chat-client")

# Add source to this project's executable.
add_executable (chat-client "chat-client.cpp" "chat-client.h")

target_link_libraries(chat-client debug "${RPCLIB_PATH}/rpclib/build/Debug/rpc.lib")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET chat-client PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
