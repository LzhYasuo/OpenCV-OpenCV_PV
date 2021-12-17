#include "PubQss.h"

#define GET_STR(x) #x

PubQss *PubQss::Get()
{
    static PubQss cx;
    return &cx;
}

QString PubQss::Get_Style(QssTask Task)
{
    QString msg = "";
    switch(Task.QssStyle)
    {
    case BOTTOMSTYLE:
    {
        msg = GET_STR
            (
                QPushButton
                {
                    border-radius:10px;
                    background-color: rgb(255, 255, 255);
                    border:2px solid rgb(0,0,0);
                    outline: none;
                }
                QPushButton:pressed
                {
                    background-color: rgb(204, 204, 215);
                }
                QPushButton:disabled
                {
                    background-color: rgb(167, 164, 170);
                }
                                );
        break;
    }
    case BOTTOMROUNDSTYLE:
    {
        QString Paint = Task.Value.at(0).toString();
        msg = QString(GET_STR
                      (
                          QPushButton
                          {
                              background-color: rgb(255, 255, 255);
        border:2px solid rgb(0,0,0);
        outline: none;
            border-radius:32px;
            background-repeat:no-repeat;
            background-position:center;
            background-image: url(%1);
                          }
                                      )).arg(Paint);
        break;
    }
    case SLIDERSTYLE:
    {
        msg = QString(GET_STR
                      (
                          QSlider::groove:horizontal
                              {
                                  border-radius:2px;height:4px;
                              }
                              QSlider::sub-page:horizontal
                              {
                                  background:rgb(%1);
                              }
                              QSlider::add-page:horizontal
                          {
                              background:rgb(200,200,209);
                          }
                          QSlider::handle:horizontal
                          {
                              background:rgb(0, 0, 0);
                              width:8px;
                                  border-radius:4px;
                              margin:-3px 0px -3px 0px
                          }
                                      )).arg(Task.Value.at(0).toString());
        break;
    }
    case SWITCHBUTTONSTYLE:
    {
        msg = GET_STR
        (
            QPushButton
            {
                border:1px solid rgb(170,170,170);
                    background-color:rgb(210,210,210);
            }
            QPushButton:hover
            {
                    background-color:rgb(220,220,220);
            }
                QPushButton:press
                {
                    background-color:rgb(230,230,230);
                }
        );
        break;
    }
    }

    return msg;
}

PubQss::PubQss()
{

}

PubQss::~PubQss()
{

}
