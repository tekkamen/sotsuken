set multiplot layout 2,1

set lmargin 12
set rmargin 5
set grid
#set tics font "Helvetica, 20"

set xl "Time[h]"
set xr[0.5:10.5]
set xtics 1
#set xlabel font "Arial, 20"

set yl "Rain Sound Intensity"
#set ylabel font "Arial, 20"
set yr[-1:20]
plot "2015-11-13-2340-0920_hpf_volume_1h.txt" w l lc rgb "red" notitle

set yl "Rainfall[mm]"
#set ylabel font "Arial, 20"
set yr[0:6.5]
plot "2015-11-13-2340-0920_rainfall_1h.txt" with boxes lc rgb "blue" notitle

unset multiplot