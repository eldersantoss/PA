#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;
  Vec3b comp;
  CvPoint p1, p2;

  p1.x=-1; p2.x=-1; p1.y=-1; p2.y=-1;

  //Carregando imagem na matriz image:
  image = imread("biel.png",CV_LOAD_IMAGE_COLOR);

  //Verificando se a imagem foi carregada:
  if(!image.data)
    cerr << "Erro! Imagem nao pode ser carregada, verifique tudo e tente novamente." << endl;

  //Criando janela para exibiçao da imagem:
  namedWindow("janela",WINDOW_AUTOSIZE);

  //Verificando se os pontos p1 e p2 estão dentro da imagem:
  while(p1.x > image.rows || p1.x < 0 || p1.y > image.cols || p1.y < 0)
  {
    cout << "Digite as coordenadas do primeiro pronto (x depois y):" << endl;
    cin >> p1.x >> p1.y;
  }
  while(p2.x > image.rows || p2.x < 0 || p2.y > image.cols || p2.y < 0)
  {
    cout << "Digite as coordenadas do segundo pronto (x depois y):" << endl;
    cin >> p2.x >> p2.y;
  }

  //Ordenando pontos por proximidade da orgiem (0,0):
  if(p2.x < p1.x)
  {
    int auxx = p1.x;
    p1.x = p2.x;
    p2.x = auxx;
  }
  if(p2.y < p1.y)
  {
    int auxy = p1.y;
    p1.y = p2.y;
    p2.y = auxy;
  }

  //Atribuindo os valores aos canais do Vec3b auxiliar:
  comp[0] = 255; //B
  comp[1] = 255; //G
  comp[2] = 255; //R

  //Invertendo as cores da imagem dentro do retângulo:
  for(int i=p1.x;i<p2.x;i++){
    for(int j=p1.y;j<p2.y;j++){
      image.at<Vec3b>(i,j) = comp-image.at<Vec3b>(i,j);
    }
  }

  //Exibindo imagem modificada na tela:
  imshow("janela", image);
  waitKey();
  return 0;
}
