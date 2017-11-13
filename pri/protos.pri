include (genproto.pri)
INCLUDEPATH += $$GENERATEDDIR
HEADERS += \
    $$GENERATEDDIR/*.pb.h


SOURCES += \
    $$GENERATEDDIR/*.pb.cc
