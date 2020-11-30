# cholmod 5.6
# Install in manjaro
# sudo pacman -S suitesparse

unix:!mac {
INCLUDEPATH += \
    /usr/include/suitesparse

LIBS += \
    -L/usr/lib -lamd -lcamd -lccolamd -lcholmod -lcolamd -lcxsparse -lblas
}
