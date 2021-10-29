FILENAME = ARG1

print FILENAME

# start fit parameters:
a = 1
b = 1

stats FILENAME using 1 name "X" # for normalize
stats FILENAME using 2 name "Y" # for normalize

f(x) = a*x+b
fitfrom = -4
fit [fitfrom:] f(x)  FILENAME using (log($1)):(log($2)) via a,b

set xlabel "log(r)"
set ylabel "log(I)"

plot FILENAME using (log($1)):(log($2)) with linespoints dt 3 pt 7 t '', \
     f(x) with lines lw 2 lc rgb 'red' \
     t sprintf("a = %f +/- %f,\nb = %f +/- %f",a,a_err,b,b_err)

# plot FILENAME using ($1/X_max/2):($2/Y_max) with linespoints dt 3 pt 7 t ''

pause -1