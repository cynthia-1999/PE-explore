#ifndef CERTINFODIALOG_H
#define CERTINFODIALOG_H

#include <QDialog>

namespace Ui {
class CertInfoDialog;
}

class CertInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CertInfoDialog(QWidget *parent = nullptr);
    ~CertInfoDialog();

private:
    Ui::CertInfoDialog *ui;
};

#endif // CERTINFODIALOG_H
