#!/usr/bin/gnuplot -persist
set terminal png
set datafile separator ";"
set output "results/t1.png"
set style data lines
set title "graph - temperature mode 1" 
set xlabel "id station" 
set ylabel "Average, Min, Max" 
plot "output/t1.dat" u 1:2:3:4 t "average,min,max" w yerr