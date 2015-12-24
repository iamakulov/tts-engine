#-------------------------------------------------
#
# Project created by QtCreator 2015-12-01T14:08:55
#
#-------------------------------------------------

QT       += core gui script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

TARGET = CourseWork
TEMPLATE = app


SOURCES += main.cpp\
        view.cpp \
    collections/List.cpp \
    tokenizer.cpp \
    structures/tokendefinition.cpp \
    structures/token.cpp \
    transformer.cpp \
    structures/transformationrule.cpp \
    loaders/tokendefinitionloader.cpp

HEADERS  += view.h \
    collections/List.h \
    tokenizer.h \
    structures/tokendefinition.h \
    structures/token.h \
    transformer.h \
    structures/transformationrule.h \
    loaders/tokendefinitionloader.h

FORMS    += view.ui
