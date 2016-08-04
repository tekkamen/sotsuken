#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  FILE *fpin, *fpout;
  double a, b;
  double x, y;
  int tmp;
  
  if(argc != 5) {
    printf("Usage: %s <infile> <outfile> <value a> <value b>\n", argv[0]);
    exit(1);
  }

  fpin = fopen(argv[1], "r");
  if(fpin == NULL) {
    printf("%s not found\n", argv[1]);
    exit(1);
  }

  fpout = fopen(argv[2], "w");  

  a = atof(argv[3]);
  b = atof(argv[4]);

  
  while(fscanf(fpin, "%d %lf", &tmp, &x) != EOF) {
    y = a * x + b;
    fprintf(fpout, "%lf %lf\n", x, y);
  }

  fclose(fpin);
  fclose(fpout);

  return 0;
}
