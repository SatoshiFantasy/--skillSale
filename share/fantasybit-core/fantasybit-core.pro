include ($$PWD/../../pri/deps.pri)

#DEFINES += GOOGLE_NAMESPACE=google

QT += core network websockets
CONFIG += qt c++11 static

TEMPLATE = lib

!contains(DEFINES, PRODFOOTBALL){
    TARGET = STAGING-fantasybit-core
}

contains(DEFINES, PRODFOOTBALL){
    TARGET = fantasybit-core
}



INCLUDEPATH += $$PWD/bitcoin-core-base58
INCLUDEPATH += $$PWD/utils

SOURCES += \
    city.cpp \
    crc.cpp \
    mnemonic.cpp \
    bitcoin-core-base58/base58.cpp \
    utils/utils.cpp

HEADERS += city.hpp \
    DataPersist.h \
    genericsingleton.h \
    mnemonic.h \
    optional.hpp \
    uint128.hpp \
    utility.hpp \
    bitcoin-core-base58/allocators.h \
    bitcoin-core-base58/base58.h \
    bitcoin-core-base58/hash.h \
    utils/utils.h

