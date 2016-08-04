/* 閾値以下のパワーを0にするプログラム */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  FILE *fpin, *fpout;
  // pow:入力ファイルから読み取ったパワー
  // thr:足切りするパワーの閾値
  double t, pow, thr; 

  if(argc != 4) {
    printf("Usage: %s <infile> <outfile> <Threshold>\n", argv[0]);
    exit(1);
  }

  /* 雨音強度ファイルを開く */
  fpin = fopen(argv[1], "r");
  if(fpin == NULL) {
    printf("%s not found.\n", argv[1]);
    exit(1);
  }

  /* 出力ファイルを作成 */
  fpout = fopen(argv[2], "w");

  thr = atof(argv[3]);
  while(fscanf(fpin, "%lf %lf", &t, &pow) != EOF) {
    if(pow < thr) {
      fprintf(fpout, "%lf %lf\n", t, 0.0);
    } else {
      fprintf(fpout, "%lf %lf\n", t, pow);
    }
  }

  fclose(fpin);
  fclose(fpout);
  return 0;
}
