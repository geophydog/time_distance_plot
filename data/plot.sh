gmt gmtset FONT_LABEL 20p,5,black
gmt gmtset FONT_ANNOT_PRIMARY 15p,5,black
gmt psxy -R300.00/700.00/27.472/43.144 -JX8i/6i -K -T>plot.ps
gmt surface out.txt -R -I4.0000/0.3134 -Gout.txt.grd
gmt grd2cpt out.txt.grd -Cpolar>tmp.cpt
gmt grdimage out.txt.grd -R -J -K -O -Bx40.00f8.00+l"Time(sec)" -By1.567f0.31+l"Distance(degree)" -BWSen -Ctmp.cpt>>plot.ps
gmt psxy -R -J -O -T>>plot.ps
gmt psconvert -A -P -E300 -Tt plot.ps
ps2pdf plot.ps plot.pdf
rm gmt.*
evince plot.pdf
