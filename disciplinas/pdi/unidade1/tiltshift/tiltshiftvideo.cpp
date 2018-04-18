#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <cmath>


using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
    Mat org_frame, dsct_frame, brr_frame, comb_frame;
    VideoCapture cap;
    VideoWriter rnd;
    double alpha, gama;
    int width, height, centro, FPS, codec;


    //Abrindo video
    cap.open(argv[1]);
    if(!cap.isOpened())
    {
        cerr << "Video nao carregou corretamente!!!" << endl;
        return (-1);
    }

    // Atribuindo valores aos parametros necessarios para abrir o canal de salvamento do video
    const string video_name = "video-tiltshift.avi";
    codec = CV_FOURCC('X','V','I','D');
    FPS = cap.get(CV_CAP_PROP_FPS);
    width = (int) cap.get(CV_CAP_PROP_FRAME_WIDTH); height = (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    Size S = Size(width, height);

    centro = height/2;

    // Abrindo canal de salvamento de video
    rnd.open(video_name, codec, FPS, S);
    if(!rnd.isOpened())
    {
        cerr << "Erro na criacao do video!!!" << endl;
        return (-1);
    }

    // Lendo quadros do video e aplicando efeitos tiltshift + stop motion
    for(;;)
    {
        // Capturando frames da imagem
        cap >> org_frame;

        // Descartando 3 frames para cada captura (stop motion)
        for(int i=0; i<3; i++) cap >> dsct_frame;

        // Condiço de parada do laço
        if(org_frame.empty()) break;

        // Preparando imagem borrada
        org_frame.copyTo(brr_frame);
        float media[] = {1,1,1,1,1,1,1,1,1};
        Mat mask(3, 3, CV_32F, media);
        scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask);
        for(int i=0; i<20; i++) filter2D(brr_frame, brr_frame, org_frame.depth(), mask, Point(1,1), 0);

        // Pre-alocando matriz que recebera o frame final
        comb_frame = Mat::zeros(S, org_frame.type());

        // Aplicando efeito tiltshift no quadro
        for(int i=0; i<org_frame.cols; i++)
        {
            for(int j=0; j<org_frame.rows; j++)
            {
                alpha = 0.5*(tanh((j-centro/2)/10)-tanh((j-(3*centro)/2)/10));
                gama = 1.0 - alpha;
                addWeighted(org_frame.row(j), alpha, brr_frame.row(j), gama, 0.0, comb_frame.row(j));
            }
        }

        // Salvando video com efeitos aplicados
        rnd << comb_frame;
    }
    return 0;
}
