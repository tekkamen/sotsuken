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
  double t, dt;
  short int *data;
  int i, N, len;
  unsigned long long sum;
  int smpl;

 /* コマンドライン引数が正しく与えられなかった場合 */
  if(argc != 3) {
    printf("usage: %s <infile.raw> <outfile.txt>\n", argv[0]);
    exit(1);
  }

  /* 入力音声(raw)ファイルオープン */  
  fpin = file_open(argv[1], "rb");

  /* 出力テキストファイルオープン */
  fpout = file_open(argv[2], "w");

  smpl = 44100;
  //N = smpl * 60 * 1; // 1分毎に積算
  N = smpl * 60 * 10; // 10分毎に積算
  //N = smpl * 60 * 60; // 1時間毎に積算
  //N = smpl * 60 * 60 * 3;
  t = dt = (double)N / (double)smpl;
  data = (short int *)malloc(sizeof(short) * N);
  if(data == NULL) {
    printf("malloc error\n");
    exit(1);
  }

  while(1) {
    len = fread((void *)data, sizeof(short), N, fpin);
    printf("len = %d\n", len);
    if(len <= 0) break;
    
    sum = 0;
    for(i = 0; i < len; i++) {
      sum += data[i] * data[i];
    }
    sum /= len;
    //fprintf(fpout, "%lf %lld\n", t, sum); //秒単位で表示
    fprintf(fpout, "%lf %lld\n", t / 60.0, sum); //分単位で表示
    //fprintf(fpout, "%lf %lld\n", t / 3600.0, sum); //時単位で表示
    t += dt;
  }
  free(data);
  fclose(fpin);
  fclose(fpout);
  return 0;
}

