#ifndef PEATTRMODDIALOG_H
#define PEATTRMODDIALOG_H
#include "pefile.h"
#include <QDialog>

namespace Ui {
class PeAttrModDialog;
}

class PeAttrModDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PeAttrModDialog(QWidget *parent = nullptr);
    ~PeAttrModDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PeAttrModDialog *ui;
    DWORD* OEP_Pointer;
    DWORD* ImageBasePointer32;
    ULONGLONG* ImageBasePointer64;
    WORD* DllChars;
};

#endif // PEATTRMODDIALOG_H
