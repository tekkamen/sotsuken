set multiplot layout 2,1

set lmargin 18
set rmargin 6
set bmargin 7
set grid
set tics font "Helvetica, 20"

#set xr[0:1200]
set xl "Time[min]"
set xlabel font "Arial, 20" offset 0,-2

set yl "Rain Sound"
set ylabel font "Arial, 20" offset -4,0
plot "161110-1907-1042_norm_10.txt" with boxes lc rgb "red" notitle

set yl "Rainfall[mm]"
set ylabel font "Arial, 20" offset -2,0
#set yr[0:2]
plot "161110-1907-1042_r10.txt" with boxes lc rgb "blue" notitle

unset multiplot