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

int main(int argc, char **argv)
{
  int i, j, k, max, index, smpl;
  short int ch, bs, bps;
  unsigned int ds;   // 波形データのサイズ
  unsigned int len; // 総フレーム数
  double sec, t, dt;
  int START, N, SHIFT;
  int dlen;
  //short int sitmp;
  unsigned long sum[2];
  double secList[2];
  FILE *fpin, *fpout;

  /* コマンドライン引数が正しく与えられなかった場合 */
  if(argc != 3) {
    printf("usage: %s <infile.wav> <outfile.txt>\n", argv[0]);
    exit(1);
  }

  /* 入力音声ファイルオープン */
  fpin = file_open(argv[1], "rb");

  /* wavファイルのヘッダをダンプし, 秒数を求める */
  sec = dump_wav(fpin, &ch, &smpl, &bs, &bps, &ds);
  //printf("seconds: %le[sec]\n", sec);	

  /* 出力テキストファイルオープン */
  fpout = file_open(argv[2], "w");

  START = 0;
  N = 2200;
  SHIFT = 1100;
  len = ds / bs;
  while(len % SHIFT != 0) {
    len++;
  }
  
  //printf("len = %d\n", len);

  short int data[N];

  t = (double)N / (double)smpl;
  dt = (double)SHIFT / (double)smpl;
  i = 0;
  sum[0] = 0; sum[1] = 0;
  secList[0] = t, secList[1] = t + dt;

  while(1) {
    if(i == 2) {
      max = index = 0;
      for(k = 0; k < 2; k++) {
	if(max < sum[k]) {
	  max = sum[k];
	  index = k;
	}
      }
      fprintf(fpout, "%lf %d\n", secList[index], max);

      if(START > len) break;

      printf("Now: %lf%%\n", (t / sec * 100));
      i = 0;
      sum[0] = 0; sum[1] = 0;
      t += 2 * dt;
      secList[0] = t; secList[1] = t + dt;
    }
    
    else {
      if(START <= len) {
	fseek(fpin, START * 2 + 44, SEEK_SET);
	//printf("before: %ld\n", ftell(fpin));
	dlen = fread(data, 2L, N, fpin);
	//printf("after: %ld\n", ftell(fpin));
	//printf("dlen = %d\n", dlen);
	for(j = 0; j < dlen; j++) {
	  //printf("%d\n", data[j]);
	  sum[i] += data[j] * data[j];
	}
	//printf("%ld\n", sum[i]);
	i++;
	START += SHIFT;
	//printf("start = %d\n", START);
      }
      
      else {
	if(i < 2) {
	  sum[i] = 0;
	  i++;
	}
      }
    }
  }

  fclose(fpin);  // 入力音声ファイルクローズ
  fclose(fpout); // 出力テキストファイルクローズ
	
  return 0;
}
