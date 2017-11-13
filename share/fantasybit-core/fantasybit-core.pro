include ($$PWD/../../pri/deps.pri)
include($$PWD/../../pri/protos.pri)

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
    utils/utils.cpp \
    FantasyAgent.cpp \
    FantasyName.cpp \
    platform.cpp \
    appsettings.cpp \
    Commissioner.cpp \
    ldbwriter.cpp \
    RestfullClient.cpp \
    RestfullService.cpp

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
    utils/utils.h \
    FantasyAgent.h \
    FantasyName.h \
    fbutils.h \
    platform.h \
    appsettings.h \
    globals.h \
    Commissioner.h \
    ldbwriter.h \
    RestfullClient.h \
    RestfullService.h


