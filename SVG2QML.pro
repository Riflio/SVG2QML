QT += quick

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        Algebra/cintersection.cpp \
        Algebra/cmatrix.cpp \
        Algebra/cpoint.cpp \
        Algebra/cpolynomial.cpp \
        Algebra/csegment.cpp \
        Algebra/csize.cpp \
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
        SVG/Classes/carc.cpp \
        SVG/Classes/cbezier.cpp \
        SVG/Classes/ccircle.cpp \
        SVG/Classes/cdoc.cpp \
        SVG/Classes/cellipse.cpp \
        SVG/Classes/cgroup.cpp \
        SVG/Classes/cimage.cpp \
        SVG/Classes/cline.cpp \
        SVG/Classes/cpath.cpp \
        SVG/Classes/cpolygon.cpp \
        SVG/Classes/cpolyline.cpp \
        SVG/Classes/crect.cpp \
        SVG/Classes/csvg.cpp \
        SVG/Classes/fclippath.cpp \
        SVG/Classes/flineargradient.cpp \
        SVG/Classes/fprimitive.cpp \
        SVG/Classes/fradialgradient.cpp \
        SVG/Generator/svggenerator.cpp \
        SVG/Parser/svgparser.cpp \
        appcore.cpp \
        main.cpp \
        qmlgenerator.cpp

RESOURCES += qml.qrc

HEADERS += \
    Algebra/cempty.h \
    Algebra/cintersection.h \
    Algebra/cmatrix.h \
    Algebra/cpoint.h \
    Algebra/cpolynomial.h \
    Algebra/csegment.h \
    Algebra/csize.h \
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
    SVG/Classes/carc.h \
    SVG/Classes/cbezier.h \
    SVG/Classes/ccircle.h \
    SVG/Classes/cdoc.h \
    SVG/Classes/cellipse.h \
    SVG/Classes/cgroup.h \
    SVG/Classes/cimage.h \
    SVG/Classes/cline.h \
    SVG/Classes/cpath.h \
    SVG/Classes/cpolygon.h \
    SVG/Classes/cpolyline.h \
    SVG/Classes/crect.h \
    SVG/Classes/csvg.h \
    SVG/Classes/fclippath.h \
    SVG/Classes/flineargradient.h \
    SVG/Classes/fprimitive.h \
    SVG/Classes/fradialgradient.h \
    SVG/Generator/svggenerator.h \
    SVG/Parser/svgparser.h \
    appcore.h \
    qmlgenerator.h

DISTFILES += \    
    mask.frag.qsb \
    readme.md \

