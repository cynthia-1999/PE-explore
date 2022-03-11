#include "depends.h"
#include "ui_depends.h"

depends::depends(DWORD pid, QString name, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::depends)
{
    this->pid = pid;
    proName = name;
    ui->setupUi(this);

    setWindowTitle("目标进程：" + name);
    moduleModle = new QStringListModel(this);
    funcModle = new QStringListModel(this);

    if(! enumModuleAndFunc(pid, moduleModle, &modAndFunc, proName)){
        QMessageBox::information(this, "错误",
                                 "枚举模块失败!错误代码：" + QString::number(GetLastError()));
    }

    ui->module->setModel(moduleModle);
    ui->importFunc->setModel(funcModle);

    ui->hook->setEnabled(false);
    ui->inlineHook->setEnabled(false);
    ui->module->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->importFunc->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

depends::~depends()
{
    delete ui;
}

void depends::on_module_clicked(const QModelIndex &index)
{
    if(index.isValid()){
        QString name = moduleModle->data(index).toString();
        funcModle->setStringList(*modAndFunc.find(name));
        ui->hook->setEnabled(false);
        ui->inlineHook->setEnabled(false);
    }
}

void depends::on_importFunc_clicked(const QModelIndex &index)
{
    if(index.isValid()){
        ui->hook->setEnabled(true);
        ui->inlineHook->setEnabled(true);
    }
    //ui->hook->setEnabled(false);
   // ui->inlineHook->setEnabled(false);
}

void depends::on_hook_clicked()
{
    QString modName = moduleModle->data(ui->module->currentIndex()).toString();
    QString funcName = funcModle->data(ui->importFunc->currentIndex()).toString();
    if(!hook(pid, proName, modName, funcName)){
        QMessageBox::information(this,"错误", "对不起，HOOK失败，错误代码：" + QString::number(GetLastError()));
    }else{
        QMessageBox::information(this,"成功", "Hook成功!");
    }
    ui->hook->setEnabled(false);
}

void depends::on_inlineHook_clicked()
{
    QString modName = moduleModle->data(ui->module->currentIndex()).toString();
    QString funcName = funcModle->data(ui->importFunc->currentIndex()).toString();


    if(!inlinehook(pid, proName, modName, funcName)){
        QMessageBox::information(this,"错误", "对不起，inHook失败，错误代码：" + QString::number(GetLastError()));
    }else{
        QMessageBox::information(this,"成功", "inHook成功!");
    }
    ui->hook->setEnabled(false);
}
