QT += widgets
QT += gui core
INCLUDEPATH += E:/Qt/boost_1_59_0
LIBS +=  -LE:\Qt\boost_lib\boost\bin.v2\libs\serialization\build\gcc-mingw-4.9.2\release\link-static\threading-multi\
        -lboost_serialization-mgw49-mt-1_59
HEADERS += \
    gameboard.h \
    game.h

SOURCES += \
    main.cpp \
    gameboard.cpp \
    game.cpp
