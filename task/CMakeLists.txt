add_executable(gen
    source/gen.cpp
)

target_link_libraries(gen PRIVATE
    testlib
    scoring
)

add_executable(checker
    source/check.cpp
)

target_link_libraries(checker PRIVATE
    testlib
    scoring
)

target_link_libraries(gen PRIVATE
    testlib
    scoring
)

add_subdirectory(solutions)
