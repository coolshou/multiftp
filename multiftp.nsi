; Script generated with the Venis Install Wizard

; Define your application name
!define APPNAME "MultiFtp"
!define APPVERSION "1.0"
!define APPNAMEANDVERSION "MultiFtp $APPVERSION"

; Main Install settings
Name "${APPNAMEANDVERSION}"
InstallDir "$PROGRAMFILES\MultiFtp"
InstallDirRegKey HKLM "Software\${APPNAME}" ""
OutFile "C:\Program Files (x86)\Venis\MultiFtp-setup-$APPVERSION.exe"

; Use compression
SetCompressor LZMA

; Modern interface settings
!include "MUI.nsh"

!define MUI_ABORTWARNING

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Set languages (first is default language)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_RESERVEFILE_LANGDLL

Section "MultiFtp" Section1

	; Set Section properties
	SetOverwrite on

	; Set Section Files and Shortcuts
	SetOutPath "$INSTDIR\"
	File "multiftp_x86_64\500M.qz"
	File "multiftp_x86_64\Qt6Core.dll"
	File "multiftp_x86_64\Qt6Gui.dll"
	File "multiftp_x86_64\Qt6Network.dll"
	File "multiftp_x86_64\Qt6Svg.dll"
	File "multiftp_x86_64\Qt6Widgets.dll"
	File "multiftp_x86_64\d3dcompiler_47.dll"
	File "multiftp_x86_64\dxcompiler.dll"
	File "multiftp_x86_64\dxil.dll"
	File "multiftp_x86_64\multiftp.exe"
	File "multiftp_x86_64\opengl32sw.dll"
	File "multiftp_x86_64\vc_redist.x64.exe"
	SetOutPath "$INSTDIR\generic"
	File "multiftp_x86_64\generic\qtuiotouchplugin.dll"
	SetOutPath "$INSTDIR\iconengines"
	File "multiftp_x86_64\iconengines\qsvgicon.dll"
	SetOutPath "$INSTDIR\imageformats"
	File "multiftp_x86_64\imageformats\qgif.dll"
	File "multiftp_x86_64\imageformats\qicns.dll"
	File "multiftp_x86_64\imageformats\qico.dll"
	File "multiftp_x86_64\imageformats\qjpeg.dll"
	File "multiftp_x86_64\imageformats\qsvg.dll"
	File "multiftp_x86_64\imageformats\qtga.dll"
	File "multiftp_x86_64\imageformats\qtiff.dll"
	File "multiftp_x86_64\imageformats\qwbmp.dll"
	File "multiftp_x86_64\imageformats\qwebp.dll"
	SetOutPath "$INSTDIR\networkinformation"
	File "multiftp_x86_64\networkinformation\qnetworklistmanager.dll"
	SetOutPath "$INSTDIR\platforms"
	File "multiftp_x86_64\platforms\qwindows.dll"
	SetOutPath "$INSTDIR\styles"
	File "multiftp_x86_64\styles\qmodernwindowsstyle.dll"
	SetOutPath "$INSTDIR\tls"
	File "multiftp_x86_64\tls\qcertonlybackend.dll"
	File "multiftp_x86_64\tls\qschannelbackend.dll"
	SetOutPath "$INSTDIR\translations"
	File "multiftp_x86_64\translations\qt_en.qm"
	File "multiftp_x86_64\translations\qt_zh_TW.qm"
	
	
	CreateShortCut "$DESKTOP\MultiFtp.lnk" "$INSTDIR\multiftp.exe"
	CreateDirectory "$SMPROGRAMS\MultiFtp"
	CreateShortCut "$SMPROGRAMS\MultiFtp\MultiFtp.lnk" "$INSTDIR\multiftp.exe"
	CreateShortCut "$SMPROGRAMS\MultiFtp\Uninstall.lnk" "$INSTDIR\uninstall.exe"

SectionEnd

Section -FinishSection

	WriteRegStr HKLM "Software\${APPNAME}" "" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

; Modern install component descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${Section1} ""
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;Uninstall section
Section Uninstall

	;Remove from registry...
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
	DeleteRegKey HKLM "SOFTWARE\${APPNAME}"

	; Delete self
	Delete "$INSTDIR\uninstall.exe"

	; Delete Shortcuts
	Delete "$DESKTOP\MultiFtp.lnk"
	Delete "$SMPROGRAMS\MultiFtp\MultiFtp.lnk"
	Delete "$SMPROGRAMS\MultiFtp\Uninstall.lnk"

	; Clean up MultiFtp
	Delete "$INSTDIR\500M.qz"
	Delete "$INSTDIR\Qt6Core.dll"
	Delete "$INSTDIR\Qt6Gui.dll"
	Delete "$INSTDIR\Qt6Network.dll"
	Delete "$INSTDIR\Qt6Svg.dll"
	Delete "$INSTDIR\Qt6Widgets.dll"
	Delete "$INSTDIR\d3dcompiler_47.dll"
	Delete "$INSTDIR\dxcompiler.dll"
	Delete "$INSTDIR\dxil.dll"
	Delete "$INSTDIR\multiftp.exe"
	Delete "$INSTDIR\opengl32sw.dll"
	Delete "$INSTDIR\vc_redist.x64.exe"
  Delete "$INSTDIR\generic\qtuiotouchplugin.dll"
	Delete "$INSTDIR\iconengines\qsvgicon.dll"
	Delete "$INSTDIR\imageformats\qgif.dll"
	Delete "$INSTDIR\imageformats\qicns.dll"
	Delete "$INSTDIR\imageformats\qico.dll"
	Delete "$INSTDIR\imageformats\qjpeg.dll"
	Delete "$INSTDIR\imageformats\qsvg.dll"
	Delete "$INSTDIR\imageformats\qtga.dll"
	Delete "$INSTDIR\imageformats\qtiff.dll"
	Delete "$INSTDIR\imageformats\qwbmp.dll"
	Delete "$INSTDIR\imageformats\qwebp.dll"
	Delete "$INSTDIR\networkinformation\qnetworklistmanager.dll"
	Delete "$INSTDIR\platforms\qwindows.dll"
	Delete "$INSTDIR\styles\qmodernwindowsstyle.dll"
	Delete "$INSTDIR\tls\qcertonlybackend.dll"
	Delete "$INSTDIR\tls\qschannelbackend.dll"
	Delete "$INSTDIR\translations\qt_en.qm"
	Delete "$INSTDIR\translations\qt_zh_TW.qm"
	; Remove remaining directories
	RMDir "$SMPROGRAMS\MultiFtp"
	RMDir "$INSTDIR\"

SectionEnd

; eof