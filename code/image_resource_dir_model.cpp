#include "image_resource_dir_model.h"
IMAGE_RESOURCE_DIR_Model::IMAGE_RESOURCE_DIR_Model(quint32 entryOffset, peFile * pe, QObject * parent)
    :QAbstractTableModel(parent)
{
    header << "peOffset" << "Data" << "Description" << "Value";
    prd = (PIMAGE_RESOURCE_DIRECTORY)((pe->getPeBuffer()) + entryOffset);
    row_count += ((prd->NumberOfIdEntries + prd->NumberOfNamedEntries) * 2);
    pefile = pe;
}

int         IMAGE_RESOURCE_DIR_Model::columnCount(const QModelIndex &parent) const{
    return 4;
}
int         IMAGE_RESOURCE_DIR_Model::rowCount(const QModelIndex &parent) const{
    return row_count;
}
QVariant    IMAGE_RESOURCE_DIR_Model::data(const QModelIndex &index, int role) const{
    if(!index.isValid()){
        return QVariant();
    }
    char buffer[100] = {0};
    int row = index.row();
    if(Qt::TextAlignmentRole == role)
    {
        return int(Qt::AlignCenter | Qt::AlignHCenter);
    }
    if(Qt::BackgroundRole == role){
        if(row > 5){
            switch ((row - 6) % 4) {
                case 0:
            case 1:{
                return QBrush(QColor(0xff, 0xf0, 0xf5));
            }
            case 2:
            case 3:{
                return QBrush(QColor(Qt::lightGray));
            }
            }
        }
    }

    if(Qt::DisplayRole == role){
        switch (index.column()) {
            case 0:
            {
            switch (row) {
            case 0:{
                sprintf(buffer, "%08X", (byte *)prd - pefile->getPeBuffer());
                break;
            }
            case 1:{
                sprintf(buffer, "%08X", (byte *)prd - pefile->getPeBuffer() + 4);
                break;
            }
            case 2:
            case 3:
            case 4:
            case 5:
            {
                sprintf(buffer, "%08X", (byte *)prd - pefile->getPeBuffer() + 8 + (row - 2) * 2);
                break;
            }
            default:{
                sprintf(buffer, "%08X", ((byte *)prd - pefile->getPeBuffer() + 16 + (row - 6) * 4));
                break;
            }
            }
                break;
            }
        case 1:
        {
            switch(row){
            case 0:
            case 1:
            {
                Util::sprintf_buffer4B(buffer, (byte *)prd + row * 4);
                break;
            }
            case 2:
            case 3:
            case 4:
            case 5:{
                Util::sprintf_buffer2B(buffer, (byte *)prd + 8 + (row - 2) * 2);
                break;
            }
            default:{
                Util::sprintf_buffer4B(buffer, (byte *)prd + 16 + (row - 6) * 4);
            }
            }

            break;
        }
        case 2:{
            switch (row) {
            case 0:{
                return "资源属性标志";
            }
            case 1:{
                return "资源建立时间";
            }
            case 2:{
                return "资源主版本";
            }
            case 3:{
                return "资源副版本";
            }
            case 4:{
                return "使用名字的资源条目个数";
            }
            case 5:{
                return "使用ID数字资源条目的个数";
            }
            default:{
                if(row % 2)
                    return "资源数据/子目录偏移地址";
                else
                    return "目录项名称字符串指针/ID";
            }
            }
            break;
        }
        case 3:{
            switch (row) {
            case 0:
            {
                break;
            }
            case 1:{
                DWORD time = 0;
                memcpy(&time, (byte *) prd + 4, 4);
                return ParsePe::parseTimeDateStampToString(time);
            }
            case 2:
            {
                break;
            }
            case 3:
            {
                break;
            }
            case 4:{
                break;
            }
            case 5:{
                break;
            }
            default:{
                if((row - 6) % 2 == 0){

                    PIMAGE_RESOURCE_DIRECTORY_ENTRY entry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((byte *)prd + 16 + (row - 6) * 4);

                    QString name;

                    //如果是字符串指针
                    if(entry->Name & 0x80000000){
                        PIMAGE_RESOURCE_DIR_STRING_U str_name = (PIMAGE_RESOURCE_DIR_STRING_U)((byte *)pefile->getIRD()
                                                                         + (entry->Name & 0X7FFFFFFF));
                         name = QString::fromUtf16((const ushort *)str_name->NameString, str_name->Length);
                    }//ID
                    else{
                         name = QString::number(entry->Id);
                    }
                    return name;
                }
                break;
            }
            }
            break;
        }
        }
        return buffer;
    }

    return QVariant();
}
QVariant    IMAGE_RESOURCE_DIR_Model::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation==Qt::Horizontal&&role==Qt::DisplayRole){
        return header.at(section);
    }else{
        return QAbstractTableModel::headerData(section,orientation,role);
    }
}
