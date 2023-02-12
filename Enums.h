#ifndef ENUMS_H
#define ENUMS_H

enum eTheme {
    Classic,
    Dark,
    OLED,
    MidnightPurple
};

enum eLanguage {
    French,
    English
};

enum eLog {
    Notice,
    Success,
    Warning,
    Error
};

// Changing the order will break databases
enum eViewType {
    Bluray = 0,
    Television = 1,
    Cinema = 2,
    CinemaReplay = 3,
    DVD = 4,
    Streaming = 5,
    Download = 6,
    VHS = 7,
    Other = 8,
    Unknown = 9,
    UHD = 10,
    MaxViewType = 11
};

enum eChart {
    ViewedByMovieYear,
    ViewedByType,
    ViewedByYear
};

#endif // ENUMS_H
