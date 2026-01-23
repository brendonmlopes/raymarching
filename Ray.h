#include <array>
#include <cstdio>
#include <cstdint>
#include <cmath>

struct Ray{
    std::array<float,3> pos;
    std::array<float,3> dir;
    uint8_t hits = 0;
    uint8_t steps = 0;
    float travel = 0.0f;
    char color;

    Ray(){
        pos.at(0)=0.f;
        pos.at(1)=0.f;
        pos.at(2)=0.f;
    };

    void step(float stepSize = 0.01f){
        pos.at(0) += dir.at(0)*stepSize;
        pos.at(1) += dir.at(1)*stepSize;
        pos.at(2) += dir.at(2)*stepSize;
        steps+=1;
        travel+=stepSize;
    };

    void print(){
        printf("Position:\t");
        printf("(%.2f\t,%.2f\t,%.2f)\n",pos.at(0),pos.at(1),pos.at(2));
        printf("Direction:\t");
        printf("(%.2f\t,%.2f\t,%.2f)\n",dir.at(0),dir.at(1),dir.at(2));
        printf("Hits:\t");
        printf("%ld\n",hits);
    };

    float norm(){
        float x = pos.at(0);
        float y = pos.at(1);
        float z = pos.at(2);

        if(x*x+y*y+z*z == 0){return 0.0f;}

        float out = std::pow(x*x + y*y + z*z,1/2);

        return out;
    }

};
