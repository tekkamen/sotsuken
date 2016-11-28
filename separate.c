/* 個々の雨粒を分離してエネルギーを計算する */
/* 入力は44.1kHz, 16bitのwavファイルとする */
/* 使い方: ./separate <in-file.wav> <out-file.txt> */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ThrIN 50  // 雨音と判断する振幅の閾値
#define ThrOUT 40 // 雨音が減衰したと判断する振幅の閾値
#define TIME 0.005 // 振幅の最大値をとる区間の長さ(秒)

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
  double t, dt, now;
  int smpl, max;
  unsigned long i, len, num, eng;
  short int ch, bs, bps, sitmp, *sidata;
  unsigned short ustmp, *usdata;
  unsigned int ds, ret, thr_in, thr_out;

  if(argc != 3) {
    printf("Usage: %s <in-file.wav> <out-file.txt>\n", argv[0]);
    exit(1);
  }

  /* 入力wavファイルオープン */
  fpin = fopen(argv[1], "rb");
  if(fpin == NULL) {
    printf("%s not found\n", argv[1]);
    exit(1);
  }

  /* wavファイルのヘッダをダンプし, 秒数を求める */
  dump_wav(fpin, &ch, &smpl, &bs, &bps, &ds);
  printf("position: %ld\n", ftell(fpin));

  /* 出力ファイルオープン */
  fpout = fopen(argv[2], "w");


  thr_in = ThrIN;
  thr_out = ThrOUT;
  t = 0;
  dt = 1.0 / smpl;
  len = (long)(smpl * TIME);
  num = sizeof(short) * len;
  sidata = (short *)malloc(num);
  if(sidata == NULL) {
    printf("malloc error\n");
    exit(1);
  }
  usdata = (unsigned short *)malloc(num);
  if(usdata == NULL) {
    printf("malloc error\n");
    exit(1);
  }

  while(fread((void *)&sitmp, sizeof(short), 1, fpin) != 0) {
    if(sitmp < 0) ustmp = -1 * sitmp; // 絶対値に変換
    else ustmp = sitmp;

    if(ustmp >= thr_in) {
      eng = 0;
      now = t;
      fseek(fpin, -2L, SEEK_CUR);
      do {
	max = 0;
	ret = fread((void *)sidata, sizeof(short), len, fpin);
	if(ret <= 0) break;

	for(i = 0; i < ret; i++) {
	  if(sidata[i] < 0) usdata[i] = -1 * sidata[i];
	  else usdata[i] = sidata[i];

	  if(usdata[i] > max) max = usdata[i];
	  eng += usdata[i] * usdata[i];
	  t += dt;
	} 
      } while(max > thr_out);
      //      printf("max: %d\n", max);
      fprintf(fpout, "%lf %lu\n", now, eng);
    } else {
      t += dt;
    }
  }
  fclose(fpin);
  fclose(fpout);
  free(sidata);
  free(usdata);
  return 0;
}
