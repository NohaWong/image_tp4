#include <stdlib.h>
#include <stdio.h>
#include "Util.h"
#include <string.h>
#include <math.h>

int main(int argc,char **argv){
	FILE *ifp,*result;
	int ich1, rows, cols, maxval=255,i,j;
	gray *graymap,*res_x,*res_y,*magn;

	if(argc != 5){
		printf("apprend a compter");
		exit(1);

	}


	ifp = fopen(argv[1],"r");

	if (ifp == NULL) {
      printf("error in opening file %s\n", argv[1]);
      exit(1);
    }
    
    
    ich1 = getc( ifp );
    if(ich1==EOF){
    	exit(1);
    }
    ich1 = getc( ifp );
    if(ich1==EOF){
    	exit(1);
    }
    cols = pm_getint( ifp );
    rows = pm_getint( ifp );
    maxval = pm_getint( ifp );
    maxval++;
   

    graymap = (gray *) malloc(cols * rows * sizeof(gray));
    res_x = (gray *) malloc(cols * rows * sizeof(gray));
    res_y = (gray *) malloc(cols * rows * sizeof(gray));
    magn = (gray *) malloc(cols * rows * sizeof(gray));

    int x,y,z;
    z=pow(5,4);
    z++;

	for(i=0; i < rows; i++){
		for(j=0; j < cols ; j++){
			graymap[i * cols + j] = pm_getrawbyte(ifp) ;
		}
	}
	

	for(i=0; i < rows; i++){
		for(j=0; j < cols ; j++){
			if((i==0)||(i==rows-1)||(j==0)||(j==cols-1)){
				res_x[i*cols+j]=0;
				res_y[i*cols+j]=0;
			}
			else{
				//printf("%i\n",graymap[i * cols + j]);
				res_x[i * cols + j] =(-1*graymap[(i-1) * cols + j-1]-2*graymap[i * cols + j-1]-1*graymap[(i+1) * cols + j-1]+1*graymap[(i-1) * cols + j+1]+2*graymap[i * cols + j+1]+1*graymap[(i+1) * cols + j+1])/4;
				res_y[i * cols + j] =(-1*graymap[(i-1) * cols + j-1]-2*graymap[(i-1) * cols + j]-1*graymap[(i-1) * cols + j+1]+1*graymap[(i+1) * cols + j-1]+2*graymap[(i+1) * cols + j]+1*graymap[(i+1) * cols + j+1])/4;
				x=(int)res_x[i * cols + j];
				x=pow(x,2);
				y=(int)res_y[i * cols + j];
				y=pow(y,2);
				x=x+y;
				magn[i*cols+j]=sqrt(x);
			}
		}
	}

	result = fopen(argv[2],"w");
	if(result == NULL){
		exit(1);
	}
	fprintf(result, "P5\n");
	fprintf(result,"%d %d \n", cols, rows);
	fprintf(result, "%d\n",255);
	for(i=0; i < rows; i++){
		for(j=0; j < cols ; j++){
			fprintf(result, "%c",abs(res_x[i * cols + j]));
			//printf("%i\n",res_x[i * cols + j]);
		}
	}

	fclose(result);
	result = fopen(argv[3],"w");
	if(result == NULL){
		exit(1);
	}
	fprintf(result, "P5\n");
	fprintf(result,"%d %d \n", cols, rows);
	fprintf(result, "%d\n",255);
	for(i=0; i < rows; i++){
		for(j=0; j < cols ; j++){
			fprintf(result, "%c",abs(res_y[i * cols + j]));
		}
	}

	fclose(result);
	result = fopen(argv[4],"w");
	if(result == NULL){
		exit(1);
	}
	fprintf(result, "P5\n");
	fprintf(result,"%d %d \n", cols, rows);
	fprintf(result, "%d\n",255);
	for(i=0; i < rows; i++){
		for(j=0; j < cols ; j++){
			fprintf(result, "%c",magn[i * cols + j]);
		}
	}
	// fprintf(result, "%s",res);
	fclose(result);

	return 0;
}