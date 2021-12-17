#include "OpenCV_PV_Thread.h"
#include "Filter/PVFilter.h"
#include <QDebug>
#include <windows.h>
#include <QElapsedTimer>

//视频
static cv::VideoCapture cap1;
//导入视频
static cv::VideoWriter capWv;

//摄像头
static cv::VideoCapture cap2;
//导出摄像头
static cv::VideoWriter capWCv;

//图片的一帧矩阵
static cv::Mat MatPaint;
//视频的一帧矩阵
static cv::Mat MatVideo;
//摄像头的一帧矩阵
static cv::Mat MatCamear;

static cv::Mat PrePaint;
static cv::Mat MidPaint;
static cv::Mat NextPaint;

OpenCV_PV_Thread::OpenCV_PV_Thread()
{

}

OpenCV_PV_Thread::~OpenCV_PV_Thread()
{
    isClose = true;
    this->quit();
    this->wait();
}

bool OpenCV_PV_Thread::OpenPaint(std::string str)
{
    mux.lock();
    MatPaint = cv::imread(str);
    mux.unlock();
    if(MatPaint.empty())
    {
        return false;
    }
    isPaint = true;
//    cv::imshow("",MatPaint);
    return true;
}

bool OpenCV_PV_Thread::OpenVoice(std::string str)
{
    mux.lock();
    bool isOpen = cap1.open(str);
    mux.unlock();
    if(!isOpen)
    {
        return false;
    }
    isVideo = true;
    Fps = cap1.get(cv::CAP_PROP_FPS);
    if(Fps <= 0)
    {
        Fps = 30;
    }
    isStopvideo = false;
    emit SwitchPlayIcon(isStopvideo);
    return true;
}

bool OpenCV_PV_Thread::Open()
{
    mux.lock();
    bool isOpen = cap2.open(0);
    mux.unlock();
    if(!isOpen)
    {
        return false;
    }
    isCamear = true;
    return true;
}

bool OpenCV_PV_Thread::Open(QList<QString> List, int index)
{
    mux.lock();
    if(List.size() >= index)
    {
        const std::string Paint = List.at(index).toLocal8Bit().data();
        PrePaint = cv::imread(Paint);
        MatPaint = cv::imread(Paint);

        if(index == List.size() - 1)
            index = 0;
        else
            index += 1;
        const std::string MidPaintimg = List.at(index).toLocal8Bit().data();
        MidPaint = cv::imread(MidPaintimg);

        if(index == List.size() - 1)
            index = 0;
        else
            index += 1;
        const std::string NextPaintimg = List.at(index).toLocal8Bit().data();
        NextPaint = cv::imread(NextPaintimg);
    }
    if(MatPaint.empty())
    {
        mux.unlock();
        return false;
    }
    isPaint = true;
    mux.unlock();
    return true;
}

bool OpenCV_PV_Thread::SavePaint(std::string str)
{
    if(str.empty())
        return false;
    if(MatPaint.empty())
        return false;
    mux.lock();
    IsPaintSavePaint = true;
    PaintStringPath = str;
    mux.unlock();
    return true;
}

bool OpenCV_PV_Thread::SaveVideoPaint(std::string str)
{
    if(str.empty())
        return false;
    if(MatVideo.empty())
        return false;
    mux.lock();
    isVideoSavePaint = true;
    VideoSavePaintpath = str;
    mux.unlock();
    return true;
}

bool OpenCV_PV_Thread::SaveCamearPaint(std::string str)
{
    if(str.empty())
        return false;
    if(MatCamear.empty())
        return false;
    mux.lock();
    IsCamearSavePaint = true;
    CamearStringPath = str;
    mux.unlock();
    return true;
}

void OpenCV_PV_Thread::run()
{
    while(1)
    {
        mux.lock();
        if(isClose)
        {
            mux.unlock();
            break;
        }
        if(!isPaint && !isVideo && !isCamear)
        {
            mux.unlock();
            continue;
        }
        if(isPaint && !MatPaint.empty())
        {
            cv::Mat Paint = PV_Filter::Get()->PVFilter(PaintModel,MatPaint,MatPaint);
            emit ShowPaint(Paint);
            if(!PrePaint.empty())
            {
                emit ShowPer(PrePaint);
            }
            if(!MidPaint.empty())
            {
                emit ShowMid(MidPaint);
            }
            if(!NextPaint.empty())
            {
                emit ShowNext(NextPaint);
            }
            if(IsPaintSavePaint)
            {
                bool isWrite = cv::imwrite(PaintStringPath,Paint);
                if(!isWrite)
                    emit SaveError();
                IsPaintSavePaint = false;
                PaintStringPath = "";
            }
        }
        if(isVideo && cap1.isOpened())
        {
            if(!isStopvideo)
            {
                bool isRead = cap1.read(MatVideo);
                if(!isRead)
                {
                    if(isVideoisWrite)
                    {
                        mux.unlock();
                        StopSaveVideo();
                        continue;
                    }
                    mux.unlock();
                    continue;
                }
            }
            cv::Mat Paint = PV_Filter::Get()->PVFilter(VodeoModel,MatVideo,MatVideo);
            emit ShowVoice(Paint);
            if(isVideoSavePaint)
            {
                cv::imwrite(VideoSavePaintpath,Paint);
                isVideoSavePaint = false;
                VideoSavePaintpath = "";
            }
            if(isVideoisWrite)
            {
                if(WriteStartPos != -1)
                {
                    WriteStartPos = -1;
                    mux.unlock();
                    if(WriteStartPos == 0)
                        Seek(0.0);
                    else
                        Seek(double(WriteStartPos / 1000.0));
                    continue;
                }
                capWv.write(Paint);
                if(WriteStopPos != -1)
                {
                    int NowFrame = cap1.get(cv::CAP_PROP_POS_FRAMES);
                    double count = cap1.get(cv::CAP_PROP_FRAME_COUNT);
                    int frame = count * (WriteStopPos / 1000.0);
                    if(NowFrame > frame)
                    {
                        mux.unlock();
                        StopSaveVideo();
                        continue;
                    }
                }
            }
        }
        if(isCamear && cap2.isOpened())
        {
            cap2.read(MatCamear);
            cv::Mat Paint = PV_Filter::Get()->PVFilter(CameraModel,MatCamear,MatCamear);
            emit ShowCamera(Paint);

            if(IsCamearSavePaint)
            {
                cv::imwrite(CamearStringPath,Paint);
                IsCamearSavePaint = false;
                CamearStringPath = "";
            }
            if(isCamearWrite)
            {
                capWCv.write(Paint);
            }

        }

        if(isVideoisWrite || isCamearWrite)
        {
//            msleep(1);
        }
        else
        {
            if(Fps == 0)
                Sleep(30);
            Sleep(Fps);
        }

        mux.unlock();
    }
}

bool OpenCV_PV_Thread::SaveVideo(std::string str,int StartPos,int EndPos,bool isRotate, bool isGary)
{
    mux.lock();
    int Width = cap1.get(cv::CAP_PROP_FRAME_WIDTH);
    int Height = cap1.get(cv::CAP_PROP_FRAME_HEIGHT);
    int FPS = cap1.get(cv::CAP_PROP_FPS);
    if(!cap1.isOpened())
    {
        mux.unlock();
        return false;
    }
    if(Width < 0 || Height < 0)
    {
        mux.unlock();
        return false;
    }
    if(StartPos != -1 && EndPos != -1)
    {
        WriteStartPos = StartPos;
        WriteStopPos = EndPos;
    }
    if(EndPos != -1)
    {
        WriteStopPos = EndPos;
    }
    if(isRotate)
    {
        int x = Width;
        Width = Height;
        Height = x;
    }
    if(!capWv.open(str,cv::VideoWriter::fourcc('X','2','6','4'),FPS,cv::Size(Width,Height),!isGary))
    {
        mux.unlock();
        return false;
    }
    if(!capWv.isOpened())
    {
        qDebug() << "OpenError";
        mux.unlock();
        return false;
    }
    isVideoisWrite = true;
    mux.unlock();
    return true;
}

void OpenCV_PV_Thread::StopSaveVideo()
{
    mux.lock();
    //因为Write 跟 Capture不一样
    //Write要写入文件末尾信息
    //如果不释放会导致文件无法查看
    //所以需要手动调用release
    capWv.release();
    WriteStartPos = -1;
    WriteStopPos = -1;
    isVideoisWrite = false;
    emit SaveVideoEnd();
    mux.unlock();
}

bool OpenCV_PV_Thread::SaveCamearVideo(std::string str, bool isRotate, bool isGary)
{
    mux.lock();
    int Width = cap2.get(cv::CAP_PROP_FRAME_WIDTH);
    int Height = cap2.get(cv::CAP_PROP_FRAME_HEIGHT);
    int FPS = cap2.get(cv::CAP_PROP_FPS);
    if(!cap2.isOpened())
    {
        qDebug() <<"error";
        mux.unlock();
        return false;
    }
    if(Width < 0 || Height < 0)
    {
        qDebug() <<"error2";
        mux.unlock();
        return false;
    }
    if(isRotate)
    {
        int x = Width;
        Width = Height;
        Height = x;
    }
    if(!capWCv.open(str,cv::VideoWriter::fourcc('X','2','6','4'),30,cv::Size(Width,Height),!isGary))
    {
//        qDebug () << str << FPS <<
        qDebug() <<"error3";
        mux.unlock();
        return false;
    }
    if(!capWCv.isOpened())
    {
        qDebug() << "OpenError";
        mux.unlock();
        return false;
    }
    isCamearWrite = true;
    mux.unlock();
    return true;
}

void OpenCV_PV_Thread::StopCamearVideo()
{
    mux.lock();
    //因为Write 跟 Capture不一样
    //Write要写入文件末尾信息
    //如果不释放会导致文件无法查看
    //所以需要手动调用release
    capWCv.release();
    isCamearWrite = false;
    emit SaveCamearEnd();
    mux.unlock();
}

bool OpenCV_PV_Thread::GetisCamearisWrite() const
{
    return isCamearWrite;
}

bool OpenCV_PV_Thread::GetisVideoisWrite() const
{
    return isVideoisWrite;
}

void OpenCV_PV_Thread::StartStopVideo()
{
    isStopvideo = !isStopvideo;
    emit SwitchPlayIcon(isStopvideo);
}

//void OpenCV_PV_Thread::StopVideo()
//{
//    isStopvideo = true;
//}

double OpenCV_PV_Thread::Get_Pos()
{
    double pos = 0.0;
    mux.lock();
    if(!cap1.isOpened())
        return pos;
    double count = cap1.get(cv::CAP_PROP_FRAME_COUNT);
    double npos = cap1.get(cv::CAP_PROP_POS_FRAMES);
    if(count > 0.001)
        pos = npos / count;
    mux.unlock();
    return pos;
}

bool OpenCV_PV_Thread::Seek(double pos)
{
    mux.lock();
    double count = cap1.get(cv::CAP_PROP_FRAME_COUNT);
    int frame = count * pos;
    if(!cap1.isOpened())
    {
        mux.unlock();
        return false;
    }
    bool re = cap1.set(cv::CAP_PROP_POS_FRAMES,frame);
    mux.unlock();
    return re;
}

int OpenCV_PV_Thread::Get_FPS() const
{
    return Fps;
}
