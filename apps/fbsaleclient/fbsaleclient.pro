include ($$PWD/../../pri/deps.pri)
include ($$PWD/../../pri/artwork.pri)
#include($$PWD/../../pri/protos.pri)
include($$PWD/../../pri/genproto.pri)
include($$PWD/../../pri/macrosAndModels.pri)

#QT += quick
CONFIG += c++11
QT += qml quick core websockets network xmlpatterns qml-private quick-private
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/runguard.cpp \
    $$PWD/src/clientgateway.cpp \
    $$PWD/sm/CoinSale_sm.cpp \
    $$PWD/src/CoinSale.cpp



HEADERS += $$PWD/src/runguard.h \
    $$PWD/src/clientgateway.h \
    $$PWD/sm/CoinSale_sm.h  \
    $$PWD/sm/statemap.h  \
    $$PWD/src/CoinSale.h


FANTASYBITLIB += fantasybit-core
!contains(DEFINES, PRODFOOTBALL){
    FBCOREPATH = STAGING-$${FANTASYBITLIB}
}

FBCOREPATH = $$OUT_PWD/../../share/$$FANTASYBITLIB


win32:CONFIG(release, debug|release): LIBS += -L$$FBCOREPATH/release/ -l$$FANTASYBITLIB
else:win32:CONFIG(debug, debug|release): LIBS += -L$$FBCOREPATH/debug/ -l$$FANTASYBITLIB
else:macx: LIBS += -L$$FBCOREPATH/ -l$$FANTASYBITLIB


win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$FBCOREPATH/release/lib$${FANTASYBITLIB}.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$FBCOREPATH/debug/lib$${FANTASYBITLIB}.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$FBCOREPATH/release/$${FANTASYBITLIB}.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$FBCOREPATH/debug/$${FANTASYBITLIB}.lib
else:macx: PRE_TARGETDEPS += $$FBCOREPATH6/lib$${FANTASYBITLIB}.a

message (PRE_TARGETDEPS $$PRE_TARGETDEPS)

INCLUDEPATH  +=  $$PWD/src $$PWD/sm
INCLUDEPATH  += $$PWD/../../share/fantasybit-core
INCLUDEPATH  += $$PWD/../../share/fantasybit-core/bitcoin-core-base58
INCLUDEPATH  += $$PWD/../../share/fantasybit-core/utils

RESOURCES += $$PWD/qml/qml.qrc

OTHER_FILES += $$PWD/sm/CoinSale.sm


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = . ..

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

TARGET = fbsaleclient
