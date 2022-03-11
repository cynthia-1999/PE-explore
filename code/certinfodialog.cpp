#include "certinfodialog.h"
#include "ui_certinfodialog.h"
#include "pefile.h"

CertInfoDialog::CertInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CertInfoDialog)
{
    ui->setupUi(this);

    Certificate sCert = GlobalStore::getInstanceP()->pefile->getSCert();
    Certificate tCert = GlobalStore::getInstanceP()->pefile->getTCert();
    SYSTEMTIME st = GlobalStore::getInstanceP()->pefile->getDot();

    ui->signerSerialLabel->setText("0x " + QString(QByteArray((const char*)sCert.serialNum,sCert.snData).toHex().data()));
    ui->signerIssuerLabel->setText(QString(QString::fromWCharArray(sCert.issuerName).toUtf8().data()));
    ui->signerSubjectLabel->setText(QString(QString::fromWCharArray(sCert.subjectName).toUtf8().data()));

    ui->timeSerialLabel->setText("0x " + QString(QByteArray((const char*)tCert.serialNum,sCert.snData).toHex().data()));
    ui->timeIssuerLabel->setText(QString(QString::fromWCharArray(tCert.issuerName).toUtf8().data()));
    ui->timeSubjectLabel->setText(QString(QString::fromWCharArray(tCert.subjectName).toUtf8().data()));

    ui->timeStampLabel->setText(QString::number(st.wYear) + "年" +  QString::number(st.wMonth) + "月" + QString::number(st.wDay) + "日" + QString::number(st.wHour) + "时 " + QString::number(st.wMinute) + "分");
}

CertInfoDialog::~CertInfoDialog()
{
    delete ui;
}
