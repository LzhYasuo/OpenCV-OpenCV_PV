#include "PVFilter.h"
#include <QMutex>
#include "PVActionPro.h"

#include <QDebug>

class PV_XFilter : public PV_Filter
{
public:
    PV_XFilter();
    virtual ~PV_XFilter();

    virtual void Add(TaskModel,PVTask);
    virtual void Clear();
    virtual void Clear(TaskModel);
    virtual cv::Mat PVFilter(TaskModel,cv::Mat,cv::Mat);

    //是否为灰度图
    virtual bool IsGaryChannels(TaskModel);
    virtual bool IsRotateX(TaskModel);
private:
    QList<PVTask>& Get_List(TaskModel);

private:
    PVActionPro pro;
    //图片处理
    QList<PVTask> _PaintList;
    //视频处理
    QList<PVTask> _VoiceList;
    //摄像头处理
    QList<PVTask> _CameraList;
    //导出的时候需要判断是否是灰度图
    bool isGary = false;
    //导出的时候需要长宽需要判断是否是旋转
    bool isCGary = false;
    QMutex mux;
};

PV_XFilter::PV_XFilter()
{

}

PV_XFilter::~PV_XFilter()
{

}

void PV_XFilter::Add(TaskModel mol,PVTask tak)
{
    QList<PVTask>& _List = Get_List(mol);

    qDebug() << _List.size();
    for(int i = 0; i < _List.size(); i++)
    {
        if(_List.at(i)._Act == tak._Act)
        {
            if(tak._Act == Gain)
            {
                _List[i]._List[0] = tak._List.at(0);
                _List[i]._List[1] = tak._List.at(1);
                return;
            }
            else if(tak._Act == ChangeRGB)
            {
                if(tak._List.at(0) == -1 && tak._List.at(1) == -1 && tak._List.at(2) == -1)
                {
                    _List.removeAt(i);
                    return;
                }
                _List[i]._List[0] = tak._List.at(0);
                _List[i]._List[1] = tak._List.at(1);
                _List[i]._List[2] = tak._List.at(2);
                return;
            }
            else if(tak._Act == Blur)
            {
                if(tak._List.at(0) == -1)
                {
                    _List.removeAt(i);
                    return;
                }
                _List[i]._List[0] = tak._List.at(0);
                return;
            }
            else if(tak._Act == Threshold)
            {
                if(tak._List.at(0) == -1)
                {
                    _List.removeAt(i);
                    return;
                }
                _List[i]._List[0] = tak._List.at(0);
                return;
            }
            else if(tak._Act == Saturability)
            {
                if(tak._List.at(0) == -1)
                {
                    _List.removeAt(i);
                    return;
                }
                _List[i]._List[0] = tak._List.at(0);
                return;
            }
            _List.removeAt(i);
            return;
        }
    }
    _List.push_back(tak);
}

void PV_XFilter::Clear()
{
//    mux.lock();
    _PaintList.clear();
    //    mux.unlock();
}

void PV_XFilter::Clear(TaskModel _Mol)
{
    QList<PVTask>& _List = Get_List(_Mol);
    _List.clear();
}

cv::Mat PV_XFilter::PVFilter(TaskModel mol,cv::Mat mat1, cv::Mat mat2)
{
    QList<PVTask>& _List = Get_List(mol);
    pro.Set(mat1,mat2);
    for(int i = 0; i < _List.size(); i++)
    {
        PVTask tak = _List.at(i);
        switch (tak._Act)
        {
        case Gary:
            pro.DoGary();
            break;
        case FilpX:
            pro.DoFlipX();
            break;
        case FilpY:
            pro.DoFlipY();
            break;
        case Rotate:
            pro.DoRotate(tak._List.at(0).toInt());
            break;
        case Dilate:
            pro.DoDilate(tak._List.at(0).toInt());
            break;
        case Eroder:
            pro.DoErode(tak._List.at(0).toInt());
            break;
        case Gain:
            pro.DoGain(tak._List.at(0).toInt(),tak._List.at(1).toInt());
            break;
        case ChangeRGB:
            pro.DoChangeRGB(tak._List.at(0).toInt(),tak._List.at(1).toInt(),tak._List.at(2).toInt());
            break;
        case Blur:
            pro.DoBlur(tak._List.at(0).toInt());
            break;
        case Canny:
            pro.DoCanny(tak._List.at(0).toInt(),tak._List.at(1).toInt());
            break;
        case Threshold:
            pro.DoThreshold(tak._List.at(0).toInt());
            break;
        case Saturability:
            pro.DoSaturability(tak._List.at(0).toInt());
            break;
        case FaceCascade:
            pro.DoFaceCascade();
            break;
        }
        if(mol == VodeoModel)
        {
            if(pro.Get().channels() == 1)
            {
                isGary = true;
            }
            else
                isGary = false;
        }
        else if(mol == CameraModel)
        {
            if(pro.Get().channels() == 1)
            {
                isCGary = true;
            }
            else
                isCGary = false;
        }
    }

    return pro.Get();
}


bool PV_XFilter::IsGaryChannels(TaskModel Mol)
{
    if(Mol == VodeoModel)
        return isGary;
    else
        return isCGary;
}

bool PV_XFilter::IsRotateX(TaskModel mol)
{
    QList<PVTask>& _List = Get_List(mol);
    bool isRotateX = false;
    for(int i = 0; i < _List.size(); i++)
    {
        if(_VoiceList.at(i)._Act == Rotate)
        {
            if(_VoiceList.at(i)._List.at(0).toInt() == 90 || _VoiceList.at(i)._List.at(0).toInt() == 270)
            {
                isRotateX = true;
            }
        }
    }
    return isRotateX;
}

QList<PVTask>& PV_XFilter::Get_List(TaskModel mol)
{
    if(mol == PaintModel)
    {
         return _PaintList;
    }
    else if(mol == VodeoModel)
    {
        return _VoiceList;
    }
    else if(mol == CameraModel)
    {
        return _CameraList;
    }
}

PV_Filter *PV_Filter::Get()
{
    static PV_XFilter cx;
    return &cx;
}


PV_Filter::PV_Filter()
{

}

PV_Filter::~PV_Filter()
{

}
