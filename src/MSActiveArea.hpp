//
//  MSActiveArea.hpp
//  ReLoop
//
//  Created by Miquel Àngel Soler on 8/6/16.
//
//

#ifndef MSActiveArea_hpp
#define MSActiveArea_hpp

#include <stdio.h>

class MSActiveArea
{
public:

    MSActiveArea(int id, float x, float y, float w, float h);

private:

    int id;
    float x, y;
    float w, h;
};


#endif /* MSActiveArea_hpp */
