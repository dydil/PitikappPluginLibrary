HEADERS += $$files($$PWD/*.h)

INCLUDEPATH += $$PWD

# Link against the library
LIBS += -L$$PWD -lpitikappPlugin

