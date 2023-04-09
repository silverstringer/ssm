#-------------------------------------------------
#
# Project created by QtCreator 2022-02-03T16:21:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StrategyStockMarket
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += $$PWD/Model
INCLUDEPATH += $$PWD/common

CONFIG += c++17

SOURCES += \
       $$files(*.cpp) \
       $$files(Model/*.cpp) \
       $$files(common/*.cpp) \
       $$files(lib/*.cpp)

HEADERS += \
       $$files(*.h) \
       $$files(Model/*.h) \
       $$files(common/*.h) \
       $$files(lib/*.h)

FORMS += \
        common/mainwindow.ui

PROJECT_ROOT_PATH = $${PWD}/

CONFIG(debug, debug|release) {
    BUILD_FLAG = debug
    LIB_SUFFIX = d
} else {
    BUILD_FLAG = release
}


BUILD_PATH = $${PROJECT_ROOT_PATH}/build/$${BUILD_FLAG}/$${TARGET}/
RCC_DIR = $${BUILD_PATH}/rcc/
UI_DIR = $${BUILD_PATH}/ui/
MOC_DIR = $${BUILD_PATH}/moc/
OBJECTS_DIR = $${BUILD_PATH}/obj/
