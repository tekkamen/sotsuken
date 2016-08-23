set multiplot layout 2,1

set lmargin 12
set rmargin 5
set grid
set tics font "Helvetica, 20"

set xl "Time[min]"
set xlabel font "Arial, 20"

set yl "Rain Sound Intensity"
set ylabel font "Arial, 20"
set yr[0:110]
plot "2015-11-13-2340-0920_hpf_volume_1min.txt" w l lc rgb "red" notitle

set yl "Rainfall[mm]"
set ylabel font "Arial, 20"
set yr[0:1]
plot "2015-11-13-2340-0920_rainfall.TXT" with boxes lc rgb "blue" notitle

unset multiplot