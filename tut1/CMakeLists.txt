cmake_minimum_required (VERSION 3.0)
project (tut1)
add_executable(tut1 tut1.c)

target_link_libraries(tut1 fftw3)
target_link_libraries(tut1 m)
set_property(TARGET tut1 PROPERTY C_STANDARD 99)
