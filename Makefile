# Build integerFFT
# includes:
# main.c
# fix_fft.c
# fix_fft.h
# integerFFTutils.c
# integerFFTutils.h
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
CC = gcc
CFLAGS  = -g -Wall

default: integerFFT

integerFFT:  fix_fft.o integerFFTutils.o main.o
	$(CC) $(CFLAGS) -o integerFFT fix_fft.o integerFFTutils.o main.o

fix_fft.o:  fix_fft.c fix_fft.h
	$(CC) $(CFLAGS) -c fix_fft.c

integerFFTutils.o:  integerFFTutils.c integerFFTutils.h
	$(CC) $(CFLAGS) -c integerFFTutils.c

main.o:  main.c
	$(CC) $(CFLAGS) -c main.c

clean: 
	$(RM) integerFFT *.o *~