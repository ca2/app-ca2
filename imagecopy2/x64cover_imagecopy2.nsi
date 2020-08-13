; example2.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------


;!include "${NSISDIR}\Contrib\Modern UI\System.nsh"
!include x64.nsh
;!define MUI_ICON "C:\eco\app-ca2\appmatter\imagecopy2\_std\_std\main\icon.ico"

; The name of the installer
Name "Image Copy"

; The default installation directory
InstallDir "$PROGRAMFILES\imagecopy2"

; The file to write
OutFile "C:\eco\setup\ImageCopyInstaller.exe"


; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\ca2\app-ca2\imagecopy2" "Install_Dir"

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
Section "imagecopy2 (required)"

  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\app\appmatter\main"
  
  ; Put file there
  File /r "C:\eco\app\appmatter\main\"
  
  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\app-ca2\appmatter\imagecopy2"
  
  ; Put file there
  File /r "C:\eco\app-ca2\appmatter\imagecopy2\"

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
  File "C:\eco\time\Win32\stage\axisfreeimage.dll"
  File "C:\eco\time\Win32\stage\image_decode_png.dll"
  File "C:\eco\time\Win32\stage\image_decode_jpeg.dll"
  File "C:\eco\time\Win32\stage\libcrypto-1_1.dll"
  File "C:\eco\time\Win32\stage\libssl-1_1.dll"
  File "C:\eco\time\Win32\stage\png.dll"
  File "C:\eco\time\Win32\stage\jpeg.dll"
  File "C:\eco\time\Win32\stage\pcre.dll"
  File "C:\eco\time\Win32\stage\sqlite.dll"
  File "C:\eco\time\Win32\stage\zlib.dll"
  File "C:\eco\time\Win32\stage\bzip2.dll"
  File "C:\eco\time\Win32\stage\app_common.dll"
  File "C:\eco\time\Win32\stage\app_ca2_imagecopy2.dll"
  File "C:\eco\time\Win32\stage\app_ca2_imagecopy2.exe"
  File "C:\eco\time\Win32\stage\ace.pdb"
  File "C:\eco\time\Win32\stage\aqua.pdb"
  File "C:\eco\time\Win32\stage\aura.pdb"
  File "C:\eco\time\Win32\stage\axis.pdb"
  File "C:\eco\time\Win32\stage\base.pdb"
  File "C:\eco\time\Win32\stage\core.pdb"
  File "C:\eco\time\Win32\stage\draw2d_gdiplus.pdb"
  File "C:\eco\time\Win32\stage\wndfrm_anthill.pdb"
  File "C:\eco\time\Win32\stage\axisfreeimage.pdb"
  File "C:\eco\time\Win32\stage\image_decode_png.pdb"
  File "C:\eco\time\Win32\stage\image_decode_jpeg.pdb"
  File "C:\eco\time\Win32\stage\libcrypto-1_1.pdb"
  File "C:\eco\time\Win32\stage\libssl-1_1.pdb"
  File "C:\eco\time\Win32\stage\png.pdb"
  File "C:\eco\time\Win32\stage\jpeg.pdb"
  File "C:\eco\time\Win32\stage\pcre.pdb"
  File "C:\eco\time\Win32\stage\sqlite.pdb"
  File "C:\eco\time\Win32\stage\zlib.pdb"
  File "C:\eco\time\Win32\stage\bzip2.pdb"
  File "C:\eco\time\Win32\stage\app_common.pdb"
  File "C:\eco\time\Win32\stage\app_ca2_imagecopy2.pdb"
  File "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Redist\MSVC\14.10.25008\vcredist_x86.exe"
  
${If} ${Runningx64}
	ReadRegStr $1 HKLM "SOFTWARE\Wow6432Node\Microsoft\VisualStudio\14.0\VC\Runtimes\x86" "Installed"
	StrCmp $1 1 installed_redist
${Else}
	ReadRegStr $1 HKLM "SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x86" "Installed"
	StrCmp $1 1 installed_redist
${EndIf}

  ;not installed, so run the installer
  ExecWait  '"$INSTDIR\time\x86\stage\vcredist_x86.exe" /install /passive /norestart'

installed_redist:

  ;we are done
  ExecWait '"$INSTDIR\time\x86\stage\app_ca2_imagecopy2.exe" : install' $0
  
  ; Write the installation path into the registry
  WriteRegStr HKLM "SOFTWARE\ca2\app-ca2\imagecopy2" "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_imagecopy2" "DisplayName" "Twitter Automator"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_imagecopy2" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_imagecopy2" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_imagecopy2" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
  CreateShortCut "$DESKTOP\Image Copy.lnk" "$INSTDIR\time\x86\stage\app_ca2_imagecopy2.exe" ""

  ; Set output path to the installation directory.
  SetOutPath "$WINDIR"
  File "C:\eco\time\Win32\stage\imagecopy.exe"

  
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Twitter Automator"
  CreateShortcut "$SMPROGRAMS\Twitter Automator\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortcut "$SMPROGRAMS\Twitter Automator\Twitter Automator.lnk" "$INSTDIR\time\x86\stage\app_ca2_imagecopy2.exe" "" "$INSTDIR\time\x86\stage\app_ca2_imagecopy2.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_imagecopy2"
  DeleteRegKey HKLM "SOFTWARE\ca2\app-ca2\imagecopy2"

  ; Remove files and uninstaller
  Delete "$INSTDIR\app_ca2_imagecopy2.nsi"
  Delete "$INSTDIR\uninstall.exe"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\app_ca2_imagecopy2\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\app_ca2_imagecopy2"
  RMDir /r "$INSTDIR/time"
  RMDir "$INSTDIR"

SectionEnd
