#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
  FILE *fp;
  int i;
  double x, y, t, dt;

  fp = fopen("hoge.txt", "w");
  
  t = 0;
  dt = 0.001;
  for(i = 0; i < 1000; i++) {
    x = sin(2 * M_PI * t);
    y = cos(2 * M_PI * t);
    fprintf(fp, "%lf %lf\n", x, y);
    t += dt;
  }
  fclose(fp);
  return 0;
}
