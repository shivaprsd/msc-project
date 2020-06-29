N = 100
beg = (ARG2 - 1) * N

load "../../std.gp"
set key bottom
plot ARG1 every ::(beg)::(beg + N - 1) u 0:2 w l t ARG2
