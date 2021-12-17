#ifndef _OPENCV_BASE_CON_H_
#define _OPENCV_BASE_CON_H_

#include <QWidget>

class QPushButton;
class QLabel;
class QSlider;
class QHBoxLayout;
class QVBoxLayout;
class QComboBox;
class OpenCVBaseCon : public QWidget
{
public:
    OpenCVBaseCon(QWidget* parent = nullptr);
    ~OpenCVBaseCon();

protected:
    QHBoxLayout* Get_HLayout();
    QHBoxLayout* Get_HLayout(int mSpaceing,int Left,int Right,int Top,int Bottom);
    QVBoxLayout* Get_VLayout();
    QVBoxLayout* Get_VLayout(int mSpaceing,int Left,int Right,int Top,int Bottom);

    QPushButton* Get_Button(QString _Text,QString _Warnning,QString Style);
    QLabel* Get_Label(QString _Text,QString Style);
    QSlider* Get_Slider(int _min,int _Max,int _Value,bool isHor,QString Style);
    QComboBox* Get_ComboBox(QStringList);
};

#endif //_OPENCV_BASE_CON_H_
