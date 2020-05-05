reset
datafiles = system("ls tmp/out*")
load 'std.gp'
set xlabel "Clique (5-cell), i\n"
set ylabel "Order parameter, |r_i|"
set title "dclnet\\_ni5\\_p07:  simplical dynamics\n"
set label at 0, 1.04 sprintf("σ = %s,  Ω (%s)", ARG1, ARG2) font fnt.",16"
set xrange [:988]
set yrange [0:1]
set tics scale 0
set xtics 100
set key at 1170, 1.1 Left reverse horizontal samplen 0

h = 1e-3
t = 1
do for [f in datafiles] {
    plot f u ($0 + 1):1:2 pt 7 ps 0.6 lc var title "cliques not containing 1", \
         1/0 w p pt 7 ps 0.6 lc 7 t "cliques containing 1", \
         1/0 lc "white" title sprintf("t = %.3f", h * t)
    t = t + 1
    pause 0.02

    #set palette defined (0 "red", 1 "white")
    #plot f every ::::999 u (10 * (int($0) % 100)):(-0.1 * (int($0) / 100) - 0.1):(sin(($1) / 2) ** 2) pt 5 ps 2 lc palette not
    #plot f every ::::999 u (0.0):(0.0):(cos($1)):(sin($1)):(sin($1/2) ** 2 - STATS_mean) w vec nohead lc palette not
}
