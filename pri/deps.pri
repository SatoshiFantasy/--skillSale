include ($$PWD/defs.pri)

##############
## Globals
##############

DIRPREFIX = $$PWD/../protoblock-prebuiltLibs


##############
##  WINDOWS
##############
win32 {
    message(win32 Build)
    INCLUDEPATH += $$DIRPREFIX/windows/3rdParty
    INCLUDEPATH += $$DIRPREFIX/windows/3rdParty/secp256k1

    LIBS+= -L$$DIRPREFIX/windows/libwin64
    CONFIG(debug, debug|release) {
       LIBS+= -llibprotobufd  \
              -lleveldbd \
              -llibeay32 \
              -lssleay32
    }
    CONFIG(release, debug|release) {
       LIBS+= -llibprotobuf \
              -lleveldb \
              -llibeay32 \
              -lssleay32
    }

    DEFINES += GOOGLE_NAMESPACE=google

}

osx {
    QMAKE_MAC_SDK = macosx10.13
}


unix:macx {
    LIBS += $$DIRPREFIX/osx/lib/libleveldb.a
    INCLUDEPATH += $$DIRPREFIX/osx/include/
}

##############
##     OSX
##############

macx{
    message(OSX BUILD)
    INCLUDEPATH += $$DIRPREFIX/osx/include/
    DEPENDPATH += $$DIRPREFIX/osx/include/

    LIBS += $$DIRPREFIX/osx/lib/libprotobuf.a
    PRE_TARGETDEPS += $$DIRPREFIX/osx/lib/libprotobuf.a

    LIBS += $$DIRPREFIX/osx/lib/libssl.a
    PRE_TARGETDEPS += $$DIRPREFIX/osx/lib/libssl.a

    LIBS+=$$DIRPREFIX/osx/lib/libcrypto.a
    PRE_TARGETDEPS+=$$DIRPREFIX/osx/lib/libcrypto.a

    DEFINES += GOOGLE_NAMESPACE=google

}

##############
##     IOS
##############
ios {
    message (IOS BUILD)
    ##PATHS
    INCLUDEPATH += $DIRPREFIX/ios/extrenal/include
    DEPENDPATH += $DIRPREFIX/ios/extrenal/include

    ##OPENSSL
    LIBS +=$DIRPREFIX/ios/extrenal/lib/libcrypto.a
    PRE_TARGETDEPS +=$DIRPREFIX/ios/extrenal/lib/libcrypto.a
    LIBS +=$DIRPREFIX/ios/extrenal/lib/libssl.a
    PRE_TARGETDEPS +=$DIRPREFIX/ios/extrenal/lib/libssl.a

    # SECP251K1
    LIBS +=$DIRPREFIX/ios/extrenal/lib/libsecp256k1.a
    PRE_TARGETDEPS +=$DIRPREFIX/ios/extrenal/lib/libsecp256k1.a

    ## PROTOBUFF
    LIBS +=$DIRPREFIX/ios/extrenal/lib/libprotobuf.a
    PRE_TARGETDEPS +=$DIRPREFIX/ios/extrenal/lib/libprotobuf.a

    DEFINES += GOOGLE_NAMESPACE=google_public

}



##############
##  ANDROID
##############
android {
        message(Android Build)
        ##PATHS
        INCLUDEPATH += $$DIRPREFIX/android/extrenal-android/include
        DEPENDPATH += $$DIRPREFIX/android/extrenal-android/include

        ##OPENSSL
        LIBS += $$DIRPREFIX/android/extrenal-android/lib/libcrypto.a
        PRE_TARGETDEPS += $$DIRPREFIX/android/extrenal-android/lib/libcrypto.a

        LIBS += $$DIRPREFIX/android/extrenal-android/lib/libssl.a
        PRE_TARGETDEPS += $$DIRPREFIX/android/extrenal-android/lib/libssl.a

        ##  SECP251K1
        LIBS +=$$DIRPREFIX/android/extrenal-android/lib/libsecp256k1.a
        PRE_TARGETDEPS += $$DIRPREFIX/android/extrenal-android/lib/libsecp256k1.a

        # PROTOBUFF
        LIBS += $$DIRPREFIX/android/extrenal-android/lib/libprotobuf.so

        DEFINES += GOOGLE_NAMESPACE=google
}



##############
##   LINUX
##############
linux!android{
        message(Linux Build)
        CONFIG += link_pkgconfig
        PKGCONFIG += openssl \
                    protobuf \

        ##  SECP251K1
        LIBS +=$$DIRPREFIX/linux/lib/libsecp256k1.a
        PRE_TARGETDEPS += $$DIRPREFIX/linux/lib/libsecp256k1.a

        DEFINES += GOOGLE_NAMESPACE=google
}

