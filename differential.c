#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

int main(int argc, char **argv) {
  FILE *fpin, *fpout;
  short int data[2];
  int smpl, diff;
  short int ch, bs, bps;
  unsigned int ds;
  unsigned long start, end;
  double s_sec, e_sec;

 /* コマンドライン引数が正しく与えられなかった場合 */
  if(argc != 3) {
    printf("usage: %s <infile.wav> <outfile.dat>\n", argv[0]);
    exit(1);
  }

  /* 入力音声ファイルオープン */  
  fpin = fopen(argv[1], "rb");
  if(fpin == NULL) {
    printf("%s not found.\n", argv[1]);
    exit(1);
  }

  /* wavファイルのヘッダをダンプし, 秒数を求める */
  dump_wav(fpin, &ch, &smpl, &bs, &bps, &ds);

  /* 出力テキストファイルオープン */
  fpout = fopen(argv[2], "w");

  printf("start: ");
  scanf("%lf", &s_sec);
  start = s_sec * 44100;
  if(start % 2 != 0) start++;

  printf("end: ");
  scanf("%lf", &e_sec);
  end = e_sec * 44100;

  fseek(fpin, start, SEEK_CUR);
  printf("position: %ld\n", ftell(fpin));
  while(ftell(fpin) < end) {
    fread((void *)data, sizeof(short), 2, fpin);
    diff = data[1] - data[0];
    fprintf(fpout, "%d\n", diff);
    //fwrite((void *)&diff, sizeof(int), 1, fpout);
    fseek(fpin, -2, SEEK_CUR);
  }
  fclose(fpin);
  fclose(fpout);
  return 0;
}
