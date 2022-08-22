SOURCES += \
    Dialogs/AboutDialog.cpp \
    Dialogs/AddViewDialog.cpp \
    Dialogs/EditMovieDialog.cpp \
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
    Dialogs/EditMovieDialog.h \
    Dialogs/EditViewsDialog.h \
    Dialogs/FiltersDialog.h \
    Dialogs/LogDialog.h \
    Dialogs/OptionsDialog.h \
    Enums.h \
    Log.h \
    MainWindow.h

QT += widgets \
      sql

FORMS += \
    Dialogs/AboutDialog.ui \
    Dialogs/AddViewDialog.ui \
    Dialogs/EditMovieDialog.ui \
    Dialogs/EditViewsDialog.ui \
    Dialogs/FiltersDialog.ui \
    Dialogs/LogDialog.ui \
    Dialogs/OptionsDialog.ui \
    MainWindow.ui

RESOURCES += \
    MovieManager.qrc

TRANSLATIONS += Localisation/MovieManager_fr_FR.ts \
                Localisation/MovieManager_en_US.ts
