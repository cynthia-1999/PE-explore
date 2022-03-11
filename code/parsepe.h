#ifndef PARSEDWORDTOSTRING_H
#define PARSEDWORDTOSTRING_H
#include "headers.h"

//这个类主要用于将一些特定的数字解析成QString
class ParsePe
{
public:
    ParsePe();

    //将时间字符串解析为DWORD,格式: 2020-04-10 01:49:43
    //返回0表示解析失败，注意，如果是4月，写04而不是4，年月日和分时秒中间有一个空格
    static DWORD parseStringToTimeDateStamp(QString & time);

    //将DWORD格式化为时间字符串
    static QString parseTimeDateStampToString(DWORD time);

    //将数字解析为平台，只支持i386、x64、amd64
    static QString parseMachine(DWORD machine){
        checkInit();
        auto tmp = machineDSTable.constFind(machine);
        if(tmp == machineDSTable.end()){
            return "未知平台";
        }
        return *tmp;
    }

    //将平台名称解析为数字
    static DWORD parseQStringToMachine(QString & machine){
        checkInit();
        auto tmp = machineSDTable.constFind(machine);
        if(tmp == machineSDTable.end()){
            return 0;
        }

        return *tmp;
    }


    static QString parseWordToImage_Characteristics(WORD char_tmp){
        checkInit();
        auto tmp = image_characteristics_table.constFind(char_tmp);
        if(tmp == image_characteristics_table.end()){
            return "未知平台";
        }
        return *tmp;
    }

    static QString parseWordToDllCharacteristics(WORD char_tmp){
        checkInit();
        auto tmp = dllCharacteristics_table.constFind(char_tmp);
        if(tmp == dllCharacteristics_table.end()){
            return "未知平台";
        }
        return *tmp;
    }

    static QString parseDWordToSectionCharacteristics(DWORD char_tmp){
        checkInit();
        auto tmp = sectionCharacteristics.constFind(char_tmp);
        if(tmp == sectionCharacteristics.end()){
            return "未知平台";
        }
        return *tmp;
    }
    static QString parseIntToResString(int id){
        checkInit();
        auto tmp = resource_id_StringTable.constFind(id);
        if(tmp == resource_id_StringTable.end()){
            return "ID未定义";
        }
        return *tmp;
    }

    //机器对应表
    static QHash<DWORD,QString> machineDSTable;

private:
    //初始化标志
    static bool initFlag;
    static QHash<QString, DWORD> machineSDTable;
    static QHash<WORD, QString> SubSystemTable;
    static QHash<WORD, QString> image_characteristics_table;
    static QHash<WORD, QString> section_characteristics_table;
    static QHash<WORD, QString> dllCharacteristics_table;
    static QHash<DWORD, QString> sectionCharacteristics;
    static QHash<int, QString> resource_id_StringTable;

    static void init();
    static void checkInit(){
        if(!initFlag){
            init();
        }
    }
};

#endif // PARSEDWORDTOSTRING_H
