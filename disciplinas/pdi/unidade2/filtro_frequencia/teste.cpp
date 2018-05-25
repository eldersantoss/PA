#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>

using namespace cv;
using namespace std;

// matrizes p/ armazenamento das imagens em cada etapa
Mat complexImage;
Mat image, padded, filter;
Mat tmp, result;
Mat_<float> realInput, zeros;
vector<Mat> planos;

// valores ideais dos tamanhos da imagem
// para calculo da DFT
int dft_M, dft_N;

// variaveis de controle das trackbars de regulacao do filtro
float g_h, g_l, corte, aten, d;
int gamaH_slider = 0, gamaH_slider_max = 100;
int gamaL_slider = 0, gamaL_slider_max = 100;
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

void on_trackbar_filtro(int, void*)
{
    //tmp = Mat::zeros(tmp.size(), CV_32F);

    // recebendo valores das trackbars
    g_h = gamaH_slider; g_l = gamaL_slider; corte = corte_slider; aten = aten_slider;

    // preparando filtro homomorfico a partir dos valores das trackbars
    for(int i=0; i<dft_M; i++)
    {
        for(int j=0; j<dft_N; j++)
        {
            d = (i-dft_M/2)*(i-dft_M/2)+(j-dft_N/2)*(j-dft_N/2);
            tmp.at<float>(i,j) = (g_h - g_l) * ((1 - exp(-1*aten*d/(corte*corte)))) + g_l;
        }
    }

    // cria a matriz com as componentes do filtro e junta ambas em uma matriz multicanal complexa
    Mat comps[]= {tmp, tmp};
    merge(comps, 2, filter);

    // cria a compoente real
    realInput = Mat_<float>(padded);

    // limpa o array de matrizes que vao compor a imagem complexa
    planos.clear();

    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // combina o array de matrizes em uma unica componente complexa
    merge(planos, complexImage);

    // aplicando logaritmo na imagem
    for(int i=0; i < dft_M; i++)
        for(int j=0; j < dft_N; j++)
            planos[0].at<char>(i,j) = log(planos[0].at<char>(i,j)+1);

    // calcula o dft
    dft(complexImage, complexImage);

    // realiza a troca de quadrantes
    deslocaDFT(complexImage);

    // aplica o filtro frequencial
    mulSpectrums(complexImage,filter,complexImage,0);

    // desloca novamente os quadrantes para posicao original
    deslocaDFT(complexImage);

    // aplica transformacao inversa
    idft(complexImage, complexImage);

    // normaliza os valores
    normalize(complexImage, complexImage, 0, 1, CV_MINMAX);

    // aplicando exponencial a imagem transformada
    for(int i=0; i < dft_M; i++)
        for(int j=0; j < dft_N; j++)
            complexImage.at<char>(i,j) = exp(complexImage.at<char>(i,j));

    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria para modifica-las
    split(complexImage, planos);

    // salvando imagem com filtro aplicado
    imwrite("homomorfico.png", planos[0]);

    // exibe resultado
    imshow("Homomorfico", planos[0]);

}

int main(int argc, char** argv)
{
    image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    namedWindow("Homomorfico", WINDOW_AUTOSIZE);

    //Criando primeira TrackBar
    sprintf(TrackbarName, "Gama H: ");
    createTrackbar(TrackbarName, "Homomorfico", &gamaH_slider, gamaH_slider_max, on_trackbar_filtro);

    sprintf(TrackbarName, "Gama L: ");
    createTrackbar(TrackbarName, "Homomorfico", &gamaL_slider, gamaL_slider_max, on_trackbar_filtro);

    //Criando segunda TrackBar
    sprintf(TrackbarName, "Corte: ");
    createTrackbar(TrackbarName, "Homomorfico", &corte_slider, corte_slider_max, on_trackbar_filtro);

    //Criando quarta TrackBar
    sprintf(TrackbarName, "Atenuacao: ");
    createTrackbar(TrackbarName, "Homomorfico", &aten_slider, aten_slider_max, on_trackbar_filtro);

    // identifica os tamanhos otimos para
    // calculo do FFT
    dft_M = getOptimalDFTSize(image.rows);
    dft_N = getOptimalDFTSize(image.cols);

    // realiza o padding da imagem logaritmica
    copyMakeBorder(image, padded, 0,
                   dft_M - image.rows, 0,
                   dft_N - image.cols,
                   BORDER_CONSTANT, Scalar::all(0));

    // parte imaginaria da matriz complexa (preenchida com zeros)
    zeros = Mat_<float>::zeros(padded.size());

    // prepara a matriz complexa para ser preenchida
    complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

    // a função de transferência (filtro frequencial) deve ter o mesmo tamanho e tipo da matriz complexa
    filter = complexImage.clone();

    // cria uma matriz temporária para criar as componentes real e imaginaria do filtro ideal
    tmp = Mat(dft_M, dft_N, CV_32F);

    waitKey(0);
    return 0;
}
