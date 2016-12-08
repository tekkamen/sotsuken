/* 3つのエネルギーからノルムを計算し, 一定時間ごとに積算する */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(int argc, char **argv) {
  FILE *fpin, *fpout;
  int i;
  unsigned long ch1, ch2, ch3, sum;
  double sec, norm, itvl;
  clock_t start, end;

  if(argc != 3) {
    printf("Usage: %s <infile> <outfile>\n", argv[0]);
    exit(1);
  }

  fpin = fopen(argv[1], "rb");
  if(fpin == NULL) {
    printf("%s not found.\n", argv[1]);
    exit(1);
  }

  fpout = fopen(argv[2], "w");

  start = clock();
  sum = 0;
  itvl = 600.0;
  i = 1;
  while(fread((void *)&sec, sizeof(double), 1, fpin) > 0) {
    while(sec > i * itvl) {
      fprintf(fpout, "%d %d\n", (int)(i * itvl / 60), 0);
      i++;
    }

    fseek(fpin, -8L, SEEK_CUR);
    do {
      if(fread((void *)&sec, sizeof(double), 1, fpin) <= 0) break;
      fread((void *)&ch1, sizeof(long), 1, fpin);
      fread((void *)&ch2, sizeof(long), 1, fpin);
      fread((void *)&ch3, sizeof(long), 1, fpin);
      norm = sqrt((double)(ch1 * ch1 + ch2 * ch2 + ch3 * ch3));
      sum += (unsigned long)norm;
    } while(sec <= i * itvl);

    fprintf(fpout, "%d %lu\n", (int)(i * itvl / 60), sum);
    sum = 0;
    i++;
  }
  end = clock();
  printf("process time: %ld[usec]\n", (long)(end - start));
  printf("process time: %ld[sec]\n", (long)(end - start) / CLOCKS_PER_SEC);

  fclose(fpin);
  fclose(fpout);
  return 0;
}
