#include <cstdio>
#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <fstream>
#include <numbers>
#include <algorithm>
#include "Ray.h"
#include "Point.h"
#include "Geometry.h"
#include "Vector.h"

void normalize(vec3 &v){
    double len = std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z );
    v.x = v.x/len;
    v.y = v.y/len;
    v.z = v.z/len;
}

int main(){
    uint16_t width = 512;
    uint16_t height = 512;
    float pif = std::numbers::pi_v<float>;

    std::array<Ray,262144> heatmap;

    for(int i = 0 ; i < width ; i++){
        for(int j = 0 ; j < height ; j++){
            Ray r;

            float aspectRatio = (float) width / (float) height;

            float fov = 60.0f; //This is in degrees
            float scale = std::tan((fov*0.5f)*(std::numbers::pi_v<float>/180.0f));

            //float u = (2.0f * (float) i / (float) width);
            //float v = (2.0f * (float) j / (float) height);

            float u = (((i+0.5f)/(float) width ) * 2.0f - 1.0f) * aspectRatio * scale;
            float v = (1.0f - ((j + 0.5f)/(float) height) * 2.0f) * scale * -1;

            //r.pos = vec3 ((float)i,(float)j,0);
            //r.dir = vec3 (0.0f,0.0f,1.0f);

            r.pos = vec3 (256.0f,256.0f,-300.0f);
            r.dir = vec3 (u,v,1.0f);
            normalize(r.dir);

            heatmap.at(j*width+i) = r;
        }
    }

    Geometry c1(vec3{0,300,700});
    Geometry c2(vec3{400,400,200});
    Geometry c3(vec3{200,100,200});

    float radius1 = 200.0f;
    float radius2 = 70.0f;
    float radius3 = 100.0f;

    for(int i = 0 ; i < heatmap.size() ; i++){
        Ray &r = heatmap.at(i);


        while(true){
            float dx1 = r.pos.x - c1.pos.x;
            float dy1 = r.pos.y - c1.pos.y;
            float dz1 = r.pos.z - c1.pos.z;

            float dx2 = r.pos.x - c2.pos.x;
            float dy2 = r.pos.y - c2.pos.y;
            float dz2 = r.pos.z - c2.pos.z;

            float dx3 = r.pos.x - c3.pos.x;
            float dy3 = r.pos.y - c3.pos.y;
            float dz3 = r.pos.z - c3.pos.z;

            double d1 = std::sqrt(dx1*dx1 + dy1*dy1 + dz1*dz1) - radius1; 
            double d2 = std::sqrt(dx2*dx2 + dy2*dy2 + dz2*dz2) - radius2; 
            double d3 = std::sqrt(dx3*dx3 + dy3*dy3 + dz3*dz3) - radius3;

            double d = std::min({d1,d2,d3});

            if(r.hits){
                break;
            }

            if(d<=0.001f){
                r.hits++;
                break;
            }

            r.step(std::sqrt((double)d)*0.8f);
            if(r.travel > 1000.0f){
                break;
            }
        }
    }

    float maxTravel = 0;
    for(int i = 0 ; i < heatmap.size() ; i++){
        if(heatmap.at(i).travel > maxTravel){
            maxTravel = heatmap.at(i).travel;
        }
    }

    std::ofstream out("image.ppm",std::ios::binary);
    if(!out){
        return false;
    };
    out << "P6\n" << (int)width << " " << (int)height << "\n255\n";


    float k = 1.0f;
    for(int i = 0 ; i < heatmap.size() ; i++){
        unsigned char rgb[3];
        Ray &r = heatmap.at(i);

        if(r.hits){
            rgb[0] = 55.0f;
            rgb[1] = 55.0f;
            rgb[2] = 55.0f;

            float v;
            v = ((float)r.travel/(float)maxTravel);
            if(v<0.0f)v=0.0f;
            if(v>1.0f)v=1.0f;

            v = 1 - v;

            v = 1.0f - expf(-k*v);

            if(v<0.0f)v=0.0f;
            if(v>1.0f)v=1.0f;

            v = v * 255.0f + 0.5f;

            if(r.color == 'r'){
                rgb[0] = (unsigned char) v;
            }else if(r.color =='g'){
                rgb[1] = (unsigned char) v;
            }else if(r.color =='b'){
                rgb[2] = (unsigned char) v;
            }else{
                rgb[0] = (unsigned char) v;
                rgb[1] = (unsigned char) v;
                rgb[2] = (unsigned char) v;
            }
        }else{
            rgb[0] = 0;
            rgb[1] = 0;
            rgb[2] = 0;
        }
        out.write((char*) rgb,3);
    }
    return 0;
}

