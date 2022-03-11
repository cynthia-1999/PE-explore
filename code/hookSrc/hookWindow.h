#ifndef HOOKMAINWINDOW_H
#define HOOKMAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <windows.h>
#include<QItemSelectionModel>
#include <TlHelp32.h>
#include <Dbghelp.h>
#include "depends.h"

#include "monitor.h"
#include "hookUtil.h"

QT_BEGIN_NAMESPACE
namespace Ui { class hookMainWindow; }
QT_END_NAMESPACE

class hookMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    hookMainWindow(QWidget *parent = nullptr);
    ~hookMainWindow();

private slots:
    void on_createNewPorecess_triggered();


    void on_setOntop_triggered(bool checked);

    void on_Resourceutilization_triggered();

    void on_refresh_triggered();

    void on_tableView_clicked(const QModelIndex &index);

    void on_exit_triggered();

    void on_hookOp_triggered();

    void on_InlineHook_triggered();

private:
    Ui::hookMainWindow *ui;
    QStandardItemModel* theModel;
    QItemSelectionModel* selectModel;
};
#endif // MAINWINDOW_H
