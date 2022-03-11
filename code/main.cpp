#include "mainwindow.h"
#include"headers.h"
#include "pefile.h"
#include "globalstore.h"
#include "util.h"
#include"hookSrc/hookWindow.h"
#include"parsepe.h"
#include"binary_ui_model.h"
#include"pe_ui_model.h"
int main(int argc, char *argv[])
{
#if 1
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
#endif
    //测试文件解析功能

#if 0
    binary_ui_model * tmp = new binary_ui_model(Q_NULLPTR,0,0,Q_NULLPTR);
    qDebug() << tmp->metaObject()->className();
    PE_UI_Model * ttmp = new PE_UI_Model(0,0,0,0,0);
    qDebug() << ttmp->metaObject()->className();
#endif
}
