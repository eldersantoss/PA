#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


Mat original, borrada, combinada, alfa, gama = Mat::ones(alfa.rows, alfa.cols, CV_32F);

char TrackbarName[50];


void on_trackbar_altura(int, void*)
{
    alfa =
    addWeighted( image1, alfa, imageTop, 1-alfa, 0.0, blended);
    imshow("addweighted", blended);
}

void on_trackbar_decaimento(int, void*)
{
    alfa = (double) alfa_slider/alfa_slider_max ;
    addWeighted( image1, alfa, imageTop, 1-alfa, 0.0, blended);
    imshow("addweighted", blended);
}

void on_trackbar_posicao(int, void*)
{
    alfa = (double) alfa_slider/alfa_slider_max ;
    addWeighted( image1, alfa, imageTop, 1-alfa, 0.0, blended);
    imshow("addweighted", blended);
}


int main()
{

    namedWindow("Tiltshift", 1);

    sprintf(TrackbarName, "Altura central:");
    createTrackbar(TrackbarName, "Tiltshift",
                    &alfa_slider,
                    alfa_slider_max,
                    on_trackbar_altura);
    on_trackbar_blend(alfa_slider, 0 );

    return 0;
}
