#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ThrIN 50   // 雨音と判断する振幅の閾値
#define ThrOUT 40  // 雨音が減衰したと判断する振幅の閾値
#define TIME 0.005 // 振幅の最大値をとる区間の長さ(秒)
#define MAG 2      /* 前区間の振幅の最大値とこの倍率をかけ, 現区間の最大値がこれを超えたら
                      別の雨粒が落ちたと判断する */

void dump_wav(FILE*);
int mimimum(int, int, int);

/* wavファイルのヘッダーをダンプする関数 */
void dump_wav(FILE *fp) {
  char buf[8];
  short int ch, bs, bps;
  int smpl, dr, ds;
	
  /* RIFFヘッダのチェック */
  fread((void *)buf, sizeof(int), 1, fp);
  buf[4] = '\0';

  /* 入力された音声ファイルがwav形式で無かった場合 */
  if((strcmp(buf, "RIFF")) != 0) {
    printf("Unsupported file was inputted.\n");
    exit(1);
  }
	
  /* チャンネル数のチェック */
  fseek(fp, 22, SEEK_SET);
  fread((void *)&ch, sizeof(short), 1, fp);
  // printf("channel: %hd\n", *ch);
	
  /* サンプリングレート */
  fseek(fp, 24, SEEK_SET);
  fread((void *)&smpl, sizeof(int), 1, fp);
  //printf("sampling rate: %d[Hz]\n", *smpl);
	
  /* データ速度（Byte/sec）*/
  fseek(fp, 28, SEEK_SET);
  fread((void *)&dr, sizeof(int), 1, fp);
  //printf("data rate: %d[Byte/sec]\n", dr);
	
  /* ブロックサイズ */
  fseek(fp, 32, SEEK_SET);
  fread((void *)&bs, sizeof(short), 1, fp);
  //printf("block size: %d[Byte/block]\n", *bs);
	
  /* サンプルあたりのビット数 */
  fseek(fp, 34, SEEK_SET);
  fread((void *)&bps, sizeof(short), 1, fp);
  //printf("bit per sample: %d[bit]\n", *bps);
	
  /* 波形データのサイズ */
  fseek(fp, 40, SEEK_SET);
  fread((void *)&ds, sizeof(int), 1, fp);
  //printf("data size: %d[Byte]\n", *ds);
	
  /* 秒数 */
  //printf("seconds: %le[sec]\n", (double)ds / (double)dr);
}

int minimum(int a, int b, int c) {
  if(a < b) {
    if(a < c) return a;
    else return c;
  } else {
    if(b < c) return b;
    else return c;
  }
}

int main(int argc, char **argv) {
  FILE *fp1, *fp2, *fp3, *fpout;
  short int sitmp1, sitmp2, sitmp3, *sidata1, *sidata2, *sidata3;
  unsigned short ustmp1, ustmp2, ustmp3, *usdata1, *usdata2, *usdata3;
  unsigned short max1, max2, max3, pmax1, pmax2, pmax3;
  int idx_min, idx1, idx2, idx3, smpl = 44100;
  long int back;
  unsigned int ret, thr_in, thr_out;
  unsigned long i, len, num, eng1, eng2, eng3;
  double t, dt, now;
  clock_t start, end;

  if(argc != 5) {
    printf("Usage: %s <ch1> <ch2> <ch3> <outfile>\n", argv[0]);
    exit(1);
  }

  /* 入力ファイルオープン */
  fp1 = fopen(argv[1], "rb");
  fp2 = fopen(argv[2], "rb");
  fp3 = fopen(argv[3], "rb");
  if(fp1 == NULL || fp2 == NULL || fp3 == NULL) {
    printf("File not found.\n");
    exit(1);
  }
  /* wavファイルかチェック */
  dump_wav(fp1);
  dump_wav(fp2);
  dump_wav(fp3);

  /* 出力ファイルオープン */
  //fpout = fopen(argv[4], "w");
  fpout = fopen(argv[4], "wb");

  thr_in = ThrIN;
  thr_out = ThrOUT;
  t = 0.0;
  dt = 1.0 / smpl;
  len = (long)(smpl * TIME);
  num = sizeof(short) * len;

  /* 振幅値を格納する領域を確保 */
  sidata1 = (short *)malloc(num);
  sidata2 = (short *)malloc(num);
  sidata3 = (short *)malloc(num);
  if(sidata1 == NULL || sidata2 == NULL || sidata3 == NULL) {
    printf("malloc error\n");
    exit(1);
  }

  /* 振幅値(絶対値)を格納する領域を確保 */
  usdata1 = (unsigned short *)malloc(num);
  usdata2 = (unsigned short *)malloc(num);
  usdata3 = (unsigned short *)malloc(num);
  if(usdata1 == NULL || usdata2 == NULL || usdata3 == NULL) {
    printf("malloc error\n");
    exit(1);
  }

  /* 処理開始 */
  start = clock();
  while(fread((void *)&sitmp1, sizeof(short), 1, fp1) != 0) {
    /* 1サンプル読み込み, 絶対値に変換 */
    if(sitmp1 < 0) ustmp1 = -1 * sitmp1;
    else ustmp1 = sitmp1;

    fread((void *)&sitmp2, sizeof(short), 1, fp2);
    if(sitmp2 < 0) ustmp2 = -1 * sitmp2;
    else ustmp2 = sitmp2;

    fread((void *)&sitmp3, sizeof(short), 1, fp3);
    if(sitmp3 < 0) ustmp3 = -1 * sitmp3;
    else ustmp3 = sitmp3;

    /* 何れかのチャンネルの振幅値が閾値を超えた */
    if(ustmp1 >= thr_in || ustmp2 >= thr_in || ustmp3 >= thr_in) {
      now = t;

      /* ファイルポインタを1サンプル分戻す */
      fseek(fp1, -2L, SEEK_CUR);
      fseek(fp2, -2L, SEEK_CUR);
      fseek(fp3, -2L, SEEK_CUR);

      /* TIMEで指定した時間分のデータを読み込む */
      ret = fread((void *)sidata1, sizeof(short), len, fp1);
      fread((void *)sidata2, sizeof(short), len, fp2);
      fread((void *)sidata3, sizeof(short), len, fp3);

      /* ここからエネルギー計算処理 */
      eng1 = eng2 = eng3 = 0;
      for(i = 0; i < ret; i++) {
	eng1 += sidata1[i] * sidata1[i];
	eng2 += sidata2[i] * sidata2[i];
	eng3 += sidata3[i] * sidata3[i];
	t += dt;
      }
      //fprintf(fpout, "%lf %lu %lu %lu\n", now, eng1, eng2, eng3);
      fwrite((void *)&now, sizeof(double), 1, fpout);
      fwrite((void *)&eng1, sizeof(long), 1, fpout);
      fwrite((void *)&eng2, sizeof(long), 1, fpout);
      fwrite((void *)&eng3, sizeof(long), 1, fpout);

      /* ここから雨音の減衰を調べる処理 */
      max1 = max2 = max3 = 0;
      idx1 = idx2 = idx3 = 0;
      do {
	pmax1 = max1;
	pmax2 = max2;
	pmax3 = max3;
	max1 = max2 = max3 = 0;
	ret = fread((void *)sidata1, sizeof(short), len, fp1);
	if(ret <= 0) break;
	fread((void *)sidata2, sizeof(short), len, fp2);
	fread((void *)sidata3, sizeof(short), len, fp3);

	/* 絶対値変換処理と最大値更新処理 */
	for(i = 0; i < ret; i++) {
	  if(sidata1[i] < 0) usdata1[i] = -1 * sidata1[i];
	  else usdata1[i] = sidata1[i];
	  if(usdata1[i] > max1) {
	    idx1 = i;
	    max1 = usdata1[i];
	  }

	  if(sidata2[i] < 0) usdata2[i] = -1 * sidata2[i];
	  else usdata2[i] = sidata2[i];
	  if(usdata2[i] > max2) {
	    idx2 = i;
	    max2 = usdata2[i];
	  }

	  if(sidata3[i] < 0) usdata3[i] = -1 * sidata3[i];
	  else usdata3[i] = sidata3[i];
	  if(usdata3[i] > max3) {
	    idx3 = i;
	    max3 = usdata3[i];
	  }

	  t += dt;
	}

	/* 現区間の最大値が, 前区間の最大値 * MAGで指定した倍率より大きかった場合,
	   別の雨粒が落ちてきたと判断し, エネルギー計算処理に戻る */
	if(pmax1 != 0 && pmax2 != 0 && pmax3 != 0) {
	  if(pmax1 * MAG < max1 || pmax2 * MAG < max2 || pmax3 * MAG < max3) {
	    //printf("hoge\n");
	    idx_min = minimum(idx1, idx2, idx3);
	    back = -2 * ((long)ret - idx_min);
	    fseek(fp1, back, SEEK_CUR);
	    fseek(fp2, back, SEEK_CUR);
	    fseek(fp3, back, SEEK_CUR);
	    t -= (ret - idx_min) * dt;
	    break;
	  }
	}
	  // 全てのチャンネルで閾値以下になるまでループを回す
      } while(max1 > thr_out || max2 > thr_out || max3 > thr_out);
      //printf("fuga\n");
    } else {
      t += dt;
    }
  }
  end = clock();
  //printf("%ld\n", CLOCKS_PER_SEC);
  printf("process time: %ld[usec]\n", (long)(end - start));
  printf("process time: %ld[sec]\n", (long)(end - start) / CLOCKS_PER_SEC);
  fclose(fp1);
  fclose(fp2);
  fclose(fp3);
  fclose(fpout);
  free(sidata1);
  free(sidata2);
  free(sidata3);
  free(usdata1);
  free(usdata2);
  free(usdata3);
  return 0;
}
