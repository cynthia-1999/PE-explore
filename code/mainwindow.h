#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"headers.h"
#include"globalstore.h"
#include"pe_ui_model.h"
#include"image_dos_header_model.h"
#include"binary_ui_model.h"
#include"image_dos_signature.h"
#include"image_file_header_model.h"
#include"parsepe.h"
#include"image_optionsal_header_model.h"
#include"image_section_header.h"
#include"code32_model.h"
#include"code64_model.h"
#include"image_import_descriptor_model.h"
#include"image_thunk_data32_model.h"
#include"image_thunk_data64_model.h"
#include"hookSrc/hookWindow.h"
#include"image_export_table_model.h"
#include"image_base_relocaltion_model.h"
#include "image_resource_data_entry_model.h"
#include "image_resource_dir_model.h"
#include<QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    enum treeItemType{itTopItem = 1001, itGroupItem, itChildItem};


    //更新树
    void updateTree();

    GlobalStore * globalStore = Q_NULLPTR;


    //打开新文件,返回文件路径
    //返回""表示未选取
    QString openFileDialog();

    //状态栏信息
    QLabel * statusLabel;

    PE_UI_Model * pe_model = Q_NULLPTR;
    IMAGE_DOS_HEADER_Model * image_dos_header_model = Q_NULLPTR;
    binary_ui_model * image_dos_stub_model = Q_NULLPTR;
    binary_ui_model * image_nt_headers_model = Q_NULLPTR;
    Image_Dos_Signature * image_dos_signature = Q_NULLPTR;
    IMAGE_FILE_HEADER_Model * image_file_header_model = Q_NULLPTR;
    IMAGE_OPTIONAL_HEADER_Model * image_optional_header_model = Q_NULLPTR;
    QList<IMAGE_SECTION_HEADER_Model *> image_section_header_model_List;
    QList<binary_ui_model*> image_section_model_list;


    QHash<QString, Code32_Model*> Code32_Instr_Model;
    QHash<QString, Code64_Model*> Code64_Instr_Model;

    IMAGE_IMPORT_DESCRIPTION_Model * iid_model = Q_NULLPTR;

    QHash<QString, IMAGE_THUNK_DATA32_Model *> itd32_Model_Table;
    QHash<QString, IMAGE_THUNK_DATA64_Model *> itd64_Model_Table;
    IMAGE_EXPORT_TABLE_Model * IED_Model = Q_NULLPTR;

    IMAGE_BASE_RELOCALTION_Model * IBR_Model = Q_NULLPTR;

    QList<IMAGE_RESOURCE_DIR_Model *> ResourceEntryList;
    QList<IMAGE_RESOURCE_Data_Entry_Model *> ResourceDataEntryList;
    QList<binary_ui_model *> ResourceDataList;

    void clear_res_list(){
        for(int i = 0; i < ResourceEntryList.size(); i++){
            delete ResourceEntryList[i];
        }
        for(int i = 0; i < ResourceDataList.size(); i++){
            delete ResourceDataList[i];
        }
        for(int i = 0; i < ResourceDataEntryList.size(); i++){
            delete ResourceDataEntryList[i];
        }
        ResourceDataList.clear();
        ResourceDataEntryList.clear();
        ResourceEntryList.clear();

    }
    void set_resource_data_ui(int index);
    void set_resource_data_entry_ui(int index);
    void set_resource_directory_ui(int index);

    void process_res_tree(int deep, QTreeWidgetItem * parent, byte * inter_data);

    void process_ibr_model();
    void set_ibr_ui();

    void process_ied_model();
    void set_ied_ui();

    void process_itd_model();
    void set_itd_ui(QString name);

    void process_iid_model();
    void set_iid_ui();
    void process_code_model();
    void set_code_ui(QString section_name);
    void process_image_section_model_list();
    void set_image_section_ui(int index);
    void process_image_section_header_model_list();
    void set_image_section_header_ui(int index);
    void process_image_optional_header_model();
    void set_image_optional_header_ui();
    void process_image_file_header_model();
    void set_image_file_header_ui();
    void process_image_dos_signature_model();
    void set_image_dos_signature_ui();
    void process_image_nt_headers_model();
    void set_image_nt_headers_ui();
    void process_image_dos_stub_model();
    void set_image_dos_stub_ui();
    void process_pe_ui();
    void process_image_dos_header_ui();
    void set_pe_ui();
    void set_image_dos_header_ui();

    //弹出文件保存对话框并根据用户选择进行操作
    //返回false表示cancel，调用函数应该return
    //返回true表示允许调用者进行下一步操作
    bool saveDialog();

    //by whl
    void set_section_header_chars(quint32 charsFileOffset, QString sectionName);

protected:
     void closeEvent(QCloseEvent *event);

public slots:
    void ui_clear();
    void fileModifySlot(bool flag);
    //更新ui的槽函数
    void ui_update();
private slots:
    void on_open_triggered();

    void on_option_triggered();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_viewRVA_triggered();
    void on_viewPeOffset_triggered();
    void on_viewVA_triggered();
    void on_viewDisassembly_triggered(bool checked);
    void on_action_process_triggered();
    void on_pe_attach_triggered();
    void on_viewDisassembly_triggered();
    //void on_data_tableView_clicked((QModelIndex &qModelIndex));  // by whl
    void on_save_triggered();
    void on_save_as_triggered();
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_checkCertAction_triggered();
    void on_actionSignatureVerification_triggered();
    void on_peAttrModAction_triggered();


    void on_actionAbout_triggered();
    void on_actionhelp_real_triggered();
};
#endif // MAINWINDOW_H
