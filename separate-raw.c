/* 個々の雨粒を分離してエネルギーを計算する */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ThrIN 50    // 雨音と判断する振幅の閾値
#define ThrOUT 15   // 雨音が減衰したと判断する振幅の閾値
#define TIME1 0.005 // エネルギーを計算する時間
#define TIME2 0.005 // 振幅の最大値をとる区間の長さ(秒)
#define MAG 2       /* 前区間の振幅の最大値とこの倍率をかけて, 
		       現区間の最大値がこれを超えたら
		       別の雨粒が落ちたと判断する */ 

int main(int argc, char **argv) {
  FILE *fpin, *fpout;
  short int sitmp, *sidata;
  unsigned short max, pmax, ustmp, *usdata;
  int idx, smpl = 44100;
  long int back;
  unsigned int ret, thr_in, thr_out;
  unsigned long i, len1, len2, num, eng;
  double t, dt, now;

  if(argc != 3) {
    printf("Usage: %s <in-file.raw> <out-file.txt>\n", argv[0]);
    exit(1);
  }

  /* 入力ファイルオープン */
  fpin = fopen(argv[1], "rb");
  if(fpin == NULL) {
    printf("%s not found\n", argv[1]);
    exit(1);
  }

  /* 出力ファイルオープン */
  fpout = fopen(argv[2], "w");

  thr_in = ThrIN;
  thr_out = ThrOUT;
  t = 0.0;
  dt = 1.0 / smpl;
  len1 = (long)(smpl * TIME1);
  len2 = (long)(smpl * TIME2);
  num = sizeof(short) * len1;

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
      now = t;

      fseek(fpin, -2L, SEEK_CUR);
      ret = fread((void *)sidata, sizeof(short), len1, fpin);

      eng = 0;
      for(i = 0; i < ret; i++) {
	eng += sidata[i] * sidata[i];
	t += dt;
      }
      //fprintf(fpout, "%lf %lu\n", now - dt, (long)0);
      fprintf(fpout, "%lf %lu\n", now - TIME1, (long)0);
      fprintf(fpout, "%lf %lu\n", now, eng);
      fprintf(fpout, "%lf %lu\n", now + TIME1, (long)0);

      max = 0;
      idx = 0;
      do {
	pmax = max;
	max = 0;
	ret = fread((void *)sidata, sizeof(short), len2, fpin);
	//printf("ret = %u\n", ret);
	if(ret <= 0) break;

	for(i = 0; i < ret; i++) {
	  if(sidata[i] < 0) usdata[i] = -1 * sidata[i];
	  else usdata[i] = sidata[i];
	  if(usdata[i] > max) {
	    idx = i;
	    max = usdata[i];
	  }
	  t += dt;
	} 

	/* 現区間の最大値が, 前区間の最大値 * MAGで指定した倍率より
	   大きかった場合, 別の雨粒が落ちてきたと判断し, 
	   エネルギー計算処理に戻る */
	if(pmax != 0) {
	  if(pmax * MAG < max) {
	    back = -2 * ((long)ret - idx);
	    fseek(fpin, back, SEEK_CUR);
	    printf("position: %ld\n", ftell(fpin));
	    t -= (ret - idx) * dt;
	    break;
	  }
	}
      } while(max > thr_out);

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
