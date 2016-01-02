#-------------------------------------------------
#
# Project created by QtCreator 2015-12-01T14:08:55
#
#-------------------------------------------------

QT       += core gui script multimedia

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
    controller.cpp \
    transformer.cpp \
    structures/transformationrule.cpp \
    loaders/tokendefinitionloader.cpp \
    loaders/transformationruleloader.cpp \
    loaders/ltsruleloader.cpp \
    structures/ltsrule.cpp \
    reassembler.cpp \
    transcriber.cpp \
    lettersplitter.cpp \
    phonetizer.cpp \
    audioconvertor.cpp \
    structures/soundtoaudiorule.cpp \
    structures/audiodefinition.cpp \
    player.cpp \
    audioqueue.cpp \
    loaders/soundtoaudioruleloader.cpp

HEADERS  += view.h \
    collections/List.h \
    tokenizer.h \
    structures/tokendefinition.h \
    structures/token.h \
    controller.h \
    transformer.h \
    structures/transformationrule.h \
    loaders/tokendefinitionloader.h \
    loaders/transformationruleloader.h \
    loaders/ltsruleloader.h \
    structures/ltsrule.h \
    reassembler.h \
    transcriber.h \
    lettersplitter.h \
    phonetizer.h \
    audioconvertor.h \
    structures/soundtoaudiorule.h \
    structures/audiodefinition.h \
    player.h \
    audioqueue.h \
    loaders/soundtoaudioruleloader.h

FORMS    += view.ui
