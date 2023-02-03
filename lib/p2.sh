#!/usr/bin/gnuplot -persist
set terminal png
set datafile separator ";"
set output "results/p2.png"
 set xdata time
 set title "graph - pressure mode 2" 
      set format x "%d/%m"
      set timefmt "%Y-%m-%dT%H"
        set xlabel "Dates/hours"
      set ylabel "Pressure"
      plot "output/p2.dat" using 1:2 w l