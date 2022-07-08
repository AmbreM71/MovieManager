SOURCES += \
    Dialogs/AboutDialog.cpp \
    Dialogs/AddViewDialog.cpp \
    Dialogs/EditViewsDialog.cpp \
    Dialogs/FiltersDialog.cpp \
    Dialogs/LogDialog.cpp \
    Dialogs/OptionsDialog.cpp \
    Log.cpp \
    MainWindow.cpp \
    main.cpp

HEADERS += \
    Dialogs/AboutDialog.h \
    Dialogs/AddViewDialog.h \
    Dialogs/EditViewsDialog.h \
    Dialogs/FiltersDialog.h \
    Dialogs/LogDialog.h \
    Dialogs/OptionsDialog.h \
    Log.h \
    MainWindow.h

QT += widgets \
      sql

FORMS += \
    Dialogs/AboutDialog.ui \
    Dialogs/AddViewDialog.ui \
    Dialogs/EditViewsDialog.ui \
    Dialogs/FiltersDialog.ui \
    Dialogs/LogDialog.ui \
    Dialogs/OptionsDialog.ui \
    MainWindow.ui
