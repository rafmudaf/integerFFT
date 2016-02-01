//
//  integerFFTutils.c
//  integerFFT
//
//  Created by Rafael M Mudafort on 1/30/16.
//  Copyright (c) 2016 Rafael M Mudafort. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "integerFFTutils.h"

int fix_log2(int n) {
    int m = 0;
    while (n >>= 1) ++m;
    return m;
}

int getNumberOfLines(char* fin) {
    FILE *fp = fopen(fin,"r");
    if (!fp) {                                                            
        printf("\nFrom getNumberOfLines: unable to open file\n");
        return 1;
    }
    
    int maxchar = 80;
    char str[maxchar];
    int numlines = 0;
    
    // read the header line
    fgets(str, maxchar, fp);
    
    // count the number of lines of data in this csv
    while(1) {        
        if (fgets(str, maxchar, fp)!=NULL) ++numlines;
        else break;
    }
    return numlines;
}

int readCSV(char* fin, int firstDim, int startline, int endline) {
    FILE *fp = fopen(fin,"r");
    if (!fp) {                                                            
        printf("\nFrom readCSV: unable to open file\n");
        return 1;
    }
    
    int maxchar = 80;
    char str[maxchar];
    
    // read the header line
    fgets(str, maxchar, fp);
    
    // read in separated data and store accordingly
    const char delimiter[4] = " , ";
    char* token;
    
    if (startline == 0) {
        for (int j=startline; j<endline; ++j) {
            fgets(str, maxchar, fp);
            token = strtok(str, delimiter);     // pre
            token = strtok(NULL, delimiter);    // count
            for ( int i=0; i<firstDim; ++i) {
                dataIn[j][i] = atoi(strtok(NULL, delimiter)); // x1,y1,z1,x2,y2,z2
            }
            token = strtok(NULL, delimiter);    // pst
        }
    }
    else {
        for (int j=0; j<startline; ++j) {
            fgets(str, maxchar, fp);
        }
        for (int j=startline; j<endline; ++j) {
            fgets(str, maxchar, fp);
            token = strtok(str, delimiter);     // pre
            token = strtok(NULL, delimiter);    // count
            for ( int i=0; i<6; ++i) {
                dataIn[j][i] = atoi(strtok(NULL, delimiter)); // x1,y1,z1,x2,y2,z2
            }
            token = strtok(NULL, delimiter);    // pst
        }
    }
    fclose(fp);
    return 0;
}


int writeCSV(char* fout, int array[][6], int firstDim, int secondDim) {
    FILE *fp = fopen(fout,"w");
    if (!fp) {                                                            
        printf("\nFrom writeCSV: unable to open file\n");
        return 1;
    }
    const char delimiter[4] = " , ";
    for (int j=0; j<secondDim; ++j) {
        for (int i=0; i<firstDim; ++i) {
            if (i==firstDim-1) {
                fprintf(fp, "%i\n", array[j][i]);
            }
            else {                    
                fprintf(fp, "%i%s", array[j][i], delimiter);
            }        
        }
    }
    return 0;
}
