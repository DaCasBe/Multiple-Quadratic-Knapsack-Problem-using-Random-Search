#!/bin/bash

cat << _end_ | gnuplot
set terminal postscript eps color
set output "grafico8.eps"
set key right bottom
set xlabel "Iteración"
set ylabel "Beneficio"
plot 'inst4_5.txt' using 1 t "Mejor Encontrado" w l, 'inst4_5.txt' using 2 t "Valores medios" w l,
_end_
