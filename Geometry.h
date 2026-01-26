#pragma once
#include <vector>
#include "Vector.h"

struct Geometry{
    vec3 pos{0,0,0};
    Geometry (vec3 pos){
        this->pos = pos;
    }
    Geometry (){
        this->pos = vec3{0,0,0};
    }
};
