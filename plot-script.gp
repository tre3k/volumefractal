FILENAME = ARG1

print FILENAME

# start fit parameters:
a = 1
b = 1

stats FILENAME using 1 name "X" # for normalize
stats FILENAME using 2 name "Y" # for normalize

f(x) = a*x+b
fitfrom = 1.6
fitto = 7
fit [fitfrom:fitto] f(x)  FILENAME using (log($2)):(log($3)) via a,b

set xlabel "log(r)"
set ylabel "log(I)"

plot FILENAME using (log($2)):(log($3)) with linespoints dt 3 pt 7 t '', \
      [fitfrom:fitto] f(x) with lines lw 2 lc rgb 'red' \
      t sprintf("a = %f +/- %f,\nb = %f +/- %f",a,a_err,b,b_err)

# plot FILENAME using ($1*pi):($2/Y_max) with linespoints dt 3 pt 7 t ''

pause -1
