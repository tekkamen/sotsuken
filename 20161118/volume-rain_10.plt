set multiplot layout 2,1

set lmargin 12
set rmargin 5
set grid
set tics font "Helvetica, 12"

#set xr[600:1000]
set xl "Time[min]"
set xlabel font "Arial, 12"

set yl "Rain Sound"
set ylabel font "Arial, 12"
#set yr[0:100]
plot "161118-1730-1341_norm_10.txt" with boxes lc rgb "red" notitle

set yl "Rainfall[mm]"
set ylabel font "Arial, 12"
#set yr[0:2]
plot "161118-1730-1341_r10.txt" with boxes lc rgb "blue" notitle

unset multiplot