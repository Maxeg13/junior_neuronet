#ifndef NODE_H
#define NODE_H
#include "myline.h"

class node
{
public:
    float frict=0.9;
    float x, y;
    float vx, vy;
    float ax, ay;
    float clr[3];
    node();
    node(int ,int );
    void spaceKinemat();
//    checkWall(myLine ML);
    checkStuck(myLine &ML);
};

#endif // NODE_H
