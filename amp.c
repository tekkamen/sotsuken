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

/* wavファイルのヘッダーをダンプし, 秒数を返す関数 */
double dump_wav(FILE *fp, short int *ch, int *smpl,
		short int *bs, short int *bps, unsigned int *ds) {
  char buf[8];
  unsigned int dr; // dr: data rate, ds: data size
	
  /* RIFFヘッダのチェック */
  fread((void *)buf, sizeof(int), 1, fp);
  buf[4] = '\0';

  /* 入力された音声ファイルがwav形式で無かった場合 */
  if((strcmp(buf, "RIFF")) != 0) {
    printf("Unsupported file was inputted.\n");
    fclose(fp);
    exit(1);
  }
	
  /* チャンネル数のチェック */
  fseek(fp, 22, SEEK_SET);
  fread((void *)ch, sizeof(short), 1, fp);
  //printf("channel: %hd\n", *ch);
	
  /* サンプリングレート */
  fseek(fp, 24, SEEK_SET);
  fread((void *)smpl, sizeof(int), 1, fp);
  //printf("sampling rate: %d[Hz]\n", *smpl);
	
  /* データ速度（Byte/sec）*/
  fseek(fp, 28, SEEK_SET);
  fread((void *)&dr, sizeof(int), 1, fp);
  //printf("data rate: %d[Byte/sec]\n", dr);
	
  /* ブロックサイズ */
  fseek(fp, 32, SEEK_SET);
  fread((void *)bs, sizeof(short), 1, fp);
  //printf("block size: %d[Byte/block]\n", *bs);
	
  /* サンプルあたりのビット数 */
  fseek(fp, 34, SEEK_SET);
  fread((void *)bps, sizeof(short), 1, fp);
  //printf("bit per sample: %d[bit]\n", *bps);
	
  /* 波形データのサイズ */
  fseek(fp, 40, SEEK_SET);
  fread((void *)ds, sizeof(int), 1, fp);
  //printf("data size: %d[Byte]\n", *ds);
	
  /* 秒数 */
  //printf("seconds: %le[sec]\n", (double)*ds / (double)dr);
  return (double)*ds / (double)dr;
}

int main(int argc, char **argv) {
  FILE *fpin, *fpout;
  double sec, t, dt;
  double avg;
  short int *data;
  int i, N, len;
  unsigned long long sum;
  int smpl;
  short int ch, bs, bps;
  unsigned int ds;

 /* コマンドライン引数が正しく与えられなかった場合 */
  if(argc != 3) {
    printf("usage: %s <infile.wav> <outfile.dat>\n", argv[0]);
    exit(1);
  }

  /* 入力音声ファイルオープン */  
  fpin = file_open(argv[1], "rb");

  /* wavファイルのヘッダをダンプし, 秒数を求める */
  sec = dump_wav(fpin, &ch, &smpl, &bs, &bps, &ds);
  //printf("seconds: %le[sec]\n", sec);	

  /* 出力テキストファイルオープン */
  fpout = file_open(argv[2], "wb");

  N = smpl * 60 * 60; // 1時間ごと
  //N = 2205;
  t = dt = (double)N / (double)smpl;
  data = (short int *)malloc(sizeof(short) * N);
  if(data == NULL) {
    printf("malloc error\n");
    exit(1);
  }

  while(1) {
    len = fread((void *)data, sizeof(short), N, fpin);
    if(len <= 0) break;
    
    sum = 0;
    for(i = 0; i < len; i++) {
      sum += data[i] * data[i];
    }
    avg = (double)sum / (double)len; // 二乗平均
    printf("sum = %lld, avg = %lf\n", sum, avg);
    //fprintf(fpout, "%lf %lf\n", t, avg);
    //fprintf(fpout, "%lf %lf\n", t / 60.0, avg);
    fprintf(fpout, "%lf %lf\n", t / 3600.0, avg);
    printf("Now: %lf%%\n", (t / sec * 100));
    t += dt;
  }
  free(data);
  fclose(fpin);
  fclose(fpout);
  return 0;
}
