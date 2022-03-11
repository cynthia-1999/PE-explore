#include "monitor.h"
#include "ui_monitor.h"

Monitor::Monitor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Monitor)
{
    ui->setupUi(this);
    ui->tabWidget->clear();


    cpu * c = new cpu(this);
    memory * m = new memory(this);

    c->setAttribute(Qt::WA_DeleteOnClose);
    m->setAttribute(Qt::WA_DeleteOnClose);
    c->setWindowTitle("CPU利用率:");
    m->setWindowTitle("内存使用率");
    ui->tabWidget->addTab(c,"CPU利用率");
    ui->tabWidget->addTab(m, "内存利用率");
    setWindowTitle("资源监视器(功能尚未完成，暂用随机数作图)");
}

Monitor::~Monitor()
{
    delete ui;
}
