#include "cell.h"

CELL::CELL() :
    parent_ix(-1),
    parent_iy(-1),
    f(0),
    g(0),
    h(0),
    f_closed(0)
{

}

CELL::CELL(int ix_, int iy_, float Gs_, float H_, const CELL *parent_):
    ix(ix_),
    iy(iy_),
    f(0),
    h(H_)
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

bool CELL::hasParent()
{
    return (parent_ix < 0 || parent_iy < 0) ? 0 : 1;
}

bool CELL::isClosed()
{
    return f_closed;
}

void CELL::setClosed()
{
    f_closed = 1;
}

void CELL::setParent(int ix_, int iy_)
{
    parent_ix = ix_;
    parent_iy = iy_;
}

void CELL::setParent(const CELL *c)
{
    parent_ix = c->ix;
    parent_iy = c->iy;
}

void CELL::init(CELL parent_, float gs, float h)
{
    parent_ix = parent_.ix;
    parent_iy = parent_.iy;
}

CELL & CELL::operator =(const CELL &cell2)
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

bool CELL::operator ==(const CELL &cell2)
{
    if ((cell2.ix==ix)&&(cell2.iy==iy))
        return true;
    return false;
}

bool CELL::operator >(const CELL &cComp)
{
    if (this->f > cComp.f)
        return true;
    else return false;
}

bool CELL::operator <(const CELL &cComp)
{
    if (this->f < cComp.f)
        return true;
    else return false;
}

bool CELL::operator !=(const CELL &cComp)
{
    if ((cComp.ix!=ix)||(cComp.iy==iy))
        return true;
    else return false;
}
