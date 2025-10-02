cmake_minimum_required(VERSION 3.10)
project(Hellapagos)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")

add_subdirectory(server)
add_subdirectory(hellapagos)

# Pokretanje servera i 4 klijenta
add_custom_target(run_all
    COMMAND ${CMAKE_SOURCE_DIR}/server/build/Desktop_Qt_6_7_3-Debug/server &
    COMMAND ${CMAKE_SOURCE_DIR}/hellapagos/build/Desktop_Qt_6_7_3-Debug/hellapagos &
    COMMAND ${CMAKE_SOURCE_DIR}/hellapagos/build/Desktop_Qt_6_7_3-Debug/hellapagos &
    COMMAND ${CMAKE_SOURCE_DIR}/hellapagos/build/Desktop_Qt_6_7_3-Debug/hellapagos &
    COMMAND ${CMAKE_SOURCE_DIR}/hellapagos/build/Desktop_Qt_6_7_3-Debug/hellapagos
    DEPENDS server hellapagos
    COMMENT "Pokrećem server i 4 klijenta"
)
