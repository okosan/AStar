#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <algorithm>

// TODO: delegate memory mgmt to stl

template<typename T>
class ARR2D
{
public:
    ARR2D();

    ARR2D(ARR2D<T> &src);

    ARR2D(int w_, int h_);

    ~ARR2D();

    int width();
    int height();

    int destroy();

    int alloc(int w_, int h_);

    int resize(int w_, int h_);

    int inline getItem(int x, int y, T &t);

    T inline getItem(int x,int y);

    inline T * getItemPtr(int x, int y);

    bool inline isValidPos(int x, int y);

    int inline setItem(int x,int y, T t);

    // TODO: This function is unsafe because it assumes conversion to float value
    int saveToFile(char *fn);

    int fill(T fill_value);

    // operator = automatically realloc space if
    // size is not suitable
    // can throw exception (badalloc&integer -1)
    ARR2D<T> & operator=(const ARR2D<T> &src);

    // low performance algorithm !!!
    // for optimization - remove fill_value
    // and place fill_value in unused cells
    int translate(int tx, int ty, T fill_value);

private:
    static T getArrValue(T* arr, int x, int y, int w);
    static void setArrValue(T* arr, int x, int y, int w, T value);

    T *pItem;
    int w;
    int h;
    int f_init;
};

#include "arr2d_impl.h"

