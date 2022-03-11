#include "image_dos_header_model.h"
const char * IMAGE_DOS_HEADER_Model::char_sig               = "Signature";
const char * IMAGE_DOS_HEADER_Model::char_Bytes             = "Bytes on Last Page Of File";
const char * IMAGE_DOS_HEADER_Model::char_page              = "Pages in File";
const char * IMAGE_DOS_HEADER_Model::char_relocations       = "Relocations";
const char * IMAGE_DOS_HEADER_Model::char_Size_Of           = "Size of Headers in Paragraphs";
const char * IMAGE_DOS_HEADER_Model::char_Min               = "Minimum Extra Paragraphs";
const char * IMAGE_DOS_HEADER_Model::char_Max               = "Maximum Extra Paragraphs";
const char * IMAGE_DOS_HEADER_Model::char_Intel_SS          = "Intel(relative) SS";
const char * IMAGE_DOS_HEADER_Model::char_Initial_SP        = "Initial SP";
const char * IMAGE_DOS_HEADER_Model::char_imgdos_sig_mz     = "IMAGE_DOS_SIGNATURE MZ";
const char * IMAGE_DOS_HEADER_Model::char_CheckSum          = "CheckSum";
const char * IMAGE_DOS_HEADER_Model::char_Initial_IP        = "Initial IP";
const char * IMAGE_DOS_HEADER_Model::char_Initial_CS        = "Initial(relative) CS";
const char * IMAGE_DOS_HEADER_Model::char_OTRT              = "Offset to Relocation Table";
const char * IMAGE_DOS_HEADER_Model::char_Overlay_Number    = "Overlay Number";
const char * IMAGE_DOS_HEADER_Model::char_reserved          = "Reserved";
const char * IMAGE_DOS_HEADER_Model::char_OEM_Identitier    = "OEM Identitier";
const char * IMAGE_DOS_HEADER_Model::char_OEM_information   = "OEM information";
const char * IMAGE_DOS_HEADER_Model::char_OTNH              = "Offset to New Header";



IMAGE_DOS_HEADER_Model::IMAGE_DOS_HEADER_Model(byte * data ,QObject * parent)
    : QAbstractTableModel(parent)
{
    header << "peOffset" << "Data" << "Description" << "Value";
    this->inter_data = data;
}

int IMAGE_DOS_HEADER_Model::columnCount(const QModelIndex &parent) const{
    return 4;
}
int IMAGE_DOS_HEADER_Model::rowCount(const QModelIndex &parent) const{
    return 31;
}
QVariant IMAGE_DOS_HEADER_Model::data(const QModelIndex &index, int role) const{
    if(!index.isValid()){
        return QVariant();
    }
    int row = index.row();
    char buffer[100] = {0};
    if(Qt::TextAlignmentRole == role)
    {
        return int(Qt::AlignCenter | Qt::AlignHCenter);
    }
    if(role == Qt::DisplayRole){
    switch (index.column()) {
        case 0:
    {
       sprintf(buffer, "%08X", row * 2);
        break;
    }
    case 1:
    {
        if(row == 0){
            sprintf(buffer, "%02X%02X", inter_data[1],inter_data[0]);
        }else if (row !=30){
            sprintf(buffer, "%02X%02X", inter_data[row * 2 + 1],inter_data[row * 2]);
        }else if(row == 30){
            sprintf(buffer, "%08X", inter_data[row * 2]);
        }
        break;
    }
    case 2:
    {
        switch (row) {
            case 0:
            return char_sig;
        case 1:
            return char_Bytes;
        case 2:
            return char_page;
        case 3:
            return char_relocations;
        case 4:
            return char_Size_Of;
        case 5:
            return char_Min;
        case 6:
            return char_Max;
        case 7:
            return char_Intel_SS;
        case 8:
            return char_Initial_SP;
        case 9:
            return char_CheckSum;
        case 10:
            return char_Initial_IP;
        case 11:
            return char_Initial_CS;
        case 12:
            return char_OTRT;
        case 13:
            return char_Overlay_Number;
        case 14:
        case 15:
        case 16:
        case 17:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
            return char_reserved;
        case 18:
            return char_OEM_Identitier;
        case 19:
            return char_OEM_information;
        case 30:
            return char_OTNH;
        }
        break;
    }
    case 3:
    {
        if(row == 0){
            return char_imgdos_sig_mz;
        }
        break;
    }
    }
    return buffer;
    }
    return QVariant();
}
QVariant IMAGE_DOS_HEADER_Model::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation==Qt::Horizontal&&role==Qt::DisplayRole){
        return header.at(section);
    }else{
        return QAbstractTableModel::headerData(section,orientation,role);
    }
}
