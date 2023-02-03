#!/usr/bin/gnuplot -persist
set terminal png
set datafile separator ";"
set output "results/w.png"
set title "graph - wind" 
set ylabel "longitude/latitude" 
set ylabel "wind" 
plot "output/w.dat" using 1:2:3:4 with vectors filled head lw 3