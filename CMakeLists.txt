cmake_minimum_required (VERSION 2.6)
set(PROJECT http_downloader)
set(CMAKE_C_FLAGS "-std=c99 -Wall -O3")

include_directories(/usr/local/include /usr/include/libxml2)

set(SOURCES main.c getXmlFields.c fpath.c download.c)

add_executable(${PROJECT} ${SOURCES})

target_link_libraries(${PROJECT} -lxml2 -lcurl -pthread)