reset
set xlabel 'N'
set ylabel 'time(s)'
set title 'clz-test runtime'
set term png enhanced font 'Verdana,10'
set output 'runtime.png'

plot [0:][0:] 'runtime.csv' using 1:2 with points title "recursive", \
'' using 1:3 with points title "iteration",\
'' using 1:4 with points title "binary search", \
'' using 1:5 with points title "byte shift" ,\
'' using 1:6 with points title "harley", 