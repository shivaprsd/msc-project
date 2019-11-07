load 'std.gp'
set size square
set key box center right Left reverse
set title "Kuramoto Model: r vs K"
set xlabel "Degree of coupling, K"
set ylabel "Order parameter, r"
set label at graph 1, graph 1.02 right "g(w): Uniform" font font.",16"
set xrange [:0.145]
plot ARG1 i 0 pt 7 not, '' i 0 smooth csplines lw 1.5 t "N = 100, t = 20s",\
     ARG1 i 2 smooth cspline lw 1.5 t "N = 100, t = 50s", '' i 2 pt 6 lc black not, \
     1/0 not, ARG1 i 1 pt 7 not, '' i 1 smooth csplines lw 1.5 t "N = 64,  t = 20s"
