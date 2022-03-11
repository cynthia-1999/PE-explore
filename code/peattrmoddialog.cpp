#include "peattrmoddialog.h"

#include "ui_peattrmoddialog.h"

PeAttrModDialog::PeAttrModDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PeAttrModDialog)
{
    ui->setupUi(this);
    peFile *pefile = GlobalStore::getInstanceP()->pefile;
    PIMAGE_NT_HEADERS32 ntHeader32 = pefile->getNtHeader32();
    PIMAGE_NT_HEADERS64 ntHeader64 = pefile->getNtHeader64();

    // 正则表达式
    QRegExp rx32("([0-9A-Fa-f]{7,8})");
    QValidator *validator32 = new QRegExpValidator(rx32, this);
    QRegExp rx64("([0-9A-Fa-f]{15,16})");
    QValidator *validator64 = new QRegExpValidator(rx64, this);

    ui->oEPLineEdit->setValidator(validator32);

    if (pefile->getPe32Flag())
    {
        // 32 位程序
        OEP_Pointer = &(ntHeader32->OptionalHeader.AddressOfEntryPoint);
        ImageBasePointer32 = &(ntHeader32->OptionalHeader.ImageBase);
        DllChars = &(ntHeader32->OptionalHeader.DllCharacteristics);

        ui->oEPLineEdit->setText(QString("%1").arg(*OEP_Pointer, 8, 16, QLatin1Char('0')));
        ui->imageBaseLineEdit->setText(QString("%1").arg(*ImageBasePointer32, 8, 16, QLatin1Char('0')));

        // reg check
        ui->imageBaseLineEdit->setValidator(validator32);
    }
    else
    {
        // 64 位程序
        OEP_Pointer = &(ntHeader64->OptionalHeader.AddressOfEntryPoint);
        ImageBasePointer64 = &(ntHeader64->OptionalHeader.ImageBase);
        DllChars = &(ntHeader64->OptionalHeader.DllCharacteristics);

        // 对于 64 位程序， ImageBase 是 64 位，但是OEP还是32位
        ui->oEPLineEdit->setText(QString("%1").arg(*OEP_Pointer, 8, 16, QLatin1Char('0')));
        ui->imageBaseLineEdit->setText(QString("%1").arg(*ImageBasePointer64, 16, 16, QLatin1Char('0')));

        // reg check
        ui->imageBaseLineEdit->setValidator(validator64);
    }

    // 由于 32 位 与 64 位的DLLCharacteristics长度都是WORD，所以可以写到一起。这里就是对于部分位的判断
    if((*DllChars & 0x0040) != 0)
    {
        ui->aSLRCheckBox->setChecked(true);
    }
    if((*DllChars & 0x0100) != 0)
    {
        ui->dEPCheckBox->setChecked(true);
    }
    if((*DllChars & 0x4000) != 0)
    {
        ui->controlFlowGuardCheckBox->setChecked(true);
    }
}

PeAttrModDialog::~PeAttrModDialog()
{
    delete ui;
}

void PeAttrModDialog::on_buttonBox_accepted()
{
    // 当用户点击同意更改后，写入修改
    *OEP_Pointer = ui->oEPLineEdit->text().toUInt(nullptr, 16);
    if (GlobalStore::getInstanceP()->pefile->getPe32Flag())
    {
        *ImageBasePointer32 = ui->imageBaseLineEdit->text().toUInt(nullptr, 16);
    }
    else
    {
        *ImageBasePointer64 = ui->imageBaseLineEdit->text().toULongLong(nullptr, 16);
    }
    // 对每个单选框进行判断 ASLR
    if(ui->aSLRCheckBox->isChecked())
    {
        *DllChars |= 0x0040;
    }
    else
    {
        *DllChars &= ~0x0040;
    }
    // DEP
    if(ui->dEPCheckBox->isChecked())
    {
        *DllChars |= 0x0100;
    }
    else
    {
        *DllChars &= ~0x0100;
    }
    // CFG
    if(ui->controlFlowGuardCheckBox->isChecked())
    {
        *DllChars |= 0x4000;
    }
    else
    {
        *DllChars &= ~0x4000;
    }

}
