#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**)
{
    Mat image;
    CvPoint pmed;

    //Carregando imagem e armazenando no obejto image:
    image = imread("biel.png",CV_LOAD_IMAGE_COLOR);
    //Verificando carregamento:
    if(!image.data)
        cerr << "Imagem nao carregada! Tente novamente." << endl;

    //Criando janela para exibição:
    namedWindow("Regioes trocadas", WINDOW_AUTOSIZE);

    //Definindo falores centrais de cada coordenada:
    pmed.x = image.rows/2;
    pmed.y = image.cols/2;

    //Criando matriz auxiliar para armazenar quadrantes:
    Mat temp(pmed.x, pmed.y, CV_8UC3);

    //Alterando posições dos quadrantes:
    for(int i=0; i<pmed.x; i++)
    {
        for(int j=0; j<pmed.y; j++)
        {
            temp.at<Vec3b>(i, j) = image.at<Vec3b>(i,j);
            image.at<Vec3b>(i,j) = image.at<Vec3b>(i+pmed.x,j+pmed.y);
            image.at<Vec3b>(i+pmed.x,j+pmed.y) = temp.at<Vec3b>(i, j);
            temp.at<Vec3b>(i, j) = image.at<Vec3b>(i+pmed.x,j);
            image.at<Vec3b>(i+pmed.x,j) = image.at<Vec3b>(i,j+pmed.y);
            image.at<Vec3b>(i,j+pmed.y) = temp.at<Vec3b>(i, j);
        }
    }

    //Exibindo imagem:
    imshow("Regioes trocadas", image);
    imwrite("regioes_trocadas.jpg", image);
    waitKey();
    return 0;
}
