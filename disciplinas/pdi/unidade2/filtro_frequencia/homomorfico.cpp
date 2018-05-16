#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>

using namespace cv;
using namespace std;

Mat imaginaryInput, complexImage, multsp;
Mat image, imageln, padded, filter, mag;
Mat imagegray, tmp;
Mat_<float> realInput, zeros;
vector<Mat> planos;

// valores ideais dos tamanhos da imagem
// para calculo da DFT
int dft_M, dft_N;

double gama, corte, aten, d;
int x, y;
int gama_slider = 0, gama_slider_max = 100;
int corte_slider = 0, corte_slider_max = 100;
int aten_slider = 0, aten_slider_max = 100;
char TrackbarName[50];

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image)
{
    Mat tmp, A, B, C, D;

    // se a imagem tiver tamanho impar, recorta a regiao para
    // evitar cópias de tamanho desigual
    image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
    int cx = image.cols/2;
    int cy = image.rows/2;

    // reorganiza os quadrantes da transformada
    // A B   ->  D C
    // C D       B A
    A = image(Rect(0, 0, cx, cy));
    B = image(Rect(cx, 0, cx, cy));
    C = image(Rect(0, cy, cx, cy));
    D = image(Rect(cx, cy, cx, cy));

    // A <-> D
    A.copyTo(tmp);
    D.copyTo(A);
    tmp.copyTo(D);

    // C <-> B
    C.copyTo(tmp);
    B.copyTo(C);
    tmp.copyTo(B);
}

void on_trackbar_gama(int, void*)
{
    tmp = Mat::zeros(tmp.size(), CV_32F);

    x = tmp.rows/2; y = tmp.cols/2;

    gama = (float)(gama_slider_max - gama_slider)/100;

    for(int i=0; i<tmp.rows; i++)
    {
        for(int j=0; j<tmp.cols; j++)
        {
            d = (i-x)*(i-x)+(j-y)*(j-y);
            if((i-x)*(i-x)+(j-y)*(j-y) <= corte*corte)
                tmp.at<float>(i,j) = (gama*(1-pow(M_E, -aten*d/(100))))+(float)gama_slider/100;
        }
    }

    // cria a matriz com as componentes do filtro e junta
    // ambas em uma matriz multicanal complexa
    Mat comps[]= {tmp, tmp};
    merge(comps, 2, filter);

    // cria a compoente real
    realInput = Mat_<float>(padded);

    // limpa o array de matrizes que vao compor a
    // imagem complexa
    planos.clear();

    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // combina o array de matrizes em uma unica
    // componente complexa
    merge(planos, complexImage);

    // calcula o dft
    dft(complexImage, complexImage);

    // realiza a troca de quadrantes
    deslocaDFT(complexImage);

    // aplica o filtro frequencial
    mulSpectrums(complexImage,filter,complexImage,0);

    // calcula a DFT inversa
    idft(complexImage, complexImage);

    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria da
    // imagem filtrada
    split(complexImage, planos);

    // aplicando exponencial na imagem filtrada
    for(int i=0; i<imageln.rows; i++)
    {
        for(int j=0; j<imageln.cols; j++)
        {
            planos[0].at<float>(i,j) = pow(M_E, planos[0].at<float>(i,j));
        }
    }

    // normaliza a parte real para exibicao
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    imshow("Homomorfico", planos[0]);
    imwrite("homomorfico.png", planos[0]);
}

void on_trackbar_corte(int, void*)
{
    corte = corte_slider*(tmp.rows*0.7)/100;
    on_trackbar_gama(gama_slider, 0);
}

void on_trackbar_aten(int, void*)
{
    aten = (float)(aten_slider*aten_slider)/100000;
    on_trackbar_gama(gama_slider, 0);
}

int main(int argc, char** argv)
{
    image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

    namedWindow("Homomorfico", WINDOW_AUTOSIZE);

    // identifica os tamanhos otimos para
    // calculo do FFT
    dft_M = getOptimalDFTSize(image.rows);
    dft_N = getOptimalDFTSize(image.cols);

    // criando matriz para receber logaritmo da imagem
    imageln = Mat(image.size(), CV_32F, Scalar(0));

    // aplicando logaritmo a imagem
    for(int i=0; i<imageln.rows; i++)
    {
        for(int j=0; j<imageln.cols; j++)
        {
            imageln.at<float>(i,j) = (float)log(image.at<uchar>(i,j));
        }
    }

    // realiza o padding da imagem logaritmica
    copyMakeBorder(imageln, padded, 0,
                   dft_M - image.rows, 0,
                   dft_N - image.cols,
                   BORDER_CONSTANT, Scalar::all(0));

    // parte imaginaria da matriz complexa (preenchida com zeros)
    zeros = Mat_<float>::zeros(padded.size());

    // prepara a matriz complexa para ser preenchida
    complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

    // a função de transferência (filtro frequencial) deve ter o
    // mesmo tamanho e tipo da matriz complexa
    filter = complexImage.clone();

    // cria uma matriz temporária para criar as componentes real
    // e imaginaria do filtro ideal
    tmp = Mat(dft_M, dft_N, CV_32F);

    //Criando primeira TrackBar
    sprintf(TrackbarName, "Gama: ");
    createTrackbar(TrackbarName, "Homomorfico", &gama_slider, gama_slider_max, on_trackbar_gama);
    on_trackbar_gama(gama_slider, 0);

    //Criando segunda TrackBar
    sprintf(TrackbarName, "Corte: ");
    createTrackbar(TrackbarName, "Homomorfico", &corte_slider, corte_slider_max, on_trackbar_corte);
    on_trackbar_corte(corte_slider, 0);

    //Criando terceira TrackBar
    sprintf(TrackbarName, "Atenuacao: ");
    createTrackbar(TrackbarName, "Homomorfico", &aten_slider, aten_slider_max, on_trackbar_aten);
    on_trackbar_aten(aten_slider, 0);

    waitKey(0);

    return 0;
}
