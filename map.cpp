#include "stable.h"

#include "map.h"

MAPDATA::MAPDATA()
{
    w=0;
    h=0;
}

MAPDATA::~MAPDATA()
{
    //f_init=0;

}

int MAPDATA::allocate(int w_,int h_)
{
    if ((w_<1)||(h_<1))
        return 1;

    try
    {
        if (XFAIL(arrCost.alloc(w_,h_))) throw 1;
        if (XFAIL(arrHeight.alloc(w_,h_))) throw 2;
    }
    catch(int ec)
    {
        destroy();
        return 1;
    }

    w = w_;
    h = h_;

    return 0;
}

int MAPDATA::destroy()
{
    arrCost.destroy();
    arrHeight.destroy();
    w = 0;
    h = 0;

    return 0;
}

int MAPDATA::setCost(int x,int y,float cost)
{
    if (isValid(x,y))
    {
        arrCost.setItem(x,y,cost);
        return 0;
    }
    return 1;
}

int MAPDATA::getCost(int x,int y,float &cost)
{
    if (isValid(x,y))
    {
        arrCost.getItem(x,y,cost);
        return 0;
    }
    cost = CELL_WALL; // !!!
    return 1;
}

int MAPDATA::setCellHeight(int x,int y,float height)
{
    if (isValid(x,y))
    {
        arrHeight.getItem(x,y,height);
        return 0;
    }
    return 1;
}

int MAPDATA::getCellHeight(int x,int y,float &height)
{
    if (isValid(x,y))
    {
        arrHeight.getItem(x,y,height);
        return 0;
    }
    return 1;
}

bool MAPDATA::isValid(int x,int y)
{
    if ( (x >= w) || (y>=h) || (x < 0) || (y < 0) )
        return 0;
    return 1;
};

int MAPDATA::printMapDbg()
{
    FILE *f = fopen("map.dbg1.txt","wt");
    fprintf(f,"\t Cost data output:\n");
    for (int y=0;y<h;y++)
    {
        for (int x=0;x<w;x++)
        {
            float cost;
            if (XFAIL(arrCost.getItem(x,y,cost)))
            {
                fprintf(f,"\nfailed to get cost for %d:%d\n",x,y);
                break;
            }
            else
            {
                if (cost == CELL_DST) fprintf(f," d");
                else
                    if (cost == CELL_SRC) fprintf(f," s");
                    else
                        fprintf(f,"%2f",cost);
            };
        };
        fprintf(f,"\n");
    };
    return 0;
}

int MAPDATA::saveToFile(char *filename)
{
    FILE *f = fopen(filename,"wt");
    if (!f) return 2;
    fprintf(f,"%d %d ",w,h);
    for (int y = 0;y<h; y++)
        for (int x=0; x<w; x++)
            fprintf(f,"%f ", arrCost.getItem(x,y));

    for (int y = 0;y<h; y++)
        for (int x=0; x<w; x++)
            fprintf(f,"%f ", arrHeight.getItem(x,y));

    if (!fclose(f)) return 7;

    return 0;
}

int MAPDATA::loadFromFile(char *filename)
{
    destroy();

    FILE *f = fopen(filename,"rt");
    if (!f) return 2;
    fscanf(f,"%d %d ",&w,&h);

    allocate(w,h);

    for (int y = 0;y<h; y++)
        for (int x=0; x<w; x++)
            fscanf(f,"%f ", arrCost.getItemPtr(x,y));

    for (int y = 0;y<h; y++)
        for (int x=0; x<w; x++)
            fscanf(f,"%f ", arrHeight.getItemPtr(x,y));

    if (!fclose(f)) return 7;

    return 0;
}

int MAPDATA::clear(int clearheight)
{
    for (int y = 0;y<h; y++)
        for (int x=0; x<w; x++)
            arrCost.setItem(x,y,CELL_CLEAR);

    if (clearheight)
    {
        for (int y = 0;y<h; y++)
            for (int x=0; x<w; x++)
                arrHeight.setItem(x,y,0.0f);
    }

    return 0;
}

int MAPDATA::resize(int w_,int h_)
{
    try
    {
        if (XFAIL(arrCost.resize(w_,h_))) throw 1;
        if (XFAIL(arrHeight.resize(w_,h_))) throw 2;
    }
    catch(int ec)
    {
        destroy();
        return 1;
    }

    w = w_;
    h = h_;

    return 0;
}

int MAPDATA::translation(int tx,int ty,float fill_value)
{
    // translate cost map & array map
    arrCost.translate(tx,ty,fill_value);
    arrHeight.translate(tx,ty,fill_value);

    return 0;
}

int MAPDATA::getSiblingInfo(UINT x, UINT y,SIBLING_STRUCT &sib)
{
    assert(isValid(x,y));
    for (UINT i = 0; i<SIBLING_STRUCT::MAX_SIBLINGS;i++)
    {

    }
    return 0;
};

