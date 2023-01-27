/// int_fft
/// An integer based FFT
/// Adapted from:  https://github.com/rafmudaf/integerFFT

// include the const Sinewave lookup array
mod sin_array;
use sin_array::{ FFT_SIZE, SIN_VALS };

const LOG2_FFTSIZE: usize = 12;  // log2(4096)
const FFT_SIZE_2: usize = FFT_SIZE >> 1;
const FFT_SIZE_4: usize = FFT_SIZE >> 2;

pub struct IntFFT {
    // Imaginary data buffer for the FFT calcs.
    // Defined here to stop dynamic creation on every int_fft() call,
    // optimisation for embedded use with limited RAM
    di: [i16; FFT_SIZE],
}

impl IntFFT {
    pub fn new() -> Result<IntFFT, u32> {

        Ok( IntFFT {
            di: [0i16; FFT_SIZE],
        })
    }

    /// Subtract the mean, to remove DC component
    /// ??? use on unsigned data eg adc readings
    pub fn remove_dc(data: &mut [i16]) {
        let mut sum: i32 = 0;
        let mut mean: i16 = 0;
        let len = data.len();
        // calc mean
        data.iter_mut().for_each(|m| sum += *m as i32);
        mean = (sum / len as i32) as i16;

        // subtract the mean
        data.iter_mut().for_each(|m| *m -= mean);
    }

    /// zero the imaginary data buffer
    fn zero_di(&mut self) {
        self.di.iter_mut().for_each(|m| *m = 0);
    }

    /// Integer FFT
    /// Data array (dr) MUST be 4096 i16 values, and MUST be mutable
    /// Returns the 'real' values in the lower half of the supplied array (dr)
    /// As this is used to calc the dominant frequency,
    /// then full amplitude is not required (it would only steal cpu cycles).
    ///
    /// Return error status, simplified for embedded [no-std] usage
    ///    0 : No errors
    ///    1 : Invalid data array length
    ///
    pub fn int_fft(&mut self, dr: &mut [i16]) -> usize {
        // # check dr[] size, set size to max n2 fit di[]
        // return max n2/2 result, zero remainder
        // Result<FFT run size, usize error code>
        let n = FFT_SIZE;
        if dr.len() != FFT_SIZE {
            return 1;
        }
        // zero the imaginary data array for this fft calculation
        self.zero_di();

        // set initial var values
        let nn = n - 1;
        let mut mr: usize = 0;
        let mut l: usize;

        // decimation in time - reorder data
        for m in 1..nn {
            l = n >> 1;
            while mr + l > nn {
                l >>= 1;
            }
            mr = (mr & (l - 1)) + l;
            if mr <= m { continue;}
            let tr = dr[m];
            dr[m] = dr[mr];
            dr[mr] = tr;
            // di data not need reordering as it is ALL zeros!
        }

        // transform
        l = 1;
        let mut k = LOG2_FFTSIZE - 1;
        while l < n {
            let istep = l << 1;
            for m in 0..l {
                let mut j = m << k;
                let mut wr = SIN_VALS[j + FFT_SIZE_4];  // >> 1; // not halve here,
                let mut wi = -SIN_VALS[j];              // >> 1; // pre done to SIN_VALS

                for i in (m..n).step_by(istep) {
                    j = i + l;
                    let tr = fix_mpy(wr, dr[j]) - fix_mpy(wi, self.di[j]);
                    let ti = fix_mpy(wr, self.di[j]) + fix_mpy(wi, dr[j]);
                    let qr = dr[i] >> 1;
                    let qi = self.di[i] >> 1;
                    dr[j] = qr - tr;
                    self.di[j] = qi - ti;
                    dr[i] = qr + tr;
                    self.di[i] = qi + ti;
                }
            }
            k -= 1;
            l = istep;
        }

        // dr[] now contains the real components of the calculated spectrum
        // condition dr[] for return to user

        // use absolute vals as this gives smallest error % in frequency interpolation
        for i in 0..FFT_SIZE_2 {
            if dr[i] < 0 { dr[i] = -dr[i]; }
        }

        // zero top half of array as this is not used (ie Nyquist)
        // ?? to save cpu cycles, this can be removed and the upper half just be ignored
        for i in FFT_SIZE_2..FFT_SIZE {
            dr[i] = 0;
        }

        // return : 0 = NO error
        0
    }

}

fn fix_log2(n: i16) -> i16 {
    let mut temp_n = n;
    let mut m = 0_i16;
    while temp_n != 0 {
        m += 1;
        temp_n >>= 1;
    }
    m
}

fn fix_mpy(a: i16, b: i16) -> i16 {
    // shift right one less bit (ie 15-1)
    let c = (a as i32 * b as i32) >> 14;
    // last bit shifted out = rounding bit
    let d = c & 0x01;
    // last shift + rounding bit
    let e = (c >> 1) + d;
    // return the result
    e as i16
}

