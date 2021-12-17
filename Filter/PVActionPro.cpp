#include "PVActionPro.h"
#include <vector>
#include <opencv2/objdetect.hpp>
#include <QDebug>

static cv::CascadeClassifier FaceCascade;
static std::vector<cv::Rect> FaceVector;

PVActionPro::PVActionPro()
{
    bool isLoad = FaceCascade.load("F://QTpro//QOpenCV//OpenCV_PV//qrc//haarcascade_frontalface_default.xml");
    if(isLoad)
    {
        qDebug() << "Load xml Error";
        return;
    }
}

PVActionPro::~PVActionPro()
{

}

void PVActionPro::Set(cv::Mat mat1, cv::Mat mat2)
{
    if(mat1.empty())
    {
        return;
    }
    inPutMat = mat1;
    inPutMat1 = mat2;
    inPutMat.copyTo(OutPutMat);
}

cv::Mat PVActionPro::Get()
{
    return OutPutMat;
}

void PVActionPro::DoFaceCascade()
{
//    cv::CascadeClassifier FaceCascade;
//    std::vector<cv::Rect> FaceVector;
//    bool isLoad = FaceCascade.load("://qrc/haarcascade_frontalface_default.xml");
//    if(isLoad)
//    {
//        qDebug() << "Load xml Error";
//        return;
//    }
    FaceCascade.detectMultiScale(OutPutMat,FaceVector,1.1,3.0,0|CV_HAAR_DO_ROUGH_SEARCH);
    for(int i = 0; i < FaceVector.size(); i++)
    {
        cv::rectangle(OutPutMat,FaceVector.at(i),cv::Scalar(0,255,0));
    }
}

void PVActionPro::DoFlipX()
{
    if(OutPutMat.empty())
        return;
    cv::flip(OutPutMat,OutPutMat,1);
}

void PVActionPro::DoFlipY()
{
    if(OutPutMat.empty())
        return;
    cv::flip(OutPutMat,OutPutMat,0);
}

void PVActionPro::DoRotate(int value)
{
    if(OutPutMat.empty())
        return;
    if(value == 90)
        cv::rotate(OutPutMat,OutPutMat,cv::ROTATE_90_CLOCKWISE);
    else if(value == 180)
        cv::rotate(OutPutMat,OutPutMat,cv::ROTATE_180);
    else if(value == 270)
        cv::rotate(OutPutMat,OutPutMat,cv::ROTATE_90_COUNTERCLOCKWISE);
}

void PVActionPro::DoGary()
{
    if(OutPutMat.empty())
        return;
    if(OutPutMat.channels() == 1)
        return;
    cv::cvtColor(OutPutMat,OutPutMat,cv::COLOR_RGB2GRAY);
}

void PVActionPro::DoDilate(int Value)
{
    if(OutPutMat.empty())
        return;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(Value,Value));
    cv::dilate(OutPutMat,OutPutMat,kernel);
}

void PVActionPro::DoErode(int Value)
{
    if(OutPutMat.empty())
        return;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(Value,Value));
    cv::erode(OutPutMat,OutPutMat,kernel);
}

void PVActionPro::DoBlur(int Size)
{
    if(OutPutMat.empty())
        return;
    if(Size > 0)
        cv::blur(OutPutMat,OutPutMat,cv::Size(Size,Size));
}

void PVActionPro::DoCanny(int Value1, int Value2)
{
    if(OutPutMat.empty())
        return;
    cv::Mat mat;
    OutPutMat.copyTo(mat);
    cv::Canny(mat,OutPutMat,Value1,Value2);
}

void PVActionPro::DoGain(double value, double value1)
{
    if(OutPutMat.empty())
        return;
    OutPutMat.convertTo(OutPutMat,-1,value,value1);
}

void PVActionPro::DoChangeRGB(int ColorR, int ColorG, int ColorB)
{
    if(OutPutMat.empty())
        return;
    if(OutPutMat.type() == CV_8UC1)
    {
        return;
    }
    else if(OutPutMat.type() == CV_8UC3)
    {
        for(int i = 0; i < OutPutMat.rows; i++)
        {
            for(int j = 0; j < OutPutMat.cols; j++)
            {
                int B = OutPutMat.at<uchar>(i, j * 3+0) + ColorB;   // B通道
                int G = OutPutMat.at<uchar>(i, j * 3+1) + ColorG;  // G通道
                int R = OutPutMat.at<uchar>(i, j * 3+2) + ColorR;  // R通道
                if(R > 255)
                {
                    R = 255;
                }
                if(G  > 255)
                {
                    G = 255;
                }
                if(B > 255)
                {
                    B = 255;
                }
                OutPutMat.at<uchar>(i, j * 3+0) = B;
                OutPutMat.at<uchar>(i, j * 3+1) = G;
                OutPutMat.at<uchar>(i, j * 3+2) = R;
            }
        }
    }
}

void PVActionPro::DoThreshold(int Value)
{
    if(OutPutMat.empty())
        return;
    cv::threshold(OutPutMat,OutPutMat,Value,255,CV_THRESH_BINARY);
}

void PVActionPro::DoSaturability(int Value)
{
    float increment = (Value - 80) * 1.0 / max_increment;
    for (int col = 0; col < OutPutMat.cols; col++)
    {
        for (int row = 0; row < OutPutMat.rows; row++)
        {
            // R,G,B 分别对应数组中下标的 2,1,0
            uchar r = OutPutMat.at<cv::Vec3b> (row, col)[2];
            uchar g = OutPutMat.at<cv::Vec3b> (row, col)[1];
            uchar b = OutPutMat.at<cv::Vec3b> (row, col)[0];

            float maxn = std::max (r, std::max (g, b));
            float minn = std::min (r, std::min (g, b));

            float delta, value;
            delta = (maxn - minn) / 255;
            value = (maxn + minn) / 255;

            float new_r, new_g, new_b;

            if (delta == 0)		 // 差为 0 不做操作，保存原像素点
            {
                OutPutMat.at<cv::Vec3b> (row, col)[0] = new_b;
                OutPutMat.at<cv::Vec3b> (row, col)[1] = new_g;
                OutPutMat.at<cv::Vec3b> (row, col)[2] = new_r;
                continue;
            }

            float light, sat, alpha;
            light = value / 2;

            if (light < 0.5)
                sat = delta / value;
            else
                sat = delta / (2 - value);

            if (increment >= 0)
            {
                if ((increment + sat) >= 1)
                    alpha = sat;
                else
                {
                    alpha = 1 - increment;
                }
                alpha = 1 / alpha - 1;
                new_r = r + (r - light * 255) * alpha;
                new_g = g + (g - light * 255) * alpha;
                new_b = b + (b - light * 255) * alpha;
            }
            else
            {
                alpha = increment;
                new_r = light * 255 + (r - light * 255) * (1 + alpha);
                new_g = light * 255 + (g - light * 255) * (1 + alpha);
                new_b = light * 255 + (b - light * 255) * (1 + alpha);
            }
            OutPutMat.at<cv::Vec3b> (row, col)[0] = new_b;
            OutPutMat.at<cv::Vec3b> (row, col)[1] = new_g;
            OutPutMat.at<cv::Vec3b> (row, col)[2] = new_r;
        }
    }
}

