# -------------------------------------------------
# Project created by QtCreator 2009-06-30T15:11:01
# -------------------------------------------------
QT += opengl
TARGET = project6
CONFIG += console
TEMPLATE = app
win32:DEFINES += _STDCALL_SUPPORTED \
    _M_IX86 \
    BOOST_ALL_NO_LIB \
    PCRE_STATIC \
    DOM_INCLUDE_LIBXML
win32:SOURCES += ./src/QBthread.cpp \
    ./src/QBtooth.cpp \
    src/ffmpegreader.cpp \
    src/ffvideothread.cpp
SOURCES += ./src/main.cpp \
    ./src/Utils3D.cpp \
    ./src/Object3D.cpp \
    ./src/Engine3D.cpp \
    ./src/ColladaReader.cpp \
    src/Widget3D.cpp \
    src/MenuItem.cpp \
    src/MenuManager.cpp \
    src/imagewidget.cpp \
    src/routegraph.cpp \
    src/routeedge.cpp \
    src/routenode.cpp \
    src/RadioButton2D.cpp \
    src/Button2D.cpp \
    src/DistanceSelector.cpp \
    src/OptionsSelector.cpp \
    src/CheckBox.cpp \
    src/CheckBoxItem.cpp \
    src/Slider2D.cpp \
    src/PricesSelector.cpp \
    src/SliderBox.cpp \
    src/EditItem.cpp \
    src/EditBox.cpp \
    src/NameSelector.cpp \
    src/FilterSelector.cpp \
    src/FilterItem.cpp \
    src/FilterBox.cpp \
    src/Dialog2D.cpp \
    src/SearchFilter.cpp \
    src/OptionsFilter.cpp \
    src/NameFilter.cpp \
    src/DistanceFilter.cpp \
    src/PricesFilter.cpp \
    src/Destination.cpp \
    src/DestinationBar.cpp \
    src/SearchResSelector.cpp \
    src/ResultItem.cpp \
    src/ResultManager.cpp \
    src/Popup.cpp \
    src/IconButton.cpp
win32:INCLUDEPATH += "./inc/boost-inc" \
    "./inc/SDL"
INCLUDEPATH += "./inc" \
    "./inc/collada-inc" \
    "./inc/collada-inc/1.4"
win32:HEADERS += ./inc/TextureLoader.h \
    ./inc/QBthread.h \
    ./inc/QBtooth.h \
    inc/ffmpegreader.h \
    inc/ffvideothread.h
HEADERS += ./inc/SceneManager.h \
    ./inc/Object3D.h \
    ./inc/errors.h \
    ./inc/Engine3D.h \
    ./inc/definesall.h \
    ./inc/ColladaReader.h \
    ./inc/Utils3D.h \
    inc/Widget3D.h \
    inc/MenuItem.h \
    inc/MenuManager.h \
    inc/imagewidget.h \
    inc/routegraph.h \
    inc/routeedge.h \
    inc/routenode.h \
    inc/RadioButton2D.h \
    inc/Button2D.h \
    inc/DistanceSelector.h \
    inc/OptionsSelector.h \
    inc/CheckBox.h \
    inc/CheckBoxItem.h \
    inc/Slider2D.h \
    inc/PricesSelector.h \
    inc/SliderBox.h \
    inc/EditItem.h \
    inc/EditBox.h \
    inc/NameSelector.h \
    inc/FilterSelector.h \
    inc/FilterItem.h \
    inc/FilterBox.h \
    inc/Dialog2D.h \
    inc/SearchFilter.h \
    inc/DistanceFilter.h \
    inc/PricesFilter.h \
    inc/OptionsFilter.h \
    inc/NameFilter.h \
    inc/Destination.h \
    inc/DestinationBar.h \
    inc/SearchResSelector.h \
    inc/ResultItem.h \
    inc/ResultManager.h \
    inc/Popup.h \
    inc/IconButton.h \
    inc/DummyDefinitions.h
win32:LIBS += -L"./lib/SDL" \
    ./lib/collada/libcollada14dom.a \
    ./lib/collada/libboost_system.a \
    ./lib/collada/libboost_filesystem.a \
    ./lib/collada/libxml2.a \
    ./lib/collada/libminizips.a \
    ./lib/collada/libz.a \
    ./lib/collada/libpcrecpp.a \
    ./lib/collada/libpcre.a \
    ./lib/ffmpeg/libavformat.a \
    ./lib/ffmpeg/libavcodec.a \
    ./lib/ffmpeg/libavutil.a \
    ./lib/ffmpeg/libswscale.a \
    -lSDLmain \
    -lSDL \
    -lws2_32 \
    ./lib/libTextureLoader.a \
    -lglu32 \
    -lopengl32 \
    -loleaut32 \
    -luuid \
    -lole32 \
    -lgdi32 \
    ./lib/libqextserialportd.a \
    -mwindows
!win32:LIBS += -L"." ./lib/collada/linux64/libcollada14dom.a \
    -lboost_system-mt \
    -lboost_filesystem-mt \
    -lxml2 \
    ./lib/collada/linux64/libminizip.a \
    -lpcrecpp \
    -lpcre

DESTDIR=./bin
OBJECTS_DIR=./obj
