QT += quick
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Algebra/cintersection.cpp \
        Algebra/cmatrix.cpp \
        Algebra/cpoint.cpp \
        Algebra/cpolynomial.cpp \
        Algebra/csegment.cpp \
        Algebra/cvector2d.cpp \
        Algebra/equal.cpp \
        CSS/block.cpp \
        CSS/cssparser.cpp \
        CSS/measureunit.cpp \
        CSS/style.cpp \
        SVG/Assets/cboundingbox.cpp \
        SVG/Assets/cdef.cpp \
        SVG/Assets/cdefs.cpp \
        SVG/Assets/cpoints.cpp \
        SVG/Assets/cprimitive.cpp \
        SVG/Assets/fgradient.cpp \
        SVG/Classes/cbezier.cpp \
        SVG/Classes/cdoc.cpp \
        SVG/Classes/cgroup.cpp \
        SVG/Classes/cimage.cpp \
        SVG/Classes/cline.cpp \
        SVG/Classes/cpath.cpp \
        SVG/Classes/cpolygon.cpp \
        SVG/Classes/fclippath.cpp \
        SVG/Classes/flineargradient.cpp \
        SVG/Classes/fradialgradient.cpp \
        SVG/Generator/svggenerator.cpp \
        SVG/Parser/svgparser.cpp \
        appcore.cpp \
        main.cpp \
        qmlgenerator.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Algebra/cintersection.h \
    Algebra/cmatrix.h \
    Algebra/cpoint.h \
    Algebra/cpolynomial.h \
    Algebra/csegment.h \
    Algebra/cvector2d.h \
    Algebra/equal.h \
    CSS/block.h \
    CSS/cssparser.h \
    CSS/measureunit.h \
    CSS/style.h \
    SVG/Assets/cboundingbox.h \
    SVG/Assets/cdef.h \
    SVG/Assets/cdefs.h \
    SVG/Assets/cnode.h \
    SVG/Assets/cpoints.h \
    SVG/Assets/cprimitive.h \
    SVG/Assets/fgradient.h \
    SVG/Classes/cbezier.h \
    SVG/Classes/cdoc.h \
    SVG/Classes/cgroup.h \
    SVG/Classes/cimage.h \
    SVG/Classes/cline.h \
    SVG/Classes/cpath.h \
    SVG/Classes/cpolygon.h \
    SVG/Classes/fclippath.h \
    SVG/Classes/flineargradient.h \
    SVG/Classes/fradialgradient.h \
    SVG/Generator/svggenerator.h \
    SVG/Parser/svgparser.h \
    appcore.h \
    qmlgenerator.h

DISTFILES += \
    tests/test1.svg
