#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;

Mat org, borrada, resultado, teste;

double alpha, gama;
int delta_slider=50, delta_slider_max = 100;
int pos_slider = 50, pos_slider_max = 100;
int aten_slider=1, aten_slider_max = 100;
char TrackbarName[50];


//Ação da primeira barra
void on_trackbar_altura(int, void*)
{
    int l = (delta_slider*org.rows)/(2*delta_slider_max);
    int centro = org.rows/2;

    //Preenchendo mat resultado linha por linha utilizando a função addWeighted
    for(int i=0; i<org.cols; i++)
    {
        for(int j=0; j<org.rows; j++)
        {
            alpha = 0.5*(tanh((j-centro+l+pos_slider)/(2*(float)aten_slider))-tanh((j-centro-l+pos_slider)/(2*(float)aten_slider)));
            gama = 1.0 - alpha;
            addWeighted(org.row(j), alpha, borrada.row(j), gama, 0.0, resultado.row(j));
        }
    }
    imshow("Tiltshild", resultado);
    imwrite("tiltshift_resultado.png", resultado);
}

//Ação da segunda barra
void on_trackbar_pos(int, void*)
{
    if(pos_slider == 50)
    {
        pos_slider = 0;
    }
    else if(pos_slider < 50)
    {
        pos_slider = 50 - pos_slider;
        pos_slider = (pos_slider*org.rows)/pos_slider_max;
    }
    else
    {
        pos_slider = (pos_slider - 50)*(-1);
        pos_slider = (pos_slider*org.rows)/pos_slider_max;
    }
    on_trackbar_altura(delta_slider, 0);
}

//Ação da terceira barra
void on_trackbar_aten(int, void*)
{
    aten_slider += 1;
    on_trackbar_altura(delta_slider, 0);
}

int main(int argc, char** argv)
{
    //Abrindo imagem passada por parâmetro
    org = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    org.copyTo(borrada);
    org.copyTo(resultado);
    teste = Mat(org.rows, org.cols, org.depth());

    namedWindow("Tiltshild", WINDOW_AUTOSIZE);

    //Criando versão borrada da imagem original
    float media[] = {1,1,1,1,1,1,1,1,1};
    Mat mask(3, 3, CV_32F, media);
    scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask);
    for(int i=0; i<20; i++) filter2D(borrada, borrada, org.depth(), mask, Point(1,1), 0);

    //Criando primeira TrackBar
    sprintf(TrackbarName, "Altura: ");
    createTrackbar(TrackbarName, "Tiltshild", &delta_slider, delta_slider_max, on_trackbar_altura);
    on_trackbar_altura(delta_slider, 0);

    //Criando segunda TrackBar
    sprintf(TrackbarName, "Posicao: ");
    createTrackbar(TrackbarName, "Tiltshild", &pos_slider, pos_slider_max, on_trackbar_pos);
    on_trackbar_pos(pos_slider, 0);

    //Criando terceira TrackBar
    sprintf(TrackbarName, "Atenuacao: ");
    createTrackbar(TrackbarName, "Tiltshild", &aten_slider, aten_slider_max, on_trackbar_aten);
    on_trackbar_aten(aten_slider, 0);

    waitKey(0);

	return 0;
}
