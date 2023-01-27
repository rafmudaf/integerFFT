cmake_minimum_required(VERSION 3.20)
project(integerFFT VERSION 0.1.0)

set(int_fft_exe_name "integerFFT")

add_executable(${int_fft_exe_name})

add_subdirectory(src)
