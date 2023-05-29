add_executable(annealing
    annealing.cpp
)

add_executable(random
    random.cpp
)

add_executable(special
    special_case.cpp
)

add_executable(full
    full.cpp
)

target_link_libraries(annealing
    scoring
)

target_link_libraries(full
    scoring
)
