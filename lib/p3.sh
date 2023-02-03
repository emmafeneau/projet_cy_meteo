#!/usr/bin/gnuplot -persist
set terminal png
set datafile separator ";"
set output "results/p3.png"
set xdata time
set format x "%d/%m"
set timefmt "%Y-%m-%dT%H"
set title "graph - pressure mode 3" 
set xlabel "Dates/hours"
set ylabel "Pressions"
plot "output/p3.dat" using 1:2, "output/p3//07005.dat" u 1:2 w l, "output/p3//07015.dat" u 1:2 w l, "output/p3//07020.dat" u 1:2 w l, "output/p3//07027.dat" u 1:2 w l, "output/p3//07037.dat" u 1:2 w l, "output/p3//07110.dat" u 1:2 w l, "output/p3//07117.dat" u 1:2 w l, "output/p3//07130.dat" u 1:2 w l, "output/p3//07139.dat" u 1:2 w l, "output/p3//07149.dat" u 1:2 w l, "output/p3//07168.dat" u 1:2 w l, "output/p3//07181.dat" u 1:2 w l, "output/p3//07190.dat" u 1:2 w l, "output/p3//07207.dat" u 1:2 w l, "output/p3//07222.dat" u 1:2 w l, "output/p3//07240.dat" u 1:2 w l, "output/p3//07255.dat" u 1:2 w l, "output/p3//07280.dat" u 1:2 w l, "output/p3//07299.dat" u 1:2 w l, "output/p3//07314.dat" u 1:2 w l, "output/p3//07335.dat" u 1:2 w l, "output/p3//07434.dat" u 1:2 w l, "output/p3//07460.dat" u 1:2 w l, "output/p3//07471.dat" u 1:2 w l, "output/p3//07481.dat" u 1:2 w l, "output/p3//07510.dat" u 1:2 w l, "output/p3//07535.dat" u 1:2 w l, "output/p3//07558.dat" u 1:2 w l, "output/p3//07577.dat" u 1:2 w l, "output/p3//07591.dat" u 1:2 w l, "output/p3//07607.dat" u 1:2 w l, "output/p3//07621.dat" u 1:2 w l, "output/p3//07627.dat" u 1:2 w l, "output/p3//07630.dat" u 1:2 w l, "output/p3//07643.dat" u 1:2 w l, "output/p3//07650.dat" u 1:2 w l, "output/p3//07661.dat" u 1:2 w l, "output/p3//07690.dat" u 1:2 w l, "output/p3//07747.dat" u 1:2 w l, "output/p3//07761.dat" u 1:2 w l, "output/p3//07790.dat" u 1:2 w l, "output/p3//61976.dat" u 1:2 w l, "output/p3//71805.dat" u 1:2 w l, "output/p3//78922.dat" u 1:2 w l, "output/p3//81401.dat" u 1:2 w l, "output/p3//81405.dat" u 1:2 w l, "output/p3//81408.dat" u 1:2 w l, "output/p3//81415.dat" u 1:2 w l, "output/p3//89642.dat" u 1:2 w l
