#include<iostream>
#include<vector>
#include<graphics.h>
#include<fstream>
#include<cmath>

using namespace std;

struct Color
{
   float b,g,r;
    Color();
    Color(float b,float g,float r);
    ~Color();


};

class Image
{
public:

    Image(int width,int height);
    ~Image();

    Color GetColor(int x, int y)const;
    void Read();
    void printManu();
    void Negative();
    void Grayscale();
    void Brightening();
    void Flip();
    void Sharpen();
    void Smoothing();
    void Darken();
    void Gaussian_Blur();
    void SobelEdgeDetection();
    void Angle_Calculation();
    void Histogram();
    void ShowImage(const char* path);
    void Export(const char* path)const;



private:
    int m_width;
    int m_height;
    vector<Color> m_colors;
    vector<Color> colors_modified;


};


Color :: Color()
    :b(0),g(0),r(0)
{

}

Color :: Color(float b,float g,float r)
    :b(b),g(g),r(r)
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
    return colors_modified[y*m_width+x];
}

Image readImage(0,0);


void Image::Read()
{

    string path;
    cout<<"Enter Image : ";
    cin>>path;


    ifstream f;
    f.open(path,std::ios::in | std::ios::binary);


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

    if (fileHeader[0] != 'B' && fileHeader[1] != 'M')
    {
        cout << "Invalid BMP file format!" << endl;
        return;
    }

    const int bytesPerPixel = 3;
    int paddingAmount = (4 - ((m_width * bytesPerPixel) % 4)) % 4;


    m_colors.resize(m_width*m_height);




    for(int y=0; y<m_height; ++y)
    {
        for(int x=0; x<m_width; ++x)
        {
            unsigned char color[3];

            f.read(reinterpret_cast<char*>(color),3);

            m_colors[y*m_width+x].r=static_cast<float>(color[2]);
            m_colors[y*m_width+x].g=static_cast<float>(color[1]);
            m_colors[y*m_width+x].b=static_cast<float>(color[0]);

        }
        f.ignore(paddingAmount);
    }
    f.close();

    cout<<"\n\n\n"<<endl;
    cout<<"\t\t\t!!----Image Reading Complete---!!"<<endl;

//    ShowImage((const char*)path);


}

void Image :: ShowImage(const char* path)
{


    int heightOfScreen = GetSystemMetrics(SM_CXSCREEN);
    int widthOfScreen = GetSystemMetrics(SM_CYSCREEN);
    char nameOfImage[20];
    snprintf(nameOfImage, 100, "%s",path);
    initwindow(800,700, nameOfImage,0,0);
    readimagefile(nameOfImage,200,200,600,600);

    delay(3000);
    closegraph();


}


void Image :: Negative()
{
    colors_modified = vector<Color> (m_height*m_width);

    for(int y=0; y<m_height; ++y)
    {
        for(int x=0; x<m_width; ++x)
        {
            float r=0,g=0,b=0;

            b=abs(255-m_colors[y*m_width+x].b);
            g=abs(255-m_colors[y*m_width+x].g);
            r=abs(255-m_colors[y*m_width+x].r);


            colors_modified[y*m_width+x].b=b;
            colors_modified[y*m_width+x].g=g;
            colors_modified[y*m_width+x].r=r;
        }

    }

    cout<<"\n"<<endl;
    cout<<"\t\t\t!!----Image Negative Complete---!!"<<endl;
    cout<<"\n"<<endl;

    cout<<"\tCreating Negative Image......"<<endl;
    cout<<"\tNegative Image Created named \"NegativeImage.bmp\"......"<<endl;
    readImage.Export("NegativeImage.bmp");

}

void Image :: Grayscale()
{

    colors_modified = vector<Color> (m_height*m_width);


    float avg=0;
    for(int y=0; y<m_height; ++y)
    {
        for(int x=0; x<m_width; ++x)
        {
            float r=0,g=0,b=0;

            b=m_colors[y*m_width+x].b;
            g=m_colors[y*m_width+x].g;
            r=m_colors[y*m_width+x].r;


            avg=(r+g+b)/3;


            colors_modified[y*m_width+x].b=avg;
            colors_modified[y*m_width+x].g=avg;
            colors_modified[y*m_width+x].r=avg;
        }

    }


    cout<<"\n"<<endl;
    cout<<"\t\t\t!!----Image Grayscale Complete---!!"<<endl;
    cout<<"\n"<<endl;

    cout<<"\tCreating Grayscale Image......"<<endl;
    cout<<"\tGrayscale Image Created named \"GrayscaleImage.bmp\"......"<<endl;
    readImage.Export("GrayscaleImage.bmp");


}

void Image :: Brightening()
{
    colors_modified = vector<Color> (m_height*m_width);

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
                colors_modified[y*m_width+x].r=max(0.0, r);

                g=m_colors[y*m_width+x].g;
                g = g + min(level, (255 - g));
                colors_modified[y*m_width+x].g=max(0.0, g);

                b=m_colors[y*m_width+x].b;
                b = b + min(level, (255 - b));
                colors_modified[y*m_width+x].b=max(0.0, b);
            }
        }
    }



    cout<<"\n"<<endl;
    cout<<"\t\t\t!!----Image Brightening Complete---!!"<<endl;
    cout<<"\n"<<endl;

    cout<<"\tCreating Brightening Image......"<<endl;
    cout<<"\tBrightening Image Created named \"BrighteningImage.bmp\"......"<<endl;
    readImage.Export("BrighteningImage.bmp");

}

void Image :: Darken()
{
    colors_modified = vector<Color> (m_height*m_width);


    cout << "Input the darkness level between -255 to +255 : ";
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
                r = r - level;
                colors_modified[y*m_width+x].r=max(0.0, r);

                g=m_colors[y*m_width+x].g;
                g = g - level;
                colors_modified[y*m_width+x].g=max(0.0, g);

                b=m_colors[y*m_width+x].b;
                b = b - level;
                colors_modified[y*m_width+x].b=max(0.0, b);
            }
        }
    }



    cout<<"\n"<<endl;
    cout<<"\t\t\t!!----Image Darkening Complete---!!"<<endl;
    cout<<"\n"<<endl;

    cout<<"\tCreating Darken Image......"<<endl;
    cout<<"\tDarken Image Created named \"DarkenImage.bmp\"......"<<endl;
    readImage.Export("DarkenImage.bmp");

}

void Image :: Flip()
{
    colors_modified = vector<Color> (m_height*m_width);


    cout<<"\t\t\t\t!!----------Flipping Image----------!!"<<endl;

    cout<<"1-> Horizontal Flip"<<endl;
    cout<<"2-> Vertical Flip "<<endl;


    int choice;

    cout<<endl;
    cout<<"Enter Choice : ";
    cin>>choice;



    if(choice == 1)
    {

        cout<<"          Horizontal Flip       "<<endl;



     for (int y = 0; y< m_height; ++y)
    {
        for (int x = 0; x < m_width/2; ++x)
        {

            colors_modified[y*m_width+(m_width-x-1)].r=m_colors[y*m_width+x].r;
            colors_modified[y*m_width+x].r=m_colors[y*m_width+(m_width-x-11)].r;


            colors_modified[y*m_width+(m_width-x-1)].g=m_colors[y*m_width+x].g;
            colors_modified[y*m_width+x].g=m_colors[y*m_width+(m_width-x-1)].g;

            colors_modified[y*m_width+(m_width-x-1)].b=m_colors[y*m_width+x].b;
            colors_modified[y*m_width+x].b=m_colors[y*m_width+(m_width-x-1)].b;



        }

    }

    }
    else if(choice == 2)
    {
        cout<<"          Vertical Flip "<<endl;


    for (int y = 0; y< m_height/2; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {

            colors_modified[(m_height-y-1)*m_width+x].r=m_colors[y*m_width+x].r;
            colors_modified[y*m_width+x].r=m_colors[(m_height-y-1)*m_width+x].r;


            colors_modified[(m_height-y-1)*m_width+x].g=m_colors[y*m_width+x].g;
            colors_modified[y*m_width+x].g=m_colors[(m_height-y-1)*m_width+x].g;

            colors_modified[(m_height-y-1)*m_width+x].b=m_colors[y*m_width+x].b;
            colors_modified[y*m_width+x].b=m_colors[(m_height-y-1)*m_width+x].b;



        }

    }

    }

    cout<<"\n"<<endl;
    cout<<"\t\t\t!!----Image Flip Complete---!!"<<endl;
    cout<<"\n"<<endl;

    cout<<"\tCreating Flip Image......"<<endl;
    cout<<"\Flip Image Created named \"FlipImage.bmp\"......"<<endl;
    readImage.Export("FlipImage.bmp");
}


void Image::Smoothing()
{

    colors_modified = vector<Color> (m_height*m_width);

    float filter[3][3]=
    {
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


            colors_modified[y*m_width+x].r=r;
            colors_modified[y*m_width+x].g=g;
            colors_modified[y*m_width+x].b=b;
        }

    }


    cout<<"\n"<<endl;
    cout<<"\t\t\t!!----Image Smoothing Complete---!!"<<endl;
    cout<<"\n"<<endl;

    cout<<"\tCreating Smooth Image......"<<endl;
    cout<<"\tSmooth Image Created named \"SmoothImage.bmp\"......"<<endl;
    readImage.Export("SmoothImage.bmp");


}

void Image::Sharpen()
{
    colors_modified = vector<Color> (m_height*m_width);

    int kernel[4][9] =
    {
        {0, 1, 0, 1, -4, 1, 0, 1, 0},
        {0, -1, 0, -1, 4, -1, 0, -1, 0},
        {1, 1, 1, 1, -8, 1, 1, 1, 1},
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
    choice--;

    cout<<"\n\n\n"<<endl;

   for(int i=0; i<3; ++i)
    {
        for(int j=0; j<3; ++j)
        {
            filter[i][j]=kernel[choice][index];
            index++;
        }

    }




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


            colors_modified[y*m_width+x].r=r;
            colors_modified[y*m_width+x].g=g;
            colors_modified[y*m_width+x].b=b;
        }

    }

    cout<<"\n"<<endl;
    cout<<"\t\t\t!!----Image Sharpen Complete---!!"<<endl;
    cout<<"\n"<<endl;

    cout<<"\tCreating Sharpen Image......"<<endl;
    cout<<"\tSharpen Image Created named \"SharpenImage.bmp\"......"<<endl;
    readImage.Export("SharpenImage.bmp");

}



void Image::Gaussian_Blur()
{
     colors_modified = vector<Color> (m_height*m_width);


    cout<<"\t\t\t\t!!----------Gaussian Blur----------!!"<<endl;

    cout<<"1-> (3 x 3) Gaussian Blur"<<endl;
    cout<<"2-> (5 x 5) Gaussian Blur"<<endl;


    int choice;

    cout<<endl;
    cout<<"Enter Choice : ";
    cin>>choice;

    //  float filter[3][3]={{-1,-2,-1},{0,0,0},{1,2,1}};

    if(choice == 1)
    {


    int filter[3][3]= {
                        {1,2,1},
                        {2,4,2},
                        {1,2,1},

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

            r/=16;
            g/=16;
            b/=16;


            colors_modified[y*m_width+x].r=r;
            colors_modified[y*m_width+x].g=g;
            colors_modified[y*m_width+x].b=b;
        }

    }

    }
    else if(choice ==2 )
    {

         int filter[5][5]=
    {
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

            colors_modified[y*m_width+x].r=r;
            colors_modified[y*m_width+x].g=g;
            colors_modified[y*m_width+x].b=b;
        }

    }
    }else
    {
        cout<<"Invalid Input!!"<<endl;
        cout<<"\tTry Again."<<endl;
        readImage.Gaussian_Blur();
    }


    cout<<"\n"<<endl;
    cout<<"\t\t\t!!----Gaussian Blur Complete---!!"<<endl;
    cout<<"\n"<<endl;

    cout<<"\tCreating Blur Image......"<<endl;
    cout<<"\tBlur Image Created named \"GaussianBlurImage.bmp\"......"<<endl;
    readImage.Export("GaussianBlurImage.bmp");
}




void Image:: SobelEdgeDetection()
{

     colors_modified = vector<Color> (m_height*m_width);

     vector<Color>colors_avg = vector<Color> (m_height*m_width);


     float avg=0;

    for(int y=0; y<m_height; ++y)
    {
        for(int x=0; x<m_width; ++x)
        {
            float r=0,g=0,b=0;

            b=m_colors[y*m_width+x].b;
            g=m_colors[y*m_width+x].g;
            r=m_colors[y*m_width+x].r;


            avg=(r+g+b)/3;

            colors_avg[y*m_width+x].b=avg;
            colors_avg[y*m_width+x].g=avg;
            colors_avg[y*m_width+x].r=avg;


        }

    }

    int gx[3][3] = {{1,0,-1},
        {2,0,-2},
        {1,0,-1}
    };

    int gy[3][3] = {{1,2,1},
        {0,0,0},
        {-1,-2,-1}
    };

    int upThreshold;
    cout<<"Enter Threshold value for Edge Detection : ";
    cin>>upThreshold;



    int gxValBlue;
    int gyValBlue;

    int gxValGreen;
    int gyValGreen;

    int gxValRed;
    int gyValRed;

    int sqrtBlue;
    int sqrtGreen;
    int sqrtRed;



    for(int y=0; y<m_height; ++y)
    {
        for(int x=0; x<m_width; ++x)
        {
            if(y!=0 && y!=m_height-1 && x!=0 && x!=m_width-1)
            {
            float Rx=0,Gx=0,Bx=0;

            Rx+=colors_avg[(y-1)*m_width+(x-1)].r*gx[0][0]+colors_avg[(y-1)*m_width+(x)].r*gx[0][1]+colors_avg[(y-1)*m_width+(x+1)].r*gx[0][2];
            Gx+=colors_avg[(y-1)*m_width+(x-1)].g*gx[0][0]+colors_avg[(y-1)*m_width+(x)].g*gx[0][1]+colors_avg[(y-1)*m_width+(x+1)].g*gx[0][2];
            Bx+=colors_avg[(y-1)*m_width+(x-1)].b*gx[0][0]+colors_avg[(y-1)*m_width+(x)].b*gx[0][1]+colors_avg[(y-1)*m_width+(x+1)].b*gx[0][2];

            Rx+=colors_avg[(y)*m_width+(x-1)].r*gx[1][0]+colors_avg[(y)*m_width+(x)].r*gx[1][1]+colors_avg[(y)*m_width+(x+1)].r*gx[1][2];
            Gx+=colors_avg[(y)*m_width+(x-1)].g*gx[1][0]+colors_avg[(y)*m_width+(x)].g*gx[1][1]+colors_avg[(y)*m_width+(x+1)].g*gx[1][2];
            Bx+=colors_avg[(y)*m_width+(x-1)].b*gx[1][0]+colors_avg[(y)*m_width+(x)].b*gx[1][1]+colors_avg[(y)*m_width+(x+1)].b*gx[1][2];

            Rx+=colors_avg[(y+1)*m_width+(x-1)].r*gx[2][0]+colors_avg[(y+1)*m_width+(x)].r*gx[2][1]+colors_avg[(y+1)*m_width+(x+1)].r*gx[2][2];
            Gx+=colors_avg[(y+1)*m_width+(x-1)].g*gx[2][0]+colors_avg[(y+1)*m_width+(x)].g*gx[2][1]+colors_avg[(y+1)*m_width+(x+1)].g*gx[2][2];
            Bx+=colors_avg[(y+1)*m_width+(x-1)].b*gx[2][0]+colors_avg[(y+1)*m_width+(x)].b*gx[2][1]+colors_avg[(y+1)*m_width+(x+1)].b*gx[2][2];

            double Ry=0,Gy=0,By=0;

            Ry+=colors_avg[(y-1)*m_width+(x-1)].r*gy[0][0]+colors_avg[(y-1)*m_width+(x)].r*gy[0][1]+colors_avg[(y-1)*m_width+(x+1)].r*gy[0][2];
            Gy+=colors_avg[(y-1)*m_width+(x-1)].g*gy[0][0]+colors_avg[(y-1)*m_width+(x)].g*gy[0][1]+colors_avg[(y-1)*m_width+(x+1)].g*gy[0][2];
            By+=colors_avg[(y-1)*m_width+(x-1)].b*gy[0][0]+colors_avg[(y-1)*m_width+(x)].b*gy[0][1]+colors_avg[(y-1)*m_width+(x+1)].b*gy[0][2];

            Ry+=colors_avg[(y)*m_width+(x-1)].r*gy[1][0]+colors_avg[(y)*m_width+(x)].r*gy[1][1]+colors_avg[(y)*m_width+(x+1)].r*gy[1][2];
            Gy+=colors_avg[(y)*m_width+(x-1)].g*gy[1][0]+colors_avg[(y)*m_width+(x)].g*gy[1][1]+colors_avg[(y)*m_width+(x+1)].g*gy[1][2];
            By+=colors_avg[(y)*m_width+(x-1)].b*gy[1][0]+colors_avg[(y)*m_width+(x)].b*gy[1][1]+colors_avg[(y)*m_width+(x+1)].b*gy[1][2];

            Ry+=colors_avg[(y+1)*m_width+(x-1)].r*gy[2][0]+colors_avg[(y+1)*m_width+(x)].r*gy[2][1]+colors_avg[(y+1)*m_width+(x+1)].r*gy[2][2];
            Gy+=colors_avg[(y+1)*m_width+(x-1)].g*gy[2][0]+colors_avg[(y+1)*m_width+(x)].g*gy[2][1]+colors_avg[(y+1)*m_width+(x+1)].g*gy[2][2];
            By+=colors_avg[(y+1)*m_width+(x-1)].b*gy[2][0]+colors_avg[(y+1)*m_width+(x)].b*gy[2][1]+colors_avg[(y+1)*m_width+(x+1)].b*gy[2][2];


            double sqrtBlue = (double)(sqrt(Bx*Bx + By*By));
            double sqrtGreen = (double)(sqrt(Gx*Gx+ Gy*Gy));
            double sqrtRed = (double)(sqrt(Rx*Rx + Ry*Ry));

            if(sqrtBlue > upThreshold)
            {
                sqrtBlue = 255;
            }
            else{
                sqrtBlue = 0;
            }

            if(sqrtGreen > upThreshold)
            {
                sqrtGreen = 255;
            }
            else{
                sqrtGreen = 0;
            }

            if(sqrtRed > upThreshold)
            {
                sqrtRed = 255;
            }
            else{
                sqrtRed = 0;
            }




            colors_modified[y*m_width+x].b=sqrtBlue;
            colors_modified[y*m_width+x].g=sqrtGreen;
            colors_modified[y*m_width+x].r=sqrtRed ;


        }else
        {

            colors_modified[y*m_width+x].r=0;
            colors_modified[y*m_width+x].g=0;
            colors_modified[y*m_width+x].b=0;

        }

        }


    }




    cout<<"\n"<<endl;
    cout<<"\t\t\t!!----Sobel EdgeDetection Complete---!!"<<endl;
    cout<<"\n"<<endl;

    cout<<"\tCreating Edge Detection Image......"<<endl;
    cout<<"\tEdge Detection Image Created named \"SobelImage.bmp\"......"<<endl;
    readImage.Export("SobelImage.bmp");

}


void Image::Angle_Calculation()
{

    colors_modified = vector<Color> (m_height*m_width);

   int gx[3][3] = {{-1,0,1},
        {-2,0,2},
        {-1,0,1}
    };

    int gy[3][3] = {{1,2,1},
        {0,0,0},
        {-1,-2,-1}
    };

    int upThreshold;
    cout<<"Enter Threshold upper value for Angle Detection : ";
    cin>>upThreshold;


    int downThreshold;
    cout<<"Enter Threshold lower value for Angle Detection : ";
    cin>>downThreshold;



    for(int y=1; y<m_height-1; ++y)
    {
        for(int x=1; x<m_width-1; ++x)
        {
            float Rx=0,Gx=0,Bx=0;


            Rx+=m_colors[(y-1)*m_width+(x-1)].r*gx[0][0]+m_colors[(y-1)*m_width+(x)].r*gx[0][1]+m_colors[(y-1)*m_width+(x+1)].r*gx[0][2];
            Gx+=m_colors[(y-1)*m_width+(x-1)].g*gx[0][0]+m_colors[(y-1)*m_width+(x)].g*gx[0][1]+m_colors[(y-1)*m_width+(x+1)].g*gx[0][2];
            Bx+=m_colors[(y-1)*m_width+(x-1)].b*gx[0][0]+m_colors[(y-1)*m_width+(x)].b*gx[0][1]+m_colors[(y-1)*m_width+(x+1)].b*gx[0][2];

            Rx+=m_colors[(y)*m_width+(x-1)].r*gx[1][0]+m_colors[(y)*m_width+(x)].r*gx[1][1]+m_colors[(y)*m_width+(x+1)].r*gx[1][2];
            Gx+=m_colors[(y)*m_width+(x-1)].g*gx[1][0]+m_colors[(y)*m_width+(x)].g*gx[1][1]+m_colors[(y)*m_width+(x+1)].g*gx[1][2];
            Bx+=m_colors[(y)*m_width+(x-1)].b*gx[1][0]+m_colors[(y)*m_width+(x)].b*gx[1][1]+m_colors[(y)*m_width+(x+1)].b*gx[1][2];

            Rx+=m_colors[(y+1)*m_width+(x-1)].r*gx[2][0]+m_colors[(y+1)*m_width+(x)].r*gx[2][1]+m_colors[(y+1)*m_width+(x+1)].r*gx[2][2];
            Gx+=m_colors[(y+1)*m_width+(x-1)].g*gx[2][0]+m_colors[(y+1)*m_width+(x)].g*gx[2][1]+m_colors[(y+1)*m_width+(x+1)].g*gx[2][2];
            Bx+=m_colors[(y+1)*m_width+(x-1)].b*gx[2][0]+m_colors[(y+1)*m_width+(x)].b*gx[2][1]+m_colors[(y+1)*m_width+(x+1)].b*gx[2][2];

            double Ry=0,Gy=0,By=0;

            Ry+=m_colors[(y-1)*m_width+(x-1)].r*gy[0][0]+m_colors[(y-1)*m_width+(x)].r*gy[0][1]+m_colors[(y-1)*m_width+(x+1)].r*gy[0][2];
            Gy+=m_colors[(y-1)*m_width+(x-1)].g*gy[0][0]+m_colors[(y-1)*m_width+(x)].g*gy[0][1]+m_colors[(y-1)*m_width+(x+1)].g*gy[0][2];
            By+=m_colors[(y-1)*m_width+(x-1)].b*gy[0][0]+m_colors[(y-1)*m_width+(x)].b*gy[0][1]+m_colors[(y-1)*m_width+(x+1)].b*gy[0][2];

            Ry+=m_colors[(y)*m_width+(x-1)].r*gy[1][0]+m_colors[(y)*m_width+(x)].r*gy[1][1]+m_colors[(y)*m_width+(x+1)].r*gy[1][2];
            Gy+=m_colors[(y)*m_width+(x-1)].g*gy[1][0]+m_colors[(y)*m_width+(x)].g*gy[1][1]+m_colors[(y)*m_width+(x+1)].g*gy[1][2];
            By+=m_colors[(y)*m_width+(x-1)].b*gy[1][0]+m_colors[(y)*m_width+(x)].b*gy[1][1]+m_colors[(y)*m_width+(x+1)].b*gy[1][2];

            Ry+=m_colors[(y+1)*m_width+(x-1)].r*gy[2][0]+m_colors[(y+1)*m_width+(x)].r*gy[2][1]+m_colors[(y+1)*m_width+(x+1)].r*gy[2][2];
            Gy+=m_colors[(y+1)*m_width+(x-1)].g*gy[2][0]+m_colors[(y+1)*m_width+(x)].g*gy[2][1]+m_colors[(y+1)*m_width+(x+1)].g*gy[2][2];
            By+=m_colors[(y+1)*m_width+(x-1)].b*gy[2][0]+m_colors[(y+1)*m_width+(x)].b*gy[2][1]+m_colors[(y+1)*m_width+(x+1)].b*gy[2][2];


            float resultR,AngleR;
            if (Rx==0)
            {
                AngleR=90;
            }
            else{
                resultR = (Ry/Rx);
                resultR=atan(resultR);
                AngleR=(resultR*180)/3.1415;

            }


            float resultG,AngleG;
            if (Gx==0)
            {
                AngleG=90;
            }
            else{
                resultG = (Gy/Gx);
                resultG=atan(resultG);
                AngleG=(resultG*180)/3.1415;

            }

            float resultB,AngleB;
            if (Bx==0)
            {
                AngleB=90;
                continue;
            }
            else{
                resultB = (By/Bx);
                resultB=atan(resultB);
                AngleB=(resultB*180)/3.1415;

            }


             if(AngleR > upThreshold)
            {
                AngleR = 255;
            }
            else
            {
               AngleR = 0;
            }
            if(AngleG > upThreshold)
            {
                AngleG = 255;
            }
            else
            {
               AngleG = 0;
            }
            if(AngleB > upThreshold)
            {
                AngleB = 255;
            }
            else
            {
               AngleB = 0;
            }





            colors_modified[y*m_width+x].r=AngleR;
            colors_modified[y*m_width+x].g=AngleG;
            colors_modified[y*m_width+x].b=AngleB;



        }

    }


    cout<<"---Angle---"<<endl;

    cout<<"\n"<<endl;
    cout<<"\t\t\t!!----Angle Calculation Complete---!!"<<endl;
    cout<<"\n"<<endl;

    cout<<"\tCreating  Image......"<<endl;
    cout<<"\tImage Created named \"AngleImage.bmp\"......"<<endl;
    readImage.Export("AngleImage.bmp");
}




void Image :: Histogram()
{

    colors_modified = vector<Color> (m_height*m_width);


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

            colors_modified[y*m_width+x].r=avg;
            colors_modified[y*m_width+x].g=avg;
            colors_modified[y*m_width+x].b=avg;
        }

    }

    int Hist[256]= {0};

    for(int y=0; y<m_height; ++y)
    {
        for(int x=0; x<m_width; ++x)
        {
            int val=0;
            val=colors_modified\
            [y*m_width+x].r;
            Hist[val]+=1;
        }
    }


    long long int cdf[256]= {0};
    cdf[0]=Hist[0];

    for(int i=1; i<256; ++i)
    {


        cdf[i]=cdf[i-1]+Hist[i];

    }


    int cdfMin;
    cdfMin=cdf[0];
    for(int i=1; i<256; ++i)
    {
        if(cdf[i]<cdfMin)
            cdfMin=cdf[i];
    }


    int histogram[256]= {0};

    for(int i=0; i<256; ++i)
    {
        histogram[i]=round((cdf[i]-cdfMin)*255)/((m_height*m_width)-cdfMin);

    }


     // Printing histogram
    printf("Histogram : \n\n");
    for(int i = 0; i < 256; i++)
    {
        if(i < 10)
        {
            printf("[%d]   | ",i);
            for(int j = 0; j <= (Hist[i]%20); j++)
            printf("*");
            printf("-->(%d)",Hist[i]);
            printf("\t\t\t\t\t\t\t   ||Equalized value {%d}",histogram[i]);
            printf("\n");
        }
        else if(i < 100)
        {
            printf("[%d]  | ",i);
            for(int j = 0; j <= (Hist[i]%20); j++)
            printf("*");
            printf("-->(%d)",Hist[i]);
            printf("\t\t\t\t\t\t\t   ||Equalized value {%d}",histogram[i]);
            printf("\n");
        }
        else if(i < 256)
        {
            printf("[%d] | ",i);
            for(int j = 0; j <= (Hist[i]%20); j++)
            printf("*");
            printf("-->(%d)",Hist[i]);
            printf("\t\t\t\t\t\t\t   ||Equalized value {%d}",histogram[i]);
            printf("\n");
        }
    }


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
    const int fileHeaderSize = 14;
    const int infoHeaderSize = 40;
    const int bytesPerPixel = 3;
    int paddingAmount = (4 - ((m_width * bytesPerPixel) % 4)) % 4;
    const uint32_t fileSize = fileHeaderSize + infoHeaderSize + (m_width * m_height * bytesPerPixel) + paddingAmount * m_height;


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

    f.write(reinterpret_cast<char*>(fileHeader),fileHeaderSize);
    f.write(reinterpret_cast<char*>(infoHeader),infoHeaderSize);


    for(int y=0; y<m_height; ++y)
    {
        for(int x=0; x<m_width; ++x)
        {
            unsigned char r=static_cast<unsigned char>(GetColor(x,y).r);
            unsigned char g=static_cast<unsigned char>(GetColor(x,y).g);
            unsigned char b=static_cast<unsigned char>(GetColor(x,y).b);

            unsigned char color[]= {b,g,r};
            f.write(reinterpret_cast<char*>(color),3);
        }
        f.write(reinterpret_cast<char*>(bmpPad),paddingAmount);
    }

    f.close();
    cout<<"File created"<<endl;

    readImage.ShowImage(path);



}



void Image :: printManu()
{
    cout<<"\n";
    cout<<"\t\t\t";
    cout<<"!!!----------Basic Image Processing Tools----------!!!"<<endl;

    cout<<endl;
    cout<<"1->Read Image"<<endl;
    cout<<"2->Negative"<<endl;
    cout<<"3->Grayscale"<<endl;
    cout<<"4->Brightening"<<endl;
    cout<<"5->Flip Image"<<endl;
    cout<<"6->Sharpen Image"<<endl;
    cout<<"7->Smoothing Image"<<endl;
    cout<<"8->Darkening"<<endl;
    cout<<"9->Gaussian Blur"<<endl;
    cout<<"10->Sobel Edge Detection"<<endl;
    cout<<"11->Angle Detection"<<endl;
    cout<<"12->Histogram"<<endl;
    cout<<"13->Export Image"<<endl;
    cout<<"14->Exit"<<endl;


}


int main()
{

    readImage.printManu();

    bool flag;
    flag=true;


    while(flag)
    {

        int choice;
        cout<<endl;;
        cout<<"choice : ";
        cin>>choice;


        switch(choice)
        {
        case 1:
            readImage.Read();
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
            readImage.Flip();
            break;

        case 6:
          readImage.Sharpen();
          break;

        case 7:
            readImage.Smoothing();
            break;

        case 8:
            readImage.Darken();
            break;

        case 9:
            readImage.Gaussian_Blur();
            break;

        case 10:
            readImage.SobelEdgeDetection();
            break;

        case 11:
            readImage.Angle_Calculation();
            break;

        case 12:
            readImage.Histogram();
            break;

        case 13:
            readImage.Export("TestImage.bmp");
            break;

        case 14:
            cout<<"\t\t\t\t...Thank You for using Image Processing Tools..."<<endl;
            flag=false;
            break;

        default :
            cout<<"Invalid Input !"<<endl;
            cout<<"Try Again."<<endl;
            flag=false;



        }

    }


    return 0;
}


