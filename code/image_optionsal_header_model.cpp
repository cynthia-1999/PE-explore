#include "image_optionsal_header_model.h"
const char * IMAGE_OPTIONAL_HEADER_Model::charMagic                            = "标志字";
const char * IMAGE_OPTIONAL_HEADER_Model::charMajor                            = "链接器主版本号";
const char * IMAGE_OPTIONAL_HEADER_Model::charMinor                            = "链接器次版本号";
const char * IMAGE_OPTIONAL_HEADER_Model::charSizeOfCode                       = "所有含有代码的区块的大小";
const char * IMAGE_OPTIONAL_HEADER_Model::charSizeOfInitializedData            = "所有初始化数据区块的大小";
const char * IMAGE_OPTIONAL_HEADER_Model::charSizeOfUnitializedData            = "所有未初始化数据区块的大小";
const char * IMAGE_OPTIONAL_HEADER_Model::charAddressOfEntryPoint              = "程序执行入口RVA";
const char * IMAGE_OPTIONAL_HEADER_Model::charBaseOfCode                       = "代码区块起始RVA";
const char * IMAGE_OPTIONAL_HEADER_Model::charBaseOfData                       = "数据区块起始RVA";
const char * IMAGE_OPTIONAL_HEADER_Model::charImageBase                        = "程序默认载入基址";
const char * IMAGE_OPTIONAL_HEADER_Model::charSectionAlignment                 = "内存中区块的对齐值";
const char * IMAGE_OPTIONAL_HEADER_Model::charFileAlignment                    = "文件中区块的对齐值";
const char * IMAGE_OPTIONAL_HEADER_Model::charMajorOperatingSystemVersion      = "操作系统主版本号";
const char * IMAGE_OPTIONAL_HEADER_Model::charMinorOperatingSystemVersion      = "操作系统次版本号";
const char * IMAGE_OPTIONAL_HEADER_Model::charMajorImageVersion                = "用户自定义主版本号";
const char * IMAGE_OPTIONAL_HEADER_Model::charMinorImageVersion                = "用户自定义次版本号";
const char * IMAGE_OPTIONAL_HEADER_Model::charMajorSubSystemVersion            = "所需子系统主版本号";
const char * IMAGE_OPTIONAL_HEADER_Model::charMionrSubSystemVersion            = "所需子系统次版本号";
const char * IMAGE_OPTIONAL_HEADER_Model::charWin32VersionValue                = "保留";
const char * IMAGE_OPTIONAL_HEADER_Model::charSizeOfImage                      = "映像载入内存后总尺寸";
const char * IMAGE_OPTIONAL_HEADER_Model::charSizeOfHeaders                    = "MS-DOS、PE文件头、区块表总大小";
const char * IMAGE_OPTIONAL_HEADER_Model::charCheckSum                         = "映像校验和";
const char * IMAGE_OPTIONAL_HEADER_Model::charSubSystem                        = "文件子系统";
const char * IMAGE_OPTIONAL_HEADER_Model::charDllCharacteristics               = "DLLCharacteristics";
const char * IMAGE_OPTIONAL_HEADER_Model::charSizeOfStackReserve               = "初始化时栈的尺寸";
const char * IMAGE_OPTIONAL_HEADER_Model::charSizeOfStackCommit                = "实际提交栈的尺寸";
const char * IMAGE_OPTIONAL_HEADER_Model::charSizeOfHeapReserve                = "初始化时堆的尺寸";
const char * IMAGE_OPTIONAL_HEADER_Model::charSizeOfHeapCommit                 = "实际提交时堆的尺寸";
const char * IMAGE_OPTIONAL_HEADER_Model::charLoaderFlags                      = "调试相关，默认为0";
const char * IMAGE_OPTIONAL_HEADER_Model::charNumberOfRvaAndSizes              = "数据目录表的项数";
const char * IMAGE_OPTIONAL_HEADER_Model::charDirectory_ET                     ="导出表";
const char * IMAGE_OPTIONAL_HEADER_Model::charDirectory_IT                     ="导入表";
const char * IMAGE_OPTIONAL_HEADER_Model::charDirectory_RT                     ="资源表";
const char * IMAGE_OPTIONAL_HEADER_Model::charDirectory_Exception_Table        ="异常表";
const char * IMAGE_OPTIONAL_HEADER_Model::charDirectory_ST                     ="安全表";
const char * IMAGE_OPTIONAL_HEADER_Model::charDirectory_BRT                    ="基址重定位表";
const char * IMAGE_OPTIONAL_HEADER_Model::charDirectory_D                      ="调试表";
const char * IMAGE_OPTIONAL_HEADER_Model::charDirectory_C                      ="CopyRight";
const char * IMAGE_OPTIONAL_HEADER_Model::charDirectory_GP                     ="Global Ptr";
const char * IMAGE_OPTIONAL_HEADER_Model::charDirectory_TLS                    ="TLS";
const char * IMAGE_OPTIONAL_HEADER_Model::charDirectory_LC                     ="Load Configuration";
const char * IMAGE_OPTIONAL_HEADER_Model::charDirectory_BI                     ="Bound Import";
const char * IMAGE_OPTIONAL_HEADER_Model::charDirectory_IAT                    ="导入地址表";
const char * IMAGE_OPTIONAL_HEADER_Model::charDirectory_DI                     ="延迟载入表";
const char * IMAGE_OPTIONAL_HEADER_Model::charDirectory_CD                     ="Com Description";
const char * IMAGE_OPTIONAL_HEADER_Model::charDirectory_Reserve                ="保留，必须为0";

IMAGE_OPTIONAL_HEADER_Model::IMAGE_OPTIONAL_HEADER_Model(quint32 addr,byte * data,bool pe32Flag,QObject * parent) :
    QAbstractTableModel(parent)
{
    this->addr = addr;
    this->inter_data = data;
    this->pe32Flag = pe32Flag;
    header << "peOffset" << "Data" << "Description" << "Value";
}

int         IMAGE_OPTIONAL_HEADER_Model::columnCount(const QModelIndex &parent) const{
    return 4;
}
int         IMAGE_OPTIONAL_HEADER_Model::rowCount(const QModelIndex &parent) const{
    if(pe32Flag)
        return 61;
    else
        return 60;
}
QVariant    IMAGE_OPTIONAL_HEADER_Model::data(const QModelIndex &index, int role) const{
    if(!index.isValid()){
        return QVariant();
    }
    char buffer[100] = {0};
    int row = index.row();
    if(Qt::TextAlignmentRole == role)
    {
        return int(Qt::AlignCenter | Qt::AlignHCenter);
    }
    if(role == Qt::DisplayRole){
        switch (index.column()) {
            case 0:{
            if(pe32Flag){
                switch(row){
                    case 0:{
                        sprintf(buffer, "%08X", addr);
                        break;
                    }
                    case 1:{
                        sprintf(buffer, "%08X", addr + 2);
                        break;
                    }
                    case 2:{
                        sprintf(buffer, "%08X", addr + 3);
                        break;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    {
                        sprintf(buffer, "%08X", addr + 4 + (row - 3) * 4);
                        break;
                    }
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    {
                        sprintf(buffer, "%08X", addr + 40 + (row - 12) * 2);
                        break;
                    }
                case 18:
                case 19:
                case 20:
                case 21:{
                    sprintf(buffer, "%08X", addr + 52 + (row - 18) * 4);
                    break;
                }
                case 22:
                case 23:
                {
                    sprintf(buffer, "%08X", addr + 68 + (row - 22) * 2);
                    break;
                }
                case 24:
                case 25:
                case 26:
                case 27:
                case 28:
                case 29:
                case 30:
                case 31:
                case 32:
                case 33:
                case 34:
                case 35:
                case 36:
                case 37:
                case 38:
                case 39:
                case 40:
                case 41:
                case 42:
                case 43:
                case 44:
                case 45:
                case 46:
                case 47:
                case 48:
                case 49:
                case 50:
                case 51:
                case 52:
                case 53:
                case 54:
                case 55:
                case 56:
                case 57:
                case 58:
                case 59:
                case 60:
                case 61:
                {
                    sprintf(buffer, "%08X", addr + 72 + (row - 24) * 4);
                    break;
                }
                }
            }else{
                switch (row) {
                case 0:{
                    sprintf(buffer, "%04X", addr);
                    break;
                }
                case 1:
                case 2:
                {
                    sprintf(buffer, "%04X", addr+ 2 + (row - 1));
                    break;
                }
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:{
                    sprintf(buffer, "%04X", addr+ 4 + (row - 3) * 4);
                    break;
                }
                case 8:
                {
                    sprintf(buffer, "%04X", addr+ 24);
                    break;

                    Util::sprintf_buffer8B(buffer, inter_data + 24);
                    break;
                }
                case 9:
                case 10:
                {
                    sprintf(buffer, "%04X", addr + 32 + (row - 9) * 4);
                    break;
                }
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:{
                    sprintf(buffer, "%04X", addr + 40 + (row - 11) * 2);
                    break;
                    Util::sprintf_buffer2B(buffer, inter_data + 40 + (row - 11) * 2);
                    break;
                }
                case 17:
                case 18:
                case 19:
                case 20:{
                    sprintf(buffer, "%04X", addr + 52 + (row - 17) * 4);
                    break;
                    Util::sprintf_buffer4B(buffer, inter_data + 52 + (row - 17) * 4);
                    break;
                }
                case 21:
                case 22:{
                    sprintf(buffer, "%04X", addr + 68 + (row - 21) * 2);
                    break;
                }
                case 23:
                case 24:
                case 25:
                case 26:
                {
                    sprintf(buffer, "%04X", addr + 72 + (row - 23) * 8);
                    break;
                }
                case 27:
                case 28:
                case 29:
                case 30:
                case 31:
                case 32:
                case 33:
                case 34:
                case 35:
                case 36:
                case 37:
                case 38:
                case 39:
                case 40:
                case 41:
                case 42:
                case 43:
                case 44:
                case 45:
                case 46:
                case 47:
                case 48:
                case 49:
                case 50:
                case 51:
                case 52:
                case 53:
                case 54:
                case 55:
                case 56:
                case 57:
                case 58:
                case 59:
                case 60:
                {
                    sprintf(buffer, "%04X", addr + 104 + (row - 27) * 4);
                    break;
                }
                }
            }
                break;
            }
            case 1:{
            if(pe32Flag){
                switch (row) {
                case 0:{
                    sprintf(buffer, "%02X%02X", inter_data[1],inter_data[0]);
                    break;
                }
                case 1:{
                    sprintf(buffer, "%02X", inter_data[2]);
                    break;
                }
                case 2:{
                    sprintf(buffer, "%02X", inter_data[3]);
                    break;
                }
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:{
                    byte * tmp = inter_data + 4 + (row - 3) * 4;
                sprintf(buffer, "%02X%02X%02X%02X", tmp[3],
                        tmp[2],
                        tmp[1],
                        tmp[0]);
                break;
                }
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                case 17:{
                    byte * tmp = inter_data+40 + (row - 12) * 2;
                    sprintf(buffer, "%02X%02X", tmp[1],tmp[0]);
                    break;
                }
                case 18:
                case 19:
                case 20:
                case 21:{
                    byte * tmp = inter_data + 52 + (row - 18) * 4;
                    sprintf(buffer, "%02X%02X%02X%02X", tmp[3], tmp[2], tmp[1], tmp[0]);
                    break;
                }
                case 22:
                case 23:{
                    byte * tmp = inter_data + 68 + (row - 22) * 2;
                    sprintf(buffer, "%02X%02X", tmp[1], tmp[0]);
                    break;
                }
                case 24:
                case 25:
                case 26:
                case 27:
                case 28:
                case 29:
                case 30:
                case 31:
                case 32:
                case 33:
                case 34:
                case 35:
                case 36:
                case 37:
                case 38:
                case 39:
                case 40:
                case 41:
                case 42:
                case 43:
                case 44:
                case 45:
                case 46:
                case 47:
                case 48:
                case 49:
                case 50:
                case 51:
                case 52:
                case 53:
                case 54:
                case 55:
                case 56:
                case 57:
                case 58:
                case 59:
                case 60:
                case 61:
                {
                    byte * tmp = inter_data + 72 + (row - 24) * 4;
                    sprintf(buffer, "%02X%02X%02X%02X", tmp[3], tmp[2], tmp[1], tmp[0]);
                    break;
                }
            }
            }else{
                switch (row) {
                case 0:{
                    Util::sprintf_buffer2B(buffer, inter_data);
                    break;
                }
                case 1:
                case 2:
                {
                    Util::sprintf_buffer1B(buffer, inter_data + 2 + (row - 1));
                    break;
                }
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:{
                    Util::sprintf_buffer4B(buffer, inter_data + 4 + (row - 3) * 4);
                    break;
                }
                case 8:
                {
                    Util::sprintf_buffer8B(buffer, inter_data + 24);
                    break;
                }
                case 9:
                case 10:
                {
                    Util::sprintf_buffer4B(buffer,inter_data + 32 + (row - 9) * 4);
                    break;
                }
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:{
                    Util::sprintf_buffer2B(buffer, inter_data + 40 + (row - 11) * 2);
                    break;
                }
                case 17:
                case 18:
                case 19:
                case 20:{
                    Util::sprintf_buffer4B(buffer, inter_data + 52 + (row - 17) * 4);
                    break;
                }
                case 21:
                case 22:{
                    Util::sprintf_buffer2B(buffer, inter_data + 68 + (row - 21) * 2);
                    break;
                }
                case 23:
                case 24:
                case 25:
                case 26:
                {
                    Util::sprintf_buffer8B(buffer, inter_data + 72 + (row - 23) * 8);
                    break;
                }
                case 27:
                case 28:
                case 29:
                case 30:
                case 31:
                case 32:
                case 33:
                case 34:
                case 35:
                case 36:
                case 37:
                case 38:
                case 39:
                case 40:
                case 41:
                case 42:
                case 43:
                case 44:
                case 45:
                case 46:
                case 47:
                case 48:
                case 49:
                case 50:
                case 51:
                case 52:
                case 53:
                case 54:
                case 55:
                case 56:
                case 57:
                case 58:
                case 59:
                case 60:
                {
                    Util::sprintf_buffer4B(buffer, inter_data + 104 + (row - 27) * 4);
                    break;
                }
                }
            }
            break;
        }
        case 2:{
            if(pe32Flag){
            switch (row) {
                case 0:
                return charMagic;
            case 1:
                return charMajor;
            case 2:
                return charMinor;
            case 3:
                return charSizeOfCode;
            case 4:
                return charSizeOfInitializedData;
            case 5:
                return charSizeOfUnitializedData;

            case 6:
               return charAddressOfEntryPoint                      ;
               case 7:return charBaseOfCode                        ;
               case 8: return charBaseOfData                       ;
               case 9: return charImageBase                        ;
               case 10: return charSectionAlignment                ;
               case 11: return charFileAlignment                   ;
               case 12: return charMajorOperatingSystemVersion     ;
               case 13: return charMinorOperatingSystemVersion     ;
               case 14: return charMajorImageVersion               ;
               case 15: return charMinorImageVersion               ;
               case 16: return charMajorSubSystemVersion           ;
               case 17: return charMionrSubSystemVersion           ;
               case 18: return charWin32VersionValue               ;
               case 19: return charSizeOfImage                     ;
               case 20: return charSizeOfHeaders                   ;
               case 21: return charCheckSum                        ;
               case 22: return charSubSystem                       ;
               case 23: return charDllCharacteristics              ;
               case 24: return charSizeOfStackReserve              ;
               case 25: return charSizeOfStackCommit               ;
               case 26: return charSizeOfHeapReserve               ;
               case 27: return charSizeOfHeapCommit                ;
               case 28: return charLoaderFlags                     ;
               case 29: return charNumberOfRvaAndSizes             ;
               case 30: return charDirectory_ET                    ;
               case 32: return charDirectory_IT                    ;
               case 34: return charDirectory_RT                    ;
               case 36: return charDirectory_Exception_Table       ;
               case 38: return charDirectory_ST                    ;
               case 40: return charDirectory_BRT                   ;
               case 42: return charDirectory_D                     ;
               case 44: return charDirectory_C                     ;
               case 46: return charDirectory_GP                    ;
               case 48: return charDirectory_TLS                   ;
               case 50: return charDirectory_LC                    ;
               case 52: return charDirectory_BI                    ;
               case 54: return charDirectory_IAT                   ;
               case 56: return charDirectory_DI                    ;
               case 58: return charDirectory_CD                    ;
               case 60: return charDirectory_Reserve               ;
            }
        }
            else{
                switch (row) {
                case 0: return  charMagic                            ;
                case 1: return  charMajor                            ;
                case 2: return  charMinor                            ;
                case 3: return  charSizeOfCode                       ;
                case 4: return  charSizeOfInitializedData            ;
                case 5: return  charSizeOfUnitializedData            ;
                case 6: return  charAddressOfEntryPoint              ;
                case 7: return  charBaseOfCode                       ;
                case 8: return  charImageBase                        ;
                case 9: return  charSectionAlignment                 ;
                case 10: return  charFileAlignment                    ;
                case 11: return  charMajorOperatingSystemVersion      ;
                case 12: return  charMinorOperatingSystemVersion      ;
                case 13: return  charMajorImageVersion                ;
                case 14: return  charMinorImageVersion                ;
                case 15: return  charMajorSubSystemVersion            ;
                case 16: return  charMionrSubSystemVersion            ;
                case 17: return  charWin32VersionValue                ;
                case 18: return  charSizeOfImage                      ;
                case 19: return  charSizeOfHeaders                    ;
                case 20: return  charCheckSum                         ;
                case 21: return  charSubSystem                        ;
                case 22: return  charDllCharacteristics               ;
                case 23: return  charSizeOfStackReserve               ;
                case 24: return  charSizeOfStackCommit                ;
                case 25: return  charSizeOfHeapReserve                ;
                case 26: return  charSizeOfHeapCommit                 ;
                case 27: return  charLoaderFlags                      ;
                case 28: return  charNumberOfRvaAndSizes              ;
                case 29: return  charDirectory_ET                     ;
                case 31: return  charDirectory_IT                     ;
                case 33: return  charDirectory_RT                     ;
                case 35: return  charDirectory_Exception_Table        ;
                case 37: return  charDirectory_ST                     ;
                case 39: return  charDirectory_BRT                    ;
                case 41: return  charDirectory_D                      ;
                case 43: return  charDirectory_C                      ;
                case 45: return  charDirectory_GP                     ;
                case 47: return  charDirectory_TLS                    ;
                case 49: return  charDirectory_LC                     ;
                case 51: return  charDirectory_BI                     ;
                case 53: return  charDirectory_IAT                    ;
                case 55: return  charDirectory_DI                     ;
                case 57: return  charDirectory_CD                     ;
                case 59: return  charDirectory_Reserve                ;
            }
            break;
        }
        }
        case 3:{
            if(pe32Flag){
                switch (row) {
                    case 0:
                {
                    WORD tmp = 0;
                    memcpy(&tmp, inter_data, 2);
                    switch (tmp) {
                        case 0x10b:
                        return  "IMAGE_NT_OPTIONAL_HDR32_MAGIC";
                    case 0x20b:
                        return "IMAGE_NT_OPTIONAL_HDR64_MAGIC";
                    case 0x107:
                        return "IMAGE_ROM_OPTIONAL_HDR_MAGIC";
                    }
                    break;
                }
                    case 22:
                {
                    WORD tmp = 0;
                    memcpy(&tmp, inter_data + 0x44, 2);
                    switch (tmp) {
                    case 0    :return "IMAGE_SUBSYSTEM_UNKNOWN"	              ;
                    case 1    :return "IMAGE_SUBSYSTEM_NATIVE"	                  ;
                    case 2    :return "IMAGE_SUBSYSTEM_WINDOWS_GUI"	          ;
                    case 3    :return "IMAGE_SUBSYSTEM_WINDOWS_CUI"	          ;
                    case 5    :return "IMAGE_SUBSYSTEM_OS2_CUI"	              ;
                    case 7    :return "IMAGE_SUBSYSTEM_POSIX_CUI"	              ;
                    case 8    :return "IMAGE_SUBSYSTEM_NATIVE_WINDOWS"	          ;
                    case 9    :return "IMAGE_SUBSYSTEM_WINDOWS_CE_GUI"	          ;
                    case 10   :return "IMAGE_SUBSYSTEM_EFI_APPLICATION"	      ;
                    case 11   :return "IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER " ;
                    case 12   :return "IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER"	      ;
                    case 13   :return "IMAGE_SUBSYSTEM_EFI_ROM"	              ;
                    case 14   :return "IMAGE_SUBSYSTEM_XBOX"	                  ;
                    case 16   :return "IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION" ;
                    }
                    break;
                }
                case 23:{
                    QString tmp = "";
                    WORD dllChar = 0;
                    memcpy(&dllChar, inter_data + 70, 2);
                    for(WORD i = 1; i != 0; i <<= 1){
                        if((i & dllChar) != 0)
                            tmp += ParsePe::parseWordToDllCharacteristics(i) + "\n";
                    }
                    return tmp;
                }
                }
            }else{
                switch (row) {
                    case 0:
                {
                    WORD tmp = 0;
                    memcpy(&tmp, inter_data, 2);
                    switch (tmp) {
                        case 0x10b:
                        return  "IMAGE_NT_OPTIONAL_HDR32_MAGIC";
                    case 0x20b:
                        return "IMAGE_NT_OPTIONAL_HDR64_MAGIC";
                    case 0x107:
                        return "IMAGE_ROM_OPTIONAL_HDR_MAGIC";
                    }
                    break;
                }
                case 21:
                {
                    WORD tmp = 0;
                    memcpy(&tmp, inter_data + 68, 2);
                    switch (tmp) {
                    case 0    :return "IMAGE_SUBSYSTEM_UNKNOWN"	              ;
                    case 1    :return "IMAGE_SUBSYSTEM_NATIVE"	                  ;
                    case 2    :return "IMAGE_SUBSYSTEM_WINDOWS_GUI"	          ;
                    case 3    :return "IMAGE_SUBSYSTEM_WINDOWS_CUI"	          ;
                    case 5    :return "IMAGE_SUBSYSTEM_OS2_CUI"	              ;
                    case 7    :return "IMAGE_SUBSYSTEM_POSIX_CUI"	              ;
                    case 8    :return "IMAGE_SUBSYSTEM_NATIVE_WINDOWS"	          ;
                    case 9    :return "IMAGE_SUBSYSTEM_WINDOWS_CE_GUI"	          ;
                    case 10   :return "IMAGE_SUBSYSTEM_EFI_APPLICATION"	      ;
                    case 11   :return "IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER " ;
                    case 12   :return "IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER"	      ;
                    case 13   :return "IMAGE_SUBSYSTEM_EFI_ROM"	              ;
                    case 14   :return "IMAGE_SUBSYSTEM_XBOX"	                  ;
                    case 16   :return "IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION" ;
                    }
                    break;
                }
                }
            }
        }
        }
        return buffer;
    }
    return QVariant();
}
QVariant    IMAGE_OPTIONAL_HEADER_Model::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation==Qt::Horizontal&&role==Qt::DisplayRole){
        return header.at(section);
    }else{
        return QAbstractTableModel::headerData(section,orientation,role);
    }
}
