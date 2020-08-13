; example2.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------


;!include "${NSISDIR}\Contrib\Modern UI\System.nsh"
;!include x64.nsh
;!define MUI_ICON "C:\eco\app-ca2\appmatter\twitter_automator\_std\_std\main\icon.ico"

; The name of the installer
Name "Twitter Automator"

; The default installation directory
InstallDir "$PROGRAMFILES\twitter_automator"

; The file to write
OutFile "C:\eco\setup\TwitterAutomatorInstaller.exe"


; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\ca2\app-ca2\twitter_automator" "Install_Dir"

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
Section "twitter_automator (required)"

  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\app\appmatter\main"
  
  ; Put file there
  File /r "C:\eco\app\appmatter\main\"
  
  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\app-ca2\appmatter\twitter_automator"
  
  ; Put file there
  File /r "C:\eco\app-ca2\appmatter\twitter_automator\"

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\time\Win32\basis"
  
  ; Put file there
  File "C:\eco\time\Win32\basis\ace.dll"
  File "C:\eco\time\Win32\basis\aqua.dll"
  File "C:\eco\time\Win32\basis\aura.dll"
  File "C:\eco\time\Win32\basis\axis.dll"
  File "C:\eco\time\Win32\basis\base.dll"
  File "C:\eco\time\Win32\basis\core.dll"
  File "C:\eco\time\Win32\basis\draw2d_gdiplus.dll"
  File "C:\eco\time\Win32\basis\wndfrm_metro.dll"
  File "C:\eco\time\Win32\basis\axisfreeimage.dll"
  File "C:\eco\time\Win32\basis\image_decode_png.dll"
  File "C:\eco\time\Win32\basis\image_decode_jpeg.dll"
  File "C:\eco\time\Win32\basis\gcom.dll"
  File "C:\eco\time\Win32\basis\png.dll"
  File "C:\eco\time\Win32\basis\jpeg.dll"
  File "C:\eco\time\Win32\basis\pcre.dll"
  File "C:\eco\time\Win32\basis\sqlite.dll"
  File "C:\eco\time\Win32\basis\zlib.dll"
  File "C:\eco\time\Win32\basis\bzip2.dll"
  File "C:\eco\time\Win32\basis\app_core.dll"
  File "C:\eco\time\Win32\basis\app_ca2_twitter_automator.dll"
  File "C:\eco\time\Win32\basis\app_ca2_twitter_automator.exe"
  File "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Redist\MSVC\14.10.25008\vcredist_x86.exe"
  
  ${If} ${RunningX64}
	ReadRegStr $1 HKLM "SOFTWARE\Wow6432Node\Microsoft\VisualStudio\14.0\VC\Runtimes\x86" "Installed"
	StrCmp $1 1 installed_redist
  ${Else}
	ReadRegStr $1 HKLM "SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x86" "Installed"
	StrCmp $1 1 installed_redist
  ${EndIf}

  ;not installed, so run the installer
  ExecWait  '"$INSTDIR\time\Win32\basis\vcredist_x86.exe" /install /passive /norestart'

  ;installed_redist:

  ;we are done
  ExecWait '"$INSTDIR\time\Win32\basis\app_ca2_twitter_automator.exe" : install' $0
  
  ; Write the installation path into the registry
  WriteRegStr HKLM "SOFTWARE\ca2\app-ca2\twitter_automator" "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_twitter_automator" "DisplayName" "Twitter Automator"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_twitter_automator" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_twitter_automator" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_twitter_automator" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
  CreateShortCut "$DESKTOP\Twitter Automator.lnk" "$INSTDIR\time\Win32\basis\app_ca2_twitter_automator.exe" ""
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Twitter Automator"
  CreateShortcut "$SMPROGRAMS\Twitter Automator\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortcut "$SMPROGRAMS\Twitter Automator\Twitter Automator.lnk" "$INSTDIR\time\Win32\basis\app_ca2_twitter_automator.exe" "" "$INSTDIR\time\Win32\basis\app_ca2_twitter_automator.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_twitter_automator"
  DeleteRegKey HKLM "SOFTWARE\ca2\app-ca2\twitter_automator"

  ; Remove files and uninstaller
  Delete "$INSTDIR\app_ca2_twitter_automator.nsi"
  Delete "$INSTDIR\uninstall.exe"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\app_ca2_twitter_automator\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\app_ca2_twitter_automator"
  RMDir /r "$INSTDIR/time"
  RMDir "$INSTDIR"

SectionEnd
