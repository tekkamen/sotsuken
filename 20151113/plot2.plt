# 座標系の指定
set ytics nomirror # 第一y軸の目盛は左側のみにする
set y2tics         # 第二y軸を描画することを指定
set y2range [0:1.0]
set grid           # 目盛線を描画することを指定
set xlabel "Time [min]"  # x軸の見出しを指定
set ylabel "Sound intensity" # 第一y軸の見出しを指定
set y2label "Rain fall [mm]" # 第二y軸の見出しを指定
# 凡例
set key top left   # 凡例は左上に描画
# データ描画
plot '2015-11-13-2340-0920_hpf_volume_1min.txt' w l title "Sound intensity", \
     '2015-11-13-2340-0920_rainfall.TXT' w l title "Rain fall" axes x1y2
     #  "axes x1y2"を指定することでデータファイルＢの目盛は第一x軸と第二y軸にとることを指定