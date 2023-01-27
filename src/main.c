//
//  main.c
//  integerFFT
//
//  Created by Rafael M Mudafort on 1/30/16.
//  Copyright (c) 2016 Rafael M Mudafort. All rights reserved.
//

#include <stdlib.h>
#include <math.h>
#include "fix_fft.h"
#include "integerFFTutils.h"

int main(int argc, char* argv[])
{
/**************************** config **********************************/
    int N = 512;
    int scaleby = 16384;
/**********************************************************************/

    if ( argc != 3 ) {
        printf( "usage: %s input-file-name output-file-name\n", argv[0] );
        return 1;
    }
    char* fin = argv[1];
    char* fout = argv[2];
    
    int i,j,axis;
    int nLines = getNumberOfLines(fin);
    int log2nLines = fix_log2(nLines);
    int padLines = (1<<(log2nLines+1)) - nLines;
    int dataLines = 1<<(log2nLines+1);

    // FFT size and input signal size
    int log2n = fix_log2(N);
    int loops = dataLines>>log2n;

    // read CSV and pad with 0's until the next power of 2^m
    readCSV(fin, 6, 0, nLines);
    for ( i=0; i<padLines; ++i ) {
        for ( axis=0; axis<6; ++axis ) {
            dataIn[nLines+i][axis] = 0;
        }
    }

    for ( axis=0; axis<6; ++axis ) {
    
        // remove the mean, keep only signal with amp > 1, and scale the data
        int sum = 0, mean = 0;
        for ( j=0; j<nLines; j++ ) {
            sum += dataIn[j][axis];
        }    
        mean = sum>>log2nLines;
        for ( j=0; j<nLines; j++ ) {
            dataIn[j][axis] -= mean;
        }
        int scalefactor = fix_log2(scaleby);
        for ( j=0; j<nLines; j++ ) {
            if (dataIn[j][axis] != 0) {
                dataIn[j][axis] <<= scalefactor;
            }
        }

        // loop over the data in N segments and FFT
        int re[N], im[N], amp[N];
        for ( j=0; j<N; ++j ) { amp[j] = 0; }
        for ( j=0; j<loops; ++j ) {
            for( i=0; i<N; i++ ) {
                im[i] = 0;
                re[i] = dataIn[N*j+i][axis];
            }

            // compute the FFT of the samples
            fix_fft(re,im,log2n,0);

            for (i=1; i<N; i++){
                amp[i] += re[i]*re[i] + im[i]*im[i];
            }
        }

        // average the frequency-bin amplitudes over nonzero sampled dataset
        int nonzeros = 0;
        for ( j=0; j<N; ++j ) {
            if ( amp[j] != 0 ) ++nonzeros;
        }
        for ( j=1; j<N; j++ ) {
            amp[j] = sqrt(amp[j]);
            amp[j] >>= fix_log2(nonzeros);
        }
        
        // store in six-axis array
        for ( j=0; j<N; ++j ) {
            ampFull[j][axis] = amp[j];
        }
    }

    writeCSV(fout, ampFull, 6, N/2);

    return 0;
}