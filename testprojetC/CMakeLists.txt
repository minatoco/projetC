cmake_minimum_required(VERSION 3.23)
project(test123 C)

set(CMAKE_C_STANDARD 99)

include_directories("C:/wamp64/bin/mariadb/mariadb10.3.12/include")
link_directories("C:/wamp64/bin/mariadb/mariadb10.3.12/lib")
link_libraries(libmariadb)
add_executable(test123 main.c)

target_link_libraries(test123 libmariadb)

