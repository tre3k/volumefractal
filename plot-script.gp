FILENAME = ARG1

print FILENAME

a = 1
b = 1

f(x) = a*x+b
fitfrom = -4
fit [fitfrom:] f(x)  FILENAME using (log($1)):(log($2)) via a,b

set xlabel "log(r)"
set ylabel "log(I)"

plot FILENAME using (log($1)):(log($2)) pt 7 t '', \
     f(x) with lines lw 2 lc rgb 'red' \
     t sprintf("a = %f +/- %f,\nb = %f +/- %f",a,a_err,b,b_err)

pause -1