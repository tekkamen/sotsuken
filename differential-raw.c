#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char **argv) {
  FILE *fpin, *fpout;
  short int data[2];
  int diff;

 /* コマンドライン引数が正しく与えられなかった場合 */
  if(argc != 3) {
    printf("usage: %s <infile.raw> <outfile.dat>\n", argv[0]);
    exit(1);
  }

  /* 入力音声ファイルオープン */  
  fpin = fopen(argv[1], "rb");
  if(fpin == NULL) {
    printf("%s not found.\n", argv[1]);
    exit(1);
  }

  /* 出力テキストファイルオープン */
  fpout = fopen(argv[2], "wb");

  while(fread((void *)data, sizeof(short), 2, fpin) == 2) {
    diff = data[1] - data[0];
    fwrite((void *)&diff, sizeof(int), 1, fpout);
    t += dt;
    fseek(fpin, -2, SEEK_CUR);
  }
  fclose(fpin);
  fclose(fpout);
  return 0;
}
