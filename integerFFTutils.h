//
//  integerFFTutils.h
//  integerFFT
//
//  Created by Rafael M Mudafort on 1/30/16.
//  Copyright (c) 2016 Rafael M Mudafort. All rights reserved.
//

#ifndef __integerFFT__integerFFTutils__
#define __integerFFT__integerFFTutils__

#include <stdio.h>

int dataIn[16384][6];
int ampFull[16384][6];

int fix_log2(int n);
int getNumberOfLines(char* fin);
int readCSV(char* fin, int firstDim, int startline, int endline);
int writeCSV(char* fout, int array[][6], int firstDim, int secondDim);

#endif /* defined(__integerFFT__integerFFTutils__) */