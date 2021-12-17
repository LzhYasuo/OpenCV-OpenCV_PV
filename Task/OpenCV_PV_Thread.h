#ifndef _OPENCV_PV_THREAD_H_
#define _OPENCV_PV_THREAD_H_

#include <QThread>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <QMutex>
#include <string.h>

enum SavePaintModel
{
    pSavePaint,
    vSavePaint,
    cSavePaint,
};

class OpenCV_PV_Thread : public QThread
{
    Q_OBJECT
public:
    OpenCV_PV_Thread();
    ~OpenCV_PV_Thread();
    bool OpenPaint(std::string);
    bool OpenVoice(std::string);
    bool Open();
    bool Open(QList<QString>,int index);

    bool SavePaint(std::string);
    bool SaveVideoPaint(std::string);
    bool SaveCamearPaint(std::string);

    void run();

    bool SaveVideo(std::string,int StartPos,int EndPos,bool isRotate = false,bool isGary = false);
    void StopSaveVideo();

    bool SaveCamearVideo(std::string,bool isRotate = false,bool isGary = false);
    void StopCamearVideo();

    bool GetisCamearisWrite() const;
    bool GetisVideoisWrite() const;

    void StartStopVideo();
    void StopVideo();
    double Get_Pos();
    bool Seek(double);
    int Get_FPS() const;

signals:
    void ShowPaint(cv::Mat);
    void ShowVoice(cv::Mat);
    void ShowCamera(cv::Mat);
    void ShowPer(cv::Mat);
    void ShowMid(cv::Mat);
    void ShowNext(cv::Mat);
    void SaveError();

    void SaveVideoEnd();
    void SaveCamearEnd();
    void SwitchPlayIcon(bool);

private:
    //图片是否被打开
    bool isPaint = false;
    //视频是否被打开
    bool isVideo = false;
    //摄像头是否被打开
    bool isCamear = false;
    //退出程序的索引
    bool isClose = false;
    //是否暂停视频
    bool isStopvideo = true;
    //是否导出视频
    bool isVideoisWrite = false;
    //是否导出摄像头
    bool isCamearWrite = false;
    QMutex mux;
    int Fps = 0;
    int WriteStartPos = -1;
    int WriteStopPos = -1;

    bool isVideoSavePaint = false;
    std::string VideoSavePaintpath = "";
    bool IsPaintSavePaint = false;
    std::string PaintStringPath = "";
    bool IsCamearSavePaint = false;
    std::string CamearStringPath = "";
};

#endif //_OPENCV_PV_THREAD_H_
