#pragma once

template <typename T>
ARR2D<T>::ARR2D():
    w(0),
    h(0),
    f_init(0),
    pItem(0)
{

}

template <typename T>
ARR2D<T>::ARR2D(ARR2D<T> &src) :
    w(0),
    h(0),
    f_init(0),
    pItem(0)
{
    if (src.w <= 0 || src.h <= 0)
    {
        throw 1;
        return;
    }

    this->alloc(src.w,src.h);

    for (int iy = 0; iy < h; iy++)
    {
        for (int ix = 0; ix < w; ix++)
        {
            this->setItem(ix,iy,src.getItem(ix,iy));
        }
    }
}

template <typename T>
ARR2D<T>::ARR2D(int w_, int h_):
    w(0),
    h(0),
    f_init(0),
    pItem(0)
{
    if (w_ <= 0 || h_<= 0)
    {
        throw 1;
        return;
    }

    this->alloc(w_,h_);
}

template <typename T>
ARR2D<T>::~ARR2D()
{
    destroy();
}

template <typename T>
int ARR2D<T>::width()
{
    return w;
}

template <typename T>
int ARR2D<T>::height()
{
    return h;
}

template <typename T>
int ARR2D<T>::destroy()
{
    if ((f_init)&&(pItem))
    {
        delete [] pItem;
        pItem = 0;
        f_init = 0;
        w = h = 0;
    }
    return 0;
}

template <typename T>
int ARR2D<T>::alloc(int w_, int h_)
{
    if (this->w == w_ && this->h == h_)
        return 0; // already allocated

    destroy();
    if (w_ <= 0 || h_ <= 0)
        return 1;

    w = w_;
    h = h_;
    size_t numItems = w_*h_;

    pItem = new T [numItems];
    for (int y = 0;y<h_;y++)
    {
        for (int x = 0; x<w_; x++)
        {
            setItem(x,y,0);
        }
    }

    f_init = 1;
    return 0;
}

template <typename T>
int ARR2D<T>::resize(int w_,int h_)
{
    if (w_ <= 0 || h_ <= 0)
        return 1;

    T *newArr = new T [w_*h_];
    for (int y = 0;y<h_;y++)
    {
        for (int x = 0; x<w_; x++)
        {
            setArrValue(newArr,x,y,w_,0.0);
        }
    }

    int minw = std::min(w,w_);
    int maxw = std::max(w,w_);
    int miny = std::min(h,h_);
    int maxy = std::max(h,h_);

    for (int y = 0; y<miny; y++)
    {
        for (int x=0; x < minw; x++)
        {
            setArrValue(newArr,x,y,w_,this->getItem(x,y));
        }
    }

    delete [] pItem;
    pItem = newArr;
    w=w_;
    h=h_;
    f_init = 1;
    return 0;
}

template <typename T>
int ARR2D<T>::getItem(int x, int y, T &t)
{
    if (x < w && y < h && x >= 0 && y >= 0)
    {
        t = getArrValue(pItem,x,y,w);
        return 0;
    }
    return 1;
}

template <typename T>
T ARR2D<T>::getItem(int x,int y)
{
    return getArrValue(this->pItem,x,y,this->w);
}

template <typename T>
T * ARR2D<T>::getItemPtr(int x, int y)
{
    if (isValidPos(x,y))
        return &(pItem[y*w+x]);
    return NULL;
}

template <typename T>
bool ARR2D<T>::isValidPos(int x, int y)
{
    if (x < w && y < h && x >= 0 && y >= 0)
        return 1;
    return 0;
}

template <typename T>
int ARR2D<T>::setItem(int x,int y, T t)
{
    if (isValidPos(x,y))
    {
        setArrValue(pItem,x,y,w,t);
        return 0;
    }
    return 1;
}

// TODO: This function is unsafe because it assumes conversion to float value
template <typename T>
int ARR2D<T>::saveToFile(char *fn)
{
    if (!f_init)
        return 1;

    FILE *f = fopen(fn,"wt");

    if (!f) return 2;
    fprintf(f,"%d %d",w,h);
    for (int y = 0;y<h; y++)
    {
        for (int x=0; x<w; x++)
        {
            fprintf(f, "%f ", getItem(x,y));
        }
    }

    if (!fclose(f))
        return 7;

    return 0;
}

template <typename T>
int ARR2D<T>::fill(T fill_value)
{
    if (pItem == NULL) return 1;
    for (int iy = 0; iy < h; iy++)
    {
        for (int ix = 0; ix < w; ix++)
        {
            setItem(ix,iy,fill_value);
        }
    }
    return 0;
}

// operator << - > copy of minimum size
/*
// copy minimum size
int minw = min(src.w,this->w);
int minh = min(src.h,this->h);

for (int iy = 0; iy < minh; iy++)
for (int ix = 0; ix < minw; ix++)
{

*/

// operator = automatically realloc space if
// size is not suitable
// can throw exception (badalloc&integer -1)
template <typename T>
ARR2D<T> & ARR2D<T>::operator=(const ARR2D<T> &src)
{
    if (this == &src) return *this;

    if (src.w <= 0 || src.h <= 0)
    {
        throw 1;
        return *this;
    }

    this->alloc(src.w,src.h);

    for (int iy = 0; iy < h; iy++)
    {
        for (int ix = 0; ix < w; ix++)
        {
            setItem(ix,iy,src.getItem(ix,iy));
        }
    }
    return *this;
}

// low performance algorithm !!!
// for optimization - remove fill_value
// and place fill_value in unused cells
template <typename T>
int ARR2D<T>::translate(int tx, int ty, T fill_value)
{
    ARR2D<T> tmp_array(*this); // source array
    // current array is a dest array
    int sx,sy; // src start index
    int sw,sh; // height to copy from src
    int dx,dy; // dest start index
    this->fill(fill_value);

    if (tx >= 0)
    {
        dx = tx;
        sw = w - tx;
        sx = 0;
    }
    else
    {
        dx = 0;
        sx = -tx;
        sw = w + tx;
    }

    if (ty >= 0)
    {
        dy = ty;
        sh = h - ty;
        sy = 0;
    }
    else
    {
        dy = 0;
        sy = -ty;
        sh = h - (-ty);
    }

    int dxi;
    int dyi;
    int sxi;
    int syi;

    for (dyi = dy,syi = sy; (dyi < h && syi <= sh); dyi++,syi++)
    {
        for (dxi = dx,sxi = sx; (dxi < w && sxi <= sw); dxi++,sxi++)
        {
            setItem(dxi,dyi,tmp_array.getItem(sxi,syi));
        }
    }

    return 0;
}

template <typename T>
T ARR2D<T>::getArrValue(T* arr, int x, int y, int w)
{
    return arr[y*w+x];
}

template <typename T>
void ARR2D<T>::setArrValue(T* arr, int x, int y, int w, T value)
{
    arr[y*w+x] = value;
}
