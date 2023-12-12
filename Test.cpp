//#include "Image.h"
#include<iostream>
#include<vector>
#include<fstream>

using namespace std;

struct Color{
    float r,g,b;
    Color();
    Color(float r,float g,float b);
    ~Color();

};

class Image
{
public:
    Image(int width,int height);
    ~Image();

    Color GetColor(int x, int y)const;
    void SetColor(const Color& color, int x,int y);
    void Read(const char* path);
    void Convolution();
    void Export(const char* path)const;
private:
    int m_width;
    int m_height;
    vector<Color> m_colors;
};


Color :: Color()
    :r(0),g(0),b(0)
{

}

Color :: Color(float r,float g,float b)
    :r(r),g(g),b(b)
{

}

Color :: ~Color()
{

}

Image :: Image(int width,int height)
    :m_width(width),m_height(height),m_colors(vector<Color>(width*height))
{

}
Image :: ~Image()
{

}

Color Image :: GetColor(int x,int y)const
{
    return m_colors[y*m_width+x];
}

void Image::SetColor(const Color& color, int x,int y)
{
    m_colors[y*m_width+x].r=color.r;
    m_colors[y*m_width+x].g=color.g;
    m_colors[y*m_width+x].b=color.b;
}

void Image::Read(const char* path)
{
    ifstream f;
    f.open(path,std::ios::in | std::ios::binary);
//    f.open(path,std::ios::in);


    if(!f.is_open())
    {
        cout<<"File can't be opened!"<<endl;
        return;
    }

    const int fileHeaderSize=14;
    const int infoHeaderSize=40;

    unsigned char fileHeader[fileHeaderSize];
    f.read(reinterpret_cast<char*>(fileHeader),fileHeaderSize);
cout<<"FileHeader : "<<fileHeader<<endl;

    unsigned char infoHeader[infoHeaderSize];
    f.read(reinterpret_cast<char*>(infoHeader),infoHeaderSize);
cout<<"InfoHeader : "<<infoHeader<<endl;

    const int fileSize=fileHeader[2]+(fileHeader[3]<<8)+(fileHeader[4]<<16)+(fileHeader[5]<<24);
    m_width=infoHeader[4]+(infoHeader[5]<<8)+(infoHeader[6]<<16)+(infoHeader[7]<<24);
    m_height=infoHeader[8]+(infoHeader[9]<<8)+(infoHeader[10]<<16)+(infoHeader[11]<<24);

    m_colors.resize(m_width*m_height);
    int paddingAmount=(4-((m_width*3)%4)%4);

    for(int y=0; y<m_height; ++y)
    {
        for(int x=0; x<m_width; ++x)
        {
            unsigned char color[3];
            f.read(reinterpret_cast<char*>(color),3);
            m_colors[y*m_width+x].r=static_cast<float>(color[2])/255.0f;
            m_colors[y*m_width+x].g=static_cast<float>(color[1])/255.0f;
            m_colors[y*m_width+x].b=static_cast<float>(color[0])/255.0f;

        }
        f.ignore(paddingAmount);
    }
    f.close();
    cout<<"File read"<<endl;
}

void Image::Convolution()
{
    //Convolusion

      float filter[3][3]={{-1,-2,-1},{0,0,0},{1,2,1}};

    //  int filter[3][3]={{0,-1,0},{-1,4,-1},{0,-1,0}};



     for(int y=1; y<m_height-1; ++y)
    {
        for(int x=1; x<m_width-1; ++x)
        {
            float r=0,g=0,b=0;

            r+=m_colors[(y-1)*m_width+(x-1)].r*filter[0][0]+m_colors[(y-1)*m_width+(x)].r*filter[0][1]+m_colors[(y-1)*m_width+(x+1)].r*filter[0][2];
            g+=m_colors[(y-1)*m_width+(x-1)].g*filter[0][0]+m_colors[(y-1)*m_width+(x)].g*filter[0][1]+m_colors[(y-1)*m_width+(x+1)].g*filter[0][2];
            b+=m_colors[(y-1)*m_width+(x-1)].b*filter[0][0]+m_colors[(y-1)*m_width+(x)].b*filter[0][1]+m_colors[(y-1)*m_width+(x+1)].b*filter[0][2];

            r+=m_colors[(y)*m_width+(x-1)].r*filter[1][0]+m_colors[(y)*m_width+(x)].r*filter[1][1]+m_colors[(y)*m_width+(x+1)].r*filter[1][2];
            g+=m_colors[(y)*m_width+(x-1)].g*filter[1][0]+m_colors[(y)*m_width+(x)].g*filter[1][1]+m_colors[(y)*m_width+(x+1)].g*filter[1][2];
            b+=m_colors[(y)*m_width+(x-1)].b*filter[1][0]+m_colors[(y)*m_width+(x)].b*filter[1][1]+m_colors[(y)*m_width+(x+1)].b*filter[1][2];

            r+=m_colors[(y+1)*m_width+(x-1)].r*filter[2][0]+m_colors[(y+1)*m_width+(x)].r*filter[2][1]+m_colors[(y+1)*m_width+(x+1)].r*filter[2][2];
            g+=m_colors[(y+1)*m_width+(x-1)].g*filter[2][0]+m_colors[(y+1)*m_width+(x)].g*filter[2][1]+m_colors[(y+1)*m_width+(x+1)].g*filter[2][2];
            b+=m_colors[(y+1)*m_width+(x-1)].b*filter[2][0]+m_colors[(y+1)*m_width+(x)].b*filter[2][1]+m_colors[(y+1)*m_width+(x+1)].b*filter[2][2];


            m_colors[y*m_width+x].r=r;
            m_colors[y*m_width+x].g=g;
            m_colors[y*m_width+x].b=b;
        }

    }

    cout<<"Convolution Complete!"<<endl;
}

void Image :: Export(const char* path)const
{
    ofstream f;
    f.open(path,std::ios::out | std::ios::binary);

    if(!f.is_open())
    {
        cout<<"File can't be opened!"<<endl;
        return;
    }

    unsigned char bmpPad[3]={0,0,0};
    int paddingAmount=(4-((m_width*3)%4)%4);
    const int fileHeaderSize=14;
    const int infoHeaderSize=40;
    const int fileSize=fileHeaderSize+infoHeaderSize+(m_width*m_height*3)+paddingAmount*m_height;

    unsigned char fileHeader[fileHeaderSize];

    //file type
    fileHeader[0]='B';
    fileHeader[1]='M';

    //file size
    fileHeader[2]=fileSize;
    fileHeader[3]=fileSize>>8;
    fileHeader[4]=fileSize>>16;
    fileHeader[5]=fileSize>>24;
//cout<<fileHeader[2]<<" "<<fileHeader[3]<<" "<<fileHeader[4]<<" "<<fileHeader[5]<<endl;

    //reserved 1 (Not used)
    fileHeader[6]=0;
    fileHeader[7]=0;

     //reserved 2 (Not used)
    fileHeader[8]=0;
    fileHeader[9]=0;

    //pixel data offset
    fileHeader[10]=fileHeaderSize+infoHeaderSize;
    fileHeader[11]=0;
    fileHeader[12]=0;
    fileHeader[13]=0;

//cout<<"FileHeader : "<<fileHeader<<endl;

    unsigned char infoHeader[infoHeaderSize];

    //Header size
    infoHeader[0]=infoHeaderSize;
    infoHeader[1]=0;
    infoHeader[2]=0;
    infoHeader[3]=0;

    //image width
    infoHeader[4]=m_width;
    infoHeader[5]=m_width>>8;
    infoHeader[6]=m_width>>16;
    infoHeader[7]=m_width>>24;

    //image height
    infoHeader[8]=m_height;
    infoHeader[9]=m_height>>8;
    infoHeader[10]=m_height>>16;
    infoHeader[11]=m_height>>24;

    //planes
    infoHeader[12]=1;
    infoHeader[13]=0;

    //bits per pixel(RGB)
    infoHeader[14]=24;
    infoHeader[15]=0;

    //compression(No compression)
    infoHeader[16]=0;
    infoHeader[17]=0;
    infoHeader[18]=0;
    infoHeader[19]=0;

    //image size (no compression)
    infoHeader[20]=0;
    infoHeader[21]=0;
    infoHeader[22]=0;
    infoHeader[23]=0;

    //X pixels per meter(not specified)
    infoHeader[24]=0;
    infoHeader[25]=0;
    infoHeader[26]=0;
    infoHeader[27]=0;

    //Y pixels per meter(not specified)
    infoHeader[28]=0;
    infoHeader[29]=0;
    infoHeader[30]=0;
    infoHeader[31]=0;

    //total colors(color palette not used)
    infoHeader[32]=0;
    infoHeader[33]=0;
    infoHeader[34]=0;
    infoHeader[35]=0;

    //important colors(generally ignored)
    infoHeader[36]=0;
    infoHeader[37]=0;
    infoHeader[38]=0;
    infoHeader[39]=0;
//cout<<"infoHeader : "<<infoHeader<<endl;

      f.write(reinterpret_cast<char*>(fileHeader),fileHeaderSize);
      f.write(reinterpret_cast<char*>(infoHeader),infoHeaderSize);


    for(int y=0; y<m_height; ++y)
    {
        for(int x=0; x<m_width; ++x)
        {
            unsigned char r=static_cast<unsigned char>(GetColor(x,y).r);//*255.0f);
            unsigned char g=static_cast<unsigned char>(GetColor(x,y).g);//*255.0f);
            unsigned char b=static_cast<unsigned char>(GetColor(x,y).b);//*255.0f);

            unsigned char color[]={r,g,b};
            f.write(reinterpret_cast<char*>(color),3);
        }
         f.write(reinterpret_cast<char*>(bmpPad),paddingAmount);
    }

    f.close();
    cout<<"File created"<<endl;

}




int main()
{
    const int width=640;
    const int height=480;

//    Image image(width,height);
//
//    for(int y=0; y<height; ++y)
//    {
//        for(int x=0; x<width; ++x)
//        {
//            image.SetColor(Color(255,0,100),x,y);
//          // image.SetColor(Color((float)x/(float)width,1.0f-(float)x/((float)width),(float)y/(float)height),x,y);
//        }
//        // f.write(reinterpret_cast<char*>(bmpPad),paddingAmount);
//    }
//
//    image.Export("image.bmp");

    Image readImage(0,0);

    readImage.Read("baboon.bmp");
//readImage.Convolution();
//readImage.Export("baboon.txt");

Color x=readImage.GetColor(10,3);
//cout<<x.r<<" "<<x.g<<" "<<x.b<<" "<<endl;

    //readImage.Export("Test01.bmp");

//readImage.Export("Test01.txt");


    return 0;
}


