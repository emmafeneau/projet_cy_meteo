#!/usr/bin/gnuplot -persist
set terminal png
set datafile separator ";"
set output "results/t3.png"
set xdata time
set format x "%d/%m"
set timefmt "%Y-%m-%dT%H"
set title "graph - temperature mode 3" 
set xlabel "Dates/hours"
set ylabel "Temperature(C)"
