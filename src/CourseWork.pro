#-------------------------------------------------
#
# Project created by QtCreator 2015-12-01T14:08:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

TARGET = CourseWork
TEMPLATE = app


SOURCES += main.cpp\
        view.cpp \
    collections/List.cpp \
    tokenizer.cpp \
    structures/tokendefinition.cpp

HEADERS  += view.h \
    collections/List.h \
    tokenizer.h \
    structures/tokendefinition.h

FORMS    += view.ui
