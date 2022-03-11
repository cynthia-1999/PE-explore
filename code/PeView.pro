QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    binary_ui_model.cpp \
    certinfodialog.cpp \
    code32_model.cpp \
    code64_model.cpp \
    globalstore.cpp \
    helpdialog.cpp \
    hookSrc/cpu.cpp \
    hookSrc/depends.cpp \
    hookSrc/hookUtil.cpp \
    hookSrc/hookWindow.cpp \
    hookSrc/memory.cpp \
    hookSrc/monitor.cpp \
    hookSrc/resource.cpp \
    image_base_relocaltion_model.cpp \
    image_dos_header_model.cpp \
    image_dos_signature.cpp \
    image_export_table_model.cpp \
    image_file_header_model.cpp \
    image_import_descriptor_model.cpp \
    image_optionsal_header_model.cpp \
    image_resource_data_entry_model.cpp \
    image_resource_dir_model.cpp \
    image_section_header.cpp \
    image_thunk_data32_model.cpp \
    image_thunk_data64_model.cpp \
    main.cpp \
    mainwindow.cpp \
    modifybinarydialog.cpp \
    parsepe.cpp \
    pe_ui_model.cpp \
    peattrmoddialog.cpp \
    pefile.cpp \
    sectioncharsdialog.cpp \
    util.cpp

HEADERS += \
    binary_ui_model.h \
    certinfodialog.h \
    code32_model.h \
    code64_model.h \
    globalstore.h \
    headers.h \
    helpdialog.h \
    hookSrc/cpu.h \
    hookSrc/depends.h \
    hookSrc/hookUtil.h \
    hookSrc/hookWindow.h \
    hookSrc/memory.h \
    hookSrc/monitor.h \
    hookSrc/resource.h \
    image_base_relocaltion_model.h \
    image_dos_header_model.h \
    image_dos_signature.h \
    image_export_table_model.h \
    image_file_header_model.h \
    image_import_descriptor_model.h \
    image_optionsal_header_model.h \
    image_resource_data_entry_model.h \
    image_resource_dir_model.h \
    image_section_header.h \
    image_thunk_data32_model.h \
    image_thunk_data64_model.h \
    mainwindow.h \
    modifybinarydialog.h \
    options.h \
    parsepe.h \
    pe_ui_model.h \
    peattrmoddialog.h \
    pefile.h \
    sectioncharsdialog.h \
    util.h

FORMS += \
    certinfodialog.ui \
    helpdialog.ui \
    hookSrc/depends.ui \
    hookSrc/hookWindow.ui \
    hookSrc/monitor.ui \
    mainwindow.ui \
    modifybinarydialog.ui \
    peattrmoddialog.ui \
    sectioncharsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -lpsapi
LIBS += -lcrypt32
LIBS += -lwintrust

win32: LIBS += -L$$PWD/lib/ -lBeaEngine

INCLUDEPATH += $$PWD/libHeaders
DEPENDPATH += $$PWD/libHeaders

win32: LIBS += -L$$PWD/lib/ -lvirus_calc

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
