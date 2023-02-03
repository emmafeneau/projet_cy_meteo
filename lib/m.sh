#!/usr/bin/gnuplot -persist
set terminal png
set output "results/m.png"
set hidden3d
set title "graph - moisture" 
set xlabel "Longitude" 
set ylabel "Latitude" 
set zlabel "Moisture" 
set pm3d implicit at s
splot "output/m.dat" matrix u 1:2:3