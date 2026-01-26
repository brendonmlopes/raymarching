#include <cstdio>
#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <fstream>
#include <numbers>
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

    for(int i = 0 ; i < heatmap.size() ; i++){
        Ray &r = heatmap.at(i);
        float radiusSq1 = 16000.0f;
        float radiusSq2 = 8000.0f;
        while(true){
            //Far sphere
            float dx = r.pos.x - 300.0f;
            float dy = r.pos.y - 250.0f;
            float dz = r.pos.z - 160.0f;
            double distSq1 = dx * dx + dy * dy + dz * dz - radiusSq2;
            if( distSq1 <= radiusSq1){
                r.color = 'b';
                r.hits ++;
            }

            //green sphere
            dx = r.pos.x - 200.0f;
            dy = r.pos.y - 250.0f;
            dz = r.pos.z - 150.0f;
            double distSq2 = dx * dx + dy * dy + dz * dz;
            if( distSq2 <= radiusSq2){
                r.color = 'g';
                r.hits ++;
            }

            //red sphere
            dx = r.pos.x - 400.0f;
            dy = r.pos.y - 150.0f;
            dz = r.pos.z - 170.0f;
            double distSq3 = dx * dx + dy * dy + dz * dz - radiusSq2;
            if( distSq3 <= radiusSq2){
                r.hits ++;
                r.color = 'r';
            }

            if(r.hits){
                break;
            }

            double minDistSq = (double)std::min(std::min(distSq1,distSq2),distSq3);

            if(minDistSq>=100000000.f){
                break;
            }

            if(minDistSq<=10.0f){
                r.hits++;
                break;
            }

            r.step(std::sqrt((double)minDistSq)/20.0f);
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
            rgb[0] = 0.0f;
            rgb[1] = 0.0f;
            rgb[2] = 0.0f;

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

