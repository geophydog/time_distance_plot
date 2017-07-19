#include <stdio.h>
#include <stdlib.h>
#include "sacio.h"

void no_spa(char *ps){
        char *pt = ps;
        while ( *ps != '\0'  ){
            if ( *ps != ' ' && *ps != '\n'  ){
                *pt++ = *ps;
            }
            ++ps;
        }
        *pt = '\0';
}


int main(int argc, char *argv[]) {
    int i, j, npts, begin_index;
    float *data, **amp, dist, t1, t2, time, b, e, delta, distmin = 180., distmax = 0.;
    char ss[256];
    SACHEAD hd;
    FILE *f, *fout, *fp;

    if( argc != 5 ) {
        fprintf(stderr,"+-----------------------------------------------------------------------------------------+\n");
        fprintf(stderr,"+--- Usage: dist-time <t1> <t2> <sacfile_lst> <out_results_file_name>                  ---+\n");
        fprintf(stderr,"+---     return \"out_results_file_name\" saving time-distance and amplitude info        ---+\n");
        fprintf(stderr,"+---  <sacfile_lst> file contsaining SAC file names, just 1 columns;                   ---+\n");
        fprintf(stderr,"+---  <t1> beginning time of inputting SAC files;                                      ---+\n");
        fprintf(stderr,"+---  <t2> ending time of inputting SAC files;                                         ---+\n");
        fprintf(stderr,"+---  <out_results_file_name> file saving time-distance mapping;                       ---+\n");
        fprintf(stderr,"+---  <and \"plot.sh\" will be saved to plot time-distance mapping                       ---+\n");
        fprintf(stderr,"+---  just execute \"sh plot.sh\" to plot time-disance power                             ---+\n");
        fprintf(stderr,"+---  ATTENTION!!! Executing \"sh plot.sh\" requires GMT(the Generic Mapping Tools)      ---+\n");
        fprintf(stderr,"+---                                                 major version 5, here 5.3.1       ---+\n");
        fprintf(stderr,"+-----------------------------------------------------------------------------------------+\n");
        exit(1);
    }

    f = fopen(argv[1],"r"); t1 = atof(argv[2]); t2 = atof(argv[3]); fout = fopen(argv[4],"w"); fp = fopen("plot.sh","w");

    while ( fgets(ss,256,f) ) {
        no_spa(ss);
        data = read_sac(ss,&hd);
        delta = hd.delta;
        b = hd.b;
        begin_index = (int)((t1-b)/delta);
        npts = (int)((t2-t1)/delta);
        dist = hd.gcarc;
        if ( hd.gcarc > distmax ) distmax = hd.gcarc;
        if ( hd.gcarc < distmin ) distmin = hd.gcarc;
        for ( i = 0; i < npts; i ++ ) {
            time = t1+i*delta;
            fprintf(fout,"%f %f %f\n", time, dist, data[begin_index+i]);
        }
        free(data);
    }

    fprintf(fp,"gmt psxy -R%.2f/%.2f/%.3f/%.3f -JX8i/6i -K -T>plot.ps\n", t1, t2, distmin, distmax);
    fprintf(fp,"gmt surface %s -R -I%.4f/%.4f -G%s.grd\n", argv[4], (t2-t1)/400, (distmax-distmin)/400,argv[4]);
    fprintf(fp,"gmt grd2cpt %s.grd -Cseismic>tmp.cpt\n",argv[4]);
    fprintf(fp,"gmt grdimage %s.grd -R -J -K -O -Bx%.2f+l\"Time(sec)\" -By%.3f+l\"Distance(degree)\" -BWSen -Ctmp.cpt>>plot.ps\n",argv[4], (t2-t1)/10, (distmax-distmin)/10);
    fprintf(fp,"gmt psxy -R -J -O -T>>plot.ps\n");
    fprintf(fp,"ps2pdf plot.ps plot.pdf\n");
    fprintf(fp,"rm gmt.*\n");
    fprintf(fp,"evince plot.pdf\n");
    fclose(f);
    fclose(fout);
    fclose(fp);

    return 0;
}

