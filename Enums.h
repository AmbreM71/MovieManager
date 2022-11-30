#ifndef ENUMS_H
#define ENUMS_H

enum Theme {
    Classic,
    Dark,
    OLED,
    MidnightPurple
};

enum Language {
    French,
    English
};

enum LogCriticity {
    Notice,
    Success,
    Warning,
    Error
};

// Changing the order will break databases
enum ViewType {
    Bluray,
    Television,
    Cinema,
    CinemaReplay,
    DVD,
    Streaming,
    Download,
    VHS,
    Other,
    Unknown,
    MaxViewType
};

#endif // ENUMS_H
