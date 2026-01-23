struct Point{
    std::array<float,3> pos;
    
    float x = pos.at(0);
    float y = pos.at(1);
    float z = pos.at(2);

    Point(){
        x=0;
        y=0;
        z=0;
    }
};
