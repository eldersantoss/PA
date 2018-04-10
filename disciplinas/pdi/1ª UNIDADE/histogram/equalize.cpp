#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstring>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    Mat src, dst, histGraySrc, histGrayDst;
    int width, height;
    VideoCapture cap;
    int nbins = 64;
    float range[] = {0, 256};
    const float *histrange = { range };
    bool uniform = true;
    bool acummulate = false;
    string img_src = "Imagem original", img_dst = "Imagem equalizada";

    cap.open(0);

    if(!cap.isOpened())
    {
        cout << "cameras indisponiveis";
        return -1;
    }

    width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    cout << "largura = " << width << endl;
    cout << "altura  = " << height << endl;

    int histw = nbins, histh = nbins/2;
    Mat histImgSrc(histh, histw, CV_8UC1, Scalar(0));
    Mat histImgDst(histh, histw, CV_8UC1, Scalar(0));

    while(1)
    {
        cap >> src;
        cvtColor(src, src, CV_BGR2GRAY);
        equalizeHist(src, dst);

        calcHist(&src, 1, 0, Mat(), histGraySrc, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
        calcHist(&dst, 1, 0, Mat(), histGrayDst, 1,
                 &nbins, &histrange,
                 uniform, acummulate);

        normalize(histGraySrc, histGraySrc, 0, histImgSrc.rows, NORM_MINMAX, -1, Mat());
        normalize(histGrayDst, histGrayDst, 0, histImgDst.rows, NORM_MINMAX, -1, Mat());

        histImgSrc.setTo(Scalar(0));
        histImgDst.setTo(Scalar(0));

        for(int i=0; i<nbins; i++)
        {
            line(histImgSrc,
                 Point(i, histh),
                 Point(i, histh-cvRound(histGraySrc.at<float>(i))),
                 Scalar(255), 1, 8, 0);
            line(histImgDst,
                 Point(i, histh),
                 Point(i, histh-cvRound(histGrayDst.at<float>(i))),
                 Scalar(255), 1, 8, 0);
        }
        histImgSrc.copyTo(src(Rect(0, 0,nbins, histh)));
        histImgDst.copyTo(dst(Rect(0, 0,nbins, histh)));

        namedWindow(img_src);
        namedWindow(img_dst);
        imshow(img_src, src);
        imshow(img_dst, dst);

        if(waitKey(30) >= 0) break;
    }
    return 0;
}
