#!/usr/bin/gnuplot -persist
set terminal png
set output "results/h.png"
set hidden3d
set title "graph - height" 
set xlabel "Longitude" 
set ylabel "Latitude" 
set zlabel "Height" 
set pm3d implicit at s
set palette defined (0 "blue", 0.5 "green", 0.8 "yellow", 1 "red")
splot "output/h.dat" matrix u 1:2:3