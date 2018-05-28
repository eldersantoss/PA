#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <numeric>

using namespace std;
using namespace cv;

#define STEP 10
#define JITTER 3

Mat image, points;

int width, height, gray;

float RAIO = 10.0;

void pontilhismo()
{
    int x,y;
    vector<int> yrange;
    vector<int> xrange;

    xrange.resize(height/STEP);
    yrange.resize(width/STEP);

    iota(xrange.begin(), xrange.end(), 0);
    iota(yrange.begin(), yrange.end(), 0);

    for(uint i=0; i<xrange.size(); i++)
    {
        xrange[i]= xrange[i]*STEP+STEP/2;
    }

    for(uint i=0; i<yrange.size(); i++)
    {
        yrange[i]= yrange[i]*STEP+STEP/2;
    }

    points = Mat(height, width, CV_8U, Scalar(255));

    random_shuffle(xrange.begin(), xrange.end());

    for(auto i : xrange)
    {
        random_shuffle(yrange.begin(), yrange.end());
        for(auto j : yrange)
        {
            x = i+rand()%(2*JITTER)-JITTER+1;
            y = j+rand()%(2*JITTER)-JITTER+1;
            gray = image.at<uchar>(x,y);
            circle(points,
                   cv::Point(y,x),
                   RAIO,
                   CV_RGB(gray,gray,gray),
                   -1,
                   CV_AA);
        }
    }
}

void canny()
{
    Mat border;
    int i, j, k, T1 = 1;

    // executando 5 vezes o algoritmo de canny para 5 limeares diferentes
    for(i=0; i<5; i++)
    {
        Canny(image, border, T1, 3*T1);
        // incrementando limear
        T1 += 10;

        // gerando pontos menores na imagem pontilhista para posicoes de borda
        for(j=0; j<height; j++)
        {
            for(k=0; k<width; k++)
            {
                if(border.at<uchar>(j,k)==255)
                {
                    gray = image.at<uchar>(j,k);
                    circle(points,cv::Point(k,j),RAIO,CV_RGB(gray,gray,gray),-1,CV_AA);
                }
            }
        }

        // diminuindo raio do circulo para proxima iterecao
        RAIO -= 1.5;
    }

}

int main(int argc, char** argv)
{
    image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

    if(!image.data)
    {
        cout << "nao abriu" << argv[1] << endl;
        cout << argv[0] << " imagem.jpg";
        exit(0);
    }

    width=image.size().width;
    height=image.size().height;

    pontilhismo();
    canny();

    imwrite("cannypoints.jpg", points);
    return 0;
}
