#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    VideoCapture cap;
    VideoWriter out;
    Mat frame, discart;
    string video_name;
    int width, height, FPS, codec, _start, _end, _duration;

    cap.open(argv[1]);

    if(!cap.isOpened())
    {
        cerr << "Erro ao abrir video!!!" << endl;
        return (-1);
    }

    codec = cap.get(CV_CAP_PROP_FOURCC);
    FPS = cap.get(CV_CAP_PROP_FPS);
    width = (int) cap.get(CV_CAP_PROP_FRAME_WIDTH); height = (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    Size S = Size(width, height);

    cout << "Nome do arquivo (inserir extensao .avi no final 'exemplo.avi'):" << endl;
    cin >> video_name;
    cout << "Inicio do video (segundos):" << endl;
    cin >> _start;
    cout << "Fim do video (segundos):" << endl;
    cin >> _end;

    _duration = _end - _start;

    out.open(video_name, codec, FPS, S);
    if(!out.isOpened())
    {
        cerr << "Erro ao salvar video!!!" << endl;
        return (-1);
    }

    cout << "Cortando video..." << endl;

    for(int i=0; i<_start*FPS; i++) cap >> discart;

    for(int i=0; i<_duration*FPS; i++)
    {
        cap >> frame;
        out << frame;
    }

    return 0;
}

