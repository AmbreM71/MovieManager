
#define MyAppName "MovieManager"
#define MyAppVersion "1.2.0"
#define MyAppPublisher "AmbreM"
#define MyAppExeName "MovieManager.exe"

; Path needs to be filled acording to the location of the development folder and the build folder
#define DevFolder ""
#define BuildFolder ""

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
ArchitecturesInstallIn64BitMode=x64
AppId={{E67FA56D-C592-4DCE-806D-66C38CB46B92}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
LicenseFile={#DevFolder}\LICENSE
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
SetupIconFile={#DevFolder}\Assets\logo.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern
OutputBaseFilename=MovieManager_{#MyAppVersion}_Installer
PrivilegesRequired=admin
OutputDir={#BuildFolder}/installer

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#BuildFolder}\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BuildFolder}\*"; DestDir: "{app}"; Flags: createallsubdirs recursesubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

