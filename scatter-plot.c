#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  FILE *fpin1, *fpin2, *fpout;
  int hoge, y;
  double fuga, x;

  if(argc != 4) {
    printf("Usage: %s <rainfall data> <sound data> <outfile>\n", 
	   argv[0]);
  }

  if((fpin1 = fopen(argv[1], "r")) == NULL) {
    printf("%s not found\n", argv[1]);
    exit(1);
  }

  if((fpin2 = fopen(argv[2], "r")) == NULL) {
    printf("%s not found\n", argv[2]);
    exit(1);
  }

  if((fpout = fopen(argv[3], "w")) == NULL) {
    printf("file open error\n");
    exit(1);
  }

  while(1) {
    if((fscanf(fpin1, "%d %lf", &hoge, &x)) != EOF) {
      //printf("x = %lf\n", x);
      if((fscanf(fpin2, "%lf %d", &fuga, &y)) != EOF) {
	fprintf(fpout, "%lf %d\n", x, y);
      } else {
	break;
      }
    } else {
      break;
    }
  }
  fclose(fpin1);
  fclose(fpin2);
  fclose(fpout);
  return 0;
}
