# integerFFT
This is a fixed point FFT program finding the frequency domain of a six-axis csv data file.
Included are:
- C-based library and main executable
- Rust-based library and pre-calculated sine lookup table

## C Library
This library reads a 6-axis csv data file. The build is configured with the included CMake
project resulting in an executable called `integerFFT`, as shown below.

```bash
mkdir build && cd build
cmake ..
make
```

### Usage
```bash
./integerFFT input-file-name output-file-name
```

### Config

- **N**: controls the window size for the main fft utility. the number of bins in the results are N/2.
- **scaleby**: controls a scale factor to keep small numbers from going to 0 in the integer division.

## Rust Library
This portion of the code was created by Allan Milbourne and given to me via email. I've
included it here with Allan's permission so that others may benefit from it.
All credit goes to Allan for the development and testing of this code.

Notes:
- The code was optimized for use in embedded applications ( `#[no-std]` )
- By making the sin array a constant means that it will be stored in program memory and not in RAM
- The sin array values are halved to eliminate this being done many times within the fft loop (lines 94 & 95).

During testing of the `int_fft` code it was found that if the "target" peak frequency is kept in
the top quarter of the resulting spectrum by varying the sample rate, then the round off error
inherent in the integer calculations can be kept below 0.01% when interpolating to the precise
frequency, in the bottom quarter of spectrum the error can be of the order of 10%.

### Code Structure
- `int_fft.rs` is the main struct file
- `int_fft/sin_array.rs` is the pre-calculated sine lookup table

### Usage
From main.rs or lib.rs located in the `...\src` directory:

```rust
// link the module
mod int_fft;
use int_fft::{IntFFT, FFT_SIZE, FFT_SIZE_2};

// define a mutable data array for passing data to/from the IntFFT struct
let mut rdata: [i16; FFT_SIZE] = [0; FFT_SIZE];
// define the IntFFT structure
let mut ifft = IntFFT::new();

// fill the rdata array
..........

// if required remove dc component, this is useful for all +ve data from an ADC
IntFFT::remove_dc(&mut rdata);

// run the FFT
ifft.int_fft(&mut rdata).unwrap();

// the returned spectrum is now in the first half rdata[] array eg:
let spectrum = &rdata[0..FFT_SIZE_2];
```
