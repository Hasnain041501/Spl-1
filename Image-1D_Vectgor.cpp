//#include "Image.h"
#include<iostream>
#include<vector>
#include<fstream>
#include<cmath>

using namespace std;

struct Color
{
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
    void printManu();
    void valueTest();
    void Negative();
    void Grayscale();
    void Brightening();
    void Flip();
    void Sharpen();
    void Smoothing();
    void Convolution();
    void Box_Blur();
    void Gaussian_Blur();
    void Gaussian_Blur5_5();
    void SobelEdgeDetection();
    void Angle_Calculation();
    void Export(const char* path)const;
    void Histogram();
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

    unsigned char infoHeader[infoHeaderSize];
    f.read(reinterpret_cast<char*>(infoHeader),infoHeaderSize);

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
            /*//original operation
            m_colors[y*m_width+x].r=static_cast<float>(color[2]);///255.0f;
            m_colors[y*m_width+x].g=static_cast<float>(color[1]);///255.0f;
            m_colors[y*m_width+x].b=static_cast<float>(color[0]);///255.0f; */

            m_colors[y*m_width+x].r=static_cast<float>(color[2]);///255.0f;
            m_colors[y*m_width+x].g=static_cast<float>(color[1]);///255.0f;
            m_colors[y*m_width+x].b=static_cast<float>(color[0]);///255.0f;

        }
        f.ignore(paddingAmount);
    }
    f.close();
    cout<<"File read"<<endl;

}

void Image :: valueTest()
{
    ////

    cout<<m_height<<endl;
    cout<<m_width<<endl;
//cout<<paddingAmount<<endl;

    for(int y=m_height/300; y<(m_height/300)+10; ++y)
    {
        for(int x=m_width/100; x<(m_width/100)+10; ++x)
        {
            Color b = m_colors[y*m_height+x];
            cout<<b.r<<" "<<b.g<<" "<<b.b<<endl;

        }

    }
    ////
}

void Image :: Negative()
{
    for(int y=0; y<m_height; ++y)
    {
        for(int x=0; x<m_width; ++x)
        {
            float r=0,g=0,b=0;

            r=abs(255-m_colors[y*m_width+x].r);
            g=abs(255-m_colors[y*m_width+x].g);
            b=abs(255-m_colors[y*m_width+x].b);


            m_colors[y*m_width+x].r=r;
            m_colors[y*m_width+x].g=g;
            m_colors[y*m_width+x].b=b;
        }

    }

    cout<<"Negative done!"<<endl;

}

void Image :: Grayscale()
{
    for(int y=0; y<m_height; ++y)
    {
        for(int x=0; x<m_width; ++x)
        {
            float r=0,g=0,b=0;

            r=m_colors[y*m_width+x].r;
            g=m_colors[y*m_width+x].g;
            b=m_colors[y*m_width+x].b;

            float avg=0;
            avg=(r+g+b)/3;

            m_colors[y*m_width+x].r=avg;
            m_colors[y*m_width+x].g=avg;
            m_colors[y*m_width+x].b=avg;
        }

    }

    cout<<"Grayscale done!"<<endl;

}

void Image :: Brightening()
{

    cout << "---------------------Brightening--------------------\n\n";
    cout << "Input the brightness level between -255 to +255 : ";
    double level;
    cin >> level;

    if(level > 255 || level < -255)
    {
        cout<<"Invalid brightness level";
        return;
    }
    else
    {
         for(int y = 0; y < m_height; y++)
        {
           for(int x = 0; x < m_width; x++)
           {
              double r=0,g=0,b=0;

            r=m_colors[y*m_width+x].r;
            r = r + min(level, (255 - r));
            m_colors[y*m_width+x].r=max(0.0, r);

            g=m_colors[y*m_width+x].g;
            g = g + min(level, (255 - g));
            m_colors[y*m_width+x].g=max(0.0, g);

            b=m_colors[y*m_width+x].b;
            b = b + min(level, (255 - b));
            m_colors[y*m_width+x].b=max(0.0, b);
           }
        }
    }


    cout<<"Brightening done!"<<endl;
}

void Image :: Flip()
{
  /*  vector<vector<Color>> M_colors;

     for (int x = 0; x< m_height; ++x)
    {
        for (int y = 0; y< m_width; ++y)
        {
            M_colors[x][y].r=m_colors[x*m_width+y].r;
            M_colors[x][y].g=m_colors[x*m_width+y].g;
            M_colors[x][y].b=m_colors[x*m_width+y].b;

        }
    }

     for (int x = 0; x< m_height; ++x)
    {
        for (int y = 0; y< m_width/2; ++y)
        {
            float  red=0,green=0,blue=0;


            red=M_colors[x][m_width-1-y].r;
            M_colors[x][m_width-1-y].r=M_colors[x][y].r;
            M_colors[x][y].r=red;

            green=M_colors[x][m_width-1-y].g;
            M_colors[x][m_width-1-y].g=M_colors[x][y].g;
            M_colors[x][y].g=green;

            blue=M_colors[x][m_width-1-y].b;
            M_colors[x][m_width-1-y].b=M_colors[x][y].b;
            M_colors[x][y].b=blue;


        }

    }

    for (int x = 0; x< m_height; ++x)
    {
        for (int y = 0; y< m_width; ++y)
        {
            m_colors[x*m_width+y].r=M_colors[x][y].r;
            m_colors[x*m_width+y].g=M_colors[x][y].g;
            m_colors[x*m_width+y].b=M_colors[x][y].b;

        }
    } */
    for (int y = 0; y< m_height; ++y)
    {
        for (int x = 0; x < m_width/2; ++x)
        {

            swap(m_colors[y*m_width+x], m_colors[y*m_width+(m_width-x-1)]);

            /*
            float  red=0,green=0,blue=0;


            red=m_colors[y*m_width+(m_width-x+1)].r;
            m_colors[y*m_width+(m_width-x+1)].r=m_colors[y*m_width+x].r;
            m_colors[y*m_width+x].r=red;

            green=m_colors[y*m_width+(m_width-x+1)].g;
            m_colors[y*m_width+(m_width-x+1)].g=m_colors[y*m_width+x].g;
            m_colors[y*m_width+x].g=green;

            blue=m_colors[y*m_width+(m_width-x+1)].b;
            m_colors[y*m_width+(m_width-x+1)].b=m_colors[y*m_width+x].b;
            m_colors[y*m_width+x].b=blue;   */


        }

    }

    cout<<"Flip complete!"<<endl;

}


void Image::Convolution()
{
    //Convolusion

    float filter[3][3]= {{1,0,-1},{2,0,-2},{1,0,-1}};

    //  int filter[3][3]={{1,2,1},{2,4,2},{1,2,1}};



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

            r/=9;
            g/=9;
            b/=9;

            /* (r>150)? r=255 : r=0;
             (g>150)? g=255 : g=0;
             (b>150)? b=255 : b=0; */

            m_colors[y*m_width+x].r=r;
            m_colors[y*m_width+x].g=g;
            m_colors[y*m_width+x].b=b;
        }

    }

    cout<<"Convolution Complete!"<<endl;
}

void Image::Smoothing()
{


    float filter[3][3]= {
                        {1,1,1},
                        {1,1,1},
                        {1,1,1}
                        };



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

            r/=9;
            g/=9;
            b/=9;


            m_colors[y*m_width+x].r=r;
            m_colors[y*m_width+x].g=g;
            m_colors[y*m_width+x].b=b;
        }

    }

    cout<<"Smoothing Complete!"<<endl;
}

void Image::Sharpen()
{
    int kernel[4][9] = {
                      {0, 1, 0, 1, -4 , 1, 0, 1 , 0},
                      {0, -1, 0, -1, 4, -1, 0, -1, 0},
                      {1, 1, 1, 1 , -8, 1, 1, 1, 1},
                      {-1, -1, -1, -1, 8, -1, -1, -1, -1},
                    };

    cout<<"\n\nChoose Filter : "<<endl;
    cout<<"\n1 -> Laplacian filter.\n";
    cout<<"2 -> Strong Laplacian filter.\n";
    cout<<"3 -> High Boost Filter.\n";
    cout<<"4 -> Strong High Boost Filter.\n";

    float filter[3][3];
    int choice;
    int index=0;

    cout<<"choice : ";
    cin>>choice;

    cout<<"\n\n\n"<<endl;

    for(int i=0; i<3; ++i)
    {
        for(int j=0; j<3; ++j)
        {
            filter[i][j]=kernel[choice][index];
            index++;
        }

    }

    // float filter[3][3]= {{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};

  //  float filter[3][3]= {{0,-1,0},{-1,5,-1},{0,-1,0}};

    //  int filter[3][3]={{1,2,1},{2,4,2},{1,2,1}};



    for(int y=1; y<m_height-1; ++y)
    {
        for(int x=1; x<m_width-1; ++x)
        {
            double r=0,g=0,b=0;

            r+=m_colors[(y-1)*m_width+(x-1)].r*filter[0][0]+m_colors[(y-1)*m_width+(x)].r*filter[0][1]+m_colors[(y-1)*m_width+(x+1)].r*filter[0][2];
            g+=m_colors[(y-1)*m_width+(x-1)].g*filter[0][0]+m_colors[(y-1)*m_width+(x)].g*filter[0][1]+m_colors[(y-1)*m_width+(x+1)].g*filter[0][2];
            b+=m_colors[(y-1)*m_width+(x-1)].b*filter[0][0]+m_colors[(y-1)*m_width+(x)].b*filter[0][1]+m_colors[(y-1)*m_width+(x+1)].b*filter[0][2];

            r+=m_colors[(y)*m_width+(x-1)].r*filter[1][0]+m_colors[(y)*m_width+(x)].r*filter[1][1]+m_colors[(y)*m_width+(x+1)].r*filter[1][2];
            g+=m_colors[(y)*m_width+(x-1)].g*filter[1][0]+m_colors[(y)*m_width+(x)].g*filter[1][1]+m_colors[(y)*m_width+(x+1)].g*filter[1][2];
            b+=m_colors[(y)*m_width+(x-1)].b*filter[1][0]+m_colors[(y)*m_width+(x)].b*filter[1][1]+m_colors[(y)*m_width+(x+1)].b*filter[1][2];

            r+=m_colors[(y+1)*m_width+(x-1)].r*filter[2][0]+m_colors[(y+1)*m_width+(x)].r*filter[2][1]+m_colors[(y+1)*m_width+(x+1)].r*filter[2][2];
            g+=m_colors[(y+1)*m_width+(x-1)].g*filter[2][0]+m_colors[(y+1)*m_width+(x)].g*filter[2][1]+m_colors[(y+1)*m_width+(x+1)].g*filter[2][2];
            b+=m_colors[(y+1)*m_width+(x-1)].b*filter[2][0]+m_colors[(y+1)*m_width+(x)].b*filter[2][1]+m_colors[(y+1)*m_width+(x+1)].b*filter[2][2];


            r = min(255.0,r);
            r=max(0.0, r);

            g = min(255.0,g);
            g=max(0.0, g);

            b = min(255.0,b);
            b=max(0.0, b);


            m_colors[y*m_width+x].r=r;
            m_colors[y*m_width+x].g=g;
            m_colors[y*m_width+x].b=b;
        }

    }

    cout<<"Sharpen Complete!"<<endl;
}

void Image::Box_Blur()
{

    //  float filter[3][3]={{-1,-2,-1},{0,0,0},{1,2,1}};

    int filter[3][3]= {{1,1,1},{1,1,1},{1,1,1}};



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

            r/=9;
            g/=9;
            b/=9;

            /* (r>150)? r=255 : r=0;
             (g>150)? g=255 : g=0;
             (b>150)? b=255 : b=0; */

            m_colors[y*m_width+x].r=r;
            m_colors[y*m_width+x].g=g;
            m_colors[y*m_width+x].b=b;
        }

    }

    cout<<"Box Blur!"<<endl;
}


void Image::Gaussian_Blur()
{

    //  float filter[3][3]={{-1,-2,-1},{0,0,0},{1,2,1}};

    int filter[3][3]= {{1,2,1},{2,4,2},{1,2,1}};



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

            r/=16;
            g/=16;
            b/=16;


            m_colors[y*m_width+x].r=r;
            m_colors[y*m_width+x].g=g;
            m_colors[y*m_width+x].b=b;
        }

    }

    cout<<"Gaussian Blur!"<<endl;
}

void Image::Gaussian_Blur5_5()
{


    int filter[5][5]= {
                    {1,4,6,4,1},
                    {4,16,24,16,4},
                    {6,24,36,24,6},
                    {4,16,24,16,4},
                    {1,4,6,4,1},
                    };



    for(int y=2; y<m_height-2; ++y)
    {
        for(int x=2; x<m_width-2; ++x)
        {
            float r=0,g=0,b=0;

            r+=m_colors[(y-2)*m_width+(x-2)].r*filter[0][0]+m_colors[(y-2)*m_width+(x-1)].r*filter[0][1]+m_colors[(y-2)*m_width+(x)].r*filter[0][2]+m_colors[(y-2)*m_width+(x+1)].r*filter[0][3]+m_colors[(y-2)*m_width+(x+2)].r*filter[0][4];
            g+=m_colors[(y-2)*m_width+(x-2)].g*filter[0][0]+m_colors[(y-2)*m_width+(x-1)].g*filter[0][1]+m_colors[(y-2)*m_width+(x)].g*filter[0][2]+m_colors[(y-2)*m_width+(x+1)].g*filter[0][3]+m_colors[(y-2)*m_width+(x+2)].g*filter[0][4];
            b+=m_colors[(y-2)*m_width+(x-2)].b*filter[0][0]+m_colors[(y-2)*m_width+(x-1)].b*filter[0][1]+m_colors[(y-2)*m_width+(x)].b*filter[0][2]+m_colors[(y-2)*m_width+(x+1)].b*filter[0][3]+m_colors[(y-2)*m_width+(x+2)].b*filter[0][4];

            r+=m_colors[(y-1)*m_width+(x-2)].r*filter[1][0]+m_colors[(y-1)*m_width+(x-1)].r*filter[1][1]+m_colors[(y-1)*m_width+(x)].r*filter[1][2]+m_colors[(y-1)*m_width+(x+1)].r*filter[1][3]+m_colors[(y-1)*m_width+(x+2)].r*filter[1][4];
            g+=m_colors[(y-1)*m_width+(x-2)].g*filter[1][0]+m_colors[(y-1)*m_width+(x-1)].g*filter[1][1]+m_colors[(y-1)*m_width+(x)].g*filter[1][2]+m_colors[(y-1)*m_width+(x+1)].g*filter[1][3]+m_colors[(y-1)*m_width+(x+2)].g*filter[1][4];
            b+=m_colors[(y-1)*m_width+(x-2)].b*filter[1][0]+m_colors[(y-1)*m_width+(x-1)].b*filter[1][1]+m_colors[(y-1)*m_width+(x)].b*filter[1][2]+m_colors[(y-1)*m_width+(x+1)].b*filter[1][3]+m_colors[(y-1)*m_width+(x+2)].b*filter[1][4];

           r+=m_colors[(y)*m_width+(x-2)].r*filter[2][0]+m_colors[(y)*m_width+(x-1)].r*filter[2][1]+m_colors[(y)*m_width+(x)].r*filter[2][2]+m_colors[(y)*m_width+(x+1)].r*filter[2][3]+m_colors[(y)*m_width+(x+2)].r*filter[2][4];
           g+=m_colors[(y)*m_width+(x-2)].g*filter[2][0]+m_colors[(y)*m_width+(x-1)].g*filter[2][1]+m_colors[(y)*m_width+(x)].g*filter[2][2]+m_colors[(y)*m_width+(x+1)].g*filter[2][3]+m_colors[(y)*m_width+(x+2)].g*filter[2][4];
           b+=m_colors[(y)*m_width+(x-2)].b*filter[2][0]+m_colors[(y)*m_width+(x-1)].b*filter[2][1]+m_colors[(y)*m_width+(x)].b*filter[2][2]+m_colors[(y)*m_width+(x+1)].b*filter[2][3]+m_colors[(y)*m_width+(x+2)].b*filter[2][4];


           r+=m_colors[(y+1)*m_width+(x-2)].r*filter[3][0]+m_colors[(y+1)*m_width+(x-1)].r*filter[3][1]+m_colors[(y+1)*m_width+(x)].r*filter[3][2]+m_colors[(y+1)*m_width+(x+1)].r*filter[3][3]+m_colors[(y+1)*m_width+(x+2)].r*filter[3][4];
           g+=m_colors[(y+1)*m_width+(x-2)].g*filter[3][0]+m_colors[(y+1)*m_width+(x-1)].g*filter[3][1]+m_colors[(y+1)*m_width+(x)].g*filter[3][2]+m_colors[(y+1)*m_width+(x+1)].g*filter[3][3]+m_colors[(y+1)*m_width+(x+2)].g*filter[3][4];
           b+=m_colors[(y+1)*m_width+(x-2)].b*filter[3][0]+m_colors[(y+1)*m_width+(x-1)].b*filter[3][1]+m_colors[(y+1)*m_width+(x)].b*filter[3][2]+m_colors[(y+1)*m_width+(x+1)].b*filter[3][3]+m_colors[(y+1)*m_width+(x+2)].b*filter[3][4];


            r+=m_colors[(y+2)*m_width+(x-2)].r*filter[4][0]+m_colors[(y+2)*m_width+(x-1)].r*filter[4][1]+m_colors[(y+2)*m_width+(x)].r*filter[4][2]+m_colors[(y+2)*m_width+(x+1)].r*filter[4][3]+m_colors[(y+2)*m_width+(x+2)].r*filter[4][4];
            g+=m_colors[(y+2)*m_width+(x-2)].g*filter[4][0]+m_colors[(y+2)*m_width+(x-1)].g*filter[4][1]+m_colors[(y+2)*m_width+(x)].g*filter[4][2]+m_colors[(y+2)*m_width+(x+1)].g*filter[4][3]+m_colors[(y+2)*m_width+(x+2)].g*filter[4][4];
            b+=m_colors[(y+2)*m_width+(x-2)].b*filter[4][0]+m_colors[(y+2)*m_width+(x-1)].b*filter[4][1]+m_colors[(y+2)*m_width+(x)].b*filter[4][2]+m_colors[(y+2)*m_width+(x+1)].b*filter[4][3]+m_colors[(y+2)*m_width+(x+2)].b*filter[4][4];



            r/=256;
            g/=256;
            b/=256;

            m_colors[y*m_width+x].r=r;
            m_colors[y*m_width+x].g=g;
            m_colors[y*m_width+x].b=b;
        }

    }

    cout<<"Gaussian Blur(5*5)!"<<endl;
}

void Image:: SobelEdgeDetection()
{
     float gx[3][3] = {{-1,0,1},
                   {-2,0,2},
                   {-1,0,1}};

    float gy[3][3] = {{1,2,1},
                    {0,0,0},
                    {-1,-2,-1}};

     for(int y=1; y<m_height-1; ++y)
    {
        for(int x=1; x<m_width-1; ++x)
        {
            float rx=0,gx=0,bx=0;


            rx+=m_colors[(y-1)*m_width+(x-1)].r*gx[0][0]+m_colors[(y-1)*m_width+(x)].r*gx[0][1]+m_colors[(y-1)*m_width+(x+1)].r*gx[0][2];
            gx+=m_colors[(y-1)*m_width+(x-1)].g*gx[0][0]+m_colors[(y-1)*m_width+(x)].g*gx[0][1]+m_colors[(y-1)*m_width+(x+1)].g*gx[0][2];
            bx+=m_colors[(y-1)*m_width+(x-1)].b*gx[0][0]+m_colors[(y-1)*m_width+(x)].b*gx[0][1]+m_colors[(y-1)*m_width+(x+1)].b*gx[0][2];

            rx+=m_colors[(y)*m_width+(x-1)].r*gx[1][0]+m_colors[(y)*m_width+(x)].r*gx[1][1]+m_colors[(y)*m_width+(x+1)].r*gx[1][2];
            gx+=m_colors[(y)*m_width+(x-1)].g*gx[1][0]+m_colors[(y)*m_width+(x)].g*gx[1][1]+m_colors[(y)*m_width+(x+1)].g*gx[1][2];
            bx+=m_colors[(y)*m_width+(x-1)].b*gx[1][0]+m_colors[(y)*m_width+(x)].b*gx[1][1]+m_colors[(y)*m_width+(x+1)].b*gx[1][2];

            rx+=m_colors[(y+1)*m_width+(x-1)].r*gx[2][0]+m_colors[(y+1)*m_width+(x)].r*gx[2][1]+m_colors[(y+1)*m_width+(x+1)].r*gx[2][2];
            gx+=m_colors[(y+1)*m_width+(x-1)].g*gx[2][0]+m_colors[(y+1)*m_width+(x)].g*gx[2][1]+m_colors[(y+1)*m_width+(x+1)].g*gx[2][2];
            bx+=m_colors[(y+1)*m_width+(x-1)].b*gx[2][0]+m_colors[(y+1)*m_width+(x)].b*gx[2][1]+m_colors[(y+1)*m_width+(x+1)].b*gx[2][2];

            double ry=0,gy=0,by=0;

            ry+=m_colors[(y-1)*m_width+(x-1)].r*gy[0][0]+m_colors[(y-1)*m_width+(x)].r*gy[0][1]+m_colors[(y-1)*m_width+(x+1)].r*gy[0][2];
            gy+=m_colors[(y-1)*m_width+(x-1)].g*gy[0][0]+m_colors[(y-1)*m_width+(x)].g*gy[0][1]+m_colors[(y-1)*m_width+(x+1)].g*gy[0][2];
            by+=m_colors[(y-1)*m_width+(x-1)].b*gy[0][0]+m_colors[(y-1)*m_width+(x)].b*gy[0][1]+m_colors[(y-1)*m_width+(x+1)].b*gy[0][2];

            ry+=m_colors[(y)*m_width+(x-1)].r*gy[1][0]+m_colors[(y)*m_width+(x)].r*gy[1][1]+m_colors[(y)*m_width+(x+1)].r*gy[1][2];
            gy+=m_colors[(y)*m_width+(x-1)].g*gy[1][0]+m_colors[(y)*m_width+(x)].g*gy[1][1]+m_colors[(y)*m_width+(x+1)].g*gy[1][2];
            by+=m_colors[(y)*m_width+(x-1)].b*gy[1][0]+m_colors[(y)*m_width+(x)].b*gy[1][1]+m_colors[(y)*m_width+(x+1)].b*gy[1][2];

            ry+=m_colors[(y+1)*m_width+(x-1)].r*gy[2][0]+m_colors[(y+1)*m_width+(x)].r*gy[2][1]+m_colors[(y+1)*m_width+(x+1)].r*gy[2][2];
            gy+=m_colors[(y+1)*m_width+(x-1)].g*gy[2][0]+m_colors[(y+1)*m_width+(x)].g*gy[2][1]+m_colors[(y+1)*m_width+(x+1)].g*gy[2][2];
            by+=m_colors[(y+1)*m_width+(x-1)].b*gy[2][0]+m_colors[(y+1)*m_width+(x)].b*gy[2][1]+m_colors[(y+1)*m_width+(x+1)].b*gy[2][2];


            double sqrtBlue = (double)(sqrt(bx*bx + by*by));
            double sqrtGreen = (double)(sqrt(gx*gx+ gy*gy));
            double sqrtRed = (double)(sqrt(rx*rx + ry*ry));


            if(sqrtBlue > 127)
            {
                sqrtBlue = 255;
            }
            if(sqrtGreen > 127)
            {
                sqrtGreen = 255;
            }
            if(sqrtRed > 127)
            {
                sqrtRed = 255;
            }


             float avg= (sqrtBlue + sqrtGreen + sqrtRed)/3;
            m_colors[y*m_width+x].r=avg;
            m_colors[y*m_width+x].g=avg;
            m_colors[y*m_width+x].b=avg;

        }

    }

}

void Image::Angle_Calculation()
{
    //Gx && Gy



    //Grayscale
    for(int y=0; y<m_height; ++y)
    {
        for(int x=0; x<m_width; ++x)
        {
            float r=0,g=0,b=0;

            r=m_colors[y*m_width+x].r;
            g=m_colors[y*m_width+x].g;
            b=m_colors[y*m_width+x].b;

            float avg=0;
            avg=(r+g+b)/3;

            m_colors[y*m_width+x].r=avg;
            m_colors[y*m_width+x].g=avg;
            m_colors[y*m_width+x].b=avg;
        }

    }




    //Gx

    float filterX[3][3]= {{1,0,-1},{2,0,-2},{1,0,-1}};



    float Gx[m_height][m_width];//G[m_height][m_width];


    for(int y=1; y<m_height-1; ++y)
    {

        for(int x=1; x<m_width-1; ++x)
        {
            // cout<<x<<" ";
            float r=0;

            r+=m_colors[(y-1)*m_width+(x-1)].r*filterX[0][0]+m_colors[(y-1)*m_width+(x)].r*filterX[0][1]+m_colors[(y-1)*m_width+(x+1)].r*filterX[0][2];

            r+=m_colors[(y)*m_width+(x-1)].r*filterX[1][0]+m_colors[(y)*m_width+(x)].r*filterX[1][1]+m_colors[(y)*m_width+(x+1)].r*filterX[1][2];


            r+=m_colors[(y+1)*m_width+(x-1)].r*filterX[2][0]+m_colors[(y+1)*m_width+(x)].r*filterX[2][1]+m_colors[(y+1)*m_width+(x+1)].r*filterX[2][2];


            Gx[y][x]=r;

            // cout<<Gx[x][y]<<" ";

            /*  Gx[x][y].r=r;
              Gx[x][y].g=g;
              Gx[x][y].b=b; */
        }

        //  cout<<"---***---";

    }


    for(int y=90; y<100-1; ++y)
    {
        for(int x=90; x<100-1; ++x)
        {
            cout<<Gx[y][x]<<" "<<endl;


        }

    }


    cout<<"\n\n\n\n";




    //Gy
    float filterY[3][3]= {{1,2,1},{0,0,0},{-1,-2,-1}};

    float Gy[m_height][m_width];

    for(int y=1; y<m_height-1; ++y)
    {
        for(int x=1; x<m_width-1; ++x)
        {
            float r=0;

            r+=m_colors[(y-1)*m_width+(x-1)].r*filterY[0][0]+m_colors[(y-1)*m_width+(x)].r*filterY[0][1]+m_colors[(y-1)*m_width+(x+1)].r*filterY[0][2];


            r+=m_colors[(y)*m_width+(x-1)].r*filterY[1][0]+m_colors[(y)*m_width+(x)].r*filterY[1][1]+m_colors[(y)*m_width+(x+1)].r*filterY[1][2];

            r+=m_colors[(y+1)*m_width+(x-1)].r*filterY[2][0]+m_colors[(y+1)*m_width+(x)].r*filterY[2][1]+m_colors[(y+1)*m_width+(x+1)].r*filterY[2][2];


            Gy[y][x]=r;

            /*   Gy[x][y].r=r;
               Gy[x][y].g=g;
               Gy[x][y].b=b; */
        }

    }





    //float G[m_height][m_width];



    for(int y=1; y<m_height-1; ++y)
    {
        for(int x=1; x<m_width-1; ++x)
        {
            float result = (Gy[y][x]/Gx[y][x]);
            result=atan(result);
            Gx[y][x]=(result*180)/3.1415;

            if(Gx[y][x] < 0)
            {
                Gx[y][x]=180+Gx[y][x];
            }



        }

    }

    for(int y=90; y<100-1; ++y)
    {
        for(int x=90; x<100-1; ++x)
        {
            cout<<Gx[y][x]<<" "<<endl;


        }

    }

    for(int y=1; y<m_height-1; ++y)
    {
        for(int x=1; x<m_width-1; ++x)
        {
            if(Gx[y][x]<20 || Gx[y][x]>120)
            {
                m_colors[y*m_width+x].r=255;
                m_colors[y*m_width+x].g=0;
                m_colors[y*m_width+x].b=0;
            }




        }

    }




    cout<<"---Angle---"<<endl;
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

    unsigned char bmpPad[3]= {0,0,0};
    int paddingAmount=(4-((m_width*3)%4)%4);
 //   cout<<"Padding in Writing Image : "<<endl;
  //  cout<<paddingAmount<<endl;
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

            unsigned char color[]= {r,g,b};
            f.write(reinterpret_cast<char*>(color),3);
        }
        f.write(reinterpret_cast<char*>(bmpPad),paddingAmount);
    }

    f.close();
    cout<<"File created"<<endl;

}

void Image :: Histogram()
{
     for(int y=0; y<m_height; ++y)
    {
        for(int x=0; x<m_width; ++x)
        {
            float r=0,g=0,b=0;

            r=m_colors[y*m_width+x].r;
            g=m_colors[y*m_width+x].g;
            b=m_colors[y*m_width+x].b;

            int avg=0;
            avg=(r+g+b)/3;

            m_colors[y*m_width+x].r=avg;
            m_colors[y*m_width+x].g=avg;
            m_colors[y*m_width+x].b=avg;
        }

    }

    int Hist[256]={0};

    for(int y=0; y<m_height; ++y)
    {
        for(int x=0; x<m_width; ++x)
        {
            int val=0;
            val=m_colors[y*m_width+x].r;
            Hist[val]+=1;
        }
    }


    long long int cdf[256]={0};
    cdf[0]=Hist[0];

    for(int i=1; i<256; ++i)
    {


       cdf[i]=cdf[i-1]+Hist[i];

    }




    int cdfMin;
  //  cdfMin=min(cdf);
   cdfMin=cdf[0];
   for(int i=1; i<256; ++i)
    {
        if(cdf[i]<cdfMin)
            cdfMin=cdf[i];
    }


    int h[256]={0};

     for(int i=0; i<256; ++i)
    {
        h[i]=round((cdf[i]-cdfMin)*255)/((m_height*m_width)-cdfMin);

    }

    for(int i=0; i<256; ++i)
    {


        cout<<h[i]<<" ";

    }

    cout<<endl;
    cout<<"---Histogram---";

}


void printManu()
{
    cout<<endl;
    cout<<"1->Read Image"<<endl;
    cout<<"2->Negative"<<endl;
    cout<<"3->Grayscale"<<endl;
    cout<<"4->Brightening"<<endl;
    cout<<"5->Box Blur"<<endl;
   // cout<<"5->Flip Image"<<endl;
    cout<<"6->Gaussian Blur"<<endl;
    cout<<"7->Angle Calculation"<<endl;
   // cout<<"8->Histogram"<<endl;
    cout<<"9->Export"<<endl;

}

int main()
{

    cout<<"------Basic Image Processing Tools-----"<<endl;
    printManu();



    int choice;
    cout<<endl<<endl;;
    cout<<"choice : ";
    cin>>choice;

    Image readImage(0,0);

    cout<<"     Reading Image....."<<endl<<endl;

    readImage.Read("baboon.bmp");



    switch(choice)
    {
       case 1:
           break;

       case 2:
           readImage.Negative();
           break;

       case 3:
           readImage.Grayscale();
           break;

       case 4:
           readImage.Brightening();
           break;

       case 5:
           //readImage.Box_Blur();
           readImage.Flip();
           break;

       case 6:
          // readImage.Gaussian_Blur5_5();
          readImage.SobelEdgeDetection();
           break;

       case 7:
           readImage.Sharpen();
           break;

       case 8:
           //readImage.SobelEdgeDetection();
           break;

       case 9:
           break;

       case 10:
           break;

    }

    cout<<"Creating NewImage......"<<endl;
    readImage.Export("NewImage.bmp");




   // readImage.Read("zebra.bmp");
//readImage.Export("baboon.txt");
//    readImage.Flip();

//Color x=readImage.GetColor(10,3);
//cout<<x.r<<" "<<x.g<<" "<<x.b<<" "<<endl;

//   readImage.Convolution();
//      readImage.Negative();
//    readImage.Grayscale();
//     readImage.Brightening();

//    readImage.valueTest();
//    readImage.Angle_Calculation();
  //  readImage.Histogram();
 //   readImage.Gaussian_Blur();

  //  readImage.Export("exportZebra.bmp");

//readImage.Export("redImage.txt");



    return 0;
}


