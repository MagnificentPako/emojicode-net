cmake_minimum_required(VERSION 3.10)

project(net VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

file(GLOB EMOJIC_DEPEND "src/*.emojic")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(net STATIC src/net.cpp include/httplib.h net_.o)

set_property(TARGET net PROPERTY POSITION_INDEPENDENT_CODE ON)

target_compile_options(net PUBLIC -Wall -Wno-unused-result -Wno-missing-braces -pedantic)

get_filename_component(MAIN_FILE src/net.emojic ABSOLUTE)

add_custom_command(OUTPUT net_.o COMMAND emojicodec -p net -o net_.o -i
    interface.emojii -c ${MAIN_FILE} --color DEPENDS ${EMOJIC_DEPEND})

install(FILES build/libnet.a DESTINATION lib/emojicode/net)
install(FILES build/interface.emojii DESTINATION lib/emojicode/net)
install(FILES include/httplib.h DESTINATION include/)

#install(TARGETS sha
#    ARCHIVE DESTINATION lib/emojicode/sha)

#install(FILES src/interface.emojii
#    DESTINATION lib/emojicode/sha)
