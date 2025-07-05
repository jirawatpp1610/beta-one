cmake_minimum_required(VERSION 3.29)
project(beta_one CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)   # Auto‑export for DLL
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# -------- header‑only helpers --------
add_library(core_headers INTERFACE)
target_include_directories(core_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# -------- trend module (Wave‑Surf) --------
add_library(trend_core SHARED cpp/trend_core.cpp)
target_link_libraries(trend_core PRIVATE core_headers)

# -------- sideway module (Mean‑Revert) --------
add_library(sideway_core SHARED cpp/sideway_core.cpp)
target_link_libraries(sideway_core PRIVATE core_headers)

# -------- optional: GoogleTest (stub placeholders) --------
# enable_testing()
# add_subdirectory(tests)  # future use