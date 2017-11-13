CONFIG += ordered
TEMPLATE = subdirs

SUBDIRS += \
    $$PWD/genprotos/genprotos.pro \
    $$PWD/apps \
    $$PWD/share

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS



OTHER_FILES += \
            $$PWD/README.md \
            $$PWD/.gitignore \
            $$PWD/TODO \
            $$PWD/pri/genproto.pri \
            $$PWD/pri/deps.pri \
#            $$PWD/pri/macrosAndModels.pri \
            $$PWD/pri/protos.pri \
#            $$PWD/pri/deployment.pri \
            $$PWD/pri/artwork.pri \
#            $$PWD/pri/staticJson.pri \
#            $$PWD/pri/qml-pages.pri \
#            $$PWD/pri/qml-utils.pri
#            $$PWD/pri/qml-realChat.pri

#OTHER_FILES += $$PWD/protos/*.proto

ios {
    IPHONEOS_DEPLOYMENT_TARGET=10.3
}
