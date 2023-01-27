
set(sources
    fix_fft.c
    integerFFTutils.c
    main.c
)

target_sources(${int_fft_exe_name} PRIVATE ${sources})
