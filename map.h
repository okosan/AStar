#pragma once
#include "stable.h"

#include "arr2d.h"

static const float CELL_CLEAR = 0;
static const float CELL_WALL = 1.f;
static const float CELL_SRC = 1000.f;
static const float CELL_DST = 2000.f;

struct XCOORD
{
    UINT x,y;
    UINT dir_index;
};

struct SIBLING_STRUCT
{
    const static UINT MAX_SIBLINGS = 8; // all siblings around
    UINT nSiblings;
    XCOORD coords[MAX_SIBLINGS];
};


class MAPDATA
{
private:
    int w;
    int h;

    ARR2D<float> arrCost;
    ARR2D<float> arrHeight;

public:
    MAPDATA();
    virtual ~MAPDATA();

    int allocate(int w,int h);
    int destroy();

    int resize(int w,int h);

    int setCost(int x,int y,float cost);
    int getCost(int x,int y,float &cost);
    int setCellHeight(int x,int y,float height);
    int getCellHeight(int x,int y,float &height);

    bool isValid(int x, int y);

    int printMapDbg();

    int clear(int clearheight = 1);
    int translation(int tx,int ty,float fill_value = 0);

    inline int height() { return h; }
    inline int width() { return w; }

    /*
    void GenerateStatic();
    int GenerateMap(float q, float m); // q = % of empty space, m - mazity
    */

    int saveToFile(char *filename);
    int loadFromFile(char *filename);

    int getSiblingInfo(UINT x, UINT y, SIBLING_STRUCT &sib);
};  // MAPDATA
