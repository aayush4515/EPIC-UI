#include "corsika_input.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CORSIKA_INPUT w;
    w.show();
    return a.exec();
}
