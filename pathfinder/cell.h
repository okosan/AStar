#pragma once

#include "map.h"

class CELL
{
public:
    CELL();
    CELL(int ix_, int iy_, float Gs_, float H_, const CELL *parent_);

    bool hasParent();
    bool isClosed();
    void setClosed();

    void setParent(int ix_ = -1, int iy_ = -1);
    void setParent(const CELL *c);

    void init(CELL parent_, float gs, float h);

    CELL & operator =(const CELL &cell2);

    // compare passability of cells (ground resistance)
    bool operator ==(const CELL &cell2);

    bool operator >(const CELL &cComp);

    bool operator <(const CELL &cComp);

    // are cells different in passability?
    bool operator !=(const CELL &cComp);

    int ix;
    int iy;
    int parent_ix;
    int parent_iy;

    float f;
    float g;
    float h;

private:
    char f_closed;

};  // class CELL
