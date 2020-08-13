; example2.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------


;!include "${NSISDIR}\Contrib\Modern UI\System.nsh"
!include x64.nsh
;!define MUI_ICON "C:\eco\app-ca2\appmatter\copy_url_context_menu\_std\_std\main\icon.ico"

	!define /date MyTIMESTAMP "%Y-%m-%d-%H-%M-%S"
	!define MUI_PRODUCT "Copy URL Context Menu" 
	Name "Copy URL Context Menu ${MyTIMESTAMP}"
	
;--------------------------------
;General

	;Properly display all languages (Installer will not work on Windows 95, 98 or ME!)
	Unicode true


; The default installation directory
InstallDir "$PROGRAMFILES\copy_url_context_menu"

; The file to write
OutFile "C:\\eco\\Setup\\Copy URL ${MyTIMESTAMP}.exe"


; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\ca2\app-ca2\copy_url_context_menu" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "copy_url_context_menu (required)"

  SectionIn RO


  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\time\x64\stage"
  
  ; Put file there
  File "C:\eco\time\x64\stage\_copy_url_context_menu.dll"
  
  ExecWait 'regsvr32.exe "$INSTDIR\time\x64\stage\_copy_url_context_menu.dll"' $0
  
  ; Write the installation path into the registry
  WriteRegStr HKLM "SOFTWARE\ca2\app-ca2\copy_url_context_menu" "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\copy_url_context_menu" "DisplayName" "Copy URL Context Menu"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\copy_url_context_menu" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\copy_url_context_menu" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\copy_url_context_menu" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Copy URL Context Menu"
  CreateShortcut "$SMPROGRAMS\Copy URL Context Menu\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"

  ExecWait 'regsvr32.exe /u "$INSTDIR\time\x64\stage\_copy_url_context_menu.dll"' $0
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\copy_url_context_menu"
  DeleteRegKey HKLM "SOFTWARE\ca2\app-ca2\copy_url_context_menu"

  ; Remove files and uninstaller
  Delete "$INSTDIR\copy_url_context_menu.nsi"
  Delete "$INSTDIR\uninstall.exe"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Copy URL Context Menu\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\copy_url_context_menu"
  RMDir /r "$INSTDIR/time"
  RMDir "$INSTDIR"

SectionEnd
