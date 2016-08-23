set xr[-0.3:6.3]
set yr[-1:20]
set grid
set key left top
set xl "Rainfall[mm/h]"
set yl "Rain Sound Intensity"

plot "2015-11-13-2340-0920_scatter-plot_rain_sound_1h_cut.txt" notitle
replot "2015-11-13-2340-0920_scatter-plot_rain_sound_1h_cut_kinji.txt" w l title "approximate line" lc rgb "blue"