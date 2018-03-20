#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    Mat image;
    int width, height, nobjects=0;
    CvPoint p;

    //Carregando imagem em tons de cinza:
    image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

    //Valindando carregamento da imagem:
    if(!image.data)
    {
        cout << "imagem nao carregou corretamente\n";
        return(-1);
    }

    //Lendo tamanho da imagem:
    width=image.cols;
    height=image.rows;

    //Contando todos objetos de tom 255 (incluindo os que tocam as bordas e os que possuem buracos):
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            //Achou um objeto:
            if(image.at<uchar>(i,j) == 255)
            {
                nobjects++;
                p.x=j;
                p.y=i;
                floodFill(image,p,254);
            }
        }
    }

    //Imprimindo resultados da contagem realizada acima:
    cout << "Numero total de bolhas: " << nobjects << endl;
    nobjects = 0;

    //Exibindo e salavando imagem (semelhante à original):
    imshow("Resultado", image);
    imwrite("labelingtotal.png", image);
    waitKey();

    //Removendo objetos das bordas verticais:
    for(int i=0; i<height; i++)
    {
        //Borda esquerda:
        if(image.at<uchar>(i,0) != 0)
        {
            p.x=0;
            p.y=i;
            floodFill(image,p,0);
        }
        //Borda direita:
        if(image.at<uchar>(i,width-1) != 0)
        {
            p.x=width-1;
            p.y=i;
            floodFill(image,p,0);
        }
    }

    //Removendo objetos das bordas horizontais:
    for(int i=0; i<width; i++)
    {
        //Borda superior:
        if(image.at<uchar>(0,i) != 0)
        {
            p.x=i;
            p.y=0;
            floodFill(image,p,0);
        }
        //Borda inferior:
        if(image.at<uchar>(height-1,i) != 0)
        {
            p.x=i;
            p.y=height-1;
            floodFill(image,p,0);
        }
    }

    //Recontando os objetos (excluindo os que tocam as bordas):
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            //Achou um objeto:
            if(image.at<uchar>(i,j) == 254)
            {
                nobjects++;
                p.x=j;
                p.y=i;
                floodFill(image,p,253);
            }
        }
    }

    //Imprimindo recontagem (excluindo objetos que tocam as bordas):
    cout << "Numero de bolhas (removendo as que tocam as bordas): " << nobjects << endl;
    nobjects = 0;

    //Exibindo e salavando imagem (sem bolhas que tocam as bordas):
    imshow("Resultado", image);
    imwrite("labelingsemborda.png", image);
    waitKey();

    p.x = 0;
    p.y = 0;
    floodFill(image,p,100);

    //Identificando e contando objetos com buracos:
    for(int i=1; i<height-1; i++)
    {
        for(int j=1; j<width-1; j++)
        {
            if(image.at<uchar>(i,j) == 0 && image.at<uchar>(i,j-1) == 253)
            {
                nobjects++;
                p.x=j-1;
                p.y=i;
                floodFill(image,p,180);
                p.x=j;
                p.y=i;
                floodFill(image,p,180);
            }
        }
    }

    //Imprimindo número de bolhas com burados:
    cout << "Numero de bolhas com buracos: " << nobjects << endl;

    //Exibindo e salavando imagem (destacando objetos que possuiam buracos):
    imshow("Resultado", image);
    imwrite("labelingcomburacos.png", image);
    waitKey();

    return 0;
}
