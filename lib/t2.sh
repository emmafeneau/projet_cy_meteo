#!/usr/bin/gnuplot -persist
set terminal png
set datafile separator ";"
set output "results/t2.png"
 set xdata time
 set title "graph - temperature mode 2" 
      set format x "%d/%m"
      set timefmt "%Y-%m-%dT%H"
      set xlabel "Dates/hours"
      set ylabel "Temperature (C)"
      plot "output/t2.dat" using 1:2 w l