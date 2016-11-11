/******************************************************************
 * 複数(1つでも可)のwavファイルの波形データ部分のみを
   1つのファイルに結合するプログラム
 * 実行方法: ./wav2raw <in-file1.wav> <in-file2.wav> ... <out.raw>
 *****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 入力されたものがwavファイルか確認する関数 */
int check_wav(FILE *fp) {
  char buf[8];
	
  /* RIFFヘッダのチェック */
  fread((void *)buf, sizeof(int), 1, fp);
  buf[4] = '\0';

  /* 入力された音声ファイルがwav形式で無かった場合 */
  if((strcmp(buf, "RIFF")) != 0) {
    printf("Unsupported file was inputted.\n");
    return -1;
  }
  return 0;	
}

int main(int argc, char **argv) {
  FILE *fpin, *fpout;
  short int *data;
  int smpl = 44100;
  int i, n;

  if(argc < 3) {
    printf("Usage: %s <infile.wav> [<infile.wav>...] <outfile.raw>\n", 
	   argv[0]);
  }
  n = argc - 2; // 入力されたwavファイルの個数

  /* 始めに, 入力されたものが全て正しいかチェックする */
  for(i = 1; i <= n; i++) {
    /* 入力wavファイルオープン */
    fpin = fopen(argv[i], "rb");
    if(fpin == NULL) {
      printf("%s cannot open\n", argv[i]);
      exit(1);
    }
    /* 入力されたものが本当にwavファイルかチェック */
    if(check_wav(fpin) < 0) {
      fclose(fpin);
      exit(1);
    }
    fclose(fpin);
  }

  /* 出力ファイルオープン */
  fpout = fopen(argv[argc - 1], "wb");
  if(fpout == NULL) {
    printf("%s cannot open\n", argv[argc - 1]);
    exit(1);
  }

  /* 波形データを格納するための領域を確保 */
  data = (short *)malloc(sizeof(short) * smpl * 10);
  if(data == NULL) {
    printf("malloc error!!!\n");
    exit(1);
  }

  for(i = 1; i <= n; i++) {
    printf("now: %d/%d\n", i, n);
    fpin = fopen(argv[i], "rb"); // wavファイルオープン
    fseek(fpin, 44L, SEEK_SET);  // 波形データ開始位置までシーク
    while(fread((void *)data, sizeof(short), smpl * 10, fpin) != 0) {
      fwrite((void *)data, sizeof(short), smpl * 10, fpout);
    }
    fclose(fpin);
  }
  fclose(fpout);
  free(data);
  return 0;
}
