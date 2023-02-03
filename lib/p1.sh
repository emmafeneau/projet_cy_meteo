#!/usr/bin/gnuplot -persist
set terminal png
set datafile separator ";"
set output "results/p1.png"
set style data lines
set title "graph - pressure mode 1" 
set xlabel "Id station" 
set ylabel "Average, Min, Max" 
plot "output/p1.dat" u 1:2:3:4 t "average,min,max" w yerr