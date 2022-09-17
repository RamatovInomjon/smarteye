QT       += core gui
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QTPLUGIN += QPSQL

#include(../QtTelegramBot/QtTelegramBot.pri)


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CaptureThread.cpp \
    aboutusfrm.cpp \
    camera_data.cpp \
    camera_model.cpp \
    cameras.cpp \
    database.cpp \
    imagerec.cpp \
    main.cpp \
    mainwindow.cpp \
    myutils.cpp \
    onecameraform.cpp \
    onecameraframe.cpp \
    onecameraviewform.cpp \
    photo_data.cpp \
    photo_model.cpp \
    popup.cpp \
    rerportfrm.cpp \
    settingfrm.cpp \
    video_data.cpp \
    video_model.cpp

HEADERS += \
    CaptureThread.h \
    Crypter.h \
    aboutusfrm.h \
    camera_data.h \
    camera_model.h \
    cameras.h \
    database.h \
    imagerec.h \
    mainwindow.h \
    myutils.h \
    onecameraform.h \
    onecameraframe.h \
    onecameraviewform.h \
    photo_data.h \
    photo_model.h \
    popup.h \
    rerportfrm.h \
    settingfrm.h \
    video_data.h \
    video_model.h

FORMS += \
    aboutusfrm.ui \
    cameras.ui \
    mainwindow.ui \
    onecameraform.ui \
    onecameraframe.ui \
    onecameraviewform.ui \
    rerportfrm.ui \
    settingfrm.ui

#INCLUDEPATH += C:/opencv/build/include/
#LIBS += \
#    -LC:/opencv/build/x64/vc15/lib \
#    -LC:/opencv/build/x64/vc15/bin

#CONFIG(debug, debug|release) {
#    LIBS += -lopencv_world454d
#}
#CONFIG(release, debug|release) {
#    LIBS += -lopencv_world454
#}


#LIBS+=$$PWD/lib/HCCore.lib
#LIBS+=$$PWD/lib/HCNetSDK.lib
#LIBS+=$$PWD/lib/PlayCtrl.lib
#LIBS+=$$PWD/lib/GdiPlus.lib

#LIBS+=$$PWD/lib/HCAlarm.lib
#LIBS+=$$PWD/lib/HCGeneralCfgMgr.lib
#LIBS+=$$PWD/lib/HCPreview.lib

#INCLUDEPATH += $$PWD\incEn
#DISTFILES += \
#    lib/GdiPlus.lib \
#    lib/HCAlarm.lib \
#    lib/HCCore.lib \
#    lib/HCGeneralCfgMgr.lib \
#    lib/HCNetSDK.lib \
#    lib/HCPreview.lib \
#    lib/PlayCtrl.lib

INCLUDEPATH += /usr/local/include/opencv4

LIBS += -I/usr/include/opencv4 -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_barcode -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_mcc -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_shape -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_ml -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_wechat_qrcode -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core
LIBS += -L/home/inomjon/project/textile_new/lib -lhcnetsdk -lPlayCtrl -lAudioRender -lSuperRender
LIBS += -L/home/inomjon/project/textile_new/lib/HCNetSDKCom -lhcnetsdk


INCLUDEPATH += /home/inomjon/project/SmartEye/incEn
DEPENDPATH += /home/inomjon/project/SmartEye/incEn

RESOURCES += \
    ipfreely.qrc



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
