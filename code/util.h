#ifndef UTIL_H
#define UTIL_H
#include"headers.h"
#include"globalstore.h"
#include"../libHeaders/BeaEngine.h"
typedef const char * (* PEATTACH)(const char *);

typedef struct OPCLASS32{
    unsigned int Addr;   //地址
    QString opCode;     //汇编机器码
    QString opInstr;    //汇编语句
}opClass32, *pOpClass32;

typedef struct OPCLASS64{
    unsigned long long Addr;   //地址
    QString opCode;     //汇编机器码
    QString opInstr;    //汇编语句
}opClass64, *pOpClass64;

//工具类，包含一些用到的工具

class Util{
public:
    static int getSectionIndexByRva(peFile * pe, DWORD rva);

    //根据RVA获取该地址相对于文件首部的偏移
    static DWORD getFileOffsetByRVA(peFile * pe, DWORD RVA);

    //根据名称获取节在节表中的索引
    static int getSectionIndexByName(peFile * pe, const QString & name);

    //QString 转宽字符方法:
    //(LPWSTR) QString("nihao").toStdWString().c_str();
    //宽字符转QString
    //QString::fromStdWString(LPCWSTR * str)


    static QList<opClass32>* Disassembly32(BYTE opCode[], long size,unsigned int VirtualAddr);
    static QList<opClass64>* Disassembly64(BYTE opCode[], long size,unsigned long long VirtualAddr);

    //传入希望附加的文件路径
    static QString PeAttach(QString & path);

    //获取文件baseName
    static QString getBaseName(QString & fullPath);
    static void sprintf_buffer1B(char * buffer, byte * data){
        sprintf(buffer, "%02X", data[0]);
    }
    static void sprintf_buffer2B(char * buffer, byte * data){
        sprintf(buffer,"%02X%02X", data[1], data[0]);
    }
    static void sprintf_buffer4B(char * buffer, byte * data){
        sprintf(buffer, "%02X%02X%02X%02X", data[3], data[2], data[1], data[0]);
    }
    static void sprintf_buffer8B(char * buffer, byte * data){
        sprintf(buffer, "%02X%02X%02X%02X%02X%02X%02X%02X",
                data[7],data[6],data[5],data[4],
                data[3],data[2],data[1],data[0]);
    }
    static void sprintf_buffer_DWORD(char * buffer, byte* num){
        DWORD tmp = 0;
        memcpy((void *)&tmp, num, 4);
        sprintf(buffer, "%08X", num);
    }
    static void sprintf_buffer_WORD(char * buffer, byte* num){
        WORD tmp = 0;
        memcpy((void *)&tmp, num, 2);
        sprintf(buffer, "%04X", num);
    }
};

#endif // UTIL_H
