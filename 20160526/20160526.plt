set multiplot layout 2,1

set lmargin 12
set rmargin 5
set grid

set xl "Time[min]"


set yl "Volume"

plot "2016-05-26-1801-0901_hpf_volume_10min.txt" w l lc rgb "red" notitle

set yl "Rainfall[mm]"
set yr[0:3.0]
plot "2016-05-26-1801-0901_rainfall_10min.TXT" with boxes lc rgb "blue" notitle

unset multiplot