#pragma once

#include "map.h"
#include "cell.h"

#include <list>

class PATHFINDER
{
public:
    static const int FINDFLAG_SHALLOW = 0;
    static const int FINDFLAG_DEEP = 1;

    PATHFINDER();

    int init(MAPDATA *map_);

    // return value < 0 if faulty or path not found
    float getPathLength() const;
    float getPathCost() const;

    void setCellG(float G, CELL *cCell);

    int clearPath();

    void printCellData(FILE *f, const char *str, const CELL &c);

    bool find(int findflag = 0, const int verbose = 0);
    bool find(int srcX, int srcY, int dstX, int dstY, int findflag = 0, const int verbose = 0);

    // path length between two sibling cells
    // function will return wrong result if distance > 1 cell (i.e. cell not sibling)
    float getGsibling(int xs, int ys, int xd, int yd, int f_deep = 0);
    float getGsibling(const CELL &c0, const CELL &c1);

    // taxicab metric
    float manhattan_length(int x0, int y0, int x1, int y1);
    float euclidian_length(int x0, int y0, int x1, int y1);
    float Heuristics(int x0, int y0, int x1, int y1);

    std::list<CELL>::iterator findInOpen(int ix, int iy);
    std::list<CELL>::iterator findInOpen(CELL *c_);
    std::list<CELL>::iterator findInClosed(int ix, int iy);
    std::list<CELL>::iterator findInClosed(CELL *c_);
    std::list<CELL>::iterator findOpenMinimalF();


    // generates path list. called inside of find
    // TODO:: put dst in closed list before generating path, free open list
    int genPathList(int verbose);

    int findOnMap(float findCost, int &ixf, int &iyf);

    int findSrcLocation(CELL &cellSrc);

    int findDstLocation(CELL &cellDst);

    int getSiblingDF(int ix, int iy, int index, int &sx, int &sy, float &cost);

    // diagonal check function
    int getSiblingDC(int ix, int iy, int index, int &sx, int &sy, float &cost);

    // returns sibling cell to Base. writes to retSibling.
    int getSibling(const CELL *base, int index, CELL *retSibling, float &cost);

    int saveToFile(char *fn);

    // FIXME: make private member
    std::list<CELL> pathList;

private:
    MAPDATA *map;
    std::list<CELL> open;
    std::list<CELL> closed;

    bool pathFound;
    float pathLength;  // in units of cells
    float pathCost;  // in units of power

    // TODO: move to locals of find function
    CELL src;
    CELL dst;
};  // class PATHFINDER
