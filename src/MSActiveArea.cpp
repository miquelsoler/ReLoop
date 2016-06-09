//
//  MSActiveArea.cpp
//  ReLoop
//
//  Created by Miquel Àngel Soler on 8/6/16.
//
//

#include "MSActiveArea.hpp"

MSActiveArea::MSActiveArea(int _id, int _x, int _y, int _w, int _h, float scaleFactor)
{
    id = _id;
    x = _x * scaleFactor;
    y = _y * scaleFactor;
    w = _w * scaleFactor;
    h = _h * scaleFactor;
}

bool MSActiveArea::isPointInside(int px, int py)
{
    return ((px>=x) && (px<x+w) && (py>=y) && (py<y+h));
}
