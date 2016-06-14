//
//  MSActiveArea.cpp
//  ReLoop
//
//  Created by Miquel Àngel Soler on 8/6/16.
//
//

#include "MSActiveArea.hpp"

MSActiveArea::MSActiveArea(int _id, float _x, float _y, float _w, float _h, float scaleFactor)
{
    id = _id;
//    x = _x * scaleFactor;
//    y = _y * scaleFactor;
//    w = _w * scaleFactor;
//    h = _h * scaleFactor;
    x = _x;
    y = _y;
    w = _w;
    h = _h;
}

bool MSActiveArea::isPointInside(float px, float py)
{
    return ((px>=x) && (px<x+w) && (py>=y) && (py<y+h));
}
