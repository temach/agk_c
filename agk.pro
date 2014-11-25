TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    quaternion.c \
    vector3.c \
    matrix44.c

QMAKE_LFLAGS += -pg

QMAKE_CFLAGS        += -v -std=c11 -pedantic -Wextra -Wall -W -Wdeclaration-after-statement \
    -Weffc++ -Wpointer-arith -Wcast-qual -Wmissing-prototypes \
    -pedantic-errors -Waggregate-return \
    -Wchar-subscripts  -Wcomment -Wconversion \
    -Wdisabled-optimization \
    -Wformat=2 -Wmultichar \
    -Wformat-nonliteral -Wformat-security  \
    -Wformat-y2k \
    -Wimplicit  -Wimport  -Winit-self  -Winline \
    -Winvalid-pch   \
    -Wunsafe-loop-optimizations  -Wlong-long -Wmissing-braces \
    -Wmissing-field-initializers -Wmissing-format-attribute   \
    -Wmissing-include-dirs -Wmissing-noreturn \
    -Wpacked -Wparentheses \
    -Wredundant-decls -Wreturn-type \
    -Wsequence-point -Wsign-compare  -Wstack-protector \
    -Wstrict-aliasing=1 -Wswitch \
    -Wswitch-enum -Wtrigraphs  -Wuninitialized \
    -Wunknown-pragmas  -Wunreachable-code -Wunused \
    -Wunused-function  -Wunused-label  -Wunused-parameter \
    -Wunused-value  -Wunused-variable  -Wvariadic-macros \
    -Wvolatile-register-var  -Wwrite-strings \
    -Wno-missing-braces -Wno-missing-field-initializers \
    -Wswitch-default -Wcast-align \
    -Wbad-function-cast -Wstrict-overflow=5 -Wstrict-prototypes \
    -Wundef -Wnested-externs -Wshadow \
    -Wlogical-op -Wfloat-equal \
    -Wold-style-definition -Wno-padded \
    -g -pg -g3 -ggdb3 \
    -O0 \
    -fno-omit-frame-pointer -fno-common -fstrict-aliasing -fstrict-overflow \
    -I/usr/include/glib-2.0/ -I/usr/include/glib-2.0/glib/ -I/usr/lib/x86_64-linux-gnu/glib-2.0/include \
    -Wno-aggregate-return


LIBS += -I/usr/include/glib-2.0/ -I/usr/include/glib-2.0/glib/ -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -L/usr/local/lib -pthread -lglib-2.0 -lpcre -lm -lc

CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0


include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    quaternion.h \
    vector3.h \
    matrix44.h

