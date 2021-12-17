#ifndef _PUB_QSS_H_
#define _PUB_QSS_H_

#include <QVector>
#include <QVariant>

enum Qss
{
    BOTTOMSTYLE,
    BOTTOMROUNDSTYLE,
    SLIDERSTYLE,
    SWITCHBUTTONSTYLE,
};

struct QssTask
{
    Qss QssStyle;
    QList<QVariant> Value;
};

class PubQss
{
public:
    static PubQss* Get();
    static QString Get_Style(QssTask);
private:
    PubQss();
    ~PubQss();


};

#endif //_PUB_QSS_H_
