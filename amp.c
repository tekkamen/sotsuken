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
  printf("channel: %hd\n", *ch);
	
  /* サンプリングレート */
  fseek(fp, 24, SEEK_SET);
  fread((void *)smpl, sizeof(int), 1, fp);
  printf("sampling rate: %d[Hz]\n", *smpl);
	
  /* データ速度（Byte/sec）*/
  fseek(fp, 28, SEEK_SET);
  fread((void *)&dr, sizeof(int), 1, fp);
  printf("data rate: %d[Byte/sec]\n", dr);
	
  /* ブロックサイズ */
  fseek(fp, 32, SEEK_SET);
  fread((void *)bs, sizeof(short), 1, fp);
  printf("block size: %d[Byte/block]\n", *bs);
	
  /* サンプルあたりのビット数 */
  fseek(fp, 34, SEEK_SET);
  fread((void *)bps, sizeof(short), 1, fp);
  printf("bit per sample: %d[bit]\n", *bps);
	
  /* 波形データのサイズ */
  fseek(fp, 40, SEEK_SET);
  fread((void *)ds, sizeof(int), 1, fp);
  printf("data size: %d[Byte]\n", *ds);
	
  /* 秒数 */
  printf("seconds: %le[sec]\n", (double)*ds / (double)dr);
  return (double)*ds / (double)dr;
}

void print_length(double sec) {
  int hr, min;

  hr = (int)sec / 3600;
  sec -= hr * 3600.0;

  min = (int)sec / 60;
  sec -= min * 60;

  printf("length: %d:%d:%lf\n", hr, min, sec);
}

int main(int argc, char **argv) {
  FILE *fpin, *fpout;
  double seconds, t, dt;
  short int data;
  int smpl;
  unsigned long i, len, pos;
  short int ch, bs, bps;
  unsigned int ds;
  int hr, min;
  double sec, start, end;

 /* コマンドライン引数が正しく与えられなかった場合 */
  if(argc != 3) {
    printf("usage: %s <infile.wav> <outfile.dat>\n", argv[0]);
    exit(1);
  }

  /* 入力音声ファイルオープン */  
  fpin = file_open(argv[1], "rb");

  /* wavファイルのヘッダをダンプし, 秒数を求める */
  seconds = dump_wav(fpin, &ch, &smpl, &bs, &bps, &ds);
  print_length(seconds);

  /* 出力テキストファイルオープン */
  fpout = file_open(argv[2], "wb");

  /* 波形を切り出す区間(時:分:秒)を入力させる */
  printf("Start: ");
  scanf("%d:%d:%lf", &hr, &min, &sec);
  start = 3600.0 * hr + 60.0 * min + sec;
  printf("Start = %lf[sec]\n", start);

  if(seconds < start) {
    printf("Error!!! (length < start)\n");
    exit(1);
  }

  printf("end: ");
  scanf("%d:%d:%lf", &hr, &min, &sec);
  end = 3600.0 * hr + 60.0 * min + sec;
  printf("end = %lf[sec]\n", end);

  if(seconds < end) {
    printf("Error!!! (length < end)\n");
    exit(1);
  }

  if(end < start) {
    printf("Error!!! (end < start)\n");
    exit(1);
  }

  t = 0.0;
  dt = 1.0 / smpl;
  len = (unsigned long)((end - start) * smpl);
  printf("len = %lu\n", len);
  pos = (long)(smpl * start * sizeof(short));
  if(pos % 2 != 0) pos--;
  if((fseek(fpin, pos, SEEK_CUR)) != 0) {
    printf("Error!!! (cannot seek)\n");
    exit(1);
  }

  printf("position: %ld\n", ftell(fpin));
  for(i = 1; i <= len; i++) {
    fread((void *)&data, sizeof(short), 1, fpin);
    // printf("%5d ", data);
    // if(i % 10 == 0) printf("\n");
    fwrite((void *)&t, sizeof(double), 1, fpout);
    fwrite((void *)&data, sizeof(short), 1, fpout);
    t += dt;
  }
  // printf("\n");
  fclose(fpin);
  fclose(fpout);
  return 0;
}
