#include "Gui/OpenCV_PV_Widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
QApplication a(argc, argv);
OpenCV_PV_Widget w;
w.show();

return a.exec();
}
