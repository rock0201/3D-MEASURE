QT       += core gui
QT       += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    photo.cpp \
    recalibrate.cpp \
    selectimg.cpp \
    mylabel.cpp \
    drawmodelthread.cpp \
    glmodelview.cpp

HEADERS += \
    mainwindow.h \
    photo.h \
    recalibrate.h \
    selectimg.h \
    mylabel.h \
    drawmodel.h \
    drawmodelthread.h \
    glmodelview.h

FORMS += \
    mainwindow.ui \
    photo.ui \
    recalibrate.ui \
    selectimg.ui

TRANSLATIONS += \
    measure1_zh_CN.ts
INCLUDEPATH += /usr/local/include \
               /usr/local/include/opencv \
               /usr/local/include/opencv2 \
               /home/rock/slamcode/SungemSDK-Cpp-master \
               /home/rock/slamcode/SungemSDK-Cpp-master/libhs_highapi \
               /home/rock/slamcode/SungemSDK-Cpp-master/SungemSDK/include \
               /home/rock/Pangolin/include \
               /usr/local/include/eigen3 \
              /home/rock/slamcode/SungemSDK-Cpp-master/libhs_highapi/include

LIBS += /usr/local/lib/libopencv_calib3d.so \
/usr/local/lib/libopencv_core.so \
/usr/local/lib/libopencv_features2d.so \
/usr/local/lib/libopencv_flann.so \
/usr/local/lib/libopencv_highgui.so \
/usr/local/lib/libopencv_imgcodecs.so \
/usr/local/lib/libopencv_imgproc.so \
/usr/local/lib/libopencv_ml.so \
/usr/local/lib/libopencv_objdetect.so \
/usr/local/lib/libopencv_photo.so \
/usr/local/lib/libopencv_shape.so \
/usr/local/lib/libopencv_stitching.so \
/usr/local/lib/libopencv_superres.so \
/usr/local/lib/libopencv_videoio.so \
/usr/local/lib/libopencv_video.so \
/usr/local/lib/libopencv_videostab.so

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Pangolin/build/src/release/ -lpangolin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Pangolin/build/src/debug/ -lpangolin
else:unix: LIBS += -L$$PWD/../Pangolin/build/src/ -lpangolin

INCLUDEPATH += $$PWD/../Pangolin/build/src/include
DEPENDPATH += $$PWD/../Pangolin/build/src/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/release/ -lGLEW
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/debug/ -lGLEW
else:unix: LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/ -lGLEW

INCLUDEPATH += $$PWD/../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../usr/lib/x86_64-linux-gnu

RESOURCES += \
    lable.qrc

DISTFILES += \
    2020-09-17 10-12-09屏幕截图.png
