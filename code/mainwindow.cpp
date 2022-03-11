#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"globalstore.h"
#include"pefile.h"
#include"options.h"
#include "modifybinarydialog.h"
#include "sectioncharsdialog.h"
#include "peattrmoddialog.h"
#include "certinfodialog.h"
#include "helpdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("PE Insight");

    // 调整 工具栏  顺序
    ui->menubar->clear();
    ui->menubar->addMenu(ui->menu);
    ui->menubar->addMenu(ui->menu_2);
    ui->menubar->addMenu(ui->menu_3);
    ui->menubar->addAction(ui->action_process);
    ui->menubar->addAction(ui->pe_attach);
    ui->menubar->addMenu(ui->menuHelp);

    globalStore = GlobalStore::getInstanceP();
    fileModifySlot(false);
    statusLabel = new QLabel("");
    ui->statusbar->addWidget(statusLabel);
    ui->treeWidget->header()->hide();

    ui->viewPeOffset->setChecked(true);
    ui->viewRVA->setChecked(false);
    ui->viewVA->setChecked(false);

    connect(globalStore->pefile,SIGNAL(modifyFlagChanged_signal(bool)), this, SLOT(fileModifySlot(bool)));
    connect(globalStore->pefile,SIGNAL(updateFile()), this, SLOT(ui_update()));
    connect(globalStore->pefile,SIGNAL(peFileClear()), this, SLOT(ui_clear()));
    connect(this, SIGNAL(close()), this, SLOT());
    Code32_Instr_Model.insert("", Q_NULLPTR);
    Code64_Instr_Model.insert("", Q_NULLPTR);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ui_clear(){
    ui->treeWidget->clear();
}

void MainWindow::on_open_triggered()
{
    //文件被修改
    if(globalStore->pefile->getModifyFlag()){
        if(saveDialog()){

            QString filePath = openFileDialog();
            if(filePath.isEmpty()){
                QMessageBox::information(this,"错误", "您未选择文件!");
                return;
            }else{
                globalStore->options->pePath = filePath;
                QString result = globalStore->pefile->setPeBuffer(filePath);

                if(result.isEmpty()){
                    QMessageBox::information(this,"错误","文件打开失败，错误原因:" + result);
                }else{
                    return;
                }
            }
        }else{
            return;
        }
    }else{
        QString filePath = openFileDialog();
        if(filePath.isEmpty()){
            QMessageBox::information(this,"错误", "您未选择文件!");
            return;
        }else{
            globalStore->options->pePath = filePath;
            QString result = globalStore->pefile->setPeBuffer(filePath);

            if(!result.isEmpty()){
                QMessageBox::information(this,"错误","文件读取失败，错误原因:" + result);
            }else{
                return;
            }
        }
    }
}
bool MainWindow::saveDialog()
{
    QString dlgTitle = "警告";
    QString strInfo ="文件已被修改，是否保存?";
    QMessageBox::StandardButton result;
    QMessageBox::StandardButton defaultBtn = QMessageBox::NoButton;
    result = QMessageBox::question(this, dlgTitle, strInfo,QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, defaultBtn);
    if(result == QMessageBox::Yes){
        if(!(globalStore->pefile->writeBuffer(globalStore->options->pePath).isEmpty())){
            QMessageBox::information(this,"错误","文件保存失败!");
            return false;
        }
        return true;
    }else if(result == QMessageBox::No){
        return true;
    }else{
        return false;
    }
}

QString MainWindow::openFileDialog(){
    QString curPath = QDir::currentPath();
    QString dlgTitle = "选择一个文件";
    QString filter = "应用程序(*.exe);;dll文件(*.dll);;";
    QString aFileName = QFileDialog::getOpenFileName(this, dlgTitle, curPath, filter);
    return aFileName;
}

void MainWindow::fileModifySlot(bool flag){
    ui->save->setEnabled(flag);
    ui->save_as->setEnabled(flag);
}

void MainWindow::ui_update(){
    statusLabel->setText(globalStore->options->pePath);
    process_pe_ui();
    process_image_dos_header_ui();
    process_image_dos_stub_model();
    process_image_nt_headers_model();
    process_image_dos_signature_model();
    process_image_file_header_model();
    process_image_optional_header_model();
    process_image_section_header_model_list();
    process_image_section_model_list();
    process_code_model();
    process_iid_model();
    process_itd_model();
    process_ied_model();
    process_ibr_model();
    clear_res_list();
    updateTree();
}


void MainWindow::updateTree(){
    ui->treeWidget->clear();
    ui->viewVA->setChecked(false);
    ui->viewRVA->setChecked(false);
    ui->viewPeOffset->setChecked(true);

    QTreeWidgetItem * rootItem = new QTreeWidgetItem(MainWindow::itTopItem);
    rootItem->setText(0, Util::getBaseName(globalStore->options->pePath));
    rootItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsAutoTristate | Qt::ItemIsEnabled);
    ui->treeWidget->addTopLevelItem(rootItem);

    QTreeWidgetItem * image_dos_header_item = new QTreeWidgetItem(MainWindow::itChildItem);
    image_dos_header_item->setText(0, "IMAGE_DOS_HEADER");
    image_dos_header_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsAutoTristate | Qt::ItemIsEnabled);
    rootItem->addChild(image_dos_header_item);

    QTreeWidgetItem * ms_dos_stub_program_item = new QTreeWidgetItem(MainWindow::itChildItem);
    ms_dos_stub_program_item->setText(0, "MS-DOS Stub Program");
    ms_dos_stub_program_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsAutoTristate | Qt::ItemIsEnabled);
    rootItem->addChild(ms_dos_stub_program_item);

    QTreeWidgetItem * image_nt_header_item = new QTreeWidgetItem(MainWindow::itGroupItem);
    image_nt_header_item->setText(0, "IMAGE_NT_HEADERS");
    image_nt_header_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsAutoTristate | Qt::ItemIsEnabled);
    rootItem->addChild(image_nt_header_item);

    QTreeWidgetItem * image_nt_header_sig_item = new QTreeWidgetItem(MainWindow::itChildItem);
    image_nt_header_sig_item->setText(0, "Signature");
    image_nt_header_sig_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsAutoTristate | Qt::ItemIsEnabled);
    image_nt_header_item->addChild(image_nt_header_sig_item);

    QTreeWidgetItem * image_file_header_item = new QTreeWidgetItem(MainWindow::itChildItem);
    image_file_header_item->setText(0, "IMAGE_FILE_HEADER");
    image_file_header_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsAutoTristate | Qt::ItemIsEnabled);
    image_nt_header_item->addChild(image_file_header_item);

    QTreeWidgetItem * image_optional_header_item = new QTreeWidgetItem(MainWindow::itChildItem);
    image_optional_header_item->setText(0, "IMAGE_OPTIONAL_HEADER");
    image_optional_header_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsAutoTristate | Qt::ItemIsEnabled);
    image_nt_header_item->addChild(image_optional_header_item);

    char buffer[10];
    for(int i = 0; i < globalStore->pefile->getNtHeader32()->FileHeader.NumberOfSections; i++){
        sprintf(buffer, "%s", globalStore->pefile->getSectionHeaderArray()[i].Name);
        QTreeWidgetItem * section_header = new QTreeWidgetItem(MainWindow::itChildItem);
        section_header->setText(0, "IMAGE_SECTION_HEADER " + QString(buffer));
        rootItem->addChild(section_header);
    }

    int iidIndex = -1;
    int iedIndex = -1;
    int ibrIndex = -1;
    int irdIndex = -1;

    if(globalStore->pefile->getIRD() != Q_NULLPTR){
        if(globalStore->pefile->getPe32Flag()){
            irdIndex = Util::getSectionIndexByRva(globalStore->pefile,
                                                  globalStore->pefile->getNtHeader32()->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress);
        }else{
            irdIndex = Util::getSectionIndexByRva(globalStore->pefile,
                                                  globalStore->pefile->getNtHeader64()->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress);
        }
    }

    if(IBR_Model != Q_NULLPTR){
        //32位
        if(globalStore->pefile->getPe32Flag()){
            ibrIndex = Util::getSectionIndexByRva(globalStore->pefile,
                                                  globalStore->pefile->getNtHeader32()->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
        }else{
            ibrIndex = Util::getSectionIndexByRva(globalStore->pefile,
                                                  globalStore->pefile->getNtHeader64()->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
        }
    }

    if(iid_model != Q_NULLPTR){
        //32位
        if(globalStore->pefile->getPe32Flag()){
            iidIndex = Util::getSectionIndexByRva(globalStore->pefile,
                                                  globalStore->pefile->getNtHeader32()->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
        }else{
            iidIndex = Util::getSectionIndexByRva(globalStore->pefile,
                                                  globalStore->pefile->getNtHeader64()->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
        }
    }
    if(IED_Model != Q_NULLPTR){
        //32位
        if(globalStore->pefile->getPe32Flag()){
            iedIndex = Util::getSectionIndexByRva(globalStore->pefile,
                                                  globalStore->pefile->getNtHeader32()->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
        }else{
            iedIndex = Util::getSectionIndexByRva(globalStore->pefile,
                                                  globalStore->pefile->getNtHeader64()->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
        }
    }

    for(int i = 0; i < globalStore->pefile->getNtHeader32()->FileHeader.NumberOfSections; i++){
        sprintf(buffer, "%s", globalStore->pefile->getSectionHeaderArray()[i].Name);
        QTreeWidgetItem * section = new QTreeWidgetItem(MainWindow::itChildItem);
        section->setText(0, "SECTION " + QString(buffer));
        rootItem->addChild(section);

        if(iidIndex == i){
            QTreeWidgetItem * image_iid_item = new QTreeWidgetItem(MainWindow::itChildItem);
            image_iid_item->setText(0, "IMAGE_IMPORT_DESCRIPTOR");
            image_iid_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsAutoTristate | Qt::ItemIsEnabled);
            section->addChild(image_iid_item);

            if(globalStore->pefile->getPe32Flag()){
                QStringList NameList = itd32_Model_Table.keys();
                for(int i = 0; i < NameList.size(); i++){
                    QTreeWidgetItem * image_itd_item = new QTreeWidgetItem(MainWindow::itChildItem);
                    image_itd_item->setText(0, "IMAGE_THUNK_DATA " + NameList[i]);
                    image_itd_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsAutoTristate | Qt::ItemIsEnabled);
                    image_iid_item->addChild(image_itd_item);
                }
            }else{
                QStringList NameList = itd64_Model_Table.keys();
                for(int i = 0; i < NameList.size(); i++){
                    QTreeWidgetItem * image_itd_item = new QTreeWidgetItem(MainWindow::itChildItem);
                    image_itd_item->setText(0, "IMAGE_THUNK_DATA " + NameList[i]);
                    image_itd_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsAutoTristate | Qt::ItemIsEnabled);
                    image_iid_item->addChild(image_itd_item);
                }
            }
        }
        if(iedIndex == i){
            QTreeWidgetItem * image_iid_item = new QTreeWidgetItem(MainWindow::itChildItem);
            image_iid_item->setText(0, "IMAGE_EXPORT_DIRECTORY");
            image_iid_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsAutoTristate | Qt::ItemIsEnabled);
            section->addChild(image_iid_item);
        }

        if(ibrIndex == i){
            QTreeWidgetItem * image_iid_item = new QTreeWidgetItem(MainWindow::itChildItem);
            image_iid_item->setText(0, "IMAGE_BASE_RELOCATION");
            image_iid_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsAutoTristate | Qt::ItemIsEnabled);
            section->addChild(image_iid_item);
        }

        if(irdIndex == i){
            process_res_tree(1, section, (byte *)(globalStore->pefile->getIRD()));
        }
    }
}

void MainWindow::process_pe_ui(){
    delete pe_model;
    pe_model = new PE_UI_Model(globalStore->pefile->getPe32Flag(),globalStore->pefile->getNtHeader32()->OptionalHeader.ImageBase,globalStore->pefile->getPeBuffer(),globalStore->pefile->getPeFileSize(),this);
}

void MainWindow::on_option_triggered()
{
    //TODO 设置
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    QString tmp = item->text(column);
    if(tmp == "IMAGE_DOS_HEADER"){
        set_image_dos_header_ui();
    }
    else if(tmp == "MS-DOS Stub Program"){
        set_image_dos_stub_ui();
    }else if(tmp == "IMAGE_NT_HEADERS"){
        set_image_nt_headers_ui();
    }else if(tmp == "Signature"){
        set_image_dos_signature_ui();
    }
    else if(tmp == "IMAGE_FILE_HEADER"){
        set_image_file_header_ui();
    }
    else if(tmp == "IMAGE_OPTIONAL_HEADER")
        set_image_optional_header_ui();
    else if(tmp.contains("IMAGE_SECTION_HEADER")){
        QStringList name = tmp.split(' ');
        int index = Util::getSectionIndexByName(globalStore->pefile, name[1]);
        set_image_section_header_ui(index);
    }else if(tmp.contains("SECTION") && !tmp.contains("IMAGE")){
        QStringList name = tmp.split(' ');
        int index = Util::getSectionIndexByName(globalStore->pefile, name[1]);
        if(!ui->viewDisassembly->isChecked()){
            set_image_section_ui(index);
        }else{
            if(globalStore->pefile->getSectionHeaderArray()[index].Characteristics & 0x20000000){
                set_code_ui(name[1]);
            }else{
                set_image_section_ui(index);
            }
        }
    }
    else if(tmp == "IMAGE_EXPORT_DIRECTORY"){
        set_ied_ui();
    }
    else if(tmp.contains("IMAGE_THUNK_DATA")){
        QStringList name = tmp.split(' ');
        set_itd_ui(name[1]);
    }
    else if(tmp == "IMAGE_IMPORT_DESCRIPTOR"){
        set_iid_ui();
    }
    else if(tmp == "IMAGE_BASE_RELOCATION"){
        set_ibr_ui();
    }
    else if(tmp.contains("IMAGE_RESOURCE_DIRECTORY_ENTRY")){
            ui->tableView->setModel(Q_NULLPTR);
    }
    else if(tmp.contains("IMAGE_RESOURCE_DIRECTORY")){
        int index = item->data(0,Qt::UserRole).toInt();
        set_resource_directory_ui(index);
    }else if(tmp.contains("IMAGE_RESOURCE_DATA_ENTRY")){
        int index = item->data(0,Qt::UserRole).toInt();
        set_resource_data_entry_ui(index);
    }else if(tmp.contains("IMAGE_RESOURCE_DATA")){
        int index = item->data(0,Qt::UserRole).toInt();
        set_resource_data_ui(index);
    }
    else{
        set_pe_ui();
    }
}

void MainWindow::set_pe_ui(){
    ui->tableView->setModel(pe_model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setShowGrid(false);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
}

void MainWindow::on_viewRVA_triggered()
{
    ui->viewRVA->setChecked(true);
    pe_model->set_addressOp(1);
    ui->viewVA->setChecked(false);
    ui->viewPeOffset->setChecked(false);
    ui->tableView->update();
}

void MainWindow::on_viewPeOffset_triggered()
{
    ui->viewRVA->setChecked(false);
    pe_model->set_addressOp(0);
    ui->viewVA->setChecked(false);
    ui->viewPeOffset->setChecked(true);
    ui->tableView->update();
}

void MainWindow::on_viewVA_triggered()
{
    ui->viewRVA->setChecked(false);
    pe_model->set_addressOp(2);
    ui->viewVA->setChecked(true);
    ui->viewPeOffset->setChecked(false);
}
void MainWindow::set_image_dos_header_ui(){
    ui->tableView->setModel(image_dos_header_model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setShowGrid(false);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
}

void MainWindow::process_image_dos_header_ui(){
    delete image_dos_header_model;
    image_dos_header_model = new IMAGE_DOS_HEADER_Model(globalStore->pefile->getPeBuffer(),this);
}

void MainWindow::process_image_dos_stub_model(){
    delete image_dos_stub_model;
    image_dos_stub_model = new binary_ui_model(globalStore->pefile->getPeBuffer() + sizeof(IMAGE_DOS_HEADER), sizeof (IMAGE_DOS_HEADER),
                                               globalStore->pefile->getImage_Dos_Header()->e_lfanew - sizeof (IMAGE_DOS_HEADER));
}
void MainWindow::set_image_dos_stub_ui(){
    ui->tableView->setModel(image_dos_stub_model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setShowGrid(false);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
}

void MainWindow::set_image_nt_headers_ui(){
    ui->tableView->setModel(image_nt_headers_model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setShowGrid(false);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
}
void MainWindow::process_image_nt_headers_model(){
    delete image_nt_headers_model;
    if(globalStore->pefile->getPe32Flag()){
    image_nt_headers_model = new binary_ui_model(
                (byte *)globalStore->pefile->getNtHeader32(),
                (byte *)globalStore->pefile->getNtHeader32() - globalStore->pefile->getPeBuffer(),
                sizeof (IMAGE_NT_HEADERS32), this);
    }else{
        image_nt_headers_model = new binary_ui_model(
                    (byte *)globalStore->pefile->getNtHeader64(),
                    (byte *)globalStore->pefile->getNtHeader64() - globalStore->pefile->getPeBuffer(),
                    sizeof (IMAGE_NT_HEADERS64), this
                    );
    }
}
void MainWindow::process_image_dos_signature_model(){
    delete image_dos_signature;
    image_dos_signature = new Image_Dos_Signature(
                globalStore->pefile->getImage_Dos_Header()->e_lfanew,
                this);
}
void MainWindow::set_image_dos_signature_ui(){
    ui->tableView->setModel(image_dos_signature);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setShowGrid(false);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
}

void MainWindow::process_image_file_header_model(){
    delete image_file_header_model;
    image_file_header_model = new IMAGE_FILE_HEADER_Model(
                globalStore->pefile->getImage_Dos_Header()->e_lfanew + 4,
                globalStore->pefile->getPeBuffer() +
                globalStore->pefile->getImage_Dos_Header()->e_lfanew + 4,
                this);
}
void MainWindow::set_image_file_header_ui(){
    ui->tableView->setModel(image_file_header_model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setShowGrid(false);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
}

void MainWindow::process_image_optional_header_model(){
    delete image_optional_header_model;
    image_optional_header_model = new IMAGE_OPTIONAL_HEADER_Model(
                globalStore->pefile->getImage_Dos_Header()->e_lfanew + 0x18,
                globalStore->pefile->getPeBuffer() +
                globalStore->pefile->getImage_Dos_Header()->e_lfanew + 0x18,
                globalStore->pefile->getPe32Flag(),
                this
                );
}
void MainWindow::set_image_optional_header_ui(){
    ui->tableView->setModel(image_optional_header_model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setShowGrid(false);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
}

void MainWindow::process_image_section_header_model_list(){
    for(int i = 0; i < image_section_header_model_List.size(); i++){
        delete image_section_header_model_List[i];
    }
    image_section_header_model_List.clear();

    WORD NumberOfSections = 0;
    NumberOfSections=globalStore->pefile->getNtHeader32()->FileHeader.NumberOfSections;
    for(WORD i = 0; i < NumberOfSections; i++){
        IMAGE_SECTION_HEADER_Model * model = new IMAGE_SECTION_HEADER_Model(
                    (byte *)&(globalStore->pefile->getSectionHeaderArray()[i]) - globalStore->pefile->getPeBuffer(),
                    (byte *)&(globalStore->pefile->getSectionHeaderArray()[i]),
                    this
                    );
        image_section_header_model_List.append(model);
    }
}

void MainWindow::set_image_section_header_ui(int index){
    ui->tableView->setModel(image_section_header_model_List[index]);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setShowGrid(false);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
}

void MainWindow::process_image_section_model_list(){
    for(int i = 0; i < image_section_model_list.size(); i++){
        delete image_section_model_list[i];
    }
    image_section_model_list.clear();
    WORD NumberOfSections = 0;
    NumberOfSections=globalStore->pefile->getNtHeader32()->FileHeader.NumberOfSections;
    for(WORD i = 0; i < NumberOfSections; i++){
        byte * tmp = (byte *)(globalStore->pefile->getSectionArray()[i]);

        binary_ui_model * section = new binary_ui_model(
                    tmp,
                    globalStore->pefile->getSectionHeaderArray()[i].PointerToRawData,
                    globalStore->pefile->getSectionHeaderArray()[i].SizeOfRawData,
                    this
                    );
        image_section_model_list.append(section);
    }
}

void MainWindow::set_image_section_ui(int index){
    ui->tableView->setModel(image_section_model_list[index]);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setShowGrid(false);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
}

void MainWindow::on_viewDisassembly_triggered(bool checked)
{
    QTreeWidgetItem * item = ui->treeWidget->currentItem();
    QString tmp = item->text(0);
    if(tmp.contains("SECTION") && !tmp.contains("IMAGE")){
            QStringList name = tmp.split(' ');
            int index = Util::getSectionIndexByName(globalStore->pefile, name[1]);
            if(!checked){
                set_image_section_ui(index);
            }else{
                if(globalStore->pefile->getSectionHeaderArray()[index].Characteristics & 0x20000000){
                    set_code_ui(name[1]);
                }else{
                    set_image_section_ui(index);
                }
            }
        }
}

void MainWindow::process_code_model(){
    auto v1 = Code32_Instr_Model.values();
    for(int i = 0; i < v1.size(); i++){
        delete v1.at(i);
    }
    auto v2 = Code64_Instr_Model.values();
    for(int i = 0; i < v2.size(); i++){
        delete v2.at(i);
    }

    Code32_Instr_Model.clear();
    Code64_Instr_Model.clear();

    int NumberOfSections = globalStore->pefile->getNtHeader32()->FileHeader.NumberOfSections;
    for(int i = 0; i < NumberOfSections; i++){
        //如果可以执行
        if((globalStore->pefile->getSectionHeaderArray()[i].Characteristics & 0x20000000)){
            //32位
            if(globalStore->pefile->getPe32Flag()){
                Code32_Model * tmp = new Code32_Model(
                            Util::Disassembly32(
                            (byte *)globalStore->pefile->getSectionArray()[i],
                            globalStore->pefile->getSectionHeaderArray()[i].SizeOfRawData,
                            globalStore->pefile->getNtHeader32()->OptionalHeader.ImageBase +
                            globalStore->pefile->getSectionHeaderArray()[i].VirtualAddress
                            ), this);
                Code32_Instr_Model.insert(QString(
                                              (char *)globalStore->pefile->getSectionHeaderArray()[i].Name),
                                          tmp);
            }else{
                Code64_Model * tmp = new Code64_Model(
                            Util::Disassembly64(
                            (byte *)globalStore->pefile->getSectionArray()[i],
                            globalStore->pefile->getSectionHeaderArray()[i].Misc.VirtualSize,
                            globalStore->pefile->getNtHeader64()->OptionalHeader.ImageBase +
                            globalStore->pefile->getSectionHeaderArray()[i].VirtualAddress
                            ), this);
                Code64_Instr_Model.insert(QString(
                                              (char *)globalStore->pefile->getSectionHeaderArray()[i].Name),
                                          tmp);
            }
        }
    }
}

void MainWindow::set_code_ui(QString section_name){
    if(globalStore->pefile->getPeBuffer() == Q_NULLPTR){
        return;
    }
    if(globalStore->pefile->getPe32Flag()){
        ui->tableView->setModel(*Code32_Instr_Model.find(section_name));
    }else{
        ui->tableView->setModel(*Code64_Instr_Model.find(section_name));
    }
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
}

void MainWindow::process_iid_model(){
    delete iid_model;
    bool iidExist = false;
    if(globalStore->pefile->getPe32Flag()){
        if(globalStore->pefile->getNtHeader32()->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress !=0){
            iidExist = true;
        }
    }else{
        if(globalStore->pefile->getNtHeader64()->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress != 0){
            iidExist = true;
        }
    }
    if(!iidExist){
        iid_model = Q_NULLPTR;
        return;
    }
    iid_model = new IMAGE_IMPORT_DESCRIPTION_Model(globalStore->pefile,this);
}

void MainWindow::set_iid_ui(){
    if(iid_model == Q_NULLPTR){
        return;
    }
    ui->tableView->setModel(iid_model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
}
void MainWindow::process_itd_model(){
    auto v1 = itd32_Model_Table.values();
    for(int i = 0; i < v1.size(); i++){
        delete v1.at(i);
    }
    auto v2 = itd64_Model_Table.values();
    for(int i = 0; i < v2.size(); i++){
        delete v2.at(i);
    }

    itd64_Model_Table.clear();
    itd32_Model_Table.clear();

    PIMAGE_IMPORT_DESCRIPTOR tmp = Q_NULLPTR;
    byte * base = globalStore->pefile->getPeBuffer();
    if(globalStore->pefile->getPe32Flag()){
        if(globalStore->pefile->getNtHeader32()->OptionalHeader.DataDirectory[1].VirtualAddress == 0){
            return;
        }else{
            tmp = globalStore->pefile->getIIDArray();
            while(tmp->Name != 0){
                QString name = QString((char *)(
                            base + Util::getFileOffsetByRVA(globalStore->pefile, tmp->Name))
                            );

                itd32_Model_Table.insert(name, new IMAGE_THUNK_DATA32_Model(
                                             globalStore->pefile,
                            Util::getFileOffsetByRVA(globalStore->pefile, tmp->FirstThunk),
                            base + Util::getFileOffsetByRVA(globalStore->pefile, tmp->FirstThunk) ,
                            name,
                            this
                            ));
                tmp++;
            }
        }
    }else{
        if(globalStore->pefile->getNtHeader64()->OptionalHeader.DataDirectory[1].VirtualAddress == 0){
            return;
        }else{
            tmp = globalStore->pefile->getIIDArray();
            while(tmp->Name != 0){
                QString name = QString((char *)(
                            base + Util::getFileOffsetByRVA(globalStore->pefile, tmp->Name))
                            );

                itd64_Model_Table.insert(name, new IMAGE_THUNK_DATA64_Model(
                                             globalStore->pefile,
                            Util::getFileOffsetByRVA(globalStore->pefile, tmp->FirstThunk),
                            base + Util::getFileOffsetByRVA(globalStore->pefile, tmp->FirstThunk) ,
                            name,
                            this
                            ));
                tmp++;
            }
        }
    }
}
void MainWindow::set_itd_ui(QString name){
    if(globalStore->pefile->getPe32Flag()){
        ui->tableView->setModel(*itd32_Model_Table.find(name));
        ui->tableView->verticalHeader()->setVisible(false);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
    }else{
        ui->tableView->setModel(*itd64_Model_Table.find(name));
        ui->tableView->verticalHeader()->setVisible(false);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
    }
}

void MainWindow::on_action_process_triggered()
{
    hookMainWindow * h = new hookMainWindow();
    h->show();
    h->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::on_pe_attach_triggered()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("打开可执行文件"));
    fileDialog->setDirectory(".");
    fileDialog->setNameFilter(tr("可执行文件(*.exe)"));
    fileDialog->setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if(fileDialog->exec())
    {
        QString fileName;
        fileNames =  fileDialog->selectedFiles();
    }
    if(fileNames.isEmpty()){
        QMessageBox::information(this, "错误", "您未选择应用程序!");
        return;
    }
    QString exePath = fileNames[0];
    QString result = Util::PeAttach(exePath);
    if(result.isEmpty()){
        QMessageBox::information(this, "成功", "文件附加成功!");
    }else{
        QMessageBox::information(this,"错误","文件附加失败! 失败原因:" + result);
    }
}

void MainWindow::process_ied_model(){
    delete IED_Model;
    if(globalStore->pefile->getIEDArray() != Q_NULLPTR)
        IED_Model = new IMAGE_EXPORT_TABLE_Model(globalStore->pefile, this);
    else
        IED_Model = Q_NULLPTR;
}

void MainWindow::set_ied_ui(){
    if(IED_Model == Q_NULLPTR){
        return;
    }
    ui->tableView->setModel(IED_Model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
}

void MainWindow::process_ibr_model(){
    delete IBR_Model;
    if(globalStore->pefile->getIBR() != Q_NULLPTR)
        IBR_Model = new IMAGE_BASE_RELOCALTION_Model(globalStore->pefile, this);
    else
        IBR_Model = Q_NULLPTR;
}

void MainWindow::set_ibr_ui(){
    if(IBR_Model == Q_NULLPTR){
        return;
    }
    ui->tableView->setModel(IBR_Model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
}

void MainWindow::process_res_tree(int deep, QTreeWidgetItem *parent, byte *inter_data){
    PIMAGE_RESOURCE_DIRECTORY ird = (PIMAGE_RESOURCE_DIRECTORY)inter_data;
    QTreeWidgetItem * ird_item = new QTreeWidgetItem(MainWindow::itGroupItem);
    ird_item->setText(0, "IMAGE_RESOURCE_DIRECTORY");
    IMAGE_RESOURCE_DIR_Model * ird_model = new IMAGE_RESOURCE_DIR_Model((byte *)ird - globalStore->pefile->getPeBuffer(),
                                                                       globalStore->pefile, this);
    ResourceEntryList.append(ird_model);
    ird_item->setData(0, Qt::UserRole, ResourceEntryList.size() - 1);
    parent->addChild(ird_item);

    for(int i = 0; i < ird->NumberOfNamedEntries + ird->NumberOfIdEntries; i++){
        QTreeWidgetItem * item = new QTreeWidgetItem(MainWindow::itGroupItem);
        PIMAGE_RESOURCE_DIRECTORY_ENTRY entry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(inter_data + sizeof (IMAGE_RESOURCE_DIRECTORY) + i * sizeof (IMAGE_RESOURCE_DIRECTORY_ENTRY));
        QString name;

        //如果是字符串指针
        if(entry->Name & 0x80000000){
            PIMAGE_RESOURCE_DIR_STRING_U str_name = (PIMAGE_RESOURCE_DIR_STRING_U)((byte *)globalStore->pefile->getIRD()
                                                             + (entry->Name & 0X7FFFFFFF));
             name = QString::fromUtf16((const ushort *)str_name->NameString, str_name->Length);
        }//ID
        else{
            if(deep == 1){
                name = ParsePe::parseIntToResString(entry->Id);
                if(name == "ID未定义"){
                    name = QString::number(entry->Id);
                }
            }else{
                name = QString::number(entry->Id);
            }
        }

        item->setText(0,"IMAGE_RESOURCE_DIRECTORY_ENTRY " + name);
        ird_item->addChild(item);
        if(entry->OffsetToData & 0x80000000){
            process_res_tree(deep + 1,
                             item,
                             (byte *)globalStore->pefile->getIRD()
                             + (entry->OffsetToData & 0x7fffffff)
                             );

        }else{
            IMAGE_RESOURCE_Data_Entry_Model * model = new
                    IMAGE_RESOURCE_Data_Entry_Model(
                        (byte *)globalStore->pefile->getIRD() - globalStore->pefile->getPeBuffer()
                        + (entry->OffsetToData),
                        globalStore->pefile,
                        this
                        );

            PIMAGE_RESOURCE_DATA_ENTRY irde = (PIMAGE_RESOURCE_DATA_ENTRY)((byte *)globalStore->pefile->getIRD()
                    + (entry->OffsetToData));
            qDebug("%X", irde->Size);
            qDebug("%X", irde->OffsetToData);
            ResourceDataEntryList.append(model);
            QTreeWidgetItem * citem = new QTreeWidgetItem(MainWindow::itGroupItem);
            citem->setText(0, "IMAGE_RESOURCE_DATA_ENTRY");
            citem->setData(0, Qt::UserRole, ResourceDataEntryList.size() - 1);
            item->addChild(citem);

            binary_ui_model * data_model = new binary_ui_model(
                        globalStore->pefile->getPeBuffer() +
                        Util::getFileOffsetByRVA(globalStore->pefile, irde->OffsetToData),
                        Util::getFileOffsetByRVA(globalStore->pefile, irde->OffsetToData),
                        irde->Size,
                        this
                        );
            ResourceDataList.append(data_model);
            QTreeWidgetItem * data_tree_item = new QTreeWidgetItem(MainWindow::itChildItem);
            data_tree_item->setText(0, "IMAGE_RESOURCE_DATA");
            data_tree_item->setData(0, Qt::UserRole, ResourceDataList.size() - 1);
            citem->addChild(data_tree_item);
        }
    }
}

void MainWindow::set_resource_data_ui(int index){
    if(ResourceDataList.size() == 0){
        return;
    }
    ui->tableView->setModel(ResourceDataList[index]);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
}
void MainWindow::set_resource_data_entry_ui(int index){
    if(ResourceDataEntryList.size() == 0){
        return;
    }
    ui->tableView->setModel(ResourceDataEntryList[index]);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
}
void MainWindow::set_resource_directory_ui(int index){
    if(ResourceEntryList.size() == 0){
        return;
    }
    ui->tableView->setModel(ResourceEntryList[index]);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
}

void MainWindow::on_viewDisassembly_triggered()
{

}

void MainWindow::on_save_triggered()
{
    QString result = globalStore->pefile->writeBuffer(globalStore->options->pePath);
    if(result.isEmpty()){
        QMessageBox::information(this,"成功","保存成功!");
    }else{
        QMessageBox::information(this,"错误","保存失败，错误原因: " + result);
    }
}

void MainWindow::on_save_as_triggered()
{
    //TODO:文件另存为
    QString filePath = QFileDialog::getExistingDirectory(this, "文件路径选择", "../");
    filePath = filePath + "/" + Util::getBaseName(globalStore->options->pePath);
    qDebug() << filePath;
    QString result = globalStore->pefile->writeBuffer(filePath);
    if(result.isEmpty()){
        QMessageBox::information(this,"成功","保存成功!");
    }else{
        QMessageBox::information(this,"错误","保存失败，错误原因: " + result);
    }
}

// edited by whl, 双击tableView 进入修改
// 目前实现：二进制model下双击数据的修改；
void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;  // 不合法的index直接退出
    }
    const QAbstractItemModel *abModel = index.model();
    QString modelName = QString(abModel->metaObject()->className());
    // 如果点击了 Section header model 下的元素，且位于 characteristics
    if (modelName == "IMAGE_SECTION_HEADER_Model" && index.row() >= 9)
    {
        // 获取第九行第一列的元素的内容，解析成16进制文件偏移，调用函数
        QModelIndex charsAddressIndex = abModel->index(9, 0);
        quint32 addressOffset = charsAddressIndex.data().toString().toUInt(Q_NULLPTR, 16);
        QModelIndex sectionNameIndex = abModel->index(0,3);  // 获取节的名称
        QString sectionName = sectionNameIndex.data().toString();
        set_section_header_chars(addressOffset, sectionName);
        return;
    }
    // 如果不是二进制类型的model，就返回
    if(modelName != "PE_UI_Model" && modelName != "binary_ui_model")
    {
        return;
    }

    int rowIndex = index.row();
    QModelIndex addressIndex = abModel->index(rowIndex, 0);
    QModelIndex dataIndex = abModel->index(rowIndex, 1);

    // 因为第一列存储的是16进制的字符串，先读出字符串，再按照16进制解析成 uint32
    // QVarient 不能直接按照16进制转 int，所以先转字符串
    quint32 addressOffset = addressIndex.data().toString().toUInt(Q_NULLPTR, 16);
    QString cellText = dataIndex.data().toString();
    //qDebug() << addressOffset;
    //qDebug() << cellText;
    int bytesNum = (cellText.length()-2) / 3 + 1;
    BYTE* chosenAddr = globalStore->pefile->getPeBuffer() + addressOffset;
    //qDebug() << chosenAddr[0];
    //qDebug() << chosenAddr[1];

    //qDebug() << index.row();
    //qDebug() << index.column();
    //qDebug() << index.model();
    ModifyBinaryDialog *modifyBinaryDialog = new ModifyBinaryDialog (this, chosenAddr, addressOffset, bytesNum);
    Qt::WindowFlags dlgFlags = modifyBinaryDialog->windowFlags();
    modifyBinaryDialog->setWindowFlags(dlgFlags | Qt::MSWindowsFixedSizeDialogHint);
    int ret = modifyBinaryDialog->exec();
    // qDebug() << ret;
    // 如果选择保存这16个字节
    if (ret == 1)
    {

        if (GlobalStore::getInstanceP()->pefile->getModifyFlag() == FALSE)
        {
            globalStore->pefile->setMidifyFlag(true);
            GlobalStore::getInstanceP()->pefile->modifyFlagChanged_signal(TRUE);
        }
        // 发出更新UI的信号
     //   GlobalStore::getInstanceP()->pefile->updateFile();
    }
}

// Created by whl, section header characteristics setter

void MainWindow::set_section_header_chars(quint32 charsFileOffset, QString sectionName)
{
    //当文件大于4GB时会出问题！但是这里不考虑
    quint32 *charsAddr = (quint32 *)(GlobalStore::getInstanceP()->pefile->getPeBuffer() + charsFileOffset);
    qDebug() << *charsAddr;
    SectionCharsDialog *sectionCharsDialog = new SectionCharsDialog (this, charsAddr, sectionName);
    Qt::WindowFlags dlgFlags = sectionCharsDialog->windowFlags();
    sectionCharsDialog->setWindowFlags(dlgFlags | Qt::MSWindowsFixedSizeDialogHint);
    int ret = sectionCharsDialog->exec();
    // 如果选择保存当前的 节属性
    if (ret == 1)
    {

        if (GlobalStore::getInstanceP()->pefile->getModifyFlag() == FALSE)
        {
            globalStore->pefile->setMidifyFlag(true);
            GlobalStore::getInstanceP()->pefile->modifyFlagChanged_signal(TRUE);
        }
        // 发出更新UI的信号
        GlobalStore::getInstanceP()->pefile->updateFile();
    }
}

// By whl, 检查证书表  目前 issuer 的显示仍然有问题
void MainWindow::on_checkCertAction_triggered()
{
    peFile *pefile = GlobalStore::getInstanceP()->pefile;
    // 确认是否打开PE文件
    if (pefile->getPeBuffer() == nullptr)
    {
        QMessageBox::information(this, "错误", "请先打开任意PE文件！");
        return;
    }
    if (!GlobalStore::getInstanceP()->pefile->getPe32Flag())
    {
        QMessageBox::information(this, "错误", "由于接口问题，64位PE文件暂时无法查询证书表！");
        return;
    }
    // 确认是否被签名
    if (!pefile->getIfsigned())
    {
        QMessageBox::information(this, "错误", "该程序没有证书！");
        return;
    }

    // 32位情况下做显示
    /*
    Certificate sCert = GlobalStore::getInstanceP()->pefile->getSCert();
    Certificate tCert = GlobalStore::getInstanceP()->pefile->getTCert();
    SYSTEMTIME st = GlobalStore::getInstanceP()->pefile->getDot();


    qDebug()<<"Signer Certificate:";
    qDebug()<<"Serial Number: "<<"0x"<<QByteArray((const char*)sCert.serialNum,sCert.snData).toHex().data();
    qDebug()<<"Issuer Name: "<<  QString::fromWCharArray(sCert.issuerName).toUtf8().data();
    qDebug()<<"Subject Name: "<< QString::fromWCharArray(sCert.subjectName).toUtf8().data();

    qDebug()<<"TimeStamp Certificate:";
    qDebug()<<"Serial Number: "<<"0x"<<QByteArray((const char*)tCert.serialNum,tCert.snData).toHex().data();
    qDebug()<<"Issuer Name: "<<QString::fromWCharArray(tCert.issuerName).toUtf8().data();
    qDebug()<<"Subject Name: "<< QString::fromWCharArray(tCert.subjectName).toUtf8().data();

    qDebug()<<"Date of TimeStamp : "<<st.wMonth<<"/"<< st.wDay<<"/" << st.wYear<< "/" << st.wHour<<":"<<st.wMinute<<endl;


    QString infoStr = "Signer Certificate:\n";
    infoStr += "Serial Number: 0x" + QString(QByteArray((const char*)sCert.serialNum,sCert.snData).toHex().data()) + "\n";
    infoStr += "Issuer Name: " + QString(QString::fromWCharArray(sCert.issuerName).toUtf8().data()) + "\n";
    infoStr += "Subject Name: " + QString(QString::fromWCharArray(sCert.subjectName).toUtf8().data()) + "\n";

    infoStr += "TimeStamp Certificate: \n";
    infoStr += "Serial Number: 0x" + QString(QByteArray((const char*)sCert.serialNum,sCert.snData).toHex().data()) + "\n";
    infoStr += "Issuer Name: " + QString(QString::fromWCharArray(sCert.issuerName).toUtf8().data()) + "\n";
    infoStr += "Subject Name: " + QString(QString::fromWCharArray(sCert.subjectName).toUtf8().data()) + "\n";

    infoStr += "Date of TimeStamp : " + QString::number(st.wMonth) + "/" +  QString::number(st.wDay) + "/" + QString::number(st.wYear) + "/" + QString::number(st.wHour) + ":" + QString::number(st.wMinute) + "\n";

    //QMessageBox::information(this, "证书表", infoStr);

    */
    CertInfoDialog *certInfoDialog = new CertInfoDialog (this);
    Qt::WindowFlags dlgFlags = certInfoDialog->windowFlags();
    certInfoDialog->setWindowFlags(dlgFlags | Qt::MSWindowsFixedSizeDialogHint);
    certInfoDialog->exec();

}

// by Whl 当点击验证签名的时候
void MainWindow::on_actionSignatureVerification_triggered()
{
    peFile *pefile = GlobalStore::getInstanceP()->pefile;

    // 确认是否打开PE文件
    if (pefile->getPeBuffer() == nullptr)
    {
        QMessageBox::information(this, "错误", "请先打开任意PE文件！");
        return;
    }

    QString infoStr = "";
    qDebug() << pefile->getPePath();

    QMessageBox::information(this, "验证结果", pefile->VerifyEmbeddedSignature(pefile->getPePath()));
}

// by whl 当点击修改常用PE属性时
void MainWindow::on_peAttrModAction_triggered()
{
    peFile *pefile = GlobalStore::getInstanceP()->pefile;
    // 确认是否打开PE文件
    if (pefile->getPeBuffer() == nullptr)
    {
        QMessageBox::information(this, "错误", "请先打开任意PE文件！");
        return;
    }

    PeAttrModDialog *peAttrModDialog = new PeAttrModDialog (this);
    Qt::WindowFlags dlgFlags = peAttrModDialog->windowFlags();
    peAttrModDialog->setWindowFlags(dlgFlags | Qt::MSWindowsFixedSizeDialogHint);
    int ret = peAttrModDialog->exec();
    // 如果选择保存当前的 节属性
    if (ret == 1)
    {

        if (GlobalStore::getInstanceP()->pefile->getModifyFlag() == FALSE)
        {
            globalStore->pefile->setMidifyFlag(true);
            GlobalStore::getInstanceP()->pefile->modifyFlagChanged_signal(TRUE);
        }
        // 这里没必要发出更新UI的信号
        // GlobalStore::getInstanceP()->pefile->updateFile();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(globalStore->pefile->getModifyFlag()){
        saveDialog();
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "关于", "这是一个用于 PE 文件分析的工具程序。由 WHU 17级软件安全课程第六组的成员共同编写。\n"
                                         "作者：\n"
                                         "MagicConch\thttps://magicwx.xyz/\n"
                                         "Haulyn Wu\thttps://haulyn5.xyz/\n"
                                         "H.D.C\t\thttps://gitee.com/H_DC\n"
                                         "Loup\t\thttps://gitee.com/puro");
    return;
}

void MainWindow::on_actionhelp_real_triggered()
{
    /*
    QMessageBox::information(this, "帮助", "首先使用 Ctrl + O 快捷键打开 PE 文件。\n"
                                         "PE 文件的结构将在解析后展示在窗口左侧，您可以选择查看任何一个部分。\n"
                                         "程序内容通用修改：如果需要对程序的任意部分进行修改，请在左侧的树结构中选择合适的部分，当右侧出现二进制表示时，则可以双击任意行进行修改。\n"
                                         "节权限修改：在窗口左侧点选任意节头，当右侧解析出节头信息后，在右侧窗口最下方会有区块属性的信息，双击区块属性，或任意一条属性即会弹出对话框引导您对节的属性（包括权限）进行修改。\n"
                                         "查看与修改安全指示：您可以在工具栏的工具箱中点击“PE 常用属性修改”，即可查看修改文件的安全指示。\n"
                                         "查看与修改 OEP 及 ImageBase ：您可以在工具栏的工具箱中点击“PE 常用属性修改”，即可查看修改 OEP 及 ImageBase。\n"
                                         "验证签名、查看证书表：点击工具栏的工具箱即可找到相关的按钮。\n"
                                         "Hooking、Inline Hooking：点击工具栏的进程操作，在弹出的窗口中点选需要的进程，并点击新窗口工具栏中的 Hook即可。\n"
                                         "Shellcode 注入：点击工具栏的文件附加，在弹出的窗口中选择需要注入的PE文件。\n"
                                         "代码节的反汇编：点击工具栏的视图，即可看到选项，打开后，在左侧选择 Section code,右侧窗口即出现反汇编后的代码。\n");
    */
    HelpDialog *helpDialog = new HelpDialog (this);
    Qt::WindowFlags dlgFlags = helpDialog->windowFlags();
    helpDialog->setWindowFlags(dlgFlags | Qt::MSWindowsFixedSizeDialogHint);
    helpDialog->exec();
    return;
}
