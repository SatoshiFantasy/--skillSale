#DIRPREFIX = $$PWD/../protoblock-prebuiltLibs

osx{
#    DEFINES += GOOGLE_NAMESPACE=google
    GENERATEDDIR = $$PWD/../generated
    INCLUDEPATH += $$DIRPREFIX/osx/include
    DEPENDPATH+=  $$DIRPREFIX/osx/include
}

ios{
#    DEFINES += GOOGLE_NAMESPACE=google_public
    GENERATEDDIR = $$PWD/../generated-public
    INCLUDEPATH +=  $$DIRPREFIX/ios/extrenal/include
    DEPENDPATH += $$DIRPREFIX/ios/extrenal/include

}

android{
#    DEFINES += GOOGLE_NAMESPACE=google
    GENERATEDDIR = $$PWD/../generated
}

win32{
    GENERATEDDIR = $$PWD/../generated
#    DEFINES += GOOGLE_NAMESPACE=google
}


INCLUDEPATH += $$GENERATEDDIR
