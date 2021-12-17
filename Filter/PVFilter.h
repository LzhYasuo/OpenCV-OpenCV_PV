#ifndef _PV_FILTER_H_
#define _PV_FILTER_H_

#include <QList>
#include <QVariant>
#include <opencv2/imgproc.hpp>

enum TaskModel
{
    PaintModel,
    VodeoModel,
    CameraModel,
};

enum PV_Action
{
    Gary,
    FilpX,
    FilpY,
    Rotate,
    Dilate,
    Eroder,
    Gain,
    ChangeRGB,
    Blur,
    Canny,
    Threshold,
    Saturability,
    FaceCascade,
};

struct PVTask
{
    enum PV_Action _Act;
    QList<QVariant> _List;
};

class PV_Filter
{
public:
    static PV_Filter* Get();
    virtual void Add(TaskModel,PVTask) = 0;
    virtual void Clear() = 0;
    virtual void Clear(TaskModel) = 0;
    virtual cv::Mat PVFilter(TaskModel,cv::Mat,cv::Mat) = 0;

    virtual bool IsGaryChannels(TaskModel) = 0;
    virtual bool IsRotateX(TaskModel) = 0;
protected:
    PV_Filter();
    ~PV_Filter();

};

#endif //_PV_FILTER_H_
