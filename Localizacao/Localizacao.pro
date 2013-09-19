#-------------------------------------------------
#
# Project created by QtCreator 2013-06-13T14:57:28
#
#-------------------------------------------------

QT       += core gui

TARGET = Localizacao
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    interval.cpp \
    iboolean.cpp \
    dados.cpp \
    box.cpp \
    particula.cpp \
    landmark.cpp \
    imagem.cpp \
    paletacores.cpp \
    filtroParticulas.cpp

HEADERS  += mainwindow.h \
    interval.h \
    iboolean.h \
    dados.h \
    configuracaoes.h \
    box.h \
    particula.h \
    sivia.h \
    landmark.h \
    imagem.h \
    paletacores.h \
    filtroParticulas.h

FORMS    += mainwindow.ui

CONFIG += link_pkgconfig
PKGCONFIG += /usr/local/share/pkgconfig/ibex.pc
