// 3つのマイクの音量比を計算する
// 最大の音量を100とする
#include <stdio.h>
#include <stdlib.h>

int maximum(int a, int b, int c) {
  if(a < b) {
    if(b < c) return c;
    else return b;
  } else {
    if(a < c) return c;
    else return a;
  }
}

int main(int argc, char **argv) {
  FILE *fpin, *fpout;
  unsigned long m1, m2, m3, max;
  double t, r1, r2, r3;

  if(argc != 3) {
    printf("Usage: %s <in-file> <out-file>\n", argv[0]);
    exit(1);
  }

  fpin = fopen(argv[1], "r");
  if(fpin == NULL) {
    printf("File not found.\n");
    exit(1);
  }

  fpout = fopen(argv[2], "w");

  while(fscanf(fpin, "%lf %lu %lu %lu", &t, &m1, &m2, &m3) != EOF) {
    max = maximum(m1, m2, m3);
    r1 = (double)m1 / (double)max * 100;
    r2 = (double)m2 / (double)max * 100;
    r3 = (double)m3 / (double)max * 100;
    fprintf(fpout, "%lf %lf %lf %lf\n", t, r1, r2, r3);
  }
  fclose(fpin);
  fclose(fpout);
  return 0;
}
