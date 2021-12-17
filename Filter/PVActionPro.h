#ifndef _PV_ACTION
#define _PV_ACTION

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

class PVActionPro
{
public:
    PVActionPro();
    ~PVActionPro();
    void Set(cv::Mat,cv::Mat);
    cv::Mat Get();

    //人脸识别
    void DoFaceCascade();
    //X镜像
    void DoFlipX();
    //Y镜像
    void DoFlipY();
    //旋转
    void DoRotate(int value);
    //灰度图
    void DoGary();
    //膨胀
    void DoDilate(int Value);
    //腐蚀
    void DoErode(int Value);
    //均值滤波
    void DoBlur(int Size);
    //边缘检测
    void DoCanny(int Value1,int Value2);
    //亮度对比度
    void DoGain(double value,double value1);
    //RGB修改
    void DoChangeRGB(int ColorR,int ColorG,int ColorB);
    //二值化
    void DoThreshold(int Value);
    //饱和度
    void DoSaturability(int Value);

private:
    cv::Mat inPutMat;
    cv::Mat inPutMat1;
    cv::Mat OutPutMat;

    const int max_increment = 200;
};

#endif //_PV_ACTION
