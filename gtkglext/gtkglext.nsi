;
; NSIS script for GtkGLExt Win32 binary installer.
;
; written by Naofumi Yasufuku <naofumi@users.sourceforge.net>
;
; about NSIS, see http://nsis.sourceforge.net/
;

!define GTKGLEXT_VERSION         2.99.0
!define GTKGLEXT_API_VERSION     3.0
!define GTKGLEXT_PUBLISHER       "GtkGLExt"
!define GTKGLEXT_URL_INFO_ABOUT  "http://projects.gnome.org/gtkglext/"
!define GTKGLEXT_HELP_LINK       "mailto:gtkglext-list@gnome.org"

;--------------------------------
;Configuration

OutFile gtkglext-win32-${GTKGLEXT_VERSION}.exe
SetCompressor lzma

InstType "Full (Runtime w/ Development and Examples)"
InstType "Runtime (w/o Development and Examples)"

ShowInstDetails show
ShowUninstDetails show
SetDateSave on

; the default installation drive is changed by .onInit
InstallDir C:\GtkGLExt\${GTKGLEXT_API_VERSION}
InstallDirRegKey HKLM SOFTWARE\GtkGLExt\${GTKGLEXT_API_VERSION} "Path"

;--------------------------------
;Header Files

!include "MUI.nsh"

;--------------------------------
;Configuration

; Names
Name "GtkGLExt"
Caption "GtkGLExt ${GTKGLEXT_VERSION} Setup"

;--------------------------------
;Variables

Var MUI_TEMP
Var STARTMENU_FOLDER

;--------------------------------
;Interface Settings

!define MUI_ABORTWARNING

!define MUI_COMPONENTSPAGE_SMALLDESC

!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_UNFINISHPAGE_NOAUTOCLOSE

;--------------------------------
;Pages

;Welcome Page
!define MUI_WELCOMEPAGE_TEXT "This wizard will guide you through the installation of GtkGLExt, OpenGL Extension to GTK.\r\n\r\n\r\n$_CLICK"
!insertmacro MUI_PAGE_WELCOME

;License Page
!insertmacro MUI_PAGE_LICENSE "COPYING.LIB"

;Components Page
!insertmacro MUI_PAGE_COMPONENTS

;Directory Page
!insertmacro MUI_PAGE_DIRECTORY

;Start Menu Folder Page
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "GtkGLExt ${GTKGLEXT_API_VERSION}"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "SOFTWARE\GtkGLExt\${GTKGLEXT_API_VERSION}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "StartMenuFolder"
!insertmacro MUI_PAGE_STARTMENU StartMenu $STARTMENU_FOLDER

;Installation Page
!insertmacro MUI_PAGE_INSTFILES

;Finish Page
!define MUI_FINISHPAGE_NOREBOOTSUPPORT
!insertmacro MUI_PAGE_FINISH

;Uninstall Welcome Page
!insertmacro MUI_UNPAGE_WELCOME

;Uninstall Confirm Page
!insertmacro MUI_UNPAGE_CONFIRM

;Uninstallation Page
!insertmacro MUI_UNPAGE_INSTFILES

;Uninstall Finish Page
!insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

!insertmacro MUI_LANGUAGE "English"
  
;--------------------------------
;Reserve Files

;!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

;--------------------------------
;Installer Sections

Section "GtkGLExt Runtime" SecRuntime
  SectionIn 1 2 RO

  SetOutPath $INSTDIR\bin
  File bin\*.dll

  ; Registry
  WriteRegStr HKLM "SOFTWARE\GtkGLExt\${GTKGLEXT_API_VERSION}\Runtime" "Path" "$INSTDIR"
  WriteRegStr HKLM "SOFTWARE\GtkGLExt\${GTKGLEXT_API_VERSION}\Runtime" "Version" "${GTKGLEXT_VERSION}"

SectionEnd

Section "GtkGLExt Development" SecDevelopment
  SectionIn 1

  SetOutPath $INSTDIR\include\gtkglext-${GTKGLEXT_API_VERSION}\gdk
  File include\gtkglext-${GTKGLEXT_API_VERSION}\gdk\*.h

  SetOutPath $INSTDIR\include\gtkglext-${GTKGLEXT_API_VERSION}\gdk\glext
  File include\gtkglext-${GTKGLEXT_API_VERSION}\gdk\glext\*.h

  SetOutPath $INSTDIR\include\gtkglext-${GTKGLEXT_API_VERSION}\gdk\win32
  File include\gtkglext-${GTKGLEXT_API_VERSION}\gdk\win32\*.h

  SetOutPath $INSTDIR\include\gtkglext-${GTKGLEXT_API_VERSION}\gtk
  File include\gtkglext-${GTKGLEXT_API_VERSION}\gtk\*.h

  SetOutPath $INSTDIR\lib\gtkglext-${GTKGLEXT_API_VERSION}\include
  File lib\gtkglext-${GTKGLEXT_API_VERSION}\include\gdkglext-config.h

  SetOutPath $INSTDIR\lib
  File lib\*.a
  File lib\*.lib

  SetOutPath $INSTDIR\lib\pkgconfig
  File lib\pkgconfig\*.pc

  SetOutPath $INSTDIR\share\aclocal
  File share\aclocal\*.m4

  SetOutPath $INSTDIR\share\gtk-doc\html\gtkglext
  File share\gtk-doc\html\gtkglext\*.html
  File share\gtk-doc\html\gtkglext\*.sgml
  File share\gtk-doc\html\gtkglext\*.devhelp
  File share\gtk-doc\html\gtkglext\*.png

  ; Update .pc files
  ; short name will cause a problem with current libtool :-<
  ;GetFullPathName /SHORT $0 $INSTDIR
  StrCpy $0 $INSTDIR
  Push $0
  Call ChangeDirSeparator
  Pop $0

  DetailPrint "Updating $INSTDIR\lib\pkgconfig\gdkglext-${GTKGLEXT_API_VERSION}.pc"
  Push $INSTDIR\lib\pkgconfig\gdkglext-${GTKGLEXT_API_VERSION}.pc
  Push $0
  Call UpdatePCFile
  DetailPrint "Done"

  DetailPrint "Updating $INSTDIR\lib\pkgconfig\gdkglext-win32-${GTKGLEXT_API_VERSION}.pc"
  Push $INSTDIR\lib\pkgconfig\gdkglext-win32-${GTKGLEXT_API_VERSION}.pc
  Push $0
  Call UpdatePCFile
  DetailPrint "Done"

  DetailPrint "Updating $INSTDIR\lib\pkgconfig\gtkglext-${GTKGLEXT_API_VERSION}.pc"
  Push $INSTDIR\lib\pkgconfig\gtkglext-${GTKGLEXT_API_VERSION}.pc
  Push $0
  Call UpdatePCFile
  DetailPrint "Done"

  DetailPrint "Updating $INSTDIR\lib\pkgconfig\gtkglext-win32-${GTKGLEXT_API_VERSION}.pc"
  Push $INSTDIR\lib\pkgconfig\gtkglext-win32-${GTKGLEXT_API_VERSION}.pc
  Push $0
  Call UpdatePCFile
  DetailPrint "Done"

  ; Registry
  WriteRegStr HKLM "SOFTWARE\GtkGLExt\${GTKGLEXT_API_VERSION}\Development" "Path" "$INSTDIR"
  WriteRegStr HKLM "SOFTWARE\GtkGLExt\${GTKGLEXT_API_VERSION}\Development" "Version" "${GTKGLEXT_VERSION}"

SectionEnd

Section "GtkGLExt Examples" SecExamples
  SectionIn 1

  SetOutPath $INSTDIR\gtkglext-examples
  File gtkglext-examples\Makefile.mingw
  File gtkglext-examples\Makefile.msc
  File gtkglext-examples\*.h
  File gtkglext-examples\*.c
  File gtkglext-examples\*.lwo
  File gtkglext-examples\*.rgb
  File gtkglext-examples\*.exe

SectionEnd

Section "Start Menu" SecStartMenu
  SectionIn 1 2

  SetShellVarContext all

  !insertmacro MUI_STARTMENU_WRITE_BEGIN StartMenu

    CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"

    WriteINIStr "$SMPROGRAMS\$STARTMENU_FOLDER\GtkGLExt Website.url" "InternetShortcut" "URL" "http://projects.gnome.org/gtkglext/"

    IfFileExists "$INSTDIR\share\gtk-doc\html\gtkglext\index.html" "" +2
      CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\GtkGLExt Reference Manual.lnk" "$INSTDIR\share\gtk-doc\html\gtkglext\index.html"

    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall GtkGLExt.lnk" "$INSTDIR\uninst-gtkglext.exe"

  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

Section -post

  ; Write $INSTDIR\bin\gtkglext-env.sh
  DetailPrint "Generating $INSTDIR\bin\gtkglext-env.sh"
  Push $INSTDIR\bin\gtkglext-env.sh
  Call WriteEnvSh
  DetailPrint "Done"

  ; Write $INSTDIR\bin\gtkglext-env.bat
  DetailPrint "Generating $INSTDIR\bin\gtkglext-env.bat"
  Push $INSTDIR\bin\gtkglext-env.bat
  Call WriteEnvBat
  DetailPrint "Done"

  ; Registry
  WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\GtkGLExt" "DisplayName" "GtkGLExt ${GTKGLEXT_VERSION}"
  WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\GtkGLExt" "DisplayVersion" "${GTKGLEXT_VERSION}"
  WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\GtkGLExt" "Publisher" "${GTKGLEXT_PUBLISHER}"
  WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\GtkGLExt" "URLInfoAbout" "${GTKGLEXT_URL_INFO_ABOUT}"
  WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\GtkGLExt" "URLUpdateInfo" "${GTKGLEXT_URL_INFO_ABOUT}"
  WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\GtkGLExt" "HelpLink" "${GTKGLEXT_HELP_LINK}"
  WriteRegExpandStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\GtkGLExt" "UninstallString" "$INSTDIR\uninst-gtkglext.exe"

  ;; Uninstaller
  Delete $INSTDIR\uninst-gtkglext.exe
  WriteUninstaller $INSTDIR\uninst-gtkglext.exe

SectionEnd

;--------------------------------
;Descriptions

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecRuntime} "GtkGLExt runtime libraries"
  !insertmacro MUI_DESCRIPTION_TEXT ${SecDevelopment} "GtkGLExt development headers, libraries, and documentations"
  !insertmacro MUI_DESCRIPTION_TEXT ${SecExamples} "GtkGLExt example programs"
  !insertmacro MUI_DESCRIPTION_TEXT ${SecStartMenu} "Adds icons to your start menu for easy access"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Installer Functions

Function .onInit
  ; Change the default installation drive
  StrLen $R0 $INSTDIR
  StrCpy $R1 $INSTDIR $R0 1
  StrCpy $INSTDIR $PROGRAMFILES 1
  StrCpy $INSTDIR "$INSTDIR$R1"
FunctionEnd

; ChangeDirSeparator
; input, top of stack  (e.g. C:\Program Files\...)
; output, top of stack (replaces, with e.g. C:/Program Files/...)
; modifies no other variables.
Function ChangeDirSeparator
  Exch $R0
  Push $R1
  Push $R2
  Push $R3
    StrCpy $R1 $R0
    StrCpy $R0 ""
    StrCpy $R2 0
    goto loop
    replace:
      StrCpy $R0 "$R0/"
    loop:
      StrCpy $R3 $R1 1 $R2
      IntOp $R2 $R2 + 1
      StrCmp $R3 "\" replace
      StrCmp $R3 "" done
      StrCpy $R0 "$R0$R3"
      goto loop
    done:
  Pop $R3
  Pop $R2
  Pop $R1
  Exch $R0
FunctionEnd

; UpdatePCFile
; update pkg-config .pc file
Function UpdatePCFile
  Pop $R0 ; prefix
  Pop $R1 ; target .pc file
  Push $R7
  Push $R8
  Push $R9
    Rename $R1 $R1.orig
    FileOpen $R7 $R1.orig r
    FileOpen $R8 $R1 w
    FileRead $R7 $R9 ; skip original prefix
    FileWrite $R8 "prefix=$R0$\n"
    loop:
      FileRead $R7 $R9
      StrCmp $R9 "" done
      FileWrite $R8 $R9
      goto loop
    done:
    FileClose $R7
    FileClose $R8
    Delete $R1.orig
  Pop $R9
  Pop $R8
  Pop $R7
FunctionEnd

; WriteEnvSh
Function WriteEnvSh
  Pop $R0 ; Output file
  Push $R8
  Push $R9
    GetFullPathName /SHORT $R8 $INSTDIR
    Push $R8
    Call ChangeDirSeparator
    Pop $R8
    FileOpen $R9 $R0 w
    FileWrite $R9 "#!/bin/sh$\n"
    FileWrite $R9 "$\n"
    FileWrite $R9 "export GTKGLEXT_PREFIX=$R8$\n"
    FileWrite $R9 "$\n"
    FileWrite $R9 "echo $\"Setting environment variables for GtkGLExt.$\"$\n"
    FileWrite $R9 "echo $\"$\"$\n"
    FileWrite $R9 "$\n"
    FileWrite $R9 "case $$TERM in$\n"
    FileWrite $R9 "cygwin)$\n"
    FileWrite $R9 "  GTKGLEXT_PATH=`cygpath -u $${GTKGLEXT_PREFIX}/bin`$\n"
    FileWrite $R9 "  ;;$\n"
    FileWrite $R9 "msys)$\n"
    FileWrite $R9 "  GTKGLEXT_PATH=/$${GTKGLEXT_PREFIX%:*}$${GTKGLEXT_PREFIX#*:}/bin$\n"
    FileWrite $R9 "  ;;$\n"
    FileWrite $R9 "*)$\n"
    FileWrite $R9 "  echo $\"Unknown TERM$\"$\n"
    FileWrite $R9 "  ;;$\n"
    FileWrite $R9 "esac$\n"
    FileWrite $R9 "$\n"
    FileWrite $R9 "echo $\"export PATH=\$\"$${GTKGLEXT_PATH}:\$${PATH}\$\"$\"$\n"
    FileWrite $R9 "export PATH=$\"$${GTKGLEXT_PATH}:$${PATH}$\"$\n"
    FileWrite $R9 "$\n"
    FileWrite $R9 "echo $\"export PKG_CONFIG_PATH=\$\"$${GTKGLEXT_PREFIX}/lib/pkgconfig;\$${PKG_CONFIG_PATH}\$\"$\"$\n"
    FileWrite $R9 "export PKG_CONFIG_PATH=$\"$${GTKGLEXT_PREFIX}/lib/pkgconfig;$${PKG_CONFIG_PATH}$\"$\n"
    FileWrite $R9 "$\n"
    FileWrite $R9 "echo $\"$\"$\n"
    FileClose $R9
  Pop $R9
  Pop $R8
FunctionEnd

; WriteEnvBat
Function WriteEnvBat
  Pop $R0 ; Output file
  Push $R9
    FileOpen $R9 $R0 w
    FileWrite $R9 "@set GTKGLEXT_PREFIX=$INSTDIR$\r$\n"
    FileWrite $R9 "$\r$\n"
    FileWrite $R9 "@echo Setting environment variables for GtkGLExt.$\r$\n"
    FileWrite $R9 "@echo.$\r$\n"
    FileWrite $R9 "$\r$\n"
    FileWrite $R9 "@echo set PATH=%GTKGLEXT_PREFIX%\bin;%%PATH%%$\r$\n"
    FileWrite $R9 "@set PATH=%GTKGLEXT_PREFIX%\bin;%PATH%$\r$\n"
    FileWrite $R9 "$\r$\n"
    FileWrite $R9 "@echo set PKG_CONFIG_PATH=%GTKGLEXT_PREFIX%\lib\pkgconfig;%%PKG_CONFIG_PATH%%$\r$\n"
    FileWrite $R9 "@set PKG_CONFIG_PATH=%GTKGLEXT_PREFIX%\lib\pkgconfig;%PKG_CONFIG_PATH%$\r$\n"
    FileWrite $R9 "$\r$\n"
    FileWrite $R9 "@echo.$\r$\n"
    FileClose $R9
  Pop $R9
FunctionEnd

;--------------------------------
;Uninstaller Section

Section Uninstall

  ;Remove shortcut
  SetShellVarContext all
  !insertmacro MUI_STARTMENU_GETFOLDER StartMenu $MUI_TEMP
  StrCmp $MUI_TEMP "" noshortcuts
    Delete "$SMPROGRAMS\$MUI_TEMP\GtkGLExt Website.url"
    Delete "$SMPROGRAMS\$MUI_TEMP\GtkGLExt Reference Manual.lnk"
    Delete "$SMPROGRAMS\$MUI_TEMP\Uninstall GtkGLExt.lnk"
    RMDir "$SMPROGRAMS\$MUI_TEMP" ;Only if empty, so it won't delete other shortcuts
  noshortcuts:

  DeleteRegKey HKLM "SOFTWARE\GtkGLExt\${GTKGLEXT_API_VERSION}"
  DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\GtkGLExt"

  Delete $INSTDIR\uninst-gtkglext.exe

  ; bin

  Delete $INSTDIR\bin\gtkglext-env.sh
  Delete $INSTDIR\bin\gtkglext-env.bat
  Delete $INSTDIR\bin\libgdkglext-win32-${GTKGLEXT_API_VERSION}-*.dll
  Delete $INSTDIR\bin\libgtkglext-win32-${GTKGLEXT_API_VERSION}-*.dll

  RMDir $INSTDIR\bin

  ; include

  RMDir /r $INSTDIR\include\gtkglext-3.0

  RMDir $INSTDIR\include

  ; lib

  RMDir /r $INSTDIR\lib\gtkglext-3.0

  Delete $INSTDIR\lib\libgdkglext-win32-${GTKGLEXT_API_VERSION}.dll.a
  Delete $INSTDIR\lib\libgtkglext-win32-${GTKGLEXT_API_VERSION}.dll.a
  Delete $INSTDIR\lib\gdkglext-win32-${GTKGLEXT_API_VERSION}.lib
  Delete $INSTDIR\lib\gtkglext-win32-${GTKGLEXT_API_VERSION}.lib

  Delete $INSTDIR\lib\pkgconfig\gdkglext-${GTKGLEXT_API_VERSION}.pc
  Delete $INSTDIR\lib\pkgconfig\gdkglext-win32-${GTKGLEXT_API_VERSION}.pc
  Delete $INSTDIR\lib\pkgconfig\gtkglext-${GTKGLEXT_API_VERSION}.pc
  Delete $INSTDIR\lib\pkgconfig\gtkglext-win32-${GTKGLEXT_API_VERSION}.pc
  RMDir $INSTDIR\lib\pkgconfig

  RMDir $INSTDIR\lib

  ; share

  Delete $INSTDIR\share\aclocal\gtkglext-${GTKGLEXT_API_VERSION}.m4
  RMDir $INSTDIR\share\aclocal

  RMDir /r $INSTDIR\share\gtk-doc\html\gtkglext
  RMDir $INSTDIR\share\gtk-doc\html
  RMDir $INSTDIR\share\gtk-doc

  RMDir $INSTDIR\share

  ; gtkglext-examples

  RMDir /r $INSTDIR\gtkglext-examples

  ; $INSTDIR

  RMDir $INSTDIR

SectionEnd
