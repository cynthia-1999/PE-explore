#ifndef MODIFYBINARYDIALOG_H
#define MODIFYBINARYDIALOG_H

#include <QDialog>
#include <minwindef.h>
#include <QLineEdit>
#include <QDebug>

namespace Ui {
class ModifyBinaryDialog;
}

class ModifyBinaryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyBinaryDialog(QWidget *parent = nullptr, BYTE *startAddr = nullptr, quint32 fileOffset = 0, int byteNum = 16);
    ~ModifyBinaryDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ModifyBinaryDialog *ui;
    //BYTE buffer[16];
    BYTE *startAddr;
    quint32 fileOffset;
    int byteNum;
    QList<QLineEdit*> *lineEditList;

};

#endif // MODIFYBINARYDIALOG_H
