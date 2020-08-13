; example2.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------


;!include "${NSISDIR}\Contrib\Modern UI\System.nsh"
!include x64.nsh
;!define MUI_ICON "C:\eco\app-ca2\appmatter\twitter_auto\_std\_std\main\icon.ico"

; The name of the installer
!define /date MyTIMESTAMP "%Y-%m-%d-%H-%M-%S"
!define MUI_PRODUCT "Twitter Auto Ver. 1.0" 
Name "Twitter Auto ${MyTIMESTAMP}"

; The default installation directory
InstallDir "$PROGRAMFILES\twitter_auto"

; The file to write
OutFile "C:\eco\setup\TwitterAutoInstaller ${MyTIMESTAMP}.exe"


; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\ca2\app-ca2\twitter_auto" "Install_Dir"

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
Section "twitter_auto (required)"

  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\app\appmatter\main"
  
  ; Put file there
  File /r "C:\eco\app\appmatter\main\"
  
  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\app-ca2\appmatter\twitter_auto"
  
  ; Put file there
  File /r "C:\eco\app-ca2\appmatter\twitter_auto\"

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\time\x64\cover"
  
  ; Put file there
  File "C:\eco\time\x64\cover\ace.dll"
  File "C:\eco\time\x64\cover\aqua.dll"
  File "C:\eco\time\x64\cover\aura.dll"
  File "C:\eco\time\x64\cover\axis.dll"
  File "C:\eco\time\x64\cover\base.dll"
  File "C:\eco\time\x64\cover\core.dll"
  File "C:\eco\time\x64\cover\draw2d_gdiplus.dll"
  File "C:\eco\time\x64\cover\wndfrm_anthill.dll"
  File "C:\eco\time\x64\cover\libcrypto-1_1-x64.dll"
  File "C:\eco\time\x64\cover\libssl-1_1-x64.dll"
  File "C:\eco\time\x64\cover\pcre.dll"
  File "C:\eco\time\x64\cover\sqlite.dll"
  File "C:\eco\time\x64\cover\zlib.dll"
  File "C:\eco\time\x64\cover\bzip2.dll"
  File "C:\eco\time\x64\cover\app_ca2_twitter_auto.dll"
  File "C:\eco\time\x64\cover\app_ca2_twitter_auto.exe"
  File "C:\eco\time\x64\cover\ace.pdb"
  File "C:\eco\time\x64\cover\aqua.pdb"
  File "C:\eco\time\x64\cover\aura.pdb"
  File "C:\eco\time\x64\cover\axis.pdb"
  File "C:\eco\time\x64\cover\base.pdb"
  File "C:\eco\time\x64\cover\core.pdb"
  File "C:\eco\time\x64\cover\draw2d_gdiplus.pdb"
  File "C:\eco\time\x64\cover\wndfrm_anthill.pdb"
  File "C:\eco\time\x64\cover\libcrypto-1_1-x64.pdb"
  File "C:\eco\time\x64\cover\libssl-1_1-x64.pdb"
  File "C:\eco\time\x64\cover\pcre.pdb"
  File "C:\eco\time\x64\cover\sqlite.pdb"
  File "C:\eco\time\x64\cover\zlib.pdb"
  File "C:\eco\time\x64\cover\bzip2.pdb"
  File "C:\eco\time\x64\cover\app_ca2_twitter_auto.pdb"
  File "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Redist\MSVC\14.12.25810\vc_redist.x64.exe"
  
${If} ${RunningX64}
	ReadRegStr $1 HKLM "SOFTWARE\Wow6432Node\Microsoft\VisualStudio\14.0\VC\Runtimes\x64" "Installed"
	StrCmp $1 1 installed_redist
${Else}
	ReadRegStr $1 HKLM "SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64" "Installed"
	StrCmp $1 1 installed_redist
${EndIf}

  ;not installed, so run the installer
  ExecWait  '"$INSTDIR\time\x64\cover\vc_redist.x64.exe" /install /passive /norestart'

installed_redist:

  ;we are done
  ExecWait '"$INSTDIR\time\x64\cover\app_ca2_twitter_auto.exe" : install' $0
  
  ; Write the installation path into the registry
  WriteRegStr HKLM "SOFTWARE\ca2\app-ca2\twitter_auto" "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_twitter_auto" "DisplayName" "Twitter Auto"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_twitter_auto" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_twitter_auto" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_twitter_auto" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
  CreateShortCut "$DESKTOP\Twitter Auto.lnk" "$INSTDIR\time\x64\cover\app_ca2_twitter_auto.exe" ""
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Twitter Auto"
  CreateShortcut "$SMPROGRAMS\Twitter Auto\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortcut "$SMPROGRAMS\Twitter Auto\Twitter Auto.lnk" "$INSTDIR\time\x64\cover\app_ca2_twitter_auto.exe" "" "$INSTDIR\time\x64\cover\app_ca2_twitter_auto.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_twitter_auto"
  DeleteRegKey HKLM "SOFTWARE\ca2\app-ca2\twitter_auto"

  ; Remove files and uninstaller
  Delete "$INSTDIR\app_ca2_twitter_auto.nsi"
  Delete "$INSTDIR\uninstall.exe"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\app_ca2_twitter_auto\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\app_ca2_twitter_auto"
  RMDir /r "$INSTDIR/time"
  RMDir "$INSTDIR"

SectionEnd
