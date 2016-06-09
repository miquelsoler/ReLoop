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


// Facade:

// Genre selection
/// UL
/// x = 65
/// y = 467
/// w = 386
/// h = 224
/// UR
/// x = 744
/// y = 467
/// w = 386
/// h = 224
/// DL
/// x = 65
/// h = 856
/// w = 386
/// y = 467
/// DR
/// x = 907
/// h = 856
/// h = 224
/// w = 386


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
