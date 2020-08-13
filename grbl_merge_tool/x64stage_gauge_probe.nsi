; example2.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------


;!include "${NSISDIR}\Contrib\Modern UI\System.nsh"
!include x64.nsh
;!define MUI_ICON "C:\eco\app-ca2\appmatter\grbl_merge_tool\_std\_std\main\icon.ico"

	!define /date MyTIMESTAMP "%Y-%m-%d-%H-%M-%S"
	!define MUI_PRODUCT "GRBL Merge Tool Ver. 1.0" 
	Name "GRBL Merge Tool ${MyTIMESTAMP}"
	
;--------------------------------
;General

	;Properly display all languages (Installer will not work on Windows 95, 98 or ME!)
	Unicode true


; The default installation directory
InstallDir "$PROGRAMFILES\grbl_merge_tool"

; The file to write
	OutFile "C:\\eco\\setup\\GRBL Merge Tool ${MyTIMESTAMP}.exe"


; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\ca2\app-ca2\grbl_merge_tool" "Install_Dir"

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
Section "grbl_merge_tool (required)"

  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\app\appmatter\main"
  
  ; Put file there
  File /r "C:\eco\app\appmatter\main\"
  
  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\app-ca2\appmatter\grbl_merge_tool"
  
  ; Put file there
  File /r "C:\eco\app-ca2\appmatter\grbl_merge_tool\"

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\time\x86\stage"
  
  ; Put file there
  File "C:\eco\time\Win32\stage\ace.dll"
  File "C:\eco\time\Win32\stage\aqua.dll"
  File "C:\eco\time\Win32\stage\aura.dll"
  File "C:\eco\time\Win32\stage\axis.dll"
  File "C:\eco\time\Win32\stage\base.dll"
  File "C:\eco\time\Win32\stage\core.dll"
  File "C:\eco\time\Win32\stage\draw2d_gdiplus.dll"
  File "C:\eco\time\Win32\stage\wndfrm_anthill.dll"
  File "C:\eco\time\Win32\stage\libcrypto-1_1.dll"
  File "C:\eco\time\Win32\stage\libssl-1_1.dll"
  File "C:\eco\time\Win32\stage\png.dll"
  File "C:\eco\time\Win32\stage\jpeg.dll"
  File "C:\eco\time\Win32\stage\pcre.dll"
  File "C:\eco\time\Win32\stage\sqlite.dll"
  File "C:\eco\time\Win32\stage\zlib.dll"
  File "C:\eco\time\Win32\stage\bzip2.dll"
  File "C:\eco\time\Win32\stage\app_ca2_grbl_merge_tool.dll"
  File "C:\eco\time\Win32\stage\app_ca2_grbl_merge_tool.exe"
  File "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Redist\MSVC\14.13.26020\vc_redist.x86.exe"
  
${If} ${Runningx64}
	ReadRegStr $1 HKLM "SOFTWARE\Wow6432Node\Microsoft\VisualStudio\14.0\VC\Runtimes\x86" "Installed"
	StrCmp $1 1 installed_redist
${Else}
	ReadRegStr $1 HKLM "SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x86" "Installed"
	StrCmp $1 1 installed_redist
${EndIf}

  ;not installed, so run the installer
  ExecWait  '"$INSTDIR\time\x86\stage\vc_redist.x86.exe" /install /passive /norestart'

installed_redist:

  ;we are done
  ExecWait '"$INSTDIR\time\x86\stage\app_ca2_grbl_merge_tool.exe" : install' $0
  
  ; Write the installation path into the registry
  WriteRegStr HKLM "SOFTWARE\ca2\app-ca2\grbl_merge_tool" "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_grbl_merge_tool" "DisplayName" "Twitter Automator"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_grbl_merge_tool" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_grbl_merge_tool" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_grbl_merge_tool" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
  CreateShortCut "$DESKTOP\GRBL Merge Tool.lnk" "$INSTDIR\time\x86\stage\app_ca2_grbl_merge_tool.exe" ""

  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Twitter Automator"
  CreateShortcut "$SMPROGRAMS\Twitter Automator\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortcut "$SMPROGRAMS\Twitter Automator\Twitter Automator.lnk" "$INSTDIR\time\x86\stage\app_ca2_grbl_merge_tool.exe" "" "$INSTDIR\time\x86\stage\app_ca2_grbl_merge_tool.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_grbl_merge_tool"
  DeleteRegKey HKLM "SOFTWARE\ca2\app-ca2\grbl_merge_tool"

  ; Remove files and uninstaller
  Delete "$INSTDIR\app_ca2_grbl_merge_tool.nsi"
  Delete "$INSTDIR\uninstall.exe"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\app_ca2_grbl_merge_tool\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\app_ca2_grbl_merge_tool"
  RMDir /r "$INSTDIR/time"
  RMDir "$INSTDIR"

SectionEnd
