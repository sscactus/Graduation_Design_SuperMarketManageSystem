#-------------------------------------------------
#
# Project created by QtCreator 2016-03-23T21:02:17
#
#-------------------------------------------------

QT       += core gui sql network
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SuperMarketManageSystem
TEMPLATE = app

#INCLUDEPATH +=../../GraduateDesign/SuperMarketManageSystem/include
#LIBS +=../../GraduateDesign/SuperMarketManageSystem/lib/chartdir51.lib


SOURCES += main.cpp\
        mainwindow.cpp \
    loginwindow.cpp \
    registerwindow.cpp \
    mymenu.cpp \
    mainwindow_sale.cpp \
    global.cpp \
    datedelegate.cpp \
    comboxdelegat.cpp \
    showpassword.cpp \
    memberdialog.cpp \
    querymerchantdialog.cpp \
    piechartwidget.cpp \
    outlibdialog.cpp

HEADERS  += mainwindow.h \
    global.h \
    loginwindow.h \
    registerwindow.h \
    mymenu.h \
    mainwindow_sale.h \
    datedelegate.h \
    comboxdelegat.h \
    showpassword.h \
    memberdialog.h \
    querymerchantdialog.h \
    piechartwidget.h \
    definition.h \
    outlibdialog.h

FORMS    += mainwindow.ui \
    loginwindow.ui \
    registerwindow.ui \
    mainwindow_sale.ui \
    showpassword.ui \
    memberdialog.ui \
    querymerchantdialog.ui \
    outlibdialog.ui
RESOURCES += \
    SMMSimg.qrc
RC_FILE = logo.rc
