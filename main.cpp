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

            float fov = 70.0f; //This is in degrees
            float scale = std::tan((fov*0.5f)*(std::numbers::pi_v<float>/180.0f));

            //float u = (2.0f * (float) i / (float) width);
            //float v = (2.0f * (float) j / (float) height);

            float u = ( ( (i+0.0f)/(float) width ) * 2.0f - 1.0f) * aspectRatio * scale;
            float v = ( ( 1.0f - (j) / (float) height ) * 2.0f - 1.0f) * scale ;

            r.pos = vec3 (0.0f,0.0f,0.0f);
            r.dir = vec3 (u,v,1.0f);
            normalize(r.dir);

            heatmap.at(j*width+i) = r;
        }
    }

    Geometry c1(vec3{0,0,30});
    Geometry c2(vec3{40,30,70});
    Geometry c3(vec3{20,10,70});

    float radius1 = 2.5f;
    float radius2 = 1.0f;
    float radius3 = 1.0f;

    for(int i = 0 ; i < heatmap.size() ; i++){
        Ray &r = heatmap.at(i);

        while(true){
            float dx1 = std::fmod(r.pos.x - c1.pos.x + 267.5f,10.0f) - 2.5f;
            float dy1 = std::fmod(r.pos.y - c1.pos.y + 267.5f,10.0f) - 2.5f;
            float dz1 = std::fmod(r.pos.z - c1.pos.z + 2.5f,5.0f) - 2.5f;

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

            if(d<=0.01f){
                r.hits++;
                r.color = vec3( 1.0f - r.steps/200.0f , 1.0f - r.steps/200.0f , 1.0f - r.steps/100.0f );
                break;
            }

            r.step(std::sqrt((double)d)*0.8f);
            if(r.steps > 1000 ){
                if(d<10.0f){
                    r.color = vec3(0.0f,0.0f,0.0f);
                    r.hits++;
                }
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

    float maxSteps = 0;
    for(int i = 0 ; i < heatmap.size() ; i++){
        if(heatmap.at(i).steps > maxSteps){
            maxSteps = heatmap.at(i).steps;
        }
    }

    std::ofstream out("image.ppm",std::ios::binary);
    if(!out){
        return false;
    };
    out << "P6\n" << (int)width << " " << (int)height << "\n255\n";

    float k = 0.1f;
    for(int i = 0 ; i < heatmap.size() ; i++){
        unsigned char rgb[3];
        Ray &ray = heatmap.at(i);

        if(ray.hits){
            rgb[0] = 0.0f;
            rgb[1] = 0.0f;
            rgb[2] = 0.0f;

            float r;
            float g;
            float b;

            r = ray.color[0];
            g = ray.color[1];
            b = ray.color[2];

            if(r<0.0f)r=0.0f;
            if(r>1.0f)r=1.0f;

            if(g<0.0f)r=0.0f;
            if(g>1.0f)r=1.0f;

            if(b<0.0f)r=0.0f;
            if(b>1.0f)r=1.0f;

            r = k/(r*r);
            g = k/(g*g);
            b = k/(b*b);

            if(r<0.0f)r=0.0f;
            if(r>1.0f)r=1.0f;

            if(g<0.0f)g=0.0f;
            if(g>1.0f)g=1.0f;

            if(b<0.0f)b=0.0f;
            if(b>1.0f)b=1.0f;

            r = r * 255.0f + 0.5f;
            g = g * 255.0f + 0.5f;
            b = b * 255.0f + 0.5f;

            rgb[0] = (unsigned char) r;
            rgb[1] = (unsigned char) g;
            rgb[2] = (unsigned char) b;

        }else{
            rgb[0] = 0;
            rgb[1] = 0;
            rgb[2] = 0;
        }
        out.write((char*) rgb,3);
    }
    printf("Closing file...\n");
    out.close();
    printf("File Closed.");
    return 0;
}

