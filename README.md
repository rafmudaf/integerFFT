<h3>integerFFT</h3>
is a fixed point FFT program finding the frequency domain of a six axis csv data file.

<h5>usage</h5>
./integerFFT input-file-name output-file-name

<h5>config</h5>
N: controls the window size for the main fft utility. the number of bins in the results are N/2.
scaleby: controls a scale factor to keep small numbers from going to 0 in the integer division.
