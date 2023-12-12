#pragma once
//#include<iostream>
#include<vector>

//using namespace std;

struct Color{
    float r,g,b;
    Color();
    Color(float r,float g,float b);
    ~Color();

};

class Image
{

    Image(int width,int height);
    ~Image();

    Color GetColor(int x, int y)const;
    void SetColor(const Color& color, int x,int y);
    void Export(const char* path)const;

    int m_width;
    int m_height;
    std::vector<Color> m_colors;
};
