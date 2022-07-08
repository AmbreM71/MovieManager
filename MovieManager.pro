SOURCES += \
    Dialogs/AddViewDialog.cpp \
    Dialogs/EditViewsDialog.cpp \
    Dialogs/FiltersDialog.cpp \
    Dialogs/LogDialog.cpp \
    Log.cpp \
    MainWindow.cpp \
    main.cpp

HEADERS += \
    Dialogs/AddViewDialog.h \
    Dialogs/EditViewsDialog.h \
    Dialogs/FiltersDialog.h \
    Dialogs/LogDialog.h \
    Log.h \
    MainWindow.h

QT += widgets \
      sql

FORMS += \
    Dialogs/AddViewDialog.ui \
    Dialogs/EditViewsDialog.ui \
    Dialogs/FiltersDialog.ui \
    Dialogs/LogDialog.ui \
    MainWindow.ui
