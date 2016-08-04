#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  FILE *fpin, *fpout;
  short int sitmp;

  if(argc != 3) {
    printf("Usage: %s <infile.raw> <outfile.txt>\n", argv[0]);
    exit(1);
  }

  fpin = fopen(argv[1], "rb");
  if(fpin == NULL) {
    printf("%s not found.\n", argv[1]);
    exit(1);
  }

  fpout = fopen(argv[2], "w");
  int i = 0;
  while(i < 44100) {
    fread(&sitmp, sizeof(short), 1, fpin);
    fprintf(fpout, "%d\n", sitmp);
    i++;
  }
  fclose(fpin);
  fclose(fpout);

  return 0;
}

