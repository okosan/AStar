#pragma once

#include "arr2d.h"

#include <stdint.h>

static const float CELL_CLEAR = 0;
static const float CELL_WALL = 1.f;
static const float CELL_SRC = 1000.f;
static const float CELL_DST = 2000.f;

// TODO: outdated convention
// checks if result is "failed"
bool XFAIL(int ret);

struct XCOORD
{
    uint32_t x,y;
    uint32_t dir_index;
};

struct SIBLING_STRUCT
{
    const static uint32_t MAX_SIBLINGS = 8; // all siblings around
    uint32_t nSiblings;
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

    int getSiblingInfo(uint32_t x, uint32_t y, SIBLING_STRUCT &sib);
};  // MAPDATA
