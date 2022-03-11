#ifndef SECTIONCHARSDIALOG_H
#define SECTIONCHARSDIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class SectionCharsDialog;
}

class SectionCharsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SectionCharsDialog(QWidget *parent = nullptr, quint32 *charsAddr = nullptr, QString sectionName = nullptr);
    ~SectionCharsDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SectionCharsDialog *ui;
    quint32 *charsAddr = 0;
};

#endif // SECTIONCHARSDIALOG_H
