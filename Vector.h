#pragma once
struct vec3{
    float x;
    float y;
    float z;

    vec3(){
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    vec3(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    vec3 operator+(const vec3 &w)const {
        vec3 res(x+w.x,y+w.y,z+w.z);
        return res;
    }

    vec3 operator+=(const vec3 &w){
        x += w.x;
        y += w.y;
        z += w.z;
        return *this;
    }

    vec3 operator-=(const vec3 &w){
        x -= w.x;
        y -= w.y;
        z -= w.z;
        return *this;
    }

    vec3 operator-(const vec3 &w)const {
        vec3 res(x-w.x,y-w.y,z-w.z);
        return res;
    }

    vec3 operator*(float t)const {
        vec3 res(x*t,y*t,z*t);
        return res;
    }

    float const operator[](int i)const {
        if(i==0) return x;
        if(i==1) return y;
        if(i==2) return z;
        return 0.0f;
    }

    float dot(const vec3 &w)const {
        float res = x*w.x+y*w.y+z*w.z;
        return res;
    }

    vec3 cross(const vec3 &w)const {
        vec3 res(y*w.z-z*w.y , z*w.x-x*w.z , x*w.y - y*w.x);
        return res;
    }

};
