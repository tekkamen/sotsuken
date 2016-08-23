#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv) {
  FILE *fp;
  double x, xm, ym, r, A, B, C;
  int N, i, y;

  if(argc != 2) {
    printf("Usage: %s <file>\n", argv[0]);
    exit(1);
  }

  fp = fopen(argv[1], "r");
  if(fp == NULL) {
    printf("%s not found\n", argv[1]);
    exit(1);
  }

  //平均を求める
  //rewind(fp);
  N = 0;
  xm = ym = 0.0;
  while(fscanf(fp, "%lf %d", &x, &y) != EOF) {
    xm += x;
    ym += (double)y;
    N++;
  }
  xm /= (double)N;
  ym /= (double)N;

  //printf("x mean = %lf\n", xm);
  //printf("y mean = %lf\n", ym);

  //相関係数を求める
  rewind(fp);
  A = B = C = 0.0;
  for(i = 0; i < N; i++) {
    fscanf(fp, "%lf %d", &x, &y);
    A += (x - xm) * (y - ym);
    B += (x - xm) * (x - xm);
    C += (y - ym) * (y - ym);
  }
  r = A / sqrt(B * C);
  printf("%lf\n", r);

  fclose(fp);

  return 0;
}
