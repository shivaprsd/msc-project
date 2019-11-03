#!/bin/bash
if [ "$1" != "-nc" ]; then
    echo "Compiling and running..."
    cc -I "../class" kuramoto.c util.o &&
        ./a.out > test.dat
fi
echo "Plotting..."
gnuplot -c plot_osc.gp test.dat &&
    ffmpeg -y -hide_banner -loglevel panic -framerate 50 -i tmp/img%4d.png \
           -pix_fmt yuv420p test.mp4 &&
        rm tmp/*.png
echo "Finished"
open test.mp4
