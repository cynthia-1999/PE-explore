#ifndef MONITOR_H
#define MONITOR_H

#include <QDialog>
#include "resource.h"
#include "cpu.h"
#include "memory.h"
namespace Ui {
class Monitor;
}

class Monitor : public QDialog
{
    Q_OBJECT

public:
    explicit Monitor(QWidget *parent = nullptr);
    ~Monitor();

private:
    Ui::Monitor *ui;
};

#endif // MONITOR_H
