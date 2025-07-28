cmake_minimum_required(VERSION 3.10)
project(DistributedFileSharing)

set(CMAKE_CXX_STANDARD 17)

# Find and link Poco libraries
find_package(Poco REQUIRED COMPONENTS Foundation Net Util Data DataSQLite)

include_directories(${Poco_INCLUDE_DIRS})

add_executable(file_sharing
    src/main.cpp
    src/server.cpp
    src/client.cpp
    src/database.cpp
)

target_link_libraries(file_sharing ${Poco_LIBRARIES})
