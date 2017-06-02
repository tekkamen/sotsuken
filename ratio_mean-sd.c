//音量比の平均と標準偏差を計算する
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv) {
  FILE *fp;
  int c, i;
  double t, r1, r2, r3, t1, t2, t3;
  double m1, m2, m3, sd1, sd2, sd3;
  double d1, d2, d3;

  if(argc != 2) {
    printf("Usage: %s <file>\n", argv[0]);
    exit(1);
  }

  fp = fopen(argv[1], "r");
  if(fp == NULL) {
    printf("%s not found.\n", argv[1]);
    exit(1);
  }

  t1 = t2 = t3 = 0;
  c = 0;
  while(fscanf(fp, "%lf %lf %lf %lf", &t, &r1, &r2, &r3) != EOF) {
    c++;
    t1 += r1;
    t2 += r2;
    t3 += r3;
  }
  m1 = t1 / c;
  m2 = t2 / c;
  m3 = t3 / c;

  rewind(fp);
  t1 = t2 = t3 = 0.0;
  for(i = 0; i < c; i++) {
    fscanf(fp, "%lf %lf %lf %lf", &t, &r1, &r2, &r3);
    d1 = (r1 - m1) * (r1 - m1);
    d2 = (r2 - m2) * (r2 - m2);
    d3 = (r3 - m3) * (r3 - m3);
    t1 += d1;
    t2 += d2;
    t3 += d3;
  }
  sd1 = sqrt(t1 / c);
  sd2 = sqrt(t2 / c);
  sd3 = sqrt(t3 / c);

  printf("mean: %lf %lf %lf\n", m1, m2, m3);
  printf("SD: %lf %lf %lf\n", sd1, sd2, sd3);

  fclose(fp);
  return 0;
}
