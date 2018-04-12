#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void printmask(Mat &m)
{
    for(int i=0; i<m.size().height; i++)
    {
        for(int j=0; j<m.size().width; j++)
        {
            cout << m.at<float>(i,j) << ",";
        }
        cout << endl;
    }
}

void menu()
{
    cout << "\npressione a tecla para ativar o filtro: \n"
         "a - calcular modulo\n"
         "m - media\n"
         "g - gauss\n"
         "v - vertical\n"
         "h - horizontal\n"
         "l - laplaciano\n"
         "d - laplaciano do gaussiano\n"
         "esc - sair\n";
}

int main(int argvc, char** argv)
{
    VideoCapture video;
    float media[] = {1,1,1,
                     1,1,1,
                     1,1,1
                    };
    float gauss[] = {1,2,1,
                     2,4,2,
                     1,2,1
                    };
    float horizontal[]= {-1,0,1,
                         -2,0,2,
                         -1,0,1
                        };
    float vertical[]= {-1,-2,-1,
                       0,0,0,
                       1,2,1
                      };
    float laplacian[]= {0,-1,0,
                        -1,4,-1,
                        0,-1,0
                       };

    Mat cap, frame, frame32f, frameFiltered, frameFilteredLG;
    Mat mask(3,3,CV_32F), mask1;
    Mat result, result1;
    double width, height, min, max;
    int absolut, laplgauss;
    char key;

    video.open(0);
    if(!video.isOpened())
        return -1;
    width=video.get(CV_CAP_PROP_FRAME_WIDTH);
    height=video.get(CV_CAP_PROP_FRAME_HEIGHT);
    cout << "largura=" << width << "\n";;
    cout << "altura =" << height<< "\n";;

    namedWindow("filtroespacial",1);

    mask = Mat(3, 3, CV_32F, media);
    scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
    swap(mask, mask1);
    absolut=1; // calcs abs of the image
    laplgauss=0; //ativa filtro laplaciano do gaussiano

    menu();
    for(;;)
    {
        video >> cap;
        cvtColor(cap, frame, CV_BGR2GRAY);
        flip(frame, frame, 1);
        imshow("original", frame);
        frame.convertTo(frame32f, CV_32F);
        if(laplgauss)
        {
            mask = Mat(3, 3, CV_32F, gauss);
            filter2D(frame32f, frameFilteredLG, frame32f.depth(), mask, Point(1,1), 0);
            mask = Mat(3, 3, CV_32F, laplacian);
            filter2D(frameFilteredLG, frameFiltered, frameFilteredLG.depth(), mask, Point(1,1), 0);
        }
        else filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(1,1), 0);
        if(absolut)
        {
            frameFiltered=abs(frameFiltered);
        }
        frameFiltered.convertTo(result, CV_8U);
        imshow("filtroespacial", result);
        key = (char) waitKey(10);
        if( key == 27 ) break; // esc pressed!
        switch(key)
        {
        case 'a':
            menu();
            absolut=!absolut;
            break;
        case 'm':
            menu();
            if(laplgauss) laplgauss=!laplgauss;
            mask = Mat(3, 3, CV_32F, media);
            scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
            mask = mask1;
            cout << endl;
            printmask(mask);
            break;
        case 'g':
            menu();
            if(laplgauss) laplgauss=!laplgauss;
            mask = Mat(3, 3, CV_32F, gauss);
            scaleAdd(mask, 1/16.0, Mat::zeros(3,3,CV_32F), mask1);
            mask = mask1;
            cout << endl;
            printmask(mask);
            break;
        case 'h':
            menu();
            if(laplgauss) laplgauss=!laplgauss;
            mask = Mat(3, 3, CV_32F, horizontal);
            cout << endl;
            printmask(mask);
            break;
        case 'v':
            menu();
            if(laplgauss) laplgauss=!laplgauss;
            mask = Mat(3, 3, CV_32F, vertical);
            cout << endl;
            printmask(mask);
            break;
        case 'l':
            menu();
            if(laplgauss) laplgauss=!laplgauss;
            mask = Mat(3, 3, CV_32F, laplacian);
            cout << endl;
            printmask(mask);
            break;
        case 'd':
            menu();
            laplgauss=1;
            mask = Mat(3, 3, CV_32F, gauss);
            cout << endl;
            printmask(mask);
            cout << endl;
            mask = Mat(3, 3, CV_32F, laplacian);
            printmask(mask);
            break;
        default:
            break;
        }
    }
    return 0;
}
