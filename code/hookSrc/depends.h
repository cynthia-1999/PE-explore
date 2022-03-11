#ifndef DEPENDS_H
#define DEPENDS_H

#include <QDialog>
#include<windows.h>
#include<QStringListModel>
#include"hookUtil.h"
#include<QMessageBox>
#include<QHash>

namespace Ui {
class depends;
}

class depends : public QDialog
{
    Q_OBJECT

public:
    explicit depends(DWORD pid, QString name,QWidget *parent = nullptr);
    ~depends();

private slots:

    void on_module_clicked(const QModelIndex &index);

    void on_importFunc_clicked(const QModelIndex &index);

    void on_hook_clicked();

    void on_inlineHook_clicked();

private:
    Ui::depends *ui;
    DWORD pid;
    QString proName;

    QStringListModel * moduleModle;
    QStringListModel * funcModle;

    QHash<QString, QStringList> modAndFunc;
};

#endif // DEPENDS_H
