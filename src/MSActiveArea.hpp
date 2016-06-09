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

    MSActiveArea(int id, int x, int y, int w, int h, float scaleFactor);
    bool isPointInside(int px, int py);

    int id;
    int x, y;
    int w, h;
};


// Facade:

// Genre selection
/// UL
/// x = 65
/// y = 422
/// w = 386
/// h = 224
/// UR
/// x = 744
/// y = 422
/// w = 386
/// h = 224
/// DL
/// x = 65
/// y = 854
/// w = 224
/// h = 386
/// DR
/// x = 907
/// y = 854
/// w = 224
/// h = 386


// Windows
//// w = 106
//// h = 186
//// y = 192
//// x =
//// 10
//// 160
//// 312
//// 466
//// 618
//// 768
//// 916
//// 1066


#endif /* MSActiveArea_hpp */
