#pragma once
#include "stable.h"

#include "map.h"


// Much more faster data structure
/*
CELL1
ix,iy
f,g,h
int par_ix;
int par_iy;
CELL1 *parent;
*/

/*
1) arr2d make linear array with single pointer
2) bool array of map_closed
2) map_open class (find (ix,iy),etc).
3) CELL factory
4) getSibling(index) -> genSibling (SIBLING_STRUCT -> array of 8 siblings) and getSibling is
*/

/*
CELL, PATHFINDER, MAP

Open:
*/


class CELL
{
private:
    char f_closed;
public:
    int ix;
    int iy;
    int parent_ix, parent_iy;

    float f,g,h;

    inline bool hasParent() { return (parent_ix < 0 || parent_iy < 0)?0:1; }
    bool inline isClosed() { return f_closed; }
    void inline setClosed() { f_closed = 1; }

    CELL() : parent_ix(-1),parent_iy(-1),g(0),h(0),f(0),f_closed(0) { }
    CELL(int ix_,int iy_,float Gs_,float H_,const CELL *parent_):
        ix(ix_),iy(iy_),h(H_)
    {
        if (parent_)
        {
            setParent(parent_->ix,parent_->iy);
            g = parent_->g + Gs_;
        }
        else
        {
            setParent();
            g = Gs_;
        }
    }

    inline void setParent(int ix_=-1,int iy_=-1)
    {
        parent_ix = ix_;
        parent_iy = iy_;
    }

    inline void setParent(CELL *c)
    {
        parent_ix = c->ix;
        parent_iy = c->iy;
    }

    inline void init(CELL parent_,float gs,float h)
    {
        parent_ix = parent_.ix;
        parent_iy = parent_.iy;
    }

    CELL & operator=(const CELL &cell2)
    {
        if (&cell2 == this) return *this;
        ix = cell2.ix;
        iy = cell2.iy;
        f = cell2.f;
        g = cell2.g;
        parent_ix = cell2.parent_ix;
        parent_iy = cell2.parent_iy;
        return *this;
    }

    // compare passability of cells (ground resistance)
    bool operator ==(const CELL &cell2)
    {
        if ((cell2.ix==ix)&&(cell2.iy==iy))
            return true;
        return false;
    }

    bool operator >(const CELL &cComp)
    {
        if (this->f > cComp.f)
            return true;
        else return false;
    }

    bool operator <(const CELL &cComp)
    {
        if (this->f < cComp.f)
            return true;
        else return false;
    }

    // are cells different in passability?
    bool operator !=(const CELL &cComp)
    {
        if ((cComp.ix!=ix)||(cComp.iy==iy))
            return true;
        else return false;
    }

}; // CELL

class PATHFINDER
{
private:
    MAPDATA *map;
    std::list<CELL> open;
    std::list<CELL> closed;

    bool pathFound;
    float pathLength; // in units of cells
    float pathCost; // in units of power
    CELL src,dst;


public:
    static const int FINDFLAG_SHALLOW = 0;
    static const int FINDFLAG_DEEP = 1;

    PATHFINDER() : map(0), pathFound(0), pathLength(0), pathCost(0) { }

    std::list<CELL> pathList;

    int init(MAPDATA *map_)
    {
        map = map_;
        pathFound = 0;
        pathLength = std::numeric_limits<float>::min(); // !!!
        return 0;
    }

    // return value < 0 if faulty or path not found
    float getPathLength() { return pathLength; }
    float getPathCost() { return pathCost; }

    inline void setCellG(float G,CELL *cCell)
    {
        if (cCell->hasParent())
        {
            cCell->g = G + getGsibling(cCell->ix,cCell->iy,
                cCell->parent_ix,cCell->parent_iy);
        }
        else
            cCell->g = G;
    }

    int clearPath()
    {
        pathFound = 0;
        pathLength = 0;
        pathCost = 0;

        open.clear();
        closed.clear();
        pathList.clear();
        return 0;
    }

    void printCellData(FILE *f, char *str, const CELL &c)
    {
        fprintf(f,"%s. Cell data: <%d,%d>, g = %f, h = %f, f = %f. Parent = <%d,%d>\n",
            str,
            c.ix, c.iy,
            c.g, c.h, c.f,
            c.parent_ix,c.parent_iy);
        fflush(f);
    }

    int find(int findflag = 0,const int verbose = 0)
    {
        pathFound = 0;
        pathLength = 0;
        pathCost = -1;

        FILE *f = 0;
        if (verbose) f = fopen("dbg_find.txt","wt");

        if (XFAIL(findSrcLocation(src))) return 1;
        if (XFAIL(findDstLocation(dst))) return 2;

        src.g = 0;
        src.h = Heuristics(src.ix,src.iy,dst.ix,dst.iy);
        src.f = src.g + src.h;

        if (verbose) printCellData(f,"SRC",src);
        if (verbose) printCellData(f,"DST",dst);

        // erase lists
        open.clear();
        closed.clear();

        //open.reserve(map->width()*map->height());
        //closed.reserve(map->width()*map->height());

        ARR2D<char> arrCellClosed(map->width(),map->height());;

        open.push_back(src); // add start point to open list

        while(1)
        {
            // search for dest item in open list
            std::list<CELL>::iterator destIter = findInOpen(&dst);
            if (destIter != open.end())
            {
                if (verbose) printCellData(f,"! found dst cell in open list",(CELL)(*destIter));
                pathCost = destIter->g; // now save path length (this is path cost)
                pathFound = 1;
                break;
            };

            // find with the lowest total score     cell in open list
            std::list<CELL>::iterator currIter = findOpenMinimalF();
            if (currIter == open.end())
            {
                if (verbose) fprintf(f,"open list is empty. Exitting loop.\n");
                break; // open list is empty
            }

            CELL current = *currIter;

            if (verbose) printCellData(f,"> Current = ",current);

            closed.push_back(current);// add to closed list
            arrCellClosed.setItem(current.ix,current.iy,1); // mark as closed (!!!)

            open.erase(currIter); // remove from open list

            for (int index = 0; index < 8; index++)
            {
                CELL cellSibling;
                float cost;
                if (XFAIL(getSibling(&current,index,&cellSibling,cost))) return 0;

                if (cost == CELL_WALL) continue; // simply skip walls

                if (verbose) fprintf(f," * found sibling i = %d\n\t",index);
                if (verbose) printCellData(f,"\t Sibling = ",cellSibling);

                ///*
                if (arrCellClosed.getItem(cellSibling.ix,cellSibling.iy)!=0)
                {
                    if (verbose) fprintf(f,"\t ... (!) but it is marked as closed. skip it\n");
                    continue; // ignore in closed list
                };
                //*/
                // search in closed list for current sibling
                /*
                if (findInClosed(&cellSibling) != closed.end())
                {
                if (verbose) fprintf(f,"\t ... but the sibling is in CLOSED list. skip it\n");
                continue; // ignore in closed list
                }
                */


                // cost to move: current->sibling
                float g_sibling;
                bool f_use_costing = 1;
                if (f_use_costing)
                    g_sibling = getGsibling(current.ix,current.iy,cellSibling.ix,cellSibling.iy)*(1.0+3.5*cost);
                else
                    g_sibling = getGsibling(current.ix,current.iy,cellSibling.ix,cellSibling.iy);
                float g_move = current.g + g_sibling;
                if (verbose) fprintf(f, "\t g_cost from current to sibling is = %f\n", g_move);

                // compute cost

                // search in open list
                std::list<CELL>::iterator openItem = findInOpen(&cellSibling);
                if (openItem != open.end()) // found in open list
                {
                    // sibling has a copy in open.
                    // compute cost
                    if (verbose) fprintf(f,"\t ... and this cell is already in open list\n\t");
                    if (verbose) printCellData(f,"\t > openItem info: ",(CELL)(*openItem));

                    /*
                    float g_sibling_alt = getGsibling(current.ix,current.iy,openItem->ix,openItem->iy);
                    if (g_sibling != g_sibling_alt)
                    {
                    Beep(1000,1000);
                    };
                    */

                    float alternative_G = current.g + g_sibling;
                    //float alternative_G = current.g + getGsibling(cellSibling.parent_ix,cellSibling.parent_iy,openItem->ix,openItem->iy);
                    //float G_by_found = cellSibling.g + getGsibling(cellSibling.ix,cellSibling.iy,openItem->ix,openItem->iy);
                    if (verbose) fprintf(f,"\t ... and alternative G if to go thru this item is %f\n",alternative_G);

                    CELL openC = *openItem;
                    if (openItem->g < alternative_G)
                    {
                        if (verbose) fprintf(f,"\t ... but open item is itself %f < alternative %f. so skip it\n", openItem->g, alternative_G);
                        continue; // current path is less efficent than existing in open
                    }
                    else
                    {
                        // openItem->g > alternative_G
                        // The path, previously added to open is no more efficent
                        // cost from point X to D costs more than from Curr to (Sibling to D)
                        // change siblings parent, and recompute G,F
                        if (verbose) fprintf(f,"\t ... and Yes. it is better to go our cell than thru existing in list\n");
                        if (verbose) fprintf(f,"\t              because openG > alternative_G = %f > %f\n", openItem->g, alternative_G);

                        openItem->setParent(&current); // modify cell

                        openItem->g = alternative_G;
                        openItem->f = openItem->g + openItem->h; // H is already computed for this item

                        if (verbose) fprintf(f,"\t .... so we modified existed cell as if we went there thru sibling\n");
                        if (verbose) printCellData(f,"\t openItem Modified info is",(CELL)(*openItem));
                    };
                }
                else
                {
                    if (verbose) fprintf(f,"\t ... and this cell is not in open list so we add it to open\n\t");
                    // add to list. not found in open or closed
                    cellSibling.g = g_move;

                    // deep search means no heuristics
                    if (findflag & FINDFLAG_DEEP) cellSibling.h = 0;
                    else cellSibling.h = Heuristics(cellSibling.ix,cellSibling.iy,dst.ix,dst.iy);

                    cellSibling.f = cellSibling.g + cellSibling.h;
                    if (verbose) printCellData(f,"\t New sibling push to OPEN info:",cellSibling);

                    open.push_back(cellSibling);
                } // else

            }; // for siblings
        } // while(1)
        if (verbose) fprintf(f,"\n Loop finished\n");

        // print contaminent of lists
        if (verbose)
        {
            fprintf(f,"\n\n\t\t OPEN LIST");

            for (std::list<CELL>::iterator i = open.begin(); i != open.end(); ++i)
            {
                printCellData(f,"\t\t",(CELL)(*i));
            }

            fprintf(f,"\n\n\t\t CLOSED LIST:");
            for (std::list<CELL>::iterator i = closed.begin(); i != closed.end(); ++i)
            {
                printCellData(f,"\t\t",(CELL)(*i));
            }
        }

        if (verbose) fclose(f);

        genPathList();

        open.clear();
        closed.clear();

        return 0;
    }

    int findfast()
    {

        return 0;
    }

    // path length between two sibling cells
    // function will return wrong result if distance > 1 cell (i.e. cell not sibling)
    inline float getGsibling(int xs,int ys,int xd, int yd,int f_deep = 0)
    {
        if (fabs((float)(xd-xs))+fabs((float)(yd-ys)) <= 1.0f) return 1;
        else return sqrt(2.0f);
    }

    inline float getGsibling(const CELL &c0,const CELL &c1)
    {
        return getGsibling(c0.ix,c0.iy,c1.ix,c1.iy);
    }

    // taxicab metric
    inline float manhattan_length(int x0,int y0,int x1,int y1)
    {
        return fabs((float)(x1-x0)) + fabs((float)(y1-y0));
    }

    inline float euclidian_length(int x0,int y0,int x1,int y1)
    {
        return sqrt(SQR((float)(x1-x0)) + SQR((float)(y1-y0)) );
    }

    inline float Heuristics(int x0,int y0,int x1,int y1)
    {
        //return manhattan_length(x0,y0,x1,y1); //
        return euclidian_length(x0,y0,x1,y1); // gives much better results
    }

    inline std::list<CELL>::iterator findInOpen(int ix,int iy)
    {
        std::list<CELL>::iterator openItem;
        for (openItem = open.begin(); openItem != open.end(); ++openItem)
        {
            if ((openItem->ix == ix)&&(openItem->iy == iy))
            {
                return openItem;
            };
        }
        return open.end();
    }

    inline std::list<CELL>::iterator findInOpen(CELL *c_)
    {
        return findInOpen(c_->ix,c_->iy);
    }

    inline std::list<CELL>::iterator findInClosed(int ix,int iy)
    {
        std::list<CELL>::iterator closedItem;
        for (closedItem = closed.begin(); closedItem != closed.end(); ++closedItem)
        {
            if ((closedItem->ix == ix)&&(closedItem->iy == iy))
            {
                return closedItem;
            };
        }
        return closed.end();
    }

    inline std::list<CELL>::iterator findInClosed(CELL *c_)
    {
        return findInClosed(c_->ix,c_->iy);
    }

    inline std::list<CELL>::iterator findOpenMinimalF()
    {
        std::list<CELL>::iterator min_iter=open.begin();
        for (std::list<CELL>::iterator found = open.begin();
            found != open.end(); ++found)
        {
            if (found->f < min_iter->f)
                min_iter = found;
        };
        return min_iter;
    }

    /*
    int genPathList()
    {
    // generate path now
    pathList.clear();
    if (!pathFound) return 1;

    std::list<CELL>::iterator pathItem = findInOpen(dst.ix,dst.iy);
    if (pathItem==open.end()) return 1;
    pathList.push_back(*pathItem);

    while(1)
    {
    CELL lastCell = pathList.back();
    if (lastCell == src) break;

    std::list<CELL>::iterator pathItem = findInOpen(lastCell.parent_ix,lastCell.parent_iy);
    if (pathItem==open.end())
    {
    //return 2;
    pathItem = findInClosed(lastCell.parent_ix,lastCell.parent_iy);
    if (pathItem==closed.end())
    return 2;
    };

    pathList.push_back(*pathItem);
    }

    return 0;
    }
    */

    // generates path list. called inside of find
    // TODO:: put dst in closed list before generating path, free open list
    int genPathList(int verbose = 1)
    {
        FILE *f = 0;
        if (verbose) f = fopen("path_debug.txt","wt");

        // generate path now
        pathList.clear();
        if (!pathFound) return 1;

        std::list<CELL>::iterator pathItem = findInOpen(dst.ix,dst.iy);
        if (pathItem==open.end()) return 1;
        /*
        // search in CLOSED

        pathItem = findInClosed(lastCell.parent_ix,lastCell.parent_iy);
        if (pathItem==closed.end())
        return 2;
        */
        pathList.push_back(*pathItem);

        if (verbose)
        {
            CELL c;
            findSrcLocation(c);
            fprintf(f,"src = %d,%d\n",c.ix,c.iy);

            findDstLocation(c);
            fprintf(f,"dst = %d,%d\n",c.ix,c.iy);
        }

        while(1)
        {
            CELL lastCell = pathList.back();
            if (lastCell == src) break;

            if (!lastCell.hasParent()) return 2; // it is not src but does not has parent
            if ((lastCell.ix == lastCell.parent_ix)&&(lastCell.iy == lastCell.parent_iy))
                return 3;

            if (0)
            {
                if (0) // search anywhere (OPEN+CLOSED)
                {
                    //vPoints.push_back(POINT(lastCell.parent_ix,lastCell.parent_iy));

                    // search in OPEN
                    pathItem = findInOpen(lastCell.parent_ix,lastCell.parent_iy);

                    if (verbose)
                    {
                        fprintf(f,"search for: <%d,%d>\n", lastCell.parent_ix,lastCell.parent_iy);
                        fflush(f);
                    }

                    if (pathItem==open.end()) // if not found in OPEN
                    {
                        // search in CLOSED
                        pathItem = findInClosed(lastCell.parent_ix,lastCell.parent_iy);
                        if (pathItem==closed.end())
                            return 2;

                        if (verbose)
                        {
                            fprintf(f,"found in closed: <%d,%d>.G = %f, parent = <%d,%d>\n",
                                pathItem->ix,pathItem->iy,pathItem->g, pathItem->parent_ix,pathItem->parent_iy);
                            fflush(f);
                        }
                    }
                    else // Cell found in OPEN
                    {
                        if (verbose)
                        {
                            fprintf(f,"found in open: <%d,%d>.G = %f, parent = <%d,%d>\n",
                                pathItem->ix,pathItem->iy,pathItem->g, pathItem->parent_ix,pathItem->parent_iy);
                            fflush(f);
                        }
                    }
                }
                else
                {
                    pathItem = findInClosed(lastCell.parent_ix,lastCell.parent_iy);
                    if (pathItem==closed.end())
                        return 2;
                };
            };


            if (1) // search anywhere (CLOSED+OPEN)
            {
                //vPoints.push_back(POINT(lastCell.parent_ix,lastCell.parent_iy));

                // search in OPEN
                pathItem = findInClosed(lastCell.parent_ix,lastCell.parent_iy);

                if (pathItem==closed.end()) // if not found in OPEN
                {
                    // search in CLOSED
                    pathItem = findInOpen(lastCell.parent_ix,lastCell.parent_iy);
                    if (pathItem==open.end())
                        return 2;
                }
            };

            pathList.push_back(*pathItem);
        }




        fprintf(f,"##########################\n\n");
        float l = 0;
        int lastx = src.ix;
        int lasty = src.iy;

        for(std::list<CELL>::reverse_iterator c = (++pathList.rbegin());c != pathList.rend(); ++c)
        {
            fprintf(f,"<%d,%d>\n",c->ix,c->iy);
            l+= this->getGsibling(lastx,lasty,c->ix,c->iy);
            lastx = c->ix;
            lasty = c->iy;
        }

        fprintf(f,"length = %f\n",l);

        fclose(f);

        return 0;
    }

    int findOnMap(float findCost, int &ixf, int &iyf)
    {
        for (int iy = 0; iy < map->height(); iy++)
            for (int ix = 0; ix < map->width(); ix++)
            {
                float cost;
                if (XFAIL(map->getCost(ix,iy,cost))) return 1;
                if (cost == findCost)
                {
                    ixf = ix;
                    iyf = iy;
                    return 0;
                };
            }
            return 2; // not found
    }

    int findSrcLocation(CELL &cellSrc)
    {
        return findOnMap(CELL_SRC,cellSrc.ix,cellSrc.iy);
    }

    int findDstLocation(CELL &cellDst)
    {
        return findOnMap(CELL_DST,cellDst.ix,cellDst.iy);
    }

    inline int getSiblingDF(int ix,int iy,int index,int &sx,int &sy,float &cost)
    {
        switch (index)
        {
        case(0): sx = ix; sy = iy-1; break;
        case(1): sx = ix+1; sy = iy - 1; break;
        case(2): sx = ix+1; sy = iy; break;
        case(3): sx = ix+1; sy = iy+1; break;
        case(4): sx = ix; sy = iy+1; break;
        case(5): sx = ix-1; sy = iy+1; break;
        case(6): sx = ix-1; sy = iy; break;
        case(7): sx = ix-1; sy = iy-1; break;
        default: return 1; // no more siblings
        }
        map->getCost(sx,sy,cost); // returns Wall cost if not bound
        return 0;
    }

    // diagonal check function
    inline int getSiblingDC(int ix,int iy,int index,int &sx,int &sy,float &cost)
    {
        float fCost1,fCost2;
        switch (index)
        {
        case(0): sx = ix; sy = iy-1; break;
        case(2): sx = ix+1; sy = iy; break;
        case(4): sx = ix; sy = iy+1; break;
        case(6): sx = ix-1; sy = iy; break;
            // check diagonals for 1,3,5,7
        case(1):
            {
                map->getCost(ix,iy-1,fCost1);
                map->getCost(ix+1,iy,fCost2);

                sx = ix+1; sy = iy - 1;
                if ((fCost1 == CELL_WALL)||(fCost2 == CELL_WALL))
                {
                    cost = CELL_WALL;
                    return 0;
                }
                break;
            }
        case(3):
            {
                map->getCost(ix+1,iy,fCost1);
                map->getCost(ix,iy+1,fCost2);

                sx = ix+1; sy = iy+1;
                if ((fCost1 == CELL_WALL)||(fCost2 == CELL_WALL))
                {
                    cost = CELL_WALL;
                    return 0;
                }
                break;
            }

        case(5):
            {
                map->getCost(ix,iy+1,fCost1);
                map->getCost(ix-1,iy,fCost2);

                sx = ix-1; sy = iy+1;

                if ((fCost1 == CELL_WALL)||(fCost2 == CELL_WALL))
                {
                    cost = CELL_WALL;
                    return 0;
                }

                break;
            }
        case(7):
            {
                map->getCost(ix-1,iy,fCost1);
                map->getCost(ix,iy-1,fCost2);

                sx = ix-1; sy = iy-1;

                if ((fCost1 == CELL_WALL)||(fCost2 == CELL_WALL))
                {
                    cost = CELL_WALL;
                    return 0;
                }

                break;
            }
        default: return 1; // no more siblings
        }
        map->getCost(sx,sy,cost); // returns Wall cost if not bound
        return 0;
    }

    // returns sibling cell to Base. writes to retSibling.
    inline int getSibling(const CELL *base,int index,CELL *retSibling,float &cost)
    {
        retSibling->parent_ix = base->ix;
        retSibling->parent_iy = base->iy;

        return getSiblingDC(base->ix,base->iy,index,
            retSibling->ix,retSibling->iy,cost);
    }

    int saveToFile(char *fn);


}; // PATHFINDER
