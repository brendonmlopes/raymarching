#pragma once
#include <cstdio>
#include <cstdint>
#include <cmath>
#include "Vector.h"

struct Ray{
    vec3 pos{0.0f,0.0f,0.0f};
    vec3 dir{0.0f,0.0f,1.0f};
    uint8_t hits = 0;
    uint16_t steps = 0;
    float travel = 0.0f;
    float minDist = 1e30f;
    vec3 color {0.0f,0.0f,0.0f};

    void step(float stepSize = 0.01f){
        pos.x += dir.x*stepSize;
        pos.y += dir.y*stepSize;
        pos.z += dir.z*stepSize;
        steps+=1;
        travel+=stepSize;
    };

    void print(){
        printf("Position:\t");
        printf("(%.2f\t,%.2f\t,%.2f)\n",pos.x,pos.y,pos.z);
        printf("Direction:\t");
        printf("(%.2f\t,%.2f\t,%.2f)\n",dir.x,dir.y,dir.z);
        printf("Hits:\t");
        printf("%u\n",hits);
    };

};
