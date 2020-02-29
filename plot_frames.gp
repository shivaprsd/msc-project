reset
datafiles = system("ls tmp/out*")
ones_foll = system("cat dclnet/ones-foll.txt")
load 'std.gp'
set xlabel "Node, i\n"
set ylabel "Phase, θ_i"
set title "dclnet\\_ni5\\_p07: oscillator dynamics\n"
set label at 0, 2.08 * pi sprintf("σ = %s,  Ω (%s)", ARG1, ARG2) font fnt.",16"
set xrange [-5:1005]
set yrange [0:2 * pi]
set cbrange [0:1]
set tics scale 0
set xtics 100
set ytics (0, "π/2" 0.5 * pi, "π" pi, "3π/2" 1.5 * pi, "2π" 2 * pi)
set key at 1130, 2.2 * pi Left reverse horizontal

array clr[1000]
clr[1] = 7
do for [i=2:1000] { clr[i] = 2 }
do for [s in ones_foll] { clr[s] = 4 }
h = 1e-3
t = 1
do for [f in datafiles] {
    plot f every ::::999 u ($0 + 1):1:(clr[$0 + 1]) pt 6 lw 1.7 lc var \
         title "not connected to 1", \
         1/0 w p pt 6 lc 4 t "connected to 1", \
         1/0 lc "white" title sprintf("t = %.3f", h * t)
    t = t + 1
    pause 0.02

    #set palette defined (0 "red", 1 "white")
    #plot f every ::::999 u (10 * (int($0) % 100)):(-0.1 * (int($0) / 100) - 0.1):(sin(($1) / 2) ** 2) pt 5 ps 2 lc palette not
    #plot f every ::::999 u (0.0):(0.0):(cos($1)):(sin($1)):(sin($1/2) ** 2 - STATS_mean) w vec nohead lc palette not
}
