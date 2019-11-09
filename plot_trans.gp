load 'std.gp'
set size square
set key box center right Left reverse
set title "Kuramoto Model: r vs K"
set xlabel "Degree of coupling, K"
set ylabel "Order parameter, r"
set label at graph 1, graph 1.02 right "g(w): Gaussian" font font.",16"
plot ARG1 i 1 pt 7 not, '' i 1 smooth bezier lw 1.5 t "N = 25 ", \
     ARG1 i 0 smooth bezier lw 1.5 t "N = 50 ", '' i 0 pt 6 lc black not, \
     1/0 not, ARG1 i 2 pt 7 not, '' i 2 smooth bezier lw 1.5 t "N = 75 "
