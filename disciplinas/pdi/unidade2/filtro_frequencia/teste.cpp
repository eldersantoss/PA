#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>

using namespace cv;
using namespace std;

// matrizes p/ armazenamento das imagens em cada etapa
Mat imaginaryInput, complexImage, multsp;
Mat image, imageln, padded, filter, mag;
Mat tmp;
Mat_<float> realInput, zeros;
vector<Mat> planos;

// valores ideais dos tamanhos da imagem
// para calculo da DFT
int dft_M, dft_N;

// variaveis de controle das trackbars de regulacao do filtro
float gama, corte, aten, d;
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

    gama = (float)(gama_slider_max - gama_slider)/100;

    for(int i=0; i<tmp.rows; i++)
    {
        for(int j=0; j<tmp.cols; j++)
        {
            d = (i-x)*(i-x)+(j-y)*(j-y);
            if((i-x)*(i-x)+(j-y)*(j-y) <= corte*corte)
                tmp.at<float>(i,j) = gama * (1 - exp(-1*aten*(d/(corte*corte))))+(float)gama_slider/100;
        }
    }

    // cria a matriz com as componentes do filtro e junta
    // ambas em uma matriz multicanal complexa
    Mat comps[]= {tmp, tmp};
    merge(comps, 2, filter);

    // aplica o filtro frequencial
    mulSpectrums(complexImage,filter,complexImage,0);

    deslocaDFT(complexImage);

    idft(complexImage, complexImage);

    // aplicando exponencial a imagem transformada
    for(int i=0; i<dft_M; i++)
        for(int j=0; j<dft_N; j++)
            complexImage.at<char>(i,j) = exp(complexImage.at<char>(i,j));

    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria para modifica-las
    split(complexImage, planos);

    // normaliza a parte real para exibicao
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    imshow("Homomorfico", planos[0]);
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

    //Criando primeira TrackBar
    sprintf(TrackbarName, "Gama: ");
    createTrackbar(TrackbarName, "Homomorfico", &gama_slider, gama_slider_max, on_trackbar_gama);


    //Criando segunda TrackBar
    sprintf(TrackbarName, "Corte: ");
    createTrackbar(TrackbarName, "Homomorfico", &corte_slider, corte_slider_max, on_trackbar_corte);

    //Criando terceira TrackBar
    sprintf(TrackbarName, "Atenuacao: ");
    createTrackbar(TrackbarName, "Homomorfico", &aten_slider, aten_slider_max, on_trackbar_aten);

    // identifica os tamanhos otimos para
    // calculo do FFT
    dft_M = getOptimalDFTSize(image.rows);
    dft_N = getOptimalDFTSize(image.cols);

    // realiza o padding da imagem logaritmica
    copyMakeBorder(image, padded, 0,
                   dft_M - image.rows, 0,
                   dft_N - image.cols,
                   BORDER_CONSTANT, Scalar::all(0));

    // definindo centro do filtro
    x = dft_M/2; y = dft_N/2;

    // prepara a matriz complexa para ser preenchida
    complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

    // a função de transferência (filtro frequencial) deve ter o
    // mesmo tamanho e tipo da matriz complexa
    filter = complexImage.clone();

    // cria uma matriz temporária para criar as componentes real
    // e imaginaria do filtro ideal
    tmp = Mat(dft_M, dft_N, CV_32F);

    // parte imaginaria da matriz complexa (preenchida com zeros)
    zeros = Mat_<float>::zeros(padded.size());

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

    // aplicando logaritmo a imagem
    for(int i=0; i<dft_M; i++)
        for(int j=0; j<dft_N; j++)
            complexImage.at<char>(i,j) = log(complexImage.at<char>(i,j)+1);

    // calcula o dft
    dft(complexImage, complexImage);

    // realiza a troca de quadrantes
    deslocaDFT(complexImage);

    waitKey(0);

    // salvando imagem com filtro aplicado
    imwrite("homomorfico.png", planos[0]);

    return 0;
}
