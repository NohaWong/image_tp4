#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Util.h"
#include <string.h>
#include <math.h>


void print(char *name, float *res, int rows, int cols) {
    FILE *result;
    int i, j;
    result = fopen(name, "w+");
    if (result == NULL) {
        exit(1);
    }
    fprintf(result, "P5\n");
    fprintf(result, "%d %d \n", cols, rows);
    fprintf(result, "%d\n", 255);
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            fprintf(result, "%c", (int) res[i * cols + j]);
        }
    }
    fclose(result);
}

void binom(float **in, int cols, int rows) {
    //to do attention gaymap
    float *graymap=malloc(sizeof(float)*cols*rows);
    memcpy(graymap,*in, sizeof(float)*cols*rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i <= 1 || j <= 1 || i >= rows - 1 || j >= cols -  1) {
                (*in)[i * cols + j] = 0;
            } else {
                (*in)[i * cols + j] = (graymap[(i - 2) * cols + j - 2] + 4 * graymap[(i - 2) * cols + j - 1] +
                                       6 * graymap[(i - 2) * cols + j] + 4 * graymap[(i - 2) * cols + j + 1] +
                                       graymap[(i - 2) * cols + j + 2] + 4 * graymap[(i - 1) * cols + j - 2] +
                                       16 * graymap[(i - 1) * cols + j - 1] + 24 * graymap[(i - 1) * cols + j] +
                                       16 * graymap[(i - 1) * cols + j + 1] + 4 * graymap[(i - 1) * cols + j + 2] +
                                       6 * graymap[(i) * cols + j - 2] + 24 * graymap[(i) * cols + j - 1] +
                                       36 * graymap[(i) * cols + j] + 24 * graymap[(i) * cols + j + 1] +
                                       6 * graymap[(i) * cols + j + 2] + 4 * graymap[(i + 1) * cols + j - 2] +
                                       16 * graymap[(i + 1) * cols + j - 1] + 24 * graymap[(i + 1) * cols + j] +
                                       16 * graymap[(i + 1) * cols + j + 1] + 4 * graymap[(i + 1) * cols + j + 2] +
                                       graymap[(i + 2) * cols + j - 2] + 4 * graymap[(i + 2) * cols + j - 1] +
                                       6 * graymap[(i + 2) * cols + j] + 4 * graymap[(i + 2) * cols + j + 1] +
                                       graymap[(i + 2) * cols + j + 2]) / 256;
            }
        }
    }
    free(graymap);
}



void stretch(float **res, int rows, int cols) {
    int i, j;
    float min, max;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (i == 0 && j == 0) {
                max = (*res)[0];
                min = (*res)[0];
            } else {
                if (max < (*res)[i * cols + j])
                    max = (*res)[i * cols + j];
                if (min > (*res)[i * cols + j])
                    min = (*res)[i * cols + j];
            }
        }
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            //(*res)[i * cols + j]=a*((*res)[i * cols + j])+b;
            (*res)[i * cols + j] = ((float) (*res)[i * cols + j] - min) * 255.0 / (max - min);
        }
    }
}

float *square(float *in, int rows, int cols) {
    int i;
    float *res;
    res = (float *) malloc(sizeof(float) * cols * rows);
    for (i = 0; i < rows * cols; i++) {
        res[i] = in[i] * in[i];
    }
    return res;
}

float *mult(float *in1, float *in2, int rows, int cols) {
    int i;
    float *res;
    res = (float *) malloc(sizeof(float) * cols * rows);
    for (i = 0; i < rows * cols; i++) {
        res[i] = in1[i] * in2[i];
    }
    return res;
}

float *HarrisFunction(float *x2, float *y2, float *xy, float alpha, int rows, int cols){
    float *res=malloc(sizeof(float)*cols*rows);
    float det, trace;
    for(int i=0;i<rows*cols;i++){
        det=x2[i]*y2[i]-xy[i]*xy[i];
        trace=x2[i]+y2[i];
        res[i]=det-alpha*trace*trace;
    }
    return res;
}


int main(int argc, char **argv) {
    FILE *ifp;
    int ich1, rows, cols, maxval = 255, i, j;
    gray *graymap;
    float *res_x, *res_y, *magn;
    if (argc != 7) {
        printf("fichierSource , X, Y, magnitude, Harris, alpha pour calc Harris");
        exit(1);

    }


    ifp = fopen(argv[1], "r");

    if (ifp == NULL) {
        printf("error in opening file %s\n", argv[1]);
        exit(1);
    }


    ich1 = getc(ifp);
    if (ich1 == EOF) {
        exit(1);
    }
    ich1 = getc(ifp);
    if (ich1 == EOF) {
        exit(1);
    }
    cols = pm_getint(ifp);
    rows = pm_getint(ifp);
    maxval = pm_getint(ifp);
    maxval++;


    graymap = (gray *) malloc(cols * rows * sizeof(gray));
    res_x = (float *) malloc(cols * rows * sizeof(float));
    res_y = (float *) malloc(cols * rows * sizeof(float));
    magn = (float *) malloc(cols * rows * sizeof(float));


    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            graymap[i * cols + j] = pm_getrawbyte(ifp);
        }
    }

    double x, y;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if ((i == 0) || (i == rows - 1) || (j == 0) || (j == cols - 1)) {
                res_x[i * cols + j] = 0;
                res_y[i * cols + j] = 0;
            } else {
                //printf("%i\n",graymap[i * cols + j]);
                res_x[i * cols + j] = (-1 * graymap[(i - 1) * cols + j - 1] - 2 * graymap[i * cols + j - 1] -
                                       1 * graymap[(i + 1) * cols + j - 1] + 1 * graymap[(i - 1) * cols + j + 1] +
                                       2 * graymap[i * cols + j + 1] + 1 * graymap[(i + 1) * cols + j + 1]) / 4;
                res_y[i * cols + j] = (-1 * graymap[(i - 1) * cols + j - 1] - 2 * graymap[(i - 1) * cols + j] -
                                       1 * graymap[(i - 1) * cols + j + 1] + 1 * graymap[(i + 1) * cols + j - 1] +
                                       2 * graymap[(i + 1) * cols + j] + 1 * graymap[(i + 1) * cols + j + 1]) / 4;


            }
        }
    }


    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {

            x = (double) res_x[i * cols + j];
            x = pow((double) x, (double) 2);

            y = (double) res_y[i * cols + j];
            y = pow((double) y, (double) 2);

            x = x + y;

            magn[i * cols + j] = (char) sqrt((double) x);
        }
    }
    stretch(&res_x, rows, cols);
    stretch(&res_y, rows, cols);
    stretch(&magn, rows, cols);
    print(argv[2], res_x, rows, cols);
    print(argv[3], res_y, rows, cols);
    print(argv[4], magn, rows, cols);

    //find the matrix squares
    float *res_X2=square(res_x,rows,cols);
    float *res_Y2=square(res_y,rows,cols);
    float *res_XY=mult(res_x,res_y,rows,cols);

    //filter
    binom(&res_X2,rows,cols);
    binom(&res_Y2,rows,cols);
    binom(&res_XY,rows,cols);

    float *harris=HarrisFunction(res_X2,res_Y2,res_XY,atoi(argv[6]),rows,cols);
    //stretch(&harris,rows,cols);
    print(argv[5],harris,rows,cols);


    return 0;
}