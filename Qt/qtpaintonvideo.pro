QT     += widgets
CONFIG += c++11
CONFIG -= app_bundle
TEMPLATE = app
CONFIG(debug, debug|release) {
    LIBS += -L$$OUT_PWD/debug
} else {
    LIBS += -L$$OUT_PWD/release
}
LIBS += -L$$OUT_PWD -lqtmdk

SOURCES += qtpaintonvideo.cpp

mac {
  RPATHDIR *= @executable_path/Frameworks @loader_path
  QMAKE_LFLAGS_SONAME = -Wl,-install_name,@rpath/
  isEmpty(QMAKE_LFLAGS_RPATH): QMAKE_LFLAGS_RPATH=-Wl,-rpath,
  for(R,RPATHDIR) {
    QMAKE_LFLAGS *= \'$${QMAKE_LFLAGS_RPATH}$$R\'
  }
}
