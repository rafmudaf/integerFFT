cmake_minimum_required(VERSION 3.20)
project(integerFFT VERSION 0.1.0)

set(sources
    fix_fft.c
    integerFFTutils.c
    main.c
)
add_executable(${PROJECT_NAME} ${sources})
