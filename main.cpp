#include "qrgchartdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QrgChartDialog w;
    w.show();

    return a.exec();
}
