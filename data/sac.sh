R=300/700/27/43.5
J=X10i/6i
PS=sac.ps
PDF=sac.pdf

gmtset LABEL_FONT 5
gmtset LABEL_FONT_SIZE 20
gmtset ANNOT_FONT_PRIMARY 5
gmtset ANNOT_FONT_SIZE_PRIMARY 15

psxy -R$R -J$J -K -T > $PS
saclst gcarc f *.SAC | awk '{print $1,0,$2}' | pssac2 -R -J -K -O -B40f4:"Time(sec)":/2f0.5:"Distance(deg)":WSen \
    -M0.5 -W1p -C300/700 >> $PS
psxy -R -J -O -T >> $PS

ps2pdf $PS $PDF
rm $PS .gmt*
evince $PDF
