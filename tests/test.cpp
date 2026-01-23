#include "../Ray.h"
#include "../Point.h"
#include "../Geometry.h"
#include <cstdio>
#include <cassert>

int main(){
    Ray r;
    Point p;

    r.pos.at(0) = 1;
    r.pos.at(1) = 2;
    r.pos.at(2) = -1;

    p.x = 4;
    p.y = 8;
    p.z = -3;

    printf("Start test\n");

    assert(r.pos.at(0) == 1);
    assert(r.pos.at(1) == 2);
    assert(r.pos.at(2) == -1);

    assert(p.x == 4);
    assert(p.y == 8);
    assert(p.z == -3);

    printf("====================\nEnd test");
    return 0;
}
