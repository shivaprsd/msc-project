f = "Helvetica Neue"
set term png size 1000,1000 font "/Library/Fonts/".f.".ttc,14"
set samples 1000
set size square
unset border
unset xtics
unset ytics

stats ARG1 noout
N = STATS_records / (STATS_blocks - 1)
n = sqrt(N) + 1
set xrange [-n-1:n+1]
set yrange [-n-1:n+1]
set tmargin 5
set label at graph 0.5, graph 1.05 center "Kuramoto Oscillators on Complex Networks" font f.",20"
set label at graph 1, graph 1.01 right system("head -1 test.dat | cut -d '#' -f 2") font f.",16"

th(n) = 2 * pi * n / N
a(i) = px[i + 1]
b(i) = py[i + 1]

array px[N]; array py[N]
set table $ring
plot ARG1 i 0 u (px[$1] = n * cos(th($1))):(py[$1] = n * sin(th($1))):2 w table
unset table

r = 0.4
do for [j = 1 : STATS_blocks - 2] {
    set out 'tmp/img'.sprintf("%.4d", int(j)).'.png'
    plot $ring u 1:2:(px[$3] - $1):(py[$3] - $2) w vectors nohead lc 7 lw 2 not, \
         ARG1 i j u (a($0)):(b($0)):(r):($2 > 0.98 ? 5 : 0) w circles fs solid lc variable not, \
         ARG1 i j u (a($0)):(b($0)):(r * $1):(r * $2) w vectors lc 8 lw 2.5 not
}
