#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QStackedWidget>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QComboBox>
#include <QLineEdit>
#include <QDir>

#include "Pub/PubQss.h"
#include "OpenCV_PV_Widget.h"
#include "OpenCV_PV_GLWidget.h"
#include "Task/OpenCV_PV_Thread.h"

OpenCV_PV_Widget::OpenCV_PV_Widget(QWidget *parent)
: OpenCVBaseCon(parent)
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
    Thread = new OpenCV_PV_Thread();
    Thread->start();

    this->setFixedWidth(1200);
    this->setMinimumHeight(800);
    this->Init();

    this->connect(this,&OpenCV_PV_Widget::DirPaint,this,&OpenCV_PV_Widget::DoDirPaint);
}

OpenCV_PV_Widget::~OpenCV_PV_Widget()
{

}

void OpenCV_PV_Widget::Init()
{
    QHBoxLayout* mLayout = Get_HLayout();

    QWidget* InitLeft = InitLeftWidget();
    InitLeft->setFixedWidth(150);

    QWidget* InitMid = InitMidWidget();

    QWidget* InitRight = InitRightWidget();
    InitRight->setFixedWidth(150);

    //切换按钮
//    QHBoxLayout* btnLayout = Get_HLayout();
//    QPushButton* btn_Paint = Get_Button("","","");
//    QPushButton* btn_Voice = Get_Button("","","");
//    QPushButton* btn_Camera = Get_Button("","","");
//    btnLayout->addWidget(btn_Paint);
//    btnLayout->addWidget(btn_Voice);
//    btnLayout->addWidget(btn_Camera);

//    //切换页面
//    QStackedWidget* mSWidget = new QStackedWidget();
//    mSWidget->addWidget(SwitchWid(PaintModel));
//    mSWidget->addWidget(SwitchWid(VoiceModel));
//    mSWidget->addWidget(SwitchWid(CameraModel));

//    mLayout->addLayout(btnLayout);
//    mLayout->addWidget(mSWidget);

//    this->connect(btn_Paint,&QPushButton::clicked,this,[=](){
//        mSWidget->setCurrentIndex(0);
//    });
//    this->connect(btn_Voice,&QPushButton::clicked,this,[=](){
//        mSWidget->setCurrentIndex(1);
//    });
//    this->connect(btn_Camera,&QPushButton::clicked,this,[=](){
//        mSWidget->setCurrentIndex(2);
//    });
    mLayout->addWidget(InitLeft);
    mLayout->addWidget(InitMid);
    mLayout->addWidget(InitRight);

    this->setLayout(mLayout);
}

QWidget *OpenCV_PV_Widget::InitLeftWidget()
{
    const QString SliderStyle = PubQss::Get()->Get_Style(QssTask{SLIDERSTYLE,{"0,0,0"}});
    const QString ButtonStyle = PubQss::Get()->Get_Style(QssTask{BOTTOMSTYLE,{}});

    QWidget* LeftWidget = new QWidget();
    QVBoxLayout* LeftLayout = Get_VLayout(5,0,0,0,0);

    QPushButton* btn_TopGMin = Get_Button("","","");
    const QString TopText = "工具箱";
    QWidget* TopGroupWidget = InitBtnGround(TopText,btn_TopGMin);
    //基本组
    QWidget* TopWidget = new QWidget();
    QVBoxLayout* TopLayout = Get_VLayout(20,0,0,0,0);

    QPushButton* btn_ChoosePaint = Get_Button("选择图片","",ButtonStyle);

    QPushButton* btn_ChooseVoice = Get_Button("选择视频","",ButtonStyle);

    QPushButton* btn_Camera = Get_Button("打开摄像头","",ButtonStyle);

    QPushButton* btn_Gary = Get_Button("灰度图","",ButtonStyle);

    QPushButton* btn_XFilp = Get_Button("X镜像","",ButtonStyle);

    QPushButton* btn_YFilp = Get_Button("Y镜像","",ButtonStyle);

    QHBoxLayout* LowthreshLayout = Get_HLayout(0,0,0,0,0);
    QLabel* lbl_Lowthresh = Get_Label("边缘检测低阔值","");
    QLineEdit* LowthreshEdit = new QLineEdit();
    LowthreshLayout->addWidget(lbl_Lowthresh);
    LowthreshLayout->addWidget(LowthreshEdit);

    QHBoxLayout* HighthreshLayout = Get_HLayout(0,0,0,0,0);
    QLabel* lbl_Highthresh = Get_Label("边缘检测高阔值","");
    QLineEdit* HighthreshEdit = new QLineEdit();
    HighthreshLayout->addWidget(lbl_Highthresh);
    HighthreshLayout->addWidget(HighthreshEdit);

    QPushButton* btn_EdgeDet = Get_Button("边缘检测","",ButtonStyle);
    //    QPushButton* btn_Gamma = Get_Button("伽马变换","",ButtonStyle);

    btn_ChoosePaint->setFixedHeight(30);
    btn_ChooseVoice->setFixedHeight(30);
    btn_Camera->setFixedHeight(30);
    btn_Gary->setFixedHeight(30);
    btn_EdgeDet->setFixedHeight(30);
    btn_XFilp->setFixedHeight(30);
    btn_YFilp->setFixedHeight(30);

    TopLayout->addWidget(btn_ChoosePaint);
    TopLayout->addWidget(btn_ChooseVoice);
    TopLayout->addWidget(btn_Camera);
    TopLayout->addWidget(btn_Gary);
    TopLayout->addLayout(LowthreshLayout);
    TopLayout->addLayout(HighthreshLayout);
    TopLayout->addWidget(btn_EdgeDet);
//    TopLayout->addWidget(btn_Gamma);
    TopLayout->addWidget(btn_XFilp);
    TopLayout->addWidget(btn_YFilp);
    TopWidget->setLayout(TopLayout);

    QPushButton* btn_RotateGMin = Get_Button("","","");
    const QString RotateText = "旋转组";
    QWidget* RotateGroupWidget = InitBtnGround(RotateText,btn_RotateGMin);
    //旋转组
    QWidget* RotateWidget = new QWidget();
    QVBoxLayout* RotateLayout = Get_VLayout(10,0,0,0,0);

    QComboBox* RotateBox = new QComboBox();
    QStringList str;
    str << "90" << "180" << "270";
    RotateBox->addItems(str);

    QHBoxLayout* lblRotateLayout = Get_HLayout();
    QLabel* lbl_RotateWarn = Get_Label("旋转角度","");
    QLabel* lbl_RotateText = Get_Label("90°","");
    QPushButton* btn_Rotate = Get_Button("旋转","",ButtonStyle);
    btn_Rotate->setFixedHeight(30);
    lblRotateLayout->addWidget(lbl_RotateWarn);
    lblRotateLayout->addWidget(lbl_RotateText);

    RotateLayout->addWidget(RotateBox);
    RotateLayout->addLayout(lblRotateLayout);
    RotateLayout->addWidget(btn_Rotate);
    RotateWidget->setLayout(RotateLayout);

    QPushButton* btn_SpecialGMin = Get_Button("","","");
    const QString SpecialText = "特殊组";
    QWidget* SpecialGroupWidget = InitBtnGround(SpecialText,btn_SpecialGMin);
    //特殊组
    QWidget* SpecialWidget = new QWidget();
    QVBoxLayout* SpecialLayout = Get_VLayout(10,0,0,0,0);
    QSlider* SpecialSlider = Get_Slider(1,20,0,true,SliderStyle);
    QHBoxLayout* lblSpecialLayout = Get_HLayout();
    QLabel* lbl_SpecialWarn = Get_Label("内核度数","");
    QLabel* lbl_SpecialText = Get_Label("1","");
    QHBoxLayout* SpecialBtnLayout = Get_HLayout();
    QPushButton* btn_Corrosion = Get_Button("腐蚀","",ButtonStyle);
    QPushButton* btn_Inflation = Get_Button("膨胀","",ButtonStyle);
    btn_Corrosion->setFixedHeight(30);
    btn_Inflation->setFixedHeight(30);
    SpecialBtnLayout->addWidget(btn_Corrosion);
    SpecialBtnLayout->addWidget(btn_Inflation);

    lblSpecialLayout->addWidget(lbl_SpecialWarn);
    lblSpecialLayout->addWidget(lbl_SpecialText);
    SpecialLayout->addWidget(SpecialSlider);
    SpecialLayout->addLayout(lblSpecialLayout);
    SpecialLayout->addLayout(SpecialBtnLayout);
    SpecialWidget->setLayout(SpecialLayout);

    LeftLayout->addWidget(TopGroupWidget);
    LeftLayout->addWidget(TopWidget);
    LeftLayout->addWidget(RotateGroupWidget);
    LeftLayout->addWidget(RotateWidget);
    LeftLayout->addWidget(SpecialGroupWidget);
    LeftLayout->addWidget(SpecialWidget);
    LeftLayout->addStretch();
    LeftWidget->setLayout(LeftLayout);

    this->connect(btn_TopGMin,&QPushButton::clicked,this,[=](){ShowHideWidget(TopWidget);});
    this->connect(btn_RotateGMin,&QPushButton::clicked,this,[=](){ShowHideWidget(RotateWidget);});
    this->connect(btn_SpecialGMin,&QPushButton::clicked,this,[=](){ShowHideWidget(SpecialWidget);});

    //打开图片
    this->connect(btn_ChoosePaint,&QPushButton::clicked,this,[=](){
        QString Paint = QFileDialog::getOpenFileName(this,"打开文件",QDir::currentPath());
        if(!Paint.isEmpty())
        {
            emit SwitchSWidget(0);
            Thread->OpenPaint(Paint.toLocal8Bit().data());
        }
    });
    //打开视频
    this->connect(btn_ChooseVoice,&QPushButton::clicked,this,[=](){
        QString Paint = QFileDialog::getOpenFileName(this,"打开文件",QDir::currentPath());
        if(!Paint.isEmpty())
        {
            emit SwitchSWidget(1);
            Thread->OpenVoice(Paint.toLocal8Bit().data());
            startTimer(Thread->Get_FPS());
        }
    });
    this->connect(btn_Camera,&QPushButton::clicked,this,[=](){
        emit SwitchSWidget(2);
        Thread->Open();
    });
    this->connect(btn_EdgeDet,&QPushButton::clicked,this,[=](){
        if(Thread->GetisVideoisWrite())
        {
            return;
        }
        PV_Filter::Get()->Add(Get_TaskModel(),PVTask{Canny,{LowthreshEdit->text().toInt(),HighthreshEdit->text().toInt()}});
    });
    this->connect(SpecialSlider,&QSlider::valueChanged,this,[=](){
        lbl_SpecialText->setText(QString::number(SpecialSlider->value()));
    });
    this->connect(btn_Corrosion,&QPushButton::clicked,this,[=](){
        PV_Filter::Get()->Add(Get_TaskModel(),PVTask{Eroder,{SpecialSlider->value()}});
    });
    this->connect(btn_Inflation,&QPushButton::clicked,this,[=](){
        PV_Filter::Get()->Add(Get_TaskModel(),PVTask{Dilate,{SpecialSlider->value()}});
    });
    void (QComboBox::*ComboChange)(const QString &) = &QComboBox::activated;
    this->connect(RotateBox,ComboChange,this,[=](const QString& text){
        lbl_RotateText->setText(text + "°");
    });
    this->connect(btn_Rotate,&QPushButton::clicked,this,[=](){
        if(Thread->GetisVideoisWrite())
        {
            return;
        }
        PV_Filter::Get()->Add(Get_TaskModel(),PVTask{Rotate,{RotateBox->currentText()}});
    });
    this->connect(btn_Gary,&QPushButton::clicked,this,[=](){
        if(Thread->GetisVideoisWrite())
        {
            return;
        }
        PV_Filter::Get()->Add(Get_TaskModel(),PVTask{Gary,{}});
    });
    this->connect(btn_XFilp,&QPushButton::clicked,this,[=](){
        PV_Filter::Get()->Add(Get_TaskModel(),PVTask{FilpX,{}});
    });
    this->connect(btn_YFilp,&QPushButton::clicked,this,[=](){
        PV_Filter::Get()->Add(Get_TaskModel(),PVTask{FilpY,{}});
    });


    return LeftWidget;
}

QWidget *OpenCV_PV_Widget::InitMidWidget()
{
    const QString ButtonStyle = PubQss::Get()->Get_Style(QssTask{SWITCHBUTTONSTYLE,{}});
    QWidget* mWidget = new QWidget();
    QVBoxLayout* mLayout = Get_VLayout();

    QHBoxLayout* BtnLayout = Get_HLayout(0,69,0,0,0);
    QPushButton* btn_Paint = Get_Button("图片处理","",ButtonStyle);
    QPushButton* btn_Voice = Get_Button("视频处理","",ButtonStyle);
    QPushButton* btn_Camera = Get_Button("摄像头","",ButtonStyle);
    btn_Paint->setFixedSize(70,20);
    btn_Voice->setFixedSize(70,20);
    btn_Camera->setFixedSize(70,20);
    BtnLayout->addWidget(btn_Paint);
    BtnLayout->addWidget(btn_Voice);
    BtnLayout->addWidget(btn_Camera);
    BtnLayout->addStretch();

    QStackedWidget* mSWidget = new QStackedWidget();
    mSWidget->setObjectName("mSWidget");
    mSWidget->addWidget(SwitchWid(PaintModelWid));
    mSWidget->addWidget(SwitchWid(VoiceModelWid));
    mSWidget->addWidget(SwitchWid(CameraModelWid));

    mLayout->addLayout(BtnLayout);
    mLayout->addWidget(mSWidget);
    mWidget->setLayout(mLayout);

    this->connect(btn_Paint,&QPushButton::clicked,this,[=](){
        mSWidget->setCurrentIndex(0);
    });
    this->connect(btn_Voice,&QPushButton::clicked,this,[=](){
        mSWidget->setCurrentIndex(1);
    });
    this->connect(btn_Camera,&QPushButton::clicked,this,[=](){
        mSWidget->setCurrentIndex(2);
    });
    this->connect(this,&OpenCV_PV_Widget::SwitchSWidget,this,[=](int index){
        mSWidget->setCurrentIndex(index);
    });

    return mWidget;

}

QWidget *OpenCV_PV_Widget::InitRightWidget()
{
    const QString ButtonStyle = PubQss::Get()->Get_Style(QssTask{BOTTOMSTYLE,{}});
    const QString SliderStyle = PubQss::Get()->Get_Style(QssTask{SLIDERSTYLE,{"0,0,0"}});
    const QString RSliderStyle = PubQss::Get()->Get_Style(QssTask{SLIDERSTYLE,{"255,0,0"}});
    const QString GSliderStyle = PubQss::Get()->Get_Style(QssTask{SLIDERSTYLE,{"0,255,0"}});
    const QString BSliderStyle = PubQss::Get()->Get_Style(QssTask{SLIDERSTYLE,{"0,0,255"}});
    QWidget* RightWidget = new QWidget();
    QVBoxLayout* RightLayout = Get_VLayout(5,0,0,0,0);

    QPushButton* btn_BasisGMin = Get_Button("","","");
    const QString BasisText = "基础调整";
    QWidget* BasisGroup = InitBtnGround(BasisText,btn_BasisGMin);

    QWidget* BasisWidget = new QWidget();
    QVBoxLayout* BasisLayout = Get_VLayout(10,0,0,0,0);

    QSlider* LightSlider = Get_Slider(-100,100,0,true,SliderStyle);
    QLabel* lbl_LightText = Get_Label("0","");
    QWidget* LightWidget = InitLayoutLabel("亮度",LightSlider,lbl_LightText);

    QSlider* ContrastSlider = Get_Slider(1,3,1,true,SliderStyle);
    QLabel* lbl_ContrastText = Get_Label("1","");
    QWidget* ContrastWidget = InitLayoutLabel("对比度",ContrastSlider,lbl_ContrastText);

    QSlider* BinarySlider = Get_Slider(-1,255,-1,true,SliderStyle);
    QLabel* lbl_BinaryText = Get_Label("-1","");
    QWidget* BinaryWidget = InitLayoutLabel("二值化",BinarySlider,lbl_BinaryText);

    QSlider* SaturationSlider = Get_Slider(-1,255,-1,true,SliderStyle);
    QLabel* lblSaturationText = Get_Label("-1","");
    QWidget* SaturationWidget = InitLayoutLabel("饱和度",SaturationSlider,lblSaturationText);

    QSlider* AveFitSlider = Get_Slider(-1,20,-1,true,SliderStyle);
    QLabel* lbl_AveFitText = Get_Label("-1","");
    QWidget* AveFitWidget = InitLayoutLabel("均值滤波",AveFitSlider,lbl_AveFitText);

    BasisLayout->addWidget(LightWidget);
    BasisLayout->addWidget(ContrastWidget);
    BasisLayout->addWidget(BinaryWidget);
    BasisLayout->addWidget(SaturationWidget);
    BasisLayout->addWidget(AveFitWidget);
    BasisWidget->setLayout(BasisLayout);

    QPushButton* btn_ColorGMin = Get_Button("","","");
    const QString ColorText = "色彩调整";
    QWidget* ColorGroup = InitBtnGround(ColorText,btn_ColorGMin);

    QWidget* ColorWidget = new QWidget();
    QVBoxLayout* ColorLayout = Get_VLayout(10,0,0,0,0);

    QSlider* RSlider = Get_Slider(-1,255,-1,true,RSliderStyle);
    QLabel* lblRText = Get_Label("-1","");
    QWidget* RWidget = InitLayoutLabel("R",RSlider,lblRText);

    QSlider* GSlider = Get_Slider(-1,255,-1,true,GSliderStyle);
    QLabel* lblGText = Get_Label("-1","");
    QWidget* GWidget = InitLayoutLabel("R",GSlider,lblGText);

    QSlider* BSlider = Get_Slider(-1,255,-1,true,BSliderStyle);
    QLabel* lblBText = Get_Label("-1","");
    QWidget* BWidget = InitLayoutLabel("R",BSlider,lblBText);

    QPushButton* btn_SaveGMin = Get_Button("","","");
    const QString SaveGText = "保存";
    QWidget* SaveGGroup = InitBtnGround(SaveGText,btn_SaveGMin);

    QWidget* SaveWidget = new QWidget();
    QVBoxLayout* SaveLayout = Get_VLayout(10,0,0,0,0);

    QPushButton* btn_ShowImg = Get_Button("显示原图","",ButtonStyle);

    QPushButton* btn_SavePaint = Get_Button("保存图片","",ButtonStyle);

    QPushButton* btn_SaveVodio = Get_Button("保存视频","",ButtonStyle);

    btn_ShowImg->setFixedHeight(30);
    btn_SavePaint->setFixedHeight(30);
    btn_SaveVodio->setFixedHeight(30);

    SaveLayout->addWidget(btn_ShowImg);
    SaveLayout->addWidget(btn_SavePaint);
    SaveLayout->addWidget(btn_SaveVodio);
    SaveWidget->setLayout(SaveLayout);

    ColorLayout->addWidget(RWidget);
    ColorLayout->addWidget(GWidget);
    ColorLayout->addWidget(BWidget);
    ColorWidget->setLayout(ColorLayout);

    RightLayout->addWidget(BasisGroup);
    RightLayout->addWidget(BasisWidget);
    RightLayout->addWidget(ColorGroup);
    RightLayout->addWidget(ColorWidget);
    RightLayout->addWidget(SaveGGroup);
    RightLayout->addWidget(SaveWidget);
    RightLayout->addStretch();
    RightWidget->setLayout(RightLayout);

    //-------------------隐藏显示组
    this->connect(btn_SaveGMin,&QPushButton::clicked,this,[=](){
        ShowHideWidget(SaveWidget);
    });
    this->connect(btn_BasisGMin,&QPushButton::clicked,this,[=](){
        ShowHideWidget(BasisWidget);
    });
    this->connect(btn_ColorGMin,&QPushButton::clicked,this,[=](){
        ShowHideWidget(ColorWidget);
    });

    //显示原图
    this->connect(btn_ShowImg,&QPushButton::clicked,this,[=](){
        PV_Filter::Get()->Clear(Get_TaskModel());
    });
    //保存图片
    this->connect(btn_SavePaint,&QPushButton::clicked,this,[=]()
    {
        QString FileStr = QFileDialog::getSaveFileName(this,"保存",QDir::currentPath());
        if(FileStr.isEmpty())
            return;
        if(FileStr.split(".").last() != "jpg" && FileStr.split(".").last() != "png")
            return;
        if(Get_TaskModel() == PaintModel)
            Thread->SavePaint(FileStr.toLocal8Bit().data());
        else if(Get_TaskModel() == VodeoModel)
            Thread->SaveVideoPaint(FileStr.toLocal8Bit().data());
        else
            Thread->SaveCamearPaint(FileStr.toLocal8Bit().data());
    });
    //剪切视频
    this->connect(btn_SaveVodio,&QPushButton::clicked,this,[=](){});

    //----------------------进度条增加改变值
    this->connect(BinarySlider,&QSlider::valueChanged,this,[=](){
        PV_Filter::Get()->Add(Get_TaskModel(),PVTask{Threshold,{BinarySlider->value()}});
        lbl_BinaryText->setText(QString::number(BinarySlider->value()));
    });
    this->connect(AveFitSlider,&QSlider::valueChanged,this,[=](){
        PV_Filter::Get()->Add(Get_TaskModel(),PVTask{Blur,{AveFitSlider->value()}});
        lbl_AveFitText->setText(QString::number(AveFitSlider->value()));
    });
    this->connect(RSlider,&QSlider::valueChanged,this,[=](){
        PV_Filter::Get()->Add(Get_TaskModel(),PVTask{ChangeRGB,{RSlider->value(),GSlider->value(),BSlider->value()}});
        lblRText->setText(QString::number(RSlider->value()));
    });
    this->connect(GSlider,&QSlider::valueChanged,this,[=](){
        PV_Filter::Get()->Add(Get_TaskModel(),PVTask{ChangeRGB,{RSlider->value(),GSlider->value(),BSlider->value()}});
        lblGText->setText(QString::number(GSlider->value()));
    });
    this->connect(BSlider,&QSlider::valueChanged,this,[=](){
        PV_Filter::Get()->Add(Get_TaskModel(),PVTask{ChangeRGB,{RSlider->value(),GSlider->value(),BSlider->value()}});
        lblBText->setText(QString::number(BSlider->value()));
    });
    this->connect(LightSlider,&QSlider::valueChanged,this,[=](){
        PV_Filter::Get()->Add(Get_TaskModel(),PVTask{Gain,{ContrastSlider->value(),LightSlider->value()}});
        lbl_LightText->setText(QString::number(LightSlider->value()));
    });
    this->connect(ContrastSlider,&QSlider::valueChanged,this,[=](){
        PV_Filter::Get()->Add(Get_TaskModel(),PVTask{Gain,{ContrastSlider->value(),LightSlider->value()}});
        lbl_ContrastText->setText(QString::number(ContrastSlider->value()));
    });
    this->connect(SaturationSlider,&QSlider::valueChanged,this,[=](){
        PV_Filter::Get()->Add(Get_TaskModel(),PVTask{Saturability,{SaturationSlider->value()}});
        lblSaturationText->setText(QString::number(SaturationSlider->value()));
    });

    return RightWidget;
}

QWidget *OpenCV_PV_Widget::SwitchWid(OpencvModel _Model)
{
    QWidget* mWidget = new QWidget();
    switch (_Model)
    {
    case PaintModelWid:
    {
        const QString ButtonPerStyle = PubQss::Get()->Get_Style(QssTask{BOTTOMROUNDSTYLE,{"://qrc/prev.png"}});
        const QString ButtonNextStyle = PubQss::Get()->Get_Style(QssTask{BOTTOMROUNDSTYLE,{":/qrc/next.png"}});
        const QString ButtonStyle = PubQss::Get()->Get_Style(QssTask{BOTTOMSTYLE,{}});

        QWidget* Wid = new QWidget();
        QVBoxLayout* mLayout = Get_VLayout(20,0,0,0,20);

        QWidget* TopWidget = new QWidget();
        QHBoxLayout* TopLayout = Get_HLayout();
        QPushButton* btn_PrePaint = Get_Button("","上一张",ButtonPerStyle);
        btn_PrePaint->setFixedSize(64,64);
        OpenCV_PV_GLWidget* GL_PaintWidget = new OpenCV_PV_GLWidget();
        GL_PaintWidget->setObjectName("GL_PaintWidget");
        GL_PaintWidget->setFixedWidth(760);
        QPushButton* btn_NextPaint = Get_Button("","下一张",ButtonNextStyle);
        btn_NextPaint->setFixedSize(64,64);
        TopLayout->addWidget(btn_PrePaint);
        TopLayout->addWidget(GL_PaintWidget);
        TopLayout->addWidget(btn_NextPaint);
        TopWidget->setLayout(TopLayout);

        QWidget* MidWidget = new QWidget();
        MidWidget->setFixedHeight(90);
        QHBoxLayout* MidLayout = Get_HLayout(20,0,0,0,0);
        OpenCV_PV_GLWidget* Pre_GLWidget = new OpenCV_PV_GLWidget();
        OpenCV_PV_GLWidget* Mid_GLWidget = new OpenCV_PV_GLWidget();
        OpenCV_PV_GLWidget* Next_GLWidget = new OpenCV_PV_GLWidget();
        Pre_GLWidget->setObjectName("Pre_GLWidget");
        Mid_GLWidget->setObjectName("Mid_GLWidget");
        Next_GLWidget->setObjectName("Next_GLWidget");
        Pre_GLWidget->setFixedSize(160,90);
        Mid_GLWidget->setFixedSize(160,90);
        Next_GLWidget->setFixedSize(160,90);
        MidLayout->addStretch();
        MidLayout->addWidget(Pre_GLWidget);
        MidLayout->addWidget(Mid_GLWidget);
        MidLayout->addWidget(Next_GLWidget);
        MidLayout->addStretch();
        MidWidget->setLayout(MidLayout);

        QWidget* BottomWidget = new QWidget();
        BottomWidget->setFixedHeight(30);
        QHBoxLayout* BottomLayout = Get_HLayout(10,0,0,0,0);
        QPushButton* btn_FileDir = Get_Button("打开文件夹","",ButtonStyle);
        btn_FileDir->setFixedSize(130,30);
        BottomLayout->addStretch();
        BottomLayout->addWidget(btn_FileDir);
        BottomLayout->addStretch();
        BottomWidget->setLayout(BottomLayout);

        mLayout->addWidget(TopWidget);
        mLayout->addWidget(MidWidget);
        mLayout->addWidget(BottomWidget);
        Wid->setLayout(mLayout);

        this->connect(btn_PrePaint,&QPushButton::clicked,this,[=](){
            PV_Filter::Get()->Clear();
            if(PaintIndex == 0)
            {
                PaintIndex = PaintList.size() - 1;
            }
            else
                PaintIndex -= 1;
            Thread->Open(PaintList,PaintIndex);
        });
#if 0
        this->connect(btn_ShowOrignalImg,&QPushButton::clicked,this,[=](){
            PV_Filter::Get()->Clear();
        });
        this->connect(btn_SavePaint,&QPushButton::clicked,this,[=](){
            QString FileStr = QFileDialog::getSaveFileName(this,"保存",QDir::currentPath());
            if(FileStr.isEmpty())
                return;
            if(FileStr.split(".").last() != "jpg" && FileStr.split(".").last() != "png")
                return;
            Thread->SavePaint(FileStr.toLocal8Bit().data());
        });
#endif
        this->connect(btn_NextPaint,&QPushButton::clicked,this,[=](){
            PV_Filter::Get()->Clear();
            if(PaintIndex == PaintList.size() - 1)
            {
                PaintIndex = 0;
            }
            else
                PaintIndex += 1;
            Thread->Open(PaintList,PaintIndex);
        });
        this->connect(btn_NextPaint,&QPushButton::clicked,this,[=](){});
        this->connect(Thread,&OpenCV_PV_Thread::ShowPaint,GL_PaintWidget,&OpenCV_PV_GLWidget::ShowImg);
        this->connect(Thread,&OpenCV_PV_Thread::ShowPer,Pre_GLWidget,&OpenCV_PV_GLWidget::ShowImg);
        this->connect(Thread,&OpenCV_PV_Thread::ShowMid,Mid_GLWidget,&OpenCV_PV_GLWidget::ShowImg);
        this->connect(Thread,&OpenCV_PV_Thread::ShowNext,Next_GLWidget,&OpenCV_PV_GLWidget::ShowImg);

        this->connect(btn_FileDir,&QPushButton::clicked,this,[=](){
             QString filename = QFileDialog::getExistingDirectory();
             if(filename.isEmpty())
                 return;
             QDir *dir=new QDir(filename);
             QStringList filter;
             QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
             PaintList.clear();
             for(int i = 0; i < fileInfo->size(); i++)
             {
                 QString Path = fileInfo->at(i).fileName();
                 if(Path.split(".").last() == "png" ||Path.split(".").last() == "jpg")
                 {
                     PaintList.push_back(fileInfo->at(i).path() + "/" + fileInfo->at(i).fileName());
                 }
             }
            emit DirPaint();

        });
        mWidget = Wid;

        break;
    }
    case VoiceModelWid:
    {
        const QString ButtonStyle = PubQss::Get()->Get_Style(QssTask{BOTTOMSTYLE,{}});
        const QString ButtonStartStyle = PubQss::Get()->Get_Style(QssTask{BOTTOMROUNDSTYLE,{"://qrc/start.png"}});
        const QString ButtonStopStyle = PubQss::Get()->Get_Style(QssTask{BOTTOMROUNDSTYLE,{"://qrc/stop.png"}});
        const QString SliderStyle = PubQss::Get()->Get_Style(QssTask{SLIDERSTYLE,{"0,0,0"}});

        QWidget* Wid = new QWidget();
        QVBoxLayout* mLayout = Get_VLayout();

        QWidget* TopWidget = new QWidget();
        QHBoxLayout* TopLayout = Get_HLayout();
        OpenCV_PV_GLWidget* GL_PaintWidget = new OpenCV_PV_GLWidget();
        GL_PaintWidget->setObjectName("GL_PaintWidget");
        GL_PaintWidget->setFixedWidth(760);
        TopLayout->addWidget(GL_PaintWidget);
        TopWidget->setLayout(TopLayout);

        QSlider* PlaySlider = Get_Slider(0,1000,0,true,SliderStyle);
        QSlider* BeginSlider = Get_Slider(0,1000,0,true,SliderStyle);
        QSlider* EndSlider = Get_Slider(0,1000,0,true,SliderStyle);
        PlaySlider->installEventFilter(this);
        BeginSlider->installEventFilter(this);
        EndSlider->installEventFilter(this);
        PlaySlider->setFixedWidth(760);
        BeginSlider->setFixedWidth(760);
        EndSlider->setFixedWidth(760);
        PlaySlider->setObjectName("PlaySlider");
        BeginSlider->setObjectName("BeginSlider");
        EndSlider->setObjectName("EndSlider");

        QHBoxLayout* ButtonLayout = Get_HLayout(10,0,0,0,0);
        QPushButton* btn_Play = Get_Button("","",ButtonStopStyle);
        btn_Play->setFixedSize(64,64);
        QPushButton* btn_Save = Get_Button("剪切视频","",ButtonStyle);
        btn_Save->setFixedSize(130,30);
        ButtonLayout->addStretch();
        ButtonLayout->addWidget(btn_Play);
        ButtonLayout->addWidget(btn_Save);
        ButtonLayout->addStretch();

        mLayout->addWidget(TopWidget);
        mLayout->addSpacing(20);
        mLayout->addWidget(PlaySlider,0,Qt::AlignHCenter);
        mLayout->addSpacing(5);
        mLayout->addWidget(BeginSlider,0,Qt::AlignHCenter);
        mLayout->addSpacing(5);
        mLayout->addWidget(EndSlider,0,Qt::AlignHCenter);
        mLayout->addSpacing(20);
        mLayout->addLayout(ButtonLayout);
        mLayout->addSpacing(20);
        Wid->setLayout(mLayout);

        mWidget = Wid;

        //剪切视频
        this->connect(btn_Save,&QPushButton::clicked,this,[=](){
            if(!Thread->GetisVideoisWrite())
            {
                bool isRotate = PV_Filter::Get()->IsRotateX(VodeoModel);
                bool isGary = PV_Filter::Get()->IsGaryChannels(VodeoModel);
                int StartPos = -1;
                int EndPos = -1;
                QString ExportNmae = QFileDialog::getSaveFileName(this,"保存文件","out.avi");
                if (ExportNmae.isEmpty())
                    return;
                std::string filename = ExportNmae.toLocal8Bit().data();
                //如果剪切结束进度条大于剪切开始进度条 说明进度条剪切生效
                if(EndSlider->value() > BeginSlider->value())
                {
                    StartPos = BeginSlider->value();
                    EndPos = EndSlider->value();
                }
                if(Thread->SaveVideo(filename,StartPos,EndPos,isRotate,isGary))
                {
                    btn_Save->setText("剪切中...");
                }
            }
            else
            {
                Thread->StopSaveVideo();
            }
        });
        //暂停开始更换图标
        this->connect(Thread,&OpenCV_PV_Thread::SwitchPlayIcon,this,[=](bool isPlay){
//            qDebug() << isPlay;
            const QString PlayIcon = PubQss::Get()->Get_Style(QssTask{BOTTOMROUNDSTYLE,{":/qrc/start.png"}});
            const QString StopIcon = PubQss::Get()->Get_Style(QssTask{BOTTOMROUNDSTYLE,{":/qrc/stop.png"}});
            if(isPlay)
            {
                btn_Play->setStyleSheet(StopIcon);
            }
            else
            {
                btn_Play->setStyleSheet(PlayIcon);
            }
            btn_Play->update();
        });
        //暂停/开始
        this->connect(btn_Play,&QPushButton::clicked,this,[=](){
            Thread->StartStopVideo();
        });
        //显示视频
        this->connect(Thread,&OpenCV_PV_Thread::ShowVoice,GL_PaintWidget,&OpenCV_PV_GLWidget::ShowImg);
        //
        this->connect(PlaySlider,&QSlider::sliderMoved,this,[=](){Thread->Seek(PlaySlider->value() / 1000.0);});
        //剪切取消之后更改文本
        this->connect(Thread,&OpenCV_PV_Thread::SaveVideoEnd,this,[=](){
            btn_Save->setText("剪切视频");
        });

        break;
    }
    case CameraModelWid:
    {
        const QString ButtonStyle = PubQss::Get()->Get_Style(QssTask{BOTTOMSTYLE,{}});

        QWidget* Wid = new QWidget();
        QVBoxLayout* mLayout = Get_VLayout(20,0,0,0,20);

        QWidget* TopWidget = new QWidget();
        QHBoxLayout* TopLayout = Get_HLayout();
        OpenCV_PV_GLWidget* GL_PaintWidget = new OpenCV_PV_GLWidget();
        GL_PaintWidget->setObjectName("GL_PaintWidget");
        GL_PaintWidget->setFixedWidth(760);
        TopLayout->addWidget(GL_PaintWidget);
        TopWidget->setLayout(TopLayout);

        QHBoxLayout* ButtonLayout = Get_HLayout(10,0,0,0,0);
        QPushButton* btn_Face = Get_Button("人脸识别","",ButtonStyle);
        btn_Face->setFixedSize(130,30);
        QPushButton* btn_Save = Get_Button("剪切视频","",ButtonStyle);
        btn_Save->setFixedSize(130,30);
        ButtonLayout->addStretch();
        ButtonLayout->addWidget(btn_Face);
        ButtonLayout->addWidget(btn_Save);
        ButtonLayout->addStretch();

        mLayout->addWidget(TopWidget);
        mLayout->addLayout(ButtonLayout);
        Wid->setLayout(mLayout);

        this->connect(Thread,&OpenCV_PV_Thread::ShowCamera,GL_PaintWidget,&OpenCV_PV_GLWidget::ShowImg);

        //人脸识别
        this->connect(btn_Face,&QPushButton::clicked,this,[=](){
            PV_Filter::Get()->Add(Get_TaskModel(),PVTask{FaceCascade,{}});
        });
        this->connect(btn_Save,&QPushButton::clicked,this,[=](){
            if(!Thread->GetisCamearisWrite())
            {
                bool isRotate = PV_Filter::Get()->IsRotateX(CameraModel);
                bool isGary = PV_Filter::Get()->IsGaryChannels(CameraModel);
                QString ExportNmae = QFileDialog::getSaveFileName(this,"保存文件","outC.avi");
                if (ExportNmae.isEmpty())
                    return;
                std::string filename = ExportNmae.toLocal8Bit().data();
                if(Thread->SaveCamearVideo(filename,isRotate,isGary))
                {
                    btn_Save->setText("剪切中...");
                }
            }
            else
            {
                Thread->StopCamearVideo();
            }
        });
        this->connect(Thread,&OpenCV_PV_Thread::SaveCamearEnd,this,[=](){
            btn_Save->setText("剪切视频");
        });

        mWidget = Wid;
    }
    }

    return mWidget;
}

QWidget *OpenCV_PV_Widget::InitBtnGround(QString _Text,QPushButton* btn_Min)
{
    QWidget* mWidget = new QWidget();
    mWidget->setObjectName("Widget");
    mWidget->setStyleSheet("QWidget#Widget{border:1px solid rgb(0,0,0);}");
    mWidget->setFixedHeight(20);

    QHBoxLayout* mLayout = Get_HLayout(10,0,0,0,0);
    QLabel* lbl_Text = Get_Label(_Text,"");
    btn_Min->setFixedWidth(20);
    btn_Min->setText("-");
    mLayout->addWidget(lbl_Text);
    mLayout->addWidget(btn_Min);

    mWidget->setLayout(mLayout);
    return mWidget;
}

QWidget *OpenCV_PV_Widget::InitLayoutLabel(QString _Text,QSlider *mSlider, QLabel* lbl_Text)
{
    QWidget* mWidget = new QWidget();
    QVBoxLayout* mLayout = Get_VLayout(10,0,0,0,0);
    QHBoxLayout* lblLayout = Get_HLayout();
    QLabel* lbl_Warn = Get_Label(_Text,"");
    lblLayout->addWidget(lbl_Warn);
    lblLayout->addWidget(lbl_Text);
    mLayout->addLayout(lblLayout);
    mLayout->addWidget(mSlider);
    mWidget->setLayout(mLayout);

    return mWidget;
}

void OpenCV_PV_Widget::ShowHideWidget(QWidget *Widget)
{
    if(Widget->isHidden())
        Widget->setVisible(true);
    else
        Widget->setVisible(false);
}

TaskModel OpenCV_PV_Widget::Get_TaskModel()
{
    QStackedWidget* mSWidget = this->findChild<QStackedWidget*>("mSWidget");
    if(mSWidget->currentIndex() == 0)
        return PaintModel;
    else if(mSWidget->currentIndex() == 1)
        return VodeoModel;
    else if(mSWidget->currentIndex() == 2)
        return CameraModel;
}

void OpenCV_PV_Widget::DoDirPaint()
{
#if 0
    OpenCV_PV_GLWidget* Pre_GLWidget = this->findChild<OpenCV_PV_GLWidget*>("Pre_GLWidget");
    OpenCV_PV_GLWidget* Mid_GLWidget = this->findChild<OpenCV_PV_GLWidget*>("Mid_GLWidget");
    OpenCV_PV_GLWidget* Next_GLWidget = this->findChild<OpenCV_PV_GLWidget*>("Next_GLWidget");
    OpenCV_PV_GLWidget* GL_PaintWidget = this->findChild<OpenCV_PV_GLWidget*>("GL_PaintWidget");
    if(!Thread->Open(PaintList,0))
    {
        qDebug() << "openError";
    }
#endif
    PaintIndex = 0;
    Thread->Open(PaintList,PaintIndex);
}

bool OpenCV_PV_Widget::eventFilter(QObject *obj, QEvent *event)
{
    QSlider* PlaySlider = this->findChild<QSlider*>("PlaySlider");
    QSlider* BeginSlider = this->findChild<QSlider*>("BeginSlider");
    QSlider* EndSlider = this->findChild<QSlider*>("EndSlider");
    QMouseEvent* MouseEvent = static_cast<QMouseEvent*>(event);
    if(obj == PlaySlider)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
//            isPlayerClicked = true;
//            Thread->StartStopVideo();
            int Dur = PlaySlider->maximum() - PlaySlider->minimum();
            int Pos = PlaySlider->minimum() + Dur * ((double)MouseEvent->x() / PlaySlider->width());
            if(Pos != PlaySlider->sliderPosition())
            {
                PlaySlider->setValue(Pos);
            }
            Thread->Seek(PlaySlider->value() / 1000.0);
//            Thread->StartStopVideo();
        }
        else if(event->type() == QEvent::MouseButtonRelease)
        {
//            Thread->StartStopVideo();
//            isPlayerClicked = false;
        }
    }
    if(obj == BeginSlider)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            int Dur = BeginSlider->maximum() - BeginSlider->minimum();
            int Pos = BeginSlider->minimum() + Dur * ((double)MouseEvent->x() / BeginSlider->width());
            if(Pos != BeginSlider->sliderPosition())
            {
                BeginSlider->setValue(Pos);
            }
        }
    }
    if(obj == EndSlider)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            int Dur = EndSlider->maximum() - EndSlider->minimum();
            int Pos = EndSlider->minimum() + Dur * ((double)MouseEvent->x() / EndSlider->width());
            if(Pos != EndSlider->sliderPosition())
            {
                EndSlider->setValue(Pos);
            }
        }
    }
    return OpenCVBaseCon::eventFilter(obj,event);
}

void OpenCV_PV_Widget::timerEvent(QTimerEvent *e)
{
    if(isPlayerClicked)
        return;
    QSlider* PlaySlider = this->findChild<QSlider*>("PlaySlider");
    PlaySlider->setValue(Thread->Get_Pos() * 1000);
}
