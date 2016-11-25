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
  unsigned long i, len;
  int smpl;
  int hr, min;
  double sec, start, end, t, dt;

 /* コマンドライン引数が正しく与えられなかった場合 */
  if(argc != 3) {
    printf("usage: %s <infile.raw> <outfile.dat>\n", argv[0]);
    exit(1);
  }

  /* 入力音声(raw)ファイルオープン */  
  fpin = file_open(argv[1], "rb");

  /* 出力テキストファイルオープン */
  fpout = file_open(argv[2], "wb");

  /* 波形を切り出す区間(時:分:秒)を入力させる */
  printf("Start: ");
  scanf("%d:%d:%lf", &hr, &min, &sec);
  start = 3600.0 * hr + 60.0 * min + sec;
  printf("Start = %lf[sec]\n", start);

  printf("end: ");
  scanf("%d:%d:%lf", &hr, &min, &sec);
  end = 3600.0 * hr + 60.0 * min + sec;
  printf("end = %lf[sec]\n", end);

  if(end < start) {
    printf("Error!!! (end < start)\n");
    exit(1);
  }

  smpl = 44100; // サンプリングレート
  t = 0.0;
  dt = 1.0 / smpl;
  len = (unsigned long)((end - start) * smpl);
  printf("len = %lu\n", len);

  if((fseek(fpin, (long)(smpl * start * sizeof(short)), SEEK_SET)) != 0) {
    printf("Error!!! (cannot seek)\n");
    exit(1);
  }
  printf("position: %ld\n", ftell(fpin));

  for(i = 1; i <= len; i++) {
    if((fread((void *)&data, sizeof(short), 1, fpin)) == 0) {
      printf("EOF detected!\n");
      break;
    }
    printf("%5d ", data);
    if(i % 10 == 0) printf("\n");
    fwrite((void *)&t, sizeof(double), 1, fpout);
    fwrite((void *)&data, sizeof(short), 1, fpout);
    t += dt;
  }
  fclose(fpin);
  fclose(fpout);
  return 0;
}
