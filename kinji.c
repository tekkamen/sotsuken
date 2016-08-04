#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  FILE *fp1, *fp2;
  double a, b;
  double xiyi, xi, yi, xi2;
  double x, y, dtmp;
  int itmp;
  int i, N1, N2, N;
  char buf[256];

  if(argc != 3) {
    printf("Usage: %s <file1> <file2>\n", argv[0]);
    exit(1);
  }

  fp1 = fopen(argv[1], "r");
  if(fp1 == NULL) {
    printf("%s not found\n", argv[1]);
    exit(1);
  }

  fp2 = fopen(argv[2], "r");
  if(fp2 == NULL) {
    printf("%s not found\n", argv[2]);
    exit(1);
  }

  N1 = 0;
  while(fgets(buf, 256, fp1) != NULL) {
    N1++;
  }

  N2 = 0;
  while(fgets(buf, 256, fp2) != NULL) {
    N2++;
  }

  printf("N1 = %d\n", N1);
  printf("N2 = %d\n", N2);

  if(N1 > N2) {
    N = N2;
  } else {
    N = N1;
  }
  printf("N = %d\n", N);
 
  rewind(fp1);
  rewind(fp2);
  xiyi = xi = yi = xi2 = 0.0;
  for(i = 0; i < N; i++) {
    fscanf(fp1, "%d %lf", &itmp, &x);
    fscanf(fp2, "%lf %lf", &dtmp, &y);
    xiyi += x * y;
    xi += x;
    yi += y;
    xi2 += x * x;
  }

  //傾きaを求める
  a = (N * xiyi - xi * yi) / (N * xi2 - (xi * xi));

  //切片bを求める
  b = (xi2 * yi - xiyi * xi) / (N * xi2 - (xi * xi));
  
  printf("a = %lf\n", a);
  printf("b = %lf\n", b);

  fclose(fp1);
  fclose(fp2);

  return 0;
}
