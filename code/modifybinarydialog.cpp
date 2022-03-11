#include "globalstore.h"
#include "modifybinarydialog.h"
#include "ui_modifybinarydialog.h"
#include "pefile.h"

#include <QRegExpValidator>



ModifyBinaryDialog::ModifyBinaryDialog(QWidget *parent, BYTE *startAddr, quint32 fileOffset, int byteNum) :
    QDialog(parent),
    ui(new Ui::ModifyBinaryDialog)
{
    ui->setupUi(this);
    // 设置 property
    this->startAddr = startAddr;
    this->fileOffset = fileOffset;
    this->byteNum = byteNum;
    // set Label
    // TODO 区分32位与64位
    QString labelText;
    if (GlobalStore::getInstanceP()->pefile->getPe32Flag())
    {
        labelText = "你正在修改 " + QString::asprintf("%08X", fileOffset) + " 偏移处的数据。";
    }
    else
    {
        labelText = "你正在修改 " + QString::asprintf("%016X", fileOffset) + " 偏移处的数据。";
    }
    ui->label->setText(labelText);
    // 设定 byteNum 个输入框
    QList<QLineEdit*> *lineEditList = new QList<QLineEdit*>();
    this->lineEditList = lineEditList;
    // 正则表达式
    QRegExp rx("([0-9A-Fa-f]{1,2})");
    QValidator *validator = new QRegExpValidator(rx, this);

    for (int i = 0; i < byteNum; i++) {
        BYTE tmpChar = *(startAddr + i);
        lineEditList->append(new QLineEdit(this));
        lineEditList->last()->setText(QString::asprintf("%02X", tmpChar));
        lineEditList->last()->setValidator(validator);
        ui->horizontalLayout->addWidget(lineEditList->last());
    }
}


ModifyBinaryDialog::~ModifyBinaryDialog()
{
    delete ui;
}

/* 当用户在对话框中点击保存时执行此函数，完成内容的保存
 */
void ModifyBinaryDialog::on_buttonBox_accepted()
{
    // qDebug() << "Save is accepted.";
    // 逐个保存每个字节的内容
    for(int i=0; i < this->byteNum; i++)
    {
        this->startAddr[i] = BYTE(this->lineEditList->at(i)->text().toUShort(nullptr,16));
        //qDebug() << this->lineEditList->at(i)->text().toUShort(nullptr,16);
    }
}
