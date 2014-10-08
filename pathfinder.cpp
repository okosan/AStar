#include "stable.h"

#include "pathfinder.h"

int PATHFINDER::saveToFile(char *fn)
{
    FILE *f = fopen(fn,"wt");
    if (!f) return 2;
    /*
    fprintf(f,"%d",w,h);
    for (int y = 0;y<h; y++)
    for (int x=0; x<w; x++)
    fprintf(f,"%f ", arrCost.getItem(x,y));

    for (int y = 0;y<h; y++)
    for (int x=0; x<w; x++)
    fprintf(f,"%f ", arrHeight.getItem(x,y));
    */

    if (!fclose(f)) return 7;

    return 0;

}
