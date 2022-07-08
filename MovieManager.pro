SOURCES += \
    Dialogs/AddViewDialog.cpp \
    Dialogs/EditViewsDialog.cpp \
    Dialogs/FiltersDialog.cpp \
    Log.cpp \
    MainWindow.cpp \
    main.cpp

HEADERS += \
    Dialogs/AddViewDialog.h \
    Dialogs/EditViewsDialog.h \
    Dialogs/FiltersDialog.h \
    Log.h \
    MainWindow.h

QT += widgets \
      sql

FORMS += \
    Dialogs/AddViewDialog.ui \
    Dialogs/EditViewsDialog.ui \
    Dialogs/FiltersDialog.ui \
    MainWindow.ui
