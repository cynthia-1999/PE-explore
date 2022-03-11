#include"util.h"
#include"pefile.h"
DWORD Util::getFileOffsetByRVA(peFile *pe, DWORD RVA){
    if(RVA == 0){
        return 0;
    }
    PIMAGE_SECTION_HEADER headerArray = pe->getSectionHeaderArray();
    DWORD sectionRVA = 0;
    DWORD Delta = 0;
    int sectionNum = pe->getNtHeader32()->FileHeader.NumberOfSections;
    for(int i = 0; i < sectionNum - 1; i++){
        if(headerArray[i].VirtualAddress <= RVA && headerArray[i + 1].VirtualAddress > RVA){
            sectionRVA = headerArray[i].VirtualAddress;
            Delta = sectionRVA - headerArray[i].PointerToRawData;
            break;
        }
    }
    if(!sectionRVA){
        sectionRVA = headerArray[sectionNum - 1].VirtualAddress;
        Delta = sectionRVA - headerArray[sectionNum - 1].PointerToRawData;
    }
    return (RVA - Delta);
}


QList<opClass32>* Util::Disassembly32(BYTE opCode[], long size,unsigned int VirtualAddr = 0x41000){
    QList<opClass32>* result = new QList<opClass32>();
    DISASM disAsm = { 0 };
    //首地址
    disAsm.EIP = (unsigned long long)opCode;
    disAsm.VirtualAddr = VirtualAddr;
    disAsm.Archi = 0;
    disAsm.Options = 0x000; // masm 汇编指令格式
    int nOpcodeSize = size;
    int nCount = 0;// 用于记录在循环当中，反汇编了多少个字节
    int nLen = 0 ; // 用于记录当前的汇编指令的字节数

    while(nCount < nOpcodeSize)
    {
        opClass32 tmp;
        nLen = Disasm(&disAsm); // 每次只反汇编一条汇编指令， 并且返回当前得到的汇编指令的长度
        unsigned int uAddr = disAsm.VirtualAddr;

        tmp.Addr = uAddr;
        char * eipTmp = (char *)disAsm.EIP;
        for(int i = 0; i < nLen; i++){
            tmp.opCode += QString::asprintf("%2x ", (eipTmp[i] & 0xff));
        }
        tmp.opInstr = QString::asprintf("%s", disAsm.CompleteInstr);
        if(nLen <= 0){
            break;
        }
        nCount += nLen; // 累加已经反汇编的字节数
        disAsm.EIP += nLen; // 定位到下一条汇编指令
        disAsm.VirtualAddr += nLen; // 设置到下一条汇编指令的地址
        result->append(tmp);
    }

    return result;
}


QList<opClass64>* Util::Disassembly64(BYTE opCode[], long size,unsigned long long VirtualAddr = 0x41000){
    QList<opClass64>* result = new QList<opClass64>();
    DISASM disAsm = { 0 };
    //首地址
    disAsm.EIP = (unsigned long long)opCode;
    disAsm.VirtualAddr = VirtualAddr;
    disAsm.Archi = 0x40;
    disAsm.Options = 0; // masm 汇编指令格式
    int nOpcodeSize = size;
    int nCount = 0;// 用于记录在循环当中，反汇编了多少个字节
    int nLen = 0 ; // 用于记录当前的汇编指令的字节数

    while(nCount < nOpcodeSize)
    {
        opClass64 tmp;
        nLen = Disasm(&disAsm); // 每次只反汇编一条汇编指令， 并且返回当前得到的汇编指令的长度

        tmp.Addr = disAsm.VirtualAddr;
        char * eipTmp = (char *)disAsm.EIP;

        for(int i = 0; i < nLen; i++){
            tmp.opCode += QString::asprintf("%2x ", (eipTmp[i] & 0xff));
        }
        tmp.opInstr = QString::asprintf("%s", disAsm.CompleteInstr);
        if(nLen <= 0){
            break;
        }
        nCount += nLen; // 累加已经反汇编的字节数
        disAsm.EIP += nLen; // 定位到下一条汇编指令
        disAsm.VirtualAddr += nLen; // 设置到下一条汇编指令的地址
        result->append(tmp);
    }

    return result;
}

QString Util::PeAttach(QString & path)
{
    HMODULE module = LoadLibraryA("virus_calc.dll");
    if(module == NULL){
        return "未能加载virus_calc.dll!";
    }

    PEATTACH pa = (PEATTACH)GetProcAddress(module, "PEAttached");
    if(pa == NULL){
        return "附加函数定址失败，请检测dll文件!";
    }
    return QString::fromStdString(pa(path.toStdString().data()));
}

int Util::getSectionIndexByName(peFile *pe, const QString &name){
    return pe->getSectionIndexByName(name);
}

QString Util::getBaseName(QString &fullPath){
    QFileInfo fileInfo(fullPath);
    return fileInfo.fileName();
}

int Util::getSectionIndexByRva(peFile *pe, DWORD rva){
    if(rva == 0){
        return -1;
    }
    PIMAGE_SECTION_HEADER headerArray = pe->getSectionHeaderArray();
    for(int i = 0; i < pe->getNtHeader32()->FileHeader.NumberOfSections - 1; i++){
        if(rva >= headerArray[i].VirtualAddress && rva < headerArray[i + 1].VirtualAddress){
            return i;
        }
    }
    return pe->getNtHeader32()->FileHeader.NumberOfSections - 1;
}
