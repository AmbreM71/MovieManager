SOURCES += \
    Dialogs/AboutDialog.cpp \
    Dialogs/AddViewDialog.cpp \
    Dialogs/ChartsDialog.cpp \
    Dialogs/EditMovieDialog.cpp \
    Dialogs/EditViewDialog.cpp \
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
    Dialogs/ChartsDialog.h \
    Dialogs/EditMovieDialog.h \
    Dialogs/EditViewDialog.h \
    Dialogs/EditViewsDialog.h \
    Dialogs/FiltersDialog.h \
    Dialogs/LogDialog.h \
    Dialogs/OptionsDialog.h \
    Enums.h \
    Log.h \
    MainWindow.h \
    Structures.h

QT += widgets \
      sql \
      charts

FORMS += \
    Dialogs/AboutDialog.ui \
    Dialogs/AddViewDialog.ui \
    Dialogs/ChartsDialog.ui \
    Dialogs/EditMovieDialog.ui \
    Dialogs/EditViewDialog.ui \
    Dialogs/EditViewsDialog.ui \
    Dialogs/FiltersDialog.ui \
    Dialogs/LogDialog.ui \
    Dialogs/OptionsDialog.ui \
    MainWindow.ui

RESOURCES += \
    MovieManager.qrc

TRANSLATIONS += Localisation/MovieManager_fr_FR.ts \
                Localisation/MovieManager_en_US.ts
