#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  FILE *fpin, *fpout;
  int i, t, intvl;
  double rain, sum;
  
  if(argc != 4) {
    printf("Usage: %s <infile> <outfile> interval\n", argv[0]);
    exit(1);
  }

  if((fpin = fopen(argv[1], "r")) == NULL) {
    printf("%s not found\n", argv[1]);
    exit(1);
  }

  if((fpout = fopen(argv[2], "w")) == NULL) {
    printf("file open error\n");
    exit(1);
  }
  
  intvl = atoi(argv[3]);
  sum = 0.0;
  i = 1;
  while(fscanf(fpin, "%d %lf", &t, &rain) != EOF) {
    sum += rain;
    if(t % intvl == 0) {
      //fprintf(fpout, "%d %lf\n", intvl * i, sum);
      fprintf(fpout, "%d %lf\n", intvl * i / 60, sum);
      sum = 0.0;
      i++;
    }
  }
  if(t % intvl != 0) {
    //fprintf(fpout, "%d %lf\n", intvl * i, sum);
    fprintf(fpout, "%d %lf\n", intvl * i / 60, sum);
  }
  fclose(fpin);
  fclose(fpout);
  return 0;
}
