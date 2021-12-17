#ifndef OPENCV_PV_WIDGET_H
#define OPENCV_PV_WIDGET_H

#include "OpenCVBaseCon.h"
#include "Filter/PVFilter.h"

enum OpencvModel
{
    PaintModelWid,
    VoiceModelWid,
    CameraModelWid,
};

class OpenCV_PV_Thread;
class OpenCV_PV_Widget : public OpenCVBaseCon
{
Q_OBJECT

public:
    OpenCV_PV_Widget(QWidget *parent = 0);
    ~OpenCV_PV_Widget();

    void Init();

    QWidget* InitLeftWidget();
    QWidget* InitMidWidget();
    QWidget* InitRightWidget();
    QWidget* SwitchWid(OpencvModel);

    QWidget* InitBtnGround(QString _Text,QPushButton*);
    QWidget* InitLayoutLabel(QString _Text,QSlider* mSlider,QLabel* lbl_Text);

    void ShowHideWidget(QWidget*);
    TaskModel Get_TaskModel();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void timerEvent(QTimerEvent *e);

private slots:
    void DoDirPaint();

signals:
    void MinClicked();
    void DirPaint();
    void SwitchSWidget(int index);
private:
    OpenCV_PV_Thread* Thread;
    QList<QString> PaintList;
    int PaintIndex = 0;

    bool isPlayerClicked = false;
};

#endif // OPENCV_PV_WIDGET_H
