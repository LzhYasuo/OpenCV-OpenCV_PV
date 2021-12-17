#include "OpenCVBaseCon.h"
#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QComboBox>

QHBoxLayout *OpenCVBaseCon::Get_HLayout()
{
    QHBoxLayout* mLayout = new QHBoxLayout();
    mLayout->setSpacing(0);
    mLayout->setContentsMargins(0,0,0,0);

    return mLayout;
}

QHBoxLayout *OpenCVBaseCon::Get_HLayout(int mSpaceing, int Left, int Right, int Top, int Bottom)
{
    QHBoxLayout* mLayout = new QHBoxLayout();
    mLayout->setSpacing(mSpaceing);
    mLayout->setContentsMargins(Left,Top,Right,Bottom);

    return mLayout;
}

QVBoxLayout *OpenCVBaseCon::Get_VLayout()
{
    QVBoxLayout* mLayout = new QVBoxLayout();
    mLayout->setSpacing(0);
    mLayout->setContentsMargins(0,0,0,0);

    return mLayout;
}

QVBoxLayout *OpenCVBaseCon::Get_VLayout(int mSpaceing, int Left, int Right, int Top, int Bottom)
{
    QVBoxLayout* mLayout = new QVBoxLayout();
    mLayout->setSpacing(mSpaceing);
    mLayout->setContentsMargins(Left,Top,Right,Bottom);

    return mLayout;
}

QPushButton *OpenCVBaseCon::Get_Button(QString _Text, QString _Warnning, QString _Style)
{
    QPushButton* btn = new QPushButton();
    if(!_Text.isEmpty())
        btn->setText(_Text);
    if(!_Warnning.isEmpty())
        btn->setToolTip(_Warnning);
    btn->setStyleSheet(_Style);

    return btn;
}

QLabel *OpenCVBaseCon::Get_Label(QString _Text, QString _Style)
{
     QLabel* lbl = new QLabel();
     if(!_Text.isEmpty())
         lbl->setText(_Text);
     lbl->setStyleSheet(_Style);
     return lbl;
}

QSlider *OpenCVBaseCon::Get_Slider(int _min, int _Max, int _Value,bool isHor,QString Style)
{
    QSlider* mSlider;
    if(isHor)
        mSlider = new QSlider(Qt::Horizontal);
    else
        mSlider = new QSlider(Qt::Vertical);
    mSlider->setMinimum(_min);
    mSlider->setMaximum(_Max);
    mSlider->setValue(_Value);
    mSlider->setStyleSheet(Style);

    return mSlider;
}

QComboBox *OpenCVBaseCon::Get_ComboBox(QStringList str)
{
    QComboBox* box = new QComboBox();
    box->addItems(str);

}

OpenCVBaseCon::OpenCVBaseCon(QWidget *parent)
{

}

OpenCVBaseCon::~OpenCVBaseCon()
{

}
