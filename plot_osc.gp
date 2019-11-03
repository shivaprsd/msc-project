f = "Helvetica Neue"
set term png size 1000,1000 font "/Library/Fonts/".f.".ttc,14"
set samples 1000
set tics font f.",14"
set key outside font f.",14"
set grid

set title "Kuramoto Oscillators" font f.",20"
set xtics 1
set ytics 1

stats ARG1 noout
N = STATS_records / (STATS_blocks - 1)
set xrange [0 : sqrt(N)]
set yrange [0 : sqrt(N)]
set label at graph 1, graph 1.02 right system("head -1 test.dat | cut -d '#' -f 2") font f.",16"

do for [j = 0 : STATS_blocks - 2] {
    set out 'tmp/img'.sprintf("%.4d", int(j)).'.png'
    n = int(sqrt(N))
    plot ARG1 i j u (0.5 + int($0) % n):(0.5 + int($0) / n):(0.5 * $1):(0.5 * $2):3 \
         w vectors lw 2.5 lc variable not
}
