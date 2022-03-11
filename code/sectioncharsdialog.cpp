#include "sectioncharsdialog.h"
#include "ui_sectioncharsdialog.h"

// 用于设置section的属性（包括权限）的对话框
SectionCharsDialog::SectionCharsDialog(QWidget *parent, quint32 *charsAddr, QString sectionName) :
    QDialog(parent),
    ui(new Ui::SectionCharsDialog)
{
    ui->setupUi(this);
    this->charsAddr = charsAddr;
    QString sectionNameText = "你正在修改 " + sectionName + " 节的属性。";
    ui->sectionNameLabel->setText(sectionNameText);
    // 获取原来的标志位
    quint32 originalChars = *charsAddr;
    QFormLayout *formLayout = ui->formLayout;
    //int count = ui->formLayout->count();
    //qDebug() << "formLayout " << count;  共有52个控件，26对（label+CheckBox）
    for(int i=0;i<26;i++)
    {

        QCheckBox* checkBox = (QCheckBox*)(formLayout->itemAt(i,QFormLayout::FieldRole)->widget());
        QLabel* label = (QLabel*)(formLayout->itemAt(i,QFormLayout::LabelRole)->widget());
        QString labelContent = label->text();
        QString labelValMask = labelContent.mid(2,8);  // label的第2到10个字节正好是value的16进制表示
        quint32 tmpMask = labelValMask.toUInt(nullptr, 16);
        if ((originalChars & tmpMask) != 0)
        {
            // 原来的节属性即包括该属性
            checkBox->setChecked(true);
            QFont qfont("SimSun", 9, 75);
            label->setFont(qfont);
        }
        // qDebug() << tmpMask;
    }

}

SectionCharsDialog::~SectionCharsDialog()
{
    delete ui;
}

void SectionCharsDialog::on_buttonBox_accepted()
{
    // 用户点击存储
    quint32 *charsAddr = this->charsAddr;
    for(int i=0;i<26;i++)
    {

        QCheckBox* checkBox = (QCheckBox*)(this->ui->formLayout->itemAt(i,QFormLayout::FieldRole)->widget());
        QLabel* label = (QLabel*)(this->ui->formLayout->itemAt(i,QFormLayout::LabelRole)->widget());
        QString labelContent = label->text();
        QString labelValMask = labelContent.mid(2,8);  // label的第2到10个字节正好是value的16进制表示
        quint32 tmpMask = labelValMask.toUInt(nullptr, 16);
        if (!checkBox->isChecked())
        {
            //qDebug() << "the " << labelContent << "is not Checked.";
            //qDebug() << "chars: " << *charsAddr << "  tmpMask: " << tmpMask;
            // 这里使用一种比较特殊的方法
            // 并非是将用户打勾的字段 OR 后赋值给 charsAddr
            // 而是将用户未打钩的字段 赋值0 然后 AND 给原来的 chars
            // 这样可以有效避免一些我们这里暂时没有列上去的字段在使用本功能后被莫名取消
            *charsAddr &= ~tmpMask;
            //qDebug() << ~tmpMask;
            //qDebug() << "chars: " << *charsAddr;
        }
        else
        {
            *charsAddr |= tmpMask;
        }
        // qDebug() << tmpMask;
    }

}
