QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
PROJECT_VERSION = "0.9.2"

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += TTLV_READER_VERSION=$$PROJECT_VERSION

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    about_dlg.cpp \
    auto_update_service.cpp \
    common.cpp \
    edit_dlg.cpp \
    i18n_helper.cpp \
    insert_data_dlg.cpp \
    main.cpp \
    mainwindow.cpp \
    reader_applet.cpp \
    reader_tree_item.cpp \
    reader_tree_model.cpp \
    reader_tree_view.cpp \
    req_encoder_dlg.cpp \
    send_msg_dlg.cpp \
    settings_dlg.cpp \
    settings_mgr.cpp

HEADERS += \
    about_dlg.h \
    auto_update_service.h \
    common.h \
    edit_dlg.h \
    i18n_helper.h \
    insert_data_dlg.h \
    mainwindow.h \
    reader_applet.h \
    reader_tree_item.h \
    reader_tree_model.h \
    reader_tree_view.h \
    req_encoder_dlg.h \
    send_msg_dlg.h \
    settings_dlg.h \
    settings_mgr.h \
    singleton.h

FORMS += \
    about_dlg.ui \
    edit_dlg.ui \
    insert_data_dlg.ui \
    mainwindow.ui \
    req_encoder_dlg.ui \
    send_msg_dlg.ui \
    settings_dlg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += "../../PKILib"

DEFINES += _AUTO_UPDATE

mac {
    QMAKE_LFLAGS += -Wl,-rpath,@loader_path/../Frameworks
    HEADERS += mac_sparkle_support.h
    OBJECTIVE_SOURCES += mac_sparkle_support.mm
    LIBS += -framework AppKit
    LIBS += -framework Carbon
    LIBS += -framework Foundation
    LIBS += -framework ApplicationServices
    LIBS += -framework Sparkle
    INCLUDEPATH += "/usr/local/Sparkle.framework/Headers"

    INCLUDEPATH += "../../PKILib/lib/mac/debug/openssl3/include"
    INCLUDEPATH += "/usr/local/include"
    LIBS += -L"/usr/local/lib" -lltdl
    LIBS += -L"../../build-PKILib-Desktop_Qt_5_11_3_clang_64bit-Debug" -lPKILib
    LIBS += -L"../../PKILib/lib/mac/debug/openssl3/lib" -lcrypto -lssl
    LIBS += -lldap -llber
}

win32 {
    DEFINES += _AUTO_UPDATE
    RC_ICONS = images/ttlvreader.ico

    contains(QT_ARCH, i386) {
        message( "TTLVReader 32bit" )
        INCLUDEPATH += "../../PKILib/lib/win32/winsparkle/include"
        INCLUDEPATH += "C:\msys64\mingw32\include"

        LIBS += -L"../../PKILib/lib/win32/winsparkle/Release" -lWinSparkle -lws2_32

        Debug {
            LIBS += -L"../../build-PKILib-Desktop_Qt_5_13_2_MinGW_32_bit-Debug/debug" -lPKILib -lws2_32
            LIBS += -L"../../PKILib/lib/win32/debug/openssl3/lib" -lcrypto -lssl
        } else {
            LIBS += -L"../../build-PKILib-Desktop_Qt_5_13_2_MinGW_32_bit-Release/release" -lPKILib -lws2_32
            LIBS += -L"../../PKILib/lib/win32/openssl3/lib" -lcrypto -lssl
        }

        LIBS += -L"C:\msys64\mingw32\lib" -lltdl
    } else {
        message( "TTLVReader 64bit" )
        INCLUDEPATH += "../../PKILib/lib/win64/winsparkle/include"
        INCLUDEPATH += "C:\msys64\mingw64\include"

        LIBS += -L"../../PKILib/lib/win64/winsparkle/x64/Release" -lWinSparkle -lws2_32

        Debug {
            LIBS += -L"../../build-PKILib-Desktop_Qt_5_13_2_MinGW_64_bit-Debug/debug" -lPKILib -lws2_32
            LIBS += -L"../../PKILib/lib/win64/debug/openssl3/lib64" -lcrypto -lssl
        } else {
            LIBS += -L"../../build-PKILib-Desktop_Qt_5_13_2_MinGW_64_bit-Release/release" -lPKILib -lws2_32
            LIBS += -L"../../PKILib/lib/win64/openssl3/lib64" -lcrypto -lssl
        }

        LIBS += -L"C:\msys64\mingw64\lib" -lltdl
    }
}

DISTFILES +=

RESOURCES += \
    ttlvreader.qrc

TRANSLATIONS += i18n/ttlvreader_ko_KR.ts
