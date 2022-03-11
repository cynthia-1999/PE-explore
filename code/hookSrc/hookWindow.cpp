#include "hookWindow.h"
#include "ui_hookWindow.h"

hookMainWindow::hookMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::hookMainWindow)
{
    ui->setupUi(this);
    setWindowTitle("进程选择");
    ui->menubar->addSeparator();
    ui->menubar->addSeparator();
    ui->menubar->addAction(ui->hookOp);
    ui->hookOp->setEnabled(false);

    theModel = new QStandardItemModel(this);

    if(!getPrivilege()){
        QMessageBox::information(this, "错误", "权限提升失败，尝试以管理员身份启动!");
        exit(-1);
    }
    if(!enumProcess(theModel)){
        QMessageBox::information(this,"错误","枚举进程失败!");
        exit(-1);
    }

    theModel->setHeaderData(0, Qt::Horizontal, "进程名称");
    theModel->setHeaderData(1,Qt::Horizontal, "PID");
    theModel->setHeaderData(2, Qt::Horizontal, "线程优先权");
    theModel->setHeaderData(4, Qt::Horizontal, "RAM（MB）");
    theModel->setHeaderData(3, Qt::Horizontal, "CPU");
    ui->tableView->setModel(theModel);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setSelectionMode( QAbstractItemView::SingleSelection);//设置选中单个
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    selectModel = new QItemSelectionModel(theModel);
    ui->tableView->setSelectionModel(selectModel);

    if(!getPrivilege()){
        QMessageBox::information(this, "错误", "对不起，权限提升失败，错误代码:" + QString::number(GetLastError()));
    }
}

hookMainWindow::~hookMainWindow()
{
    delete ui;
}


void hookMainWindow::on_createNewPorecess_triggered()
{
    QFileDialog *fileDialog = new QFileDialog(this);
        fileDialog->setWindowTitle(tr("打开可执行文件"));
        fileDialog->setDirectory(".");
        fileDialog->setNameFilter(tr("可执行文件(*.exe)"));
        fileDialog->setViewMode(QFileDialog::Detail);
        QStringList fileNames;
        if(fileDialog->exec())
        {
            QString fileName;
            fileNames =  fileDialog->selectedFiles();
        }
        if(fileNames.isEmpty()){
            QMessageBox::information(this, "错误", "您未选择应用程序!");
            return;
        }
        QString exePath = fileNames[0];


        const char* path = exePath.toStdString().c_str();
        STARTUPINFOA startInfo = {0};
        startInfo.cb = sizeof(startInfo);
        PROCESS_INFORMATION processInfo = {0};
        // startInfo 和 processInfo必须初始化
        BOOL ret = CreateProcessA(path,NULL,NULL,NULL,false,NULL
                      ,NULL,NULL,&startInfo,&processInfo);
        if( ret )
        {
            CloseHandle( processInfo.hProcess );
            CloseHandle( processInfo.hThread );
            on_refresh_triggered();
            QMessageBox::information(this,"成功", "创建进程成功!");
        }else
        {
            QMessageBox::information(this,"错误", "创建进程失败，错误代码：" + QString::number(GetLastError()));
        }
}

void hookMainWindow::on_setOntop_triggered(bool checked)
{
    if(checked){
        hide();
        setWindowFlags(Qt::WindowStaysOnTopHint);
        show();
    }else{
        hide();
        setWindowFlags(Qt::Widget);
        show();
    }
}

void hookMainWindow::on_Resourceutilization_triggered()
{
    Monitor m(this);
    m.exec();
}

void hookMainWindow::on_refresh_triggered()
{
    theModel->clear();
    if(!enumProcess(theModel)){
        QMessageBox::information(this,"错误","枚举进程失败!");
        exit(-1);
    }
    theModel->setHeaderData(0, Qt::Horizontal, "进程名称");
    theModel->setHeaderData(1,Qt::Horizontal, "PID");
    theModel->setHeaderData(2, Qt::Horizontal, "线程优先权");
    theModel->setHeaderData(4, Qt::Horizontal, "RAM（MB）");
    theModel->setHeaderData(3, Qt::Horizontal, "CPU");
    ui->tableView->setModel(theModel);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setSelectionMode( QAbstractItemView::SingleSelection);//设置选中单个
    ui->tableView->setSortingEnabled(true);
    selectModel->setModel(theModel);
}

void hookMainWindow::on_tableView_clicked(const QModelIndex &index)
{
    if(index.isValid()){
        ui->hookOp->setEnabled(true);
    }else{
        ui->hookOp->setEnabled(false);
    }
}

void hookMainWindow::on_exit_triggered()
{
    close();
}

void hookMainWindow::on_hookOp_triggered()
{
    QModelIndex index = ui->tableView->currentIndex();
    if(index.isValid()){
        QStandardItem *item = theModel->item(index.row(), 1);
        unsigned int pid = item->data(Qt::EditRole).toInt();
        QStandardItem *item1 = theModel->item(index.row(), 0);
        QString proName = item1->data(Qt::EditRole).toString();
        depends * d =new depends(pid, proName, nullptr);
        d->setAttribute(Qt::WA_DeleteOnClose);
        d->show();
    }else{
        QMessageBox::information(this,"错误", "您选取的进程无效!");
    }
    ui->hookOp->setEnabled(false);
}

void hookMainWindow::on_InlineHook_triggered()
{

}
