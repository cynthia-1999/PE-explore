#ifndef PEFILE_H
#define PEFILE_H
#include"headers.h"
#include"util.h"
#include"globalstore.h"

#define ENCODING (X509_ASN_ENCODING | PKCS_7_ASN_ENCODING)

typedef struct {
    LPWSTR lpszProgramName;
    LPWSTR lpszPublisherLink;
    LPWSTR lpszMoreInfoLink;
} SPROG_PUBLISHERINFO, *PSPROG_PUBLISHERINFO;
typedef struct{
    BYTE serialNum[64];
    DWORD snData;      //存储serialNum实际的字节数
    LPTSTR issuerName;
    LPTSTR subjectName;
}Certificate,*pCertificate;

//这个类就是解析函数后的运行结果
class peFile : public QObject
{
    Q_OBJECT
public:
    peFile();
    virtual ~peFile();

    const BYTE * c_getPeBuffer(){
        return peBuffer;
    }
    BYTE * getPeBuffer(){
        return peBuffer;
    }
    qint64 getPeFileSize(){
        return peFileSize;
    }

    //获取是否存在安全节
    BOOL getIfsigned()
    {
        return ifsigned;
    }


    //根据文件路径读取pe文件信息,返回""无错误，否则返回错误信息
    QString setPeBuffer(const QString & path){
        QFile file(path);
        if(!file.exists()){
            return "对不起，文件不存在!";
        }
        if(!file.open(QFile::ReadOnly)){
            return "对不起，打开文件失败!";
        }

        //清空当前对象
        clear();

        //设置文件大小
        peFileSize = file.size();

        //设置文件路径
        pePath = path;

        peBuffer = new BYTE[peFileSize];
        memset(peBuffer, 0, peFileSize);
        memcpy(peBuffer, file.readAll().data(), peFileSize);

        file.close();

        //修改文件的标志改回
        modifyFlag = false;
        emit(modifyFlagChanged_signal(modifyFlag));

        return updatePointer();
    }

    //根据文件路径写回pe文件，返回""无错误，否则返回错误信息
    QString writeBuffer(QString & path);

    //用于清空peFile对象，准备重新加载一个pe文件或者退出程序
    void clear();

    //获取标志
    bool getPe32Flag(){
        return pe32Flag;
    }
    void setMidifyFlag(bool flag){
        this->modifyFlag = flag;
    }
    PIMAGE_EXPORT_DIRECTORY getIEDArray(){
        return IEDArray;
    }

    PIMAGE_NT_HEADERS32 getNtHeader32(){
        return ntHeader32;
    }

    PIMAGE_NT_HEADERS64 getNtHeader64(){
        return ntHeader64;
    }

    PIMAGE_SECTION_HEADER getSectionHeaderArray(){
        return sectionHeaderArray;
    }

    DWORD * getSectionArray(){
        return sectionArray;
    }

    //根据节名称获取节在节表中的索引
    int getSectionIndexByName(const QString &);

    bool getModifyFlag(){
        return modifyFlag;
    }

    PIMAGE_DOS_HEADER getImage_Dos_Header(){
        return dosStub;
    }

    PIMAGE_IMPORT_DESCRIPTOR getIIDArray(){
        return IIDArray;
    }

    PIMAGE_BASE_RELOCATION getIBR(){
        return IBRArray;
    }
    PIMAGE_RESOURCE_DIRECTORY getIRD(){
        return IRD;
    }
    Certificate getSCert() const;

    Certificate getTCert() const;

    QString getPePath() const;

    // this func needs to be public.  --by whl
    QString VerifyEmbeddedSignature(QString pePath);

    SYSTEMTIME getDot() const;

private:


    //读取的文件内容，pe文件读取后，所有的信息都存储在该数组中
    BYTE* peBuffer = Q_NULLPTR;

    qint64 peFileSize = 0;

    //标志，标识程序是否对二进制文件进行修改，每次该标志变更时需要发出信号modifyFlagChanged_signal
    bool modifyFlag = false;

    //标志，标识加载的程序是否为32位程序
    bool pe32Flag = true;

    //修正指针（解析文件的关键函数）
    QString updatePointer();

    PIMAGE_DOS_HEADER dosStub = Q_NULLPTR;
    PIMAGE_NT_HEADERS32 ntHeader32 = Q_NULLPTR;
    PIMAGE_NT_HEADERS64 ntHeader64 = Q_NULLPTR;

    //指向区块表数组，数目由NT头的FileHeader的NumberOfSections指出
    //不单独存储这个数是为了减少保持数据一致性的代码量。
    PIMAGE_SECTION_HEADER sectionHeaderArray = Q_NULLPTR;
    //节数据，和secitonHeader顺序是一致的.
    DWORD * sectionArray = Q_NULLPTR;
    //导入表输入
    PIMAGE_IMPORT_DESCRIPTOR IIDArray = Q_NULLPTR;
    //导出表数组
    PIMAGE_EXPORT_DIRECTORY IEDArray = Q_NULLPTR;
    //重定位表数组
    PIMAGE_BASE_RELOCATION IBRArray = Q_NULLPTR;

    //资源节相关信息
    PIMAGE_RESOURCE_DIRECTORY IRD = Q_NULLPTR;
    PIMAGE_RESOURCE_DIRECTORY_ENTRY IRDEArray = Q_NULLPTR;

    //TLS
    PIMAGE_TLS_DIRECTORY32 ITD32 = Q_NULLPTR;
    PIMAGE_TLS_DIRECTORY64 ITD64 = Q_NULLPTR;

    PIMAGE_DEBUG_DIRECTORY IDD = Q_NULLPTR;

    //证书表
    LPWIN_CERTIFICATE PWC = Q_NULLPTR;
    //检查是否存在安全节,默认为0
    BOOL ifsigned = 0;

    //文件路径
    QString pePath;

    //证书前端接口
    Certificate sCert;
    Certificate tCert;
    SYSTEMTIME dot;

    wchar_t*  QString2wchar(QString *qstring)
    {
        wchar_t* m_wchar;
        int len = qstring->length();
        m_wchar = new wchar_t[len+1];
        qstring->toWCharArray(m_wchar);
        m_wchar[len] = '\0';
        return m_wchar;
    }

    LPWSTR AllocateAndCopyWideString(LPCWSTR inputString)

    {

        LPWSTR outputString = NULL;

        outputString = (LPWSTR)LocalAlloc(LPTR,

            (wcslen(inputString) + 1) * sizeof(WCHAR));

        if (outputString != NULL)

        {

            lstrcpyW(outputString, inputString);

        }

        return outputString;

    }

    //32位PE证书相关函数
    BOOL GetProgAndPublisherInfo(PCMSG_SIGNER_INFO pSignerInfo,PSPROG_PUBLISHERINFO Info);
    BOOL GetDateOfTimeStamp(PCMSG_SIGNER_INFO pSignerInfo, SYSTEMTIME* st);
    BOOL GetTimeStampSignerInfo(PCMSG_SIGNER_INFO pSignerInfo,PCMSG_SIGNER_INFO* pCounterSignerInfo);
    BOOL GetCertificateInfo(PCCERT_CONTEXT pCertContext,pCertificate cert);
    QString GetSecurityInfo32(QString path);

signals:
    //该信号表明程序对加载的pe文件内容进行了修改或者保存，flag为false表示已保存或重新加载
    //为true表明有程序对pe文件的内容进行了保存
    void modifyFlagChanged_signal(bool modifyFlag);

    //表明程序根据peBuffer升级了相应字段，可以将此信号绑定在UI的更新函数上
    void updateFile();

    //表明当前对象存储的pe文件已被清空
    void peFileClear();
};

#endif // PEFILE_H
