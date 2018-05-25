#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int slider=0, slider_max=100;
char TrackbarName[50] = "teste";

void on_trackbar_teste(int, void*)
{
    cout << "funcao da barrinhaaaa" << endl;
}

int main(void)
{
    Mat image = imread("biel-brilhoso.png");

    namedWindow("teste-barrinha", WINDOW_AUTOSIZE);

    sprintf(TrackbarName, "teste: ");
    createTrackbar(TrackbarName, "teste-barrinha", &slider, slider_max, on_trackbar_teste);

    cout << "funcao mainnnn" << endl;

    imshow("teste-barrinha", image);
    waitKey(0);

    return 0;
}
