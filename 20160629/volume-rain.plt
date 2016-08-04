set multiplot layout 2,1

set lmargin 12
set rmargin 5
set grid
set tics font "Helvetica, 12"

set xl "Time[min]"
set xlabel font "Arial, 12"

set yl "Rain Sound Intensity"
set ylabel font "Arial, 12"
set yr[0:100]
plot "160629-1608-0943_bp_v10.txt" w l lc rgb "red" notitle

set yl "Rainfall[mm]"
set ylabel font "Arial, 12"
set yr[0:2]
plot "160629-1608-0943_r10.txt" with boxes lc rgb "blue" notitle

unset multiplot