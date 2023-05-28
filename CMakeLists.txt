cmake_minimum_required(VERSION 3.16)

project(BubbleSort)

option(SANITIZERS "Build with sanitizers" ON)

if (SANITIZERS)
    add_compile_options(-fsanitize=address -fsanitize=leak -fsanitize=undefined -fno-sanitize-recover)
    add_link_options(-fsanitize=address -fsanitize=leak -fsanitize=undefined -fno-sanitize-recover)
endif()

add_subdirectory(libs)
add_subdirectory(task)

# add_library(scoring
#     scoring.cpp
# )

# add_executable(gen
#     gen.cpp
# )

# target_link_libraries(gen PRIVATE
#     scoring
# )

# add_executable(checker
#     check.cpp
# )

# target_link_libraries(checker PRIVATE
#     scoring
# )

# add_executable(prechecker
#     precheck.cpp
# )

# add_executable(solve
#     solve.cpp
# )