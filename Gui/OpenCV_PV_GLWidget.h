﻿#ifndef _OPENCV_PV_GLWIDGET_H_
#define _OPENCV_PV_GLWIDGET_H_

#include <QOpenGLWidget>
#include <opencv2/imgproc.hpp>

class OpenCV_PV_GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    OpenCV_PV_GLWidget(QWidget* parent = nullptr);
    ~OpenCV_PV_GLWidget();

public slots:
    //把图片传入 通过paintEvent显示
    void ShowImg(cv::Mat);
    QImage MatToQImage(const cv::Mat& mat);

protected:
    void paintEvent(QPaintEvent *e);
//    void resizeEvent(QResizeEvent *e);
//    void showEvent(QShowEvent *event);
private:
    QImage img;
    int SubHeight = 0;
    int SubWidth = 0;
    int Height = 0;
    int Width = 0;
};

#endif //_OPENCV_PV_GLWIDGET_H_
