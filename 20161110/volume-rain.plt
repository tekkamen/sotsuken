set multiplot layout 2,1

set lmargin 12
set rmargin 5
set grid
set tics font "Helvetica, 12"

#set xr[800:1250]
set xl "Time[min]"
set xlabel font "Arial, 12"

set yl "Rain Sound Intensity"
set ylabel font "Arial, 12"
#set yr[0:100]
plot "161110-1907-1042_hp_v10m.txt" w l lc rgb "red" notitle

set yl "Rainfall[mm]"
set ylabel font "Arial, 12"
#set yr[0:2]
plot "161110-1907-1042_r10.txt" with boxes lc rgb "blue" notitle

unset multiplot