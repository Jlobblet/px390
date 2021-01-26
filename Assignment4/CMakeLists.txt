set(CMAKE_C_FLAGS "-Wall -Werror -std=c99 -I/usr/include/mkl/ -L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl")
add_executable(Assignment4 Assignment4.c)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input.txt DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/coefficients.txt DESTINATION ${CMAKE_BINARY_DIR})
