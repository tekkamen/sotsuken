#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ファイル名と, 開くモードを引数に取り, ファイルポインタを返す関数 */
FILE *file_open(char *fname, char *mode) {
  FILE *fp;

  /* ファイルが開けなかった場合 */	
  if((fp = fopen(fname, mode)) == NULL) {
    printf("%s can't open\n", fname);
    exit(1);
  }
	
  return fp; // ファイルポインタを返す
}

int main(int argc, char **argv) {
  FILE *fpin, *fpout;
  short int data;
  long len;
  int smpl, i, d;
  double start, end, t, dt;

 /* コマンドライン引数が正しく与えられなかった場合 */
  if(argc != 3) {
    printf("usage: %s <infile.raw> <outfile.txt>\n", argv[0]);
    exit(1);
  }

  /* 入力音声(raw)ファイルオープン */  
  fpin = file_open(argv[1], "rb");

  /* 出力テキストファイルオープン */
  fpout = file_open(argv[2], "w");

  /* 波形を切り出す区間(秒で与える)を入力させる */
  printf("Start[sec]: ");
  scanf("%lf", &start);

  printf("end[sec]: ");
  scanf("%lf", &end);

  smpl = 44100; // サンプリングレート
  t = 0.0;
  dt = 1.0 / smpl;
  len = (int)((end - start) * smpl);
  printf("len = %ld\n", len);

  fseek(fpin, (int)(smpl * start * sizeof(short)), SEEK_SET);

  for(i = 0; i < len; i++) {
    d = fread((void *)&data, sizeof(short), 1, fpin);
    if(!d) {
      printf("cannot read\n");
      exit(1);
    }
    fprintf(fpout, "%lf %d\n", t, data);
    t += dt;
  }
  fclose(fpin);
  fclose(fpout);
  return 0;
}

