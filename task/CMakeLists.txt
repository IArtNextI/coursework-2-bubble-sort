add_executable(gen
    source/gen.cpp
)

target_link_libraries(gen PRIVATE
    testlib
    scoring
)
