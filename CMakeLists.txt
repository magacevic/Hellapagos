cmake_minimum_required(VERSION 3.12)
project(Hellapagos)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")

add_subdirectory(server)
add_subdirectory(hellapagos)

# Broj klijenata
set(NUM_CLIENTS 4)

# Custom target koji pokreće server i klijente
add_custom_target(run_game
    COMMAND ${CMAKE_BINARY_DIR}/server/server &
    COMMENT "Pokrećem server..."
)

# Dodaj sve klijente direktno
foreach(i RANGE 1 ${NUM_CLIENTS})
    add_custom_command(TARGET run_game
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Pokrećem klijenta ${i}"
        COMMAND ${CMAKE_BINARY_DIR}/hellapagos/hellapagos --player=${i} &
    )
endforeach()
