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
