#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    Mat src, histR, histROld;
    vector<Mat> planes;
    int width, height;
    VideoCapture cap;
    int nbins = 64, cont=0; //Definindo quantidade de faixas (bins) do histograma.
    float range[] = {0, 256}; //Definindo faixa de valores assumidos pelo histograma.
    const float *histrange = { range };
    bool uniform = true;
    bool acummulate = false;

    //Abrindo dispositivo de vídeo (câmera):
    cap.open(0);
    if(!cap.isOpened())
    {
        cout << "cameras indisponiveis";
        return -1;
    }

    //Capturando largura e altura do vídeo:
    width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    cout << "largura = " << width << endl;
    cout << "altura  = " << height << endl;

    //Definindo largura e altura da imagem que irá exibir o histograma:
    int histw = nbins, histh = nbins/2;
    Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));

    //Capturando imagem inicial e calculando seu histograma:
    cap >> src;
    split (src, planes);
    calcHist(&planes[0], 1, 0, Mat(), histROld, 1,
            &nbins, &histrange,
            uniform, acummulate);
    normalize(histROld, histROld, 0, histImgR.rows, NORM_MINMAX, -1, Mat());

    //Capturando imagens continuamente, calculando e verificando variação de histograma entre dois quadros sucessivos:
    while(1)
    {
        cap >> src;
        split (src, planes);
        calcHist(&planes[0], 1, 0, Mat(), histR, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
        normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());

        //Contando quantas das faixas de valores do histograma variaram mais de 70% de um quadro para o seu sucessor:
        for(int i=0; i<nbins; i++)
        {
            if(abs(cvRound(histR.at<float>(i))-cvRound(histROld.at<float>(i))) > 0.7*cvRound(histR.at<float>(i))) cont++;
        }
        //Verificando se a quantidade faixas que variaram é maior que 50% do número total de faixas:
        if(cont >= 0.5*nbins)
        {
            cout << "\aMovimento detectado!!!" << endl;
            cont = 0;
        }
        //Armazenando histograma antigo para comparar com o próximo:
        histR.copyTo(histROld);

        //Reiniciando a cor da matriz de exibição do histograma:
        histImgR.setTo(Scalar(0));

        //Desenhando linhas do histograma na matriz:
        for(int i=0; i<nbins; i++)
        {
            line(histImgR,
                 Point(i, histh),
                 Point(i, histh-cvRound(histR.at<float>(i))),
                 Scalar(0, 0, 255), 1, 8, 0);
        }
        //Copiando matriz do histograma para imagem que será exibida em tempo real:
        histImgR.copyTo(src(Rect(0, 0,nbins, histh)));

        //Exibindo imagem da câmera com o histograma superposto no canto superior esquerdo:
        imshow("Detector de movimentos", src);
        if(waitKey(30) >= 0) break;
    }
    return 0;
}
