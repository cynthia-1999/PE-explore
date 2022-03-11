#include "pefile.h"
#include<QByteArray>

peFile::peFile()
{

}
peFile::~peFile(){
    clear();
}

void peFile::clear()
{
    delete [] peBuffer;
    delete [] sectionArray;

    peBuffer = Q_NULLPTR;
    sectionArray = Q_NULLPTR;
    IIDArray = Q_NULLPTR;
    IEDArray = Q_NULLPTR;
    dosStub = Q_NULLPTR;
    ntHeader32 = Q_NULLPTR;
    ntHeader64 = Q_NULLPTR;
    sectionHeaderArray = Q_NULLPTR;
    IBRArray = Q_NULLPTR;
    IRD = Q_NULLPTR;
    IRDEArray = Q_NULLPTR;
    ITD32 = Q_NULLPTR;
    ITD64 = Q_NULLPTR;
    modifyFlag = false;
    emit(modifyFlagChanged_signal(modifyFlag));
    peFileSize = 0;
    emit(peFileClear());
}

QString peFile::writeBuffer(QString & path){
    if(modifyFlag){
        QFile file(path);
        file.open(QFile::WriteOnly | QFile::Truncate);
        file.resize(0);
        file.write((char *)peBuffer, peFileSize);
        file.close();
    }else{
        return "对不起，二进制文件尚未被修改!";
    }

    //改回修改标志
    modifyFlag = false;
    emit(modifyFlagChanged_signal(modifyFlag));

    return "";
}
QString peFile::updatePointer(){

    if(peBuffer == Q_NULLPTR){
        return "尚未加载文件!";
    }
    if(peFileSize < 0x3d){
        return "不合法的pe文件，文件过小!";
    }
    //设置dos头
    dosStub = (PIMAGE_DOS_HEADER)peBuffer;
    if(dosStub->e_magic != IMAGE_DOS_SIGNATURE){
        return "不合法的pe文件，e_magic错误!";
    }

    //设置nt头，因为目前还无法区分出是32还是64位，只能这样设置
    ntHeader32 = (PIMAGE_NT_HEADERS32)(peBuffer + dosStub->e_lfanew);
    ntHeader64 = (PIMAGE_NT_HEADERS64)(peBuffer + dosStub->e_lfanew);

    if(ntHeader32->Signature != IMAGE_NT_SIGNATURE){
        return "不合法的pe文件，nt头签名错误!";
    }
    if(ntHeader32->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC){
        pe32Flag = true;
    }else if(ntHeader32->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC){
        pe32Flag = false;
    }else{
        return "不合法的pe文件，可选头magic错误!";
    }

    //对区块表进行赋值
    if(pe32Flag){
        sectionHeaderArray = (PIMAGE_SECTION_HEADER)(peBuffer + dosStub->e_lfanew + 4 + sizeof (IMAGE_FILE_HEADER)
                                                       + ntHeader32->FileHeader.SizeOfOptionalHeader);
    }else{
        sectionHeaderArray = (PIMAGE_SECTION_HEADER)(peBuffer + dosStub->e_lfanew + 4 + sizeof (IMAGE_FILE_HEADER)
                                                       + ntHeader64->FileHeader.SizeOfOptionalHeader);
    }
    //申请空间
    sectionArray = new DWORD[ntHeader32->FileHeader.NumberOfSections];
    //设置节指针
    for(int i = 0; i < ntHeader32->FileHeader.NumberOfSections; i++){
        sectionArray[i] = (DWORD)(peBuffer + sectionHeaderArray[i].PointerToRawData);
    }

    //设置data_directory里的一些相关信息
    if(pe32Flag){
        //设置输入表数组
        if(ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress != 0)
            IIDArray = (PIMAGE_IMPORT_DESCRIPTOR)(peBuffer +
                    Util::getFileOffsetByRVA(
                            this,
                            ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));
        //设置输出表数组
        if(ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress != 0)
            IEDArray = (PIMAGE_EXPORT_DIRECTORY)(peBuffer +
                    Util::getFileOffsetByRVA(
                            this,
                            ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress));


        //设置重定位表
        if(ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress != 0){
            IBRArray = (PIMAGE_BASE_RELOCATION)(peBuffer +
                        Util::getFileOffsetByRVA(
                            this,
                            ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress));
        }

        //设置资源节相关属性
        if(ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress != 0){
            IRD = (PIMAGE_RESOURCE_DIRECTORY)(peBuffer +
                                              Util::getFileOffsetByRVA(
                                                  this,
                                                  ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress));
            IRDEArray = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((BYTE *)IRD + sizeof (IRD));
        }

        //设置TLS
        if(ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress != 0){
            ITD32 = (PIMAGE_TLS_DIRECTORY32)(peBuffer +
                                              Util::getFileOffsetByRVA(
                                                  this,
                                                  ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress));

        }

        //设置debug信息
        if(ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress != 0){
            IDD = (PIMAGE_DEBUG_DIRECTORY)(peBuffer +
                                           Util::getFileOffsetByRVA(
                                               this,
                                               ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress));
        }

        //设置证书表信息
        if(ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress != 0){
                PWC = (LPWIN_CERTIFICATE)(peBuffer +
                                          Util::getFileOffsetByRVA(
                                              this,
                                              ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress));
                //获取证书相关信息
                if(GetSecurityInfo32(pePath)!="")
                {
                    return "Get CertificateInfo Error";
                }
                else{
                    ifsigned = 1;
                }

        }

    }
    else{
        if(ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress != 0)
            IIDArray = (PIMAGE_IMPORT_DESCRIPTOR)(peBuffer +
                    Util::getFileOffsetByRVA(
                            this,
                            ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));

        if(ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress != 0)
            IEDArray = (PIMAGE_EXPORT_DIRECTORY)(peBuffer +
                    Util::getFileOffsetByRVA(
                            this,
                            ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress));

        if(ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress != 0){
            IBRArray = (PIMAGE_BASE_RELOCATION)(peBuffer +
                        Util::getFileOffsetByRVA(
                            this,
                            ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress));
        }

        //设置资源节相关属性
        if(ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress != 0){
            IRD = (PIMAGE_RESOURCE_DIRECTORY)(peBuffer +
                                              Util::getFileOffsetByRVA(
                                                  this,
                                                  ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress));
            IRDEArray = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((BYTE *)IRD + sizeof (IRD));
        }

        //设置TLS
        if(ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress != 0){
            ITD64 = (PIMAGE_TLS_DIRECTORY64)(peBuffer +
                                              Util::getFileOffsetByRVA(
                                                  this,
                                                  ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress));

        }

        //设置debug信息
        if(ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress != 0){
            IDD = (PIMAGE_DEBUG_DIRECTORY)(peBuffer +
                                           Util::getFileOffsetByRVA(
                                               this,
                                               ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress));
        }

        //设置证书表信息
        if(ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress != 0){
            PWC = (LPWIN_CERTIFICATE)(peBuffer +
                                           Util::getFileOffsetByRVA(
                                               this,
                                               ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress));
            //获取证书相关信息
     #if 0
            if(!GetSecurityInfo(path))
        {
            return "Get CertificateInfo Error";
        }
#endif


    }

    }
    emit(updateFile());
    return "";
}

SYSTEMTIME peFile::getDot() const
{
    return dot;
}

QString peFile::getPePath() const
{
    return pePath;
}

Certificate peFile::getTCert() const
{
    return tCert;
}

Certificate peFile::getSCert() const
{
    return sCert;
}

int peFile::getSectionIndexByName(const QString & name){
    int i = 0;
    for(i; i < ntHeader32->FileHeader.NumberOfSections; i++){
        if(name == (char *)sectionHeaderArray[i].Name){
            return i;
        }
    }
    return -1;
}

BOOL peFile::GetProgAndPublisherInfo(PCMSG_SIGNER_INFO pSignerInfo,PSPROG_PUBLISHERINFO Info)
{
    BOOL fReturn = FALSE;
    PSPC_SP_OPUS_INFO OpusInfo = NULL;
    DWORD dwData;
    BOOL fResult;
    {
        // Loop through authenticated attributes and find
        // SPC_SP_OPUS_INFO_OBJID OID.
        for (DWORD n = 0; n < pSignerInfo->AuthAttrs.cAttr; n++)
        {
            if (lstrcmpA(SPC_SP_OPUS_INFO_OBJID,
                pSignerInfo->AuthAttrs.rgAttr[n].pszObjId) == 0)
            {
                // Get Size of SPC_SP_OPUS_INFO structure.
                fResult = CryptDecodeObject(ENCODING,
                    SPC_SP_OPUS_INFO_OBJID,
                    pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
                    pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
                    0,
                    NULL,
                    &dwData);
                if (!fResult)
                {
                    _tprintf(_T("CryptDecodeObject failed with %x\n"),
                        GetLastError());
                    if (OpusInfo != NULL) LocalFree(OpusInfo);
                    return fReturn;
                }
                // Allocate memory for SPC_SP_OPUS_INFO structure.
                OpusInfo = (PSPC_SP_OPUS_INFO)LocalAlloc(LPTR, dwData);
                if (!OpusInfo)
                {
                    _tprintf(_T("Unable to allocate memory for Publisher Info.\n"));
                    if (OpusInfo != NULL) LocalFree(OpusInfo);
                    return fReturn;
                }
                // Decode and get SPC_SP_OPUS_INFO structure.
                fResult = CryptDecodeObject(ENCODING,
                    SPC_SP_OPUS_INFO_OBJID,
                    pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
                    pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
                    0,
                    OpusInfo,
                    &dwData);
                if (!fResult)
                {
                    _tprintf(_T("CryptDecodeObject failed with %x\n"),
                        GetLastError());
                    if (OpusInfo != NULL) LocalFree(OpusInfo);
                    return fReturn;
                }
                // Fill in Program Name if present.
                if (OpusInfo->pwszProgramName)
                {
                   Info->lpszProgramName =
                        AllocateAndCopyWideString(OpusInfo->pwszProgramName);
                }
                else
                    Info->lpszProgramName = NULL;
                // Fill in Publisher Information if present.
                if (OpusInfo->pPublisherInfo)
                {
                    switch (OpusInfo->pPublisherInfo->dwLinkChoice)
                    {
                    case SPC_URL_LINK_CHOICE:
                        Info->lpszPublisherLink =
                            AllocateAndCopyWideString(OpusInfo->pPublisherInfo->pwszUrl);
                        break;
                    case SPC_FILE_LINK_CHOICE:
                        Info->lpszPublisherLink =
                            AllocateAndCopyWideString(OpusInfo->pPublisherInfo->pwszFile);
                        break;
                    default:
                        Info->lpszPublisherLink = NULL;
                        break;
                    }
                }
                else
                {
                    Info->lpszPublisherLink = NULL;
                }
                // Fill in More Info if present.
               if (OpusInfo->pMoreInfo)
                {
                    switch (OpusInfo->pMoreInfo->dwLinkChoice)
                    {
                    case SPC_URL_LINK_CHOICE:
                        Info->lpszMoreInfoLink =
                            AllocateAndCopyWideString(OpusInfo->pMoreInfo->pwszUrl);
                        break;
                    case SPC_FILE_LINK_CHOICE:
                        Info->lpszMoreInfoLink =
                            AllocateAndCopyWideString(OpusInfo->pMoreInfo->pwszFile);
                        break;
                    default:

                        Info->lpszMoreInfoLink = NULL;

                        break;

                    }

                }
                else
                {
                    Info->lpszMoreInfoLink = NULL;
                }
                fReturn = TRUE;
                break; // Break from for loop.
            } // lstrcmp SPC_SP_OPUS_INFO_OBJID
        } // for
    }
    return fReturn;
}

BOOL peFile::GetDateOfTimeStamp(PCMSG_SIGNER_INFO pSignerInfo, SYSTEMTIME* st)
{
    BOOL fResult;
    FILETIME lft, ft;
    DWORD dwData;
    BOOL fReturn = FALSE;
    // Loop through authenticated attributes and find
    // szOID_RSA_signingTime OID.
    for (DWORD n = 0; n < pSignerInfo->AuthAttrs.cAttr; n++)
    {
        if (lstrcmpA(szOID_RSA_signingTime,
            pSignerInfo->AuthAttrs.rgAttr[n].pszObjId) == 0)
        {
            // Decode and get FILETIME structure.
            dwData = sizeof(ft);
            fResult = CryptDecodeObject(ENCODING,
                szOID_RSA_signingTime,
                pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
                pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
                0,
                (PVOID)&ft,
                &dwData);
            if (!fResult)
            {
                _tprintf(_T("CryptDecodeObject failed with %x\n"),
                    GetLastError());
                break;
            }
            // Convert to local time.
            FileTimeToLocalFileTime(&ft, &lft);
            FileTimeToSystemTime(&lft, st);
            fReturn = TRUE;
            break; // Break from for loop.
        } //lstrcmp szOID_RSA_signingTime
    } // for
    return fReturn;
}

BOOL peFile::GetTimeStampSignerInfo(PCMSG_SIGNER_INFO pSignerInfo, PCMSG_SIGNER_INFO* pCounterSignerInfo)
{
    PCCERT_CONTEXT pCertContext = NULL;
    BOOL fReturn = FALSE;
    BOOL fResult;
    DWORD dwSize;
    {
        *pCounterSignerInfo = NULL;
        // Loop through unathenticated attributes for
        // szOID_RSA_counterSign OID.
        for (DWORD n = 0; n < pSignerInfo->UnauthAttrs.cAttr; n++)
        {
            if (lstrcmpA(pSignerInfo->UnauthAttrs.rgAttr[n].pszObjId,
                szOID_RSA_counterSign) == 0)
            {
                // Get size of CMSG_SIGNER_INFO structure.
                fResult = CryptDecodeObject(ENCODING,
                    PKCS7_SIGNER_INFO,
                    pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].pbData,
                    pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].cbData,
                    0,
                    NULL,
                    &dwSize);
                if (!fResult)
                {
                    _tprintf(_T("CryptDecodeObject failed with %x\n"),
                        GetLastError());
                    if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
                }
                // Allocate memory for CMSG_SIGNER_INFO.
                *pCounterSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwSize);
                if (!*pCounterSignerInfo)
                {
                    _tprintf(_T("Unable to allocate memory for timestamp info.\n"));
                    if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
                }
                // Decode and get CMSG_SIGNER_INFO structure
                // for timestamp certificate.
                fResult = CryptDecodeObject(ENCODING,
                    PKCS7_SIGNER_INFO,
                    pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].pbData,
                    pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].cbData,
                    0,
                    (PVOID)*pCounterSignerInfo,
                    &dwSize);
                if (!fResult)
                {
                    _tprintf(_T("CryptDecodeObject failed with %x\n"),
                        GetLastError());
                    if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
                }
                fReturn = TRUE;
                break; // Break from for loop.
            }
        }
    }
    return fReturn;
}

BOOL peFile::GetCertificateInfo(PCCERT_CONTEXT pCertContext,pCertificate cert)
{
    //memset(cert,0,sizeof(Certificate));
    BOOL fReturn = FALSE;
    LPTSTR szName = NULL;
    LPTSTR suName = NULL;
    DWORD dwData;
        // Print Serial Number.
        dwData = pCertContext->pCertInfo->SerialNumber.cbData;
        cert->snData = dwData;
        for (DWORD n = 0; n < dwData; n++)
        {
            //qDebug()<<pCertContext->pCertInfo->SerialNumber.pbData[dwData - (n + 1)];
            cert->serialNum[n]=pCertContext->pCertInfo->SerialNumber.pbData[dwData - (n + 1)];
        }
        //qDebug()<<"Serial Number: "<<"0x"<<QByteArray((const char*)cert->serialNum,cert->snData).toHex().data();
        // Get Issuer name size.
        if (!(dwData = CertGetNameString(pCertContext,
            CERT_NAME_SIMPLE_DISPLAY_TYPE,
            CERT_NAME_ISSUER_FLAG,
            NULL,
            NULL,
            0)))
        {
            //qDebug()<<"CertGetNameString failed.\n";
            if (szName != NULL) LocalFree(szName);
        }
        // Allocate memory for Issuer name.
        szName = (LPTSTR)LocalAlloc(LPTR, dwData * sizeof(TCHAR));
        if (!szName)
        {
            //Debug()<<"Unable to allocate memory for issuer name.\n";;
            if (szName != NULL) LocalFree(szName);
        }
        // Get Issuer name.
        if (!(CertGetNameString(pCertContext,
            CERT_NAME_SIMPLE_DISPLAY_TYPE,
            CERT_NAME_ISSUER_FLAG,
            NULL,
            szName,
            dwData)))
        {
            //qDebug()<<"CertGetNameString failed.\n";
            if (szName != NULL) LocalFree(szName);
        }
        // print Issuer name.
        //qDebug()<<"Issuer Name: "<<QString::fromWCharArray(szName).toUtf8().data();
        cert->issuerName=szName;
       // qDebug()<<"Issuer Name: "<<QString::fromWCharArray(cert->issuerName).toUtf8().data();


        // Get Subject name size.
        if (!(dwData = CertGetNameString(pCertContext,
            CERT_NAME_SIMPLE_DISPLAY_TYPE,
            0,
            NULL,
            NULL,
            0)))
        {
            //qDebug()<<"CertGetNameString failed.\n";
            if (suName != NULL) LocalFree(szName);
        }
        // Allocate memory for subject name.
        suName = (LPTSTR)LocalAlloc(LPTR, dwData * sizeof(TCHAR));
        if (!suName)
        {
            //qDebug()<<"Unable to allocate memory for subject name.\n";
            if (suName != NULL) LocalFree(szName);
        }
        // Get subject name.
        if (!(CertGetNameString(pCertContext,
            CERT_NAME_SIMPLE_DISPLAY_TYPE,
            0,
            NULL,
            suName,
            dwData)))
        {
            //qDebug()<<"CertGetNameString failed.\n";
            if (suName != NULL) LocalFree(szName);
        }
        // Print Subject Name.
        //qDebug()<<"Subject Name:"<< QString::fromWCharArray(szName).toUtf8().data();
        //qDebug()<<"\n";
        cert->subjectName = suName;
        fReturn = TRUE;

    return fReturn;
}

QString peFile::GetSecurityInfo32(QString path)
{
    QString retFlag="";
    //证书相关数据及其函数
    HCERTSTORE hStore = Q_NULLPTR;
    HCRYPTMSG hMsg = Q_NULLPTR;
    PCCERT_CONTEXT pCertContext = Q_NULLPTR;
    BOOL fResult;
    DWORD dwEncoding, dwContentType, dwFormatType;
    PCMSG_SIGNER_INFO pSignerInfo = NULL;
    PCMSG_SIGNER_INFO pCounterSignerInfo = NULL;
    DWORD dwSignerInfo;
    CERT_INFO CertInfo;
    SPROG_PUBLISHERINFO ProgPubInfo;
    SYSTEMTIME st;
    ZeroMemory(&ProgPubInfo, sizeof(ProgPubInfo));
    fResult = CryptQueryObject(CERT_QUERY_OBJECT_FILE,
                QString2wchar(&path),
                CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
                CERT_QUERY_FORMAT_FLAG_BINARY,
                0,
                &dwEncoding,
                &dwContentType,
                &dwFormatType,
                &hStore,
                &hMsg,
                NULL);
    if(!fResult)
    {
         qDebug() << "CryptQueryObject failed with " << GetLastError();
         //清除
         if (ProgPubInfo.lpszProgramName != NULL) LocalFree(ProgPubInfo.lpszProgramName);
         if (ProgPubInfo.lpszPublisherLink != NULL) LocalFree(ProgPubInfo.lpszPublisherLink);
         if (ProgPubInfo.lpszMoreInfoLink != NULL) LocalFree(ProgPubInfo.lpszMoreInfoLink);
         if (pSignerInfo != NULL) LocalFree(pSignerInfo);
         if (pCounterSignerInfo != NULL) LocalFree(pCounterSignerInfo);
         if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
         if (hStore != NULL) CertCloseStore(hStore, 0);
         if (hMsg != NULL) CryptMsgClose(hMsg);
         retFlag = "Get Certificate Error";
         return retFlag;
    }
    fResult = CryptMsgGetParam(hMsg,
                CMSG_SIGNER_INFO_PARAM,
                0,
                NULL,
                &dwSignerInfo);
    if(!fResult)
    {
         qDebug() <<"CryptMsgGetParam failed with "<< GetLastError();
         if (ProgPubInfo.lpszProgramName != NULL) LocalFree(ProgPubInfo.lpszProgramName);
         if (ProgPubInfo.lpszPublisherLink != NULL) LocalFree(ProgPubInfo.lpszPublisherLink);
         if (ProgPubInfo.lpszMoreInfoLink != NULL) LocalFree(ProgPubInfo.lpszMoreInfoLink);
         if (pSignerInfo != NULL) LocalFree(pSignerInfo);
         if (pCounterSignerInfo != NULL) LocalFree(pCounterSignerInfo);
         if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
         if (hStore != NULL) CertCloseStore(hStore, 0);
         if (hMsg != NULL) CryptMsgClose(hMsg);
         retFlag = "Get Certificate Error";
         return retFlag;
    }
    pSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwSignerInfo);
    if (!pSignerInfo)
            {
                qDebug() <<"Unable to allocate memory for Signer Info.\n";
                if (ProgPubInfo.lpszProgramName != NULL) LocalFree(ProgPubInfo.lpszProgramName);
                if (ProgPubInfo.lpszPublisherLink != NULL) LocalFree(ProgPubInfo.lpszPublisherLink);
                if (ProgPubInfo.lpszMoreInfoLink != NULL) LocalFree(ProgPubInfo.lpszMoreInfoLink);
                if (pSignerInfo != NULL) LocalFree(pSignerInfo);
                if (pCounterSignerInfo != NULL) LocalFree(pCounterSignerInfo);
                if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
                if (hStore != NULL) CertCloseStore(hStore, 0);
                if (hMsg != NULL) CryptMsgClose(hMsg);
                retFlag = "Get Certificate Error";
                return retFlag;
            }

    fResult = CryptMsgGetParam(hMsg,
                CMSG_SIGNER_INFO_PARAM,
                0,
                (PVOID)pSignerInfo,
                &dwSignerInfo);
            if (!fResult)
            {
               qDebug() <<"CryptMsgGetParam failed with "<<GetLastError();
               if (ProgPubInfo.lpszProgramName != NULL) LocalFree(ProgPubInfo.lpszProgramName);
               if (ProgPubInfo.lpszPublisherLink != NULL) LocalFree(ProgPubInfo.lpszPublisherLink);
               if (ProgPubInfo.lpszMoreInfoLink != NULL) LocalFree(ProgPubInfo.lpszMoreInfoLink);
               if (pSignerInfo != NULL) LocalFree(pSignerInfo);
               if (pCounterSignerInfo != NULL) LocalFree(pCounterSignerInfo);
               if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
               if (hStore != NULL) CertCloseStore(hStore, 0);
               if (hMsg != NULL) CryptMsgClose(hMsg);
               retFlag = "Get Certificate Error";
               return retFlag;
            }
     fResult = GetProgAndPublisherInfo(pSignerInfo, &ProgPubInfo);
     if (!fResult)
     {
         qDebug() <<"ProgAndPublisherInfo failed with" <<GetLastError();
         if (ProgPubInfo.lpszProgramName != NULL) LocalFree(ProgPubInfo.lpszProgramName);
         if (ProgPubInfo.lpszPublisherLink != NULL) LocalFree(ProgPubInfo.lpszPublisherLink);
         if (ProgPubInfo.lpszMoreInfoLink != NULL) LocalFree(ProgPubInfo.lpszMoreInfoLink);
         if (pSignerInfo != NULL) LocalFree(pSignerInfo);
         if (pCounterSignerInfo != NULL) LocalFree(pCounterSignerInfo);
         if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
         if (hStore != NULL) CertCloseStore(hStore, 0);
         if (hMsg != NULL) CryptMsgClose(hMsg);
         retFlag = "Get Certificate Error";
         return retFlag;
     }
     CertInfo.Issuer = pSignerInfo->Issuer;
             CertInfo.SerialNumber = pSignerInfo->SerialNumber;
             pCertContext = CertFindCertificateInStore(hStore,
                 ENCODING,
                 0,
                 CERT_FIND_SUBJECT_CERT,
                 (PVOID)&CertInfo,
                 NULL);
             if (!pCertContext)
             {
                 qDebug() <<"CertFindCertificateInStore failed with"<< GetLastError();
                 if (ProgPubInfo.lpszProgramName != NULL) LocalFree(ProgPubInfo.lpszProgramName);
                 if (ProgPubInfo.lpszPublisherLink != NULL) LocalFree(ProgPubInfo.lpszPublisherLink);
                 if (ProgPubInfo.lpszMoreInfoLink != NULL) LocalFree(ProgPubInfo.lpszMoreInfoLink);
                 if (pSignerInfo != NULL) LocalFree(pSignerInfo);
                 if (pCounterSignerInfo != NULL) LocalFree(pCounterSignerInfo);
                 if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
                 if (hStore != NULL) CertCloseStore(hStore, 0);
                 if (hMsg != NULL) CryptMsgClose(hMsg);
                 retFlag = "Get Certificate Error";
                 return retFlag;
             }
             // Print Signer certificate information.

            //qDebug()<<"Signer Certificate:";
            GetCertificateInfo(pCertContext,&sCert);
            _tprintf(_T("\n"));
            // Get the timestamp certificate signerinfo structure.
            if (GetTimeStampSignerInfo(pSignerInfo, &pCounterSignerInfo))
                    {
                        // Search for Timestamp certificate in the temporary
                        // certificate store.
                        CertInfo.Issuer = pCounterSignerInfo->Issuer;
                        CertInfo.SerialNumber = pCounterSignerInfo->SerialNumber;
                        pCertContext = CertFindCertificateInStore(hStore,
                            ENCODING,
                            0,
                            CERT_FIND_SUBJECT_CERT,
                            (PVOID)&CertInfo,
                            NULL);
                        if (!pCertContext)
                        {
                            qDebug() <<"CertFindCertificateInStore failed with"<<GetLastError();
                            if (ProgPubInfo.lpszProgramName != NULL) LocalFree(ProgPubInfo.lpszProgramName);
                            if (ProgPubInfo.lpszPublisherLink != NULL) LocalFree(ProgPubInfo.lpszPublisherLink);
                            if (ProgPubInfo.lpszMoreInfoLink != NULL) LocalFree(ProgPubInfo.lpszMoreInfoLink);
                            if (pSignerInfo != NULL) LocalFree(pSignerInfo);
                            if (pCounterSignerInfo != NULL) LocalFree(pCounterSignerInfo);
                            if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
                            if (hStore != NULL) CertCloseStore(hStore, 0);
                            if (hMsg != NULL) CryptMsgClose(hMsg);
                            retFlag = "Get Certificate Error";
                            return retFlag;
                        }
                        // Print timestamp certificate information.
                        //qDebug()<<"TimeStamp Certificate:";
                        GetCertificateInfo(pCertContext,&tCert);
                        GetDateOfTimeStamp(pCounterSignerInfo, &st);
                        dot = st;
                        //qDebug()<<"Date of TimeStamp : "<<st.wMonth<<"/"<< st.wDay<<"/" << st.wYear<< "/" << st.wHour<<":"<<st.wMinute<<endl;

            }
            return retFlag;
}

QString peFile::VerifyEmbeddedSignature(QString pePath)
{
    int lStatus;
    DWORD dwLastError;

    // Initialize the WINTRUST_FILE_INFO structure.

    WINTRUST_FILE_INFO FileData;
//    LPCWSTR pwszSourceFile=LPCWSTR(&pePath);
    // modified by whl
    // 将正斜杠换成Windows的反斜杠
    pePath.replace("/", "\\");

    LPCWSTR pwszSourceFile= pePath.toStdWString().c_str();
    //qDebug() << "pwszSourceFile" << *pwszSourceFile;
    memset(&FileData, 0, sizeof(FileData));
    FileData.cbStruct = sizeof(WINTRUST_FILE_INFO);
    FileData.pcwszFilePath = pwszSourceFile;
    FileData.hFile = NULL;
    FileData.pgKnownSubject = NULL;

    /*
    WVTPolicyGUID specifies the policy to apply on the file
    WINTRUST_ACTION_GENERIC_VERIFY_V2 policy checks:

    1) The certificate used to sign the file chains up to a root
    certificate located in the trusted root certificate store. This
    implies that the identity of the publisher has been verified by
    a certification authority.

    2) In cases where user interface is displayed (which this example
    does not do), WinVerifyTrust will check for whether the
    end entity certificate is stored in the trusted publisher store,
    implying that the user trusts content from this publisher.

    3) The end entity certificate has sufficient permission to sign
    code, as indicated by the presence of a code signing EKU or no
    EKU.
    */

    GUID WVTPolicyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
    WINTRUST_DATA WinTrustData;

    // Initialize the WinVerifyTrust input data structure.

    // Default all fields to 0.
    memset(&WinTrustData, 0, sizeof(WinTrustData));

    WinTrustData.cbStruct = sizeof(WinTrustData);

    // Use default code signing EKU.
    WinTrustData.pPolicyCallbackData = NULL;

    // No data to pass to SIP.
    WinTrustData.pSIPClientData = NULL;

    // Disable WVT UI.
    WinTrustData.dwUIChoice = WTD_UI_NONE;

    // No revocation checking.
    WinTrustData.fdwRevocationChecks = WTD_REVOKE_NONE;

    // Verify an embedded signature on a file.
    WinTrustData.dwUnionChoice = WTD_CHOICE_FILE;

    // Default verification.
    WinTrustData.dwStateAction = 0;

    // Not applicable for default verification of embedded signature.
    WinTrustData.hWVTStateData = NULL;

    // Not used.
    WinTrustData.pwszURLReference = NULL;

    // Default.
    WinTrustData.dwProvFlags = WTD_SAFER_FLAG;

    // This is not applicable if there is no UI because it changes
    // the UI to accommodate running applications instead of
    // installing applications.
    WinTrustData.dwUIContext = 0;

    // Set pFile.
    WinTrustData.pFile = &FileData;

    // WinVerifyTrust verifies signatures as specified by the GUID
    // and Wintrust_Data.
    lStatus = WinVerifyTrust(
        NULL,
        &WVTPolicyGUID,
        &WinTrustData);

    switch (lStatus)
    {
    case ERROR_SUCCESS:
        /*
        Signed file:
        - Hash that represents the subject is trusted.

        - Trusted publisher without any verification errors.

        - UI was disabled in dwUIChoice. No publisher or
        time stamp chain errors.

        - UI was enabled in dwUIChoice and the user clicked
        "Yes" when asked to install and run the signed
        subject.
        */
//        wprintf_s(L"The file \"%s\" is signed and the signature "
//            L"was verified.\n",
//            pwszSourceFile);
        return QString("该文件已经签名，且签名成功通过验证。");
        break;

    case TRUST_E_NOSIGNATURE:
        // The file was not signed or had a signature
        // that was not valid.

        // Get the reason for no signature.
        dwLastError = GetLastError();
        if (TRUST_E_NOSIGNATURE == dwLastError ||
            TRUST_E_SUBJECT_FORM_UNKNOWN == dwLastError ||
            TRUST_E_PROVIDER_UNKNOWN == dwLastError)
        {
            // The file was not signed.
            return QString("文件未被签名。");
        }
        else
        {
            // The signature was not valid or there was an error
            // opening the file.
//            wprintf_s(L"An unknown error occurred trying to "
//                L"verify the signature of the \"%s\" file.\n",
//                pwszSourceFile);
            return QString("在验证签名过程中出现未知错误。");
        }

        break;

    case TRUST_E_EXPLICIT_DISTRUST:
        // The hash that represents the subject or the publisher
        // is not allowed by the admin or user.
//        wprintf_s(L"The signature is present, but specifically "
//            L"disallowed.\n");
        return QString("签名存在，但是被显式规定不允许使用。");
        break;

    case TRUST_E_SUBJECT_NOT_TRUSTED:
        // The user clicked "No" when asked to install and run.
//        wprintf_s(L"The signature is present, but not "
//            L"trusted.\n");
        return QString("签名存在，但是不被信任。");
        break;

    case CRYPT_E_SECURITY_SETTINGS:
        /*
        The hash that represents the subject or the publisher
        was not explicitly trusted by the admin and the
        admin policy has disabled user trust. No signature,
        publisher or time stamp errors.
        */
//        wprintf_s(L"CRYPT_E_SECURITY_SETTINGS - The hash "
//            L"representing the subject or the publisher wasn't "
//            L"explicitly trusted by the admin and admin policy "
//            L"has disabled user trust. No signature, publisher "
//            L"or timestamp errors.\n");
        return QString("RYPT_E_SECURITY_SETTINGS - The hash representing the subject or the publisher wasn't "
                       "explicitly trusted by the admin and admin policy "
                       "has disabled user trust. No signature, publisher "
                       "or timestamp errors.");
        break;

    default:
        // The UI was disabled in dwUIChoice or the admin policy
        // has disabled user trust. lStatus contains the
        // publisher or time stamp chain error.
//        wprintf_s(L"Error is: 0x%x.\n",
//            lStatus);
        return QString("Error is 0x") + QString::number(lStatus, 16);
        break;
    }
    // Any hWVTStateData must be released by a call with close.
        WinTrustData.dwStateAction = WTD_STATEACTION_CLOSE;

        lStatus = WinVerifyTrust(
            NULL,
            &WVTPolicyGUID,
            &WinTrustData);

    return "Error at last.";
}
