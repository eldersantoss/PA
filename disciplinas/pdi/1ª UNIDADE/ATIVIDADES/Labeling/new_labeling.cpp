#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    Mat image, mask;
    int width, height;
    Vec3i nobjects;

    CvPoint p;
    image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

    if(!image.data)
    {
        cout << "imagem nao carregou corretamente\n";
        return(-1);
    }
    width=image.cols;
    height=image.rows;

    p.x=0;
    p.y=0;

    nobjects[0]=0;
    nobjects[1]=0;
    nobjects[2]=0;

    //Contando todos os objetos (com ou sem buracos):
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            //Achou um objeto:
            if(image.at<uchar>(i,j) == 255)
            {
                nobjects[0]++;
                //Reiniciando a contagem quando alacançar 255 objetos e guardando quantidade:
                if(nobjects[0] == 255)
                {
                    nobjects[1] += nobjects[0];
                    nobjects[0] = 0;
                }
                p.x=j;
                p.y=i;
                floodFill(image,p,nobjects[0]);
            }
        }
    }
    nobjects[1] += nobjects[0];
    nobjects[0] = 0;

    //Numero total de obejtos na figura:
    cout << "Numero de objetos detectados na figura (incluindo os que tocam as bordas): " << nobjects[1] << endl;

    //Exibindo e salavando imagem modificada (excluindo objetos que tocam as bordas):
    imshow("image", image);
    imwrite("labeling.png", image);
    waitKey();

    //Abrindo novamente a imagem:
    image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

    if(!image.data)
    {
        cout << "imagem nao carregou corretamente\n";
        return(-1);
    }

    //Removendo objetos das bordas verticais:
    for(int i=0; i<height; i++)
    {
        if(image.at<uchar>(i,0) == 255)
        {
            p.x=0;
            p.y=i;
            floodFill(image,p,nobjects[0]);
        }
        if(image.at<uchar>(i,width-1) == 255)
        {
            p.x=width-1;
            p.y=i;
            floodFill(image,p,nobjects[0]);
        }
    }

    //Removendo objetos das bordas horizontais:
    for(int i=0; i<width; i++)
    {
        if(image.at<uchar>(0,i) == 255)
        {
            p.x=i;
            p.y=0;
            floodFill(image,p,nobjects[0]);
        }
        if(image.at<uchar>(height-1,i) == 255)
        {
            p.x=i;
            p.y=height-1;
            floodFill(image,p,nobjects[0]);
        }
    }

    //Identificando e contando objetos com buracos:
    for(int i=0; i<height;i++)
    {
        for(int j=0; j<width; j++)
        {
            if(image.at<uchar>(i,j) == 0 && image.at<uchar>(i,j-1) == 255)
            {
                nobjects[0]++;
                //Reiniciando a contagem quando alacançar 255 objetos e guardando quantidade:
                if(nobjects[0] == 255)
                {
                    nobjects[2] += nobjects[0];
                    nobjects[0] = 0;
                }
                p.x=j;
                p.y=i-1;
                floodFill(image,p,0);
                p.x=j;
                p.y=i;
                floodFill(image,p,255);
            }
        }
    }
    nobjects[2] += nobjects[0];

    //Numero total de obejtos com burados na figura:
    cout << "Numero de objetos com buracos detectados na figura: " << nobjects[2] << endl;

    //Exibindo e salavando imagem modificada (contadando objetos com buracos):
    imshow("image", image);
    imwrite("labeling_2.png", image);
    waitKey();

    return 0;
}
