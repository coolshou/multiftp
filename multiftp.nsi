; Script generated with the Venis Install Wizard
!addplugindir "nsis\"
; Define your application name
!define APPNAME "multiftp"
!ifndef APPVERSION
!define APPVERSION "1.0"
!endif
!define APPNAMEANDVERSION "${APPNAME} ${APPVERSION}"
!define APPDOMAIN "coolshou.idv.tw"
!define APPURL "https://github.com/coolshou/qiperf"
!ifndef APPFileVersion
!define APPFileVersion 1.0.11404.09
!endif
!ifndef WIN64
!define WIN64 ; force  64 bit, comment out for 32 bit
!endif
!define PRODUCT_REG_KEY "Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"

; Main Install settings
Name "${APPNAMEANDVERSION}"
InstallDir "$PROGRAMFILES\${APPNAME}"
InstallDirRegKey HKLM "Software\${APPNAME}" ""
OutFile "..\${APPNAME}-setup-${APPFileVersion}.exe"

!include "FileFunc.nsh"
; Use compression
SetCompressor LZMA
!include "x64.nsh"
; Modern interface settings
!include "MUI.nsh"
!include "nsis\nsProcess.nsh"
!include "nsis\FileAssociation.nsh"
!include "WordFunc.nsh"
!insertmacro VersionCompare

!define MUI_ABORTWARNING
!define MUI_ICON "images\${APPNAME}.ico"
# uninstall icon
!define MUI_UNICON "images\uninstall.ico"
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Set languages (first is default language)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_RESERVEFILE_LANGDLL

VIProductVersion ${APPFileVersion}
VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "${APPNAMEANDVERSION}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductVersion" "${APPVERSION}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "Comments" "multi ftp"
VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "${APPDOMAIN}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalTrademarks" "${APPNAME} is a trademark of ${APPDOMAIN}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "(C)2023-2025 ${APPDOMAIN}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "${APPNAME}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "${APPFileVersion}"

Section "MultiFtp" Section1
	call kill_process
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

	Call check_vc_redist
	; Add an application to the firewall exception list
	SimpleFC::AddApplication "multiftp" "$INSTDIR\${APPNAME}.exe" 0 2 "" 1
	Pop $0 ; return error(1)/success(0)

SectionEnd

Section -FinishSection

	WriteRegStr HKLM "Software\${APPNAME}" "" "$INSTDIR"
	WriteRegStr HKLM "Software\${PRODUCT_REG_KEY}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\${PRODUCT_REG_KEY}" "DisplayIcon" "$INSTDIR\${APPNAME}.ico"
	WriteRegStr HKLM "Software\${PRODUCT_REG_KEY}" "Publisher" "${APPDOMAIN}"
	WriteRegStr HKLM "Software\${PRODUCT_REG_KEY}" "DisplayVersion" "${APPFileVersion}"
	WriteRegStr HKLM "Software\${PRODUCT_REG_KEY}" "HelpLink" "${APPURL}"
	WriteRegDWORD HKLM "Software\${PRODUCT_REG_KEY}" "NoModify" "1"
	WriteRegDWORD HKLM "Software\${PRODUCT_REG_KEY}" "NoRepair" "1"
	WriteRegStr HKLM "Software\${PRODUCT_REG_KEY}" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteUninstaller "$INSTDIR\uninstall.exe"
	# size
	${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
	IntFmt $0 "0x%08X" $0
	WriteRegDWORD HKLM "Software\${PRODUCT_REG_KEY}" "EstimatedSize" "$0"

SectionEnd

; Modern install component descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${Section1} "multiftp"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;Uninstall section
Section Uninstall
	Call un.install_multiftp
!ifdef WIN64
        SetRegView 64
!endif
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
	RMDir "$INSTDIR\generic\"
	RMDir "$INSTDIR\iconengines\"
	RMDir "$INSTDIR\imageformats\"
	RMDir "$INSTDIR\networkinformation\"
	RMDir "$INSTDIR\platforms\"
	RMDir "$INSTDIR\styles\"
	RMDir "$INSTDIR\tls\"
	RMDir "$INSTDIR\translations\"
	RMDir "$INSTDIR\"

SectionEnd

Function .onInit
	${If} ${RunningX64}
	!ifdef WIN64
    		SetRegView 64
	!endif
	${EndIf}

    !ifdef WIN64
      strcpy $INSTDIR "$PROGRAMFILES64\${APPNAME}"
    !endif

FunctionEnd

Function install_vc_redist
    DetailPrint "Install VC runtime for qiperf..."
    ;not installed, so run the installer in quiet mode
    ExecWait '$INSTDIR\vc_redist.x64.exe /q /norestart'
FunctionEnd

Function check_vc_redist
    DetailPrint "Check VC runtime..."
    ${If} ${RunningX64}
            ReadRegStr $1 HKLM "SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64" "Installed"
            StrCmp $1 1 installed install_vc_redist
    ${Else}
            ReadRegStr $1 HKLM "SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x86" "Installed"
            StrCmp $1 1 installed install_vc_redist
    ${EndIf}

    installed:
    ; Visual Studio 2017 version 15.6 introduced msvcp140_1
    ; Visual Studio 2019 ? introduced msvcp140_2
    ; 14.24 OKs

    ;check vc_redist version <14.20 is not good
    ${If} ${RunningX64}
            ReadRegStr $1 HKLM "SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64" "Minor"
            ;StrCmp $1 24 version_ok
    ${Else}
            ReadRegStr $1 HKLM "SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x86" "Minor"
            ;StrCmp $1 24 version_ok
    ${EndIf}
    IntCmp $1 24 issame install_vc_redist morethan
    install_vc_redist:
    ;${If} $var >= 2
        call install_vc_redist
    ;${EndIf}

    issame:
    morethan:
    ;we are done

FunctionEnd

!macro kill_process un
Function ${un}kill_process
    #kill multiftp
    ${nsProcess::FindProcess} "${APPNAME}" $R0
    ${If} $R0 == 0
        DetailPrint "${APPNAME} is running. Closing it down"
        ${nsProcess::KillProcess} "${APPNAME}" $R0
        ;DetailPrint "Waiting for ${APPNAME} to close"
        ;Sleep 2000
    ${Else}
        DetailPrint "${APPNAME} was not found to be running"
    ${EndIf}
    ${nsProcess::Unload}
FunctionEnd
!macroend
!insertmacro kill_process ""
!insertmacro kill_process "un."

Function un.install_multiftp
    call un.kill_process
    ; Remove an application from the firewall exception list
    SimpleFC::RemoveApplication "$INSTDIR\${APPNAME}.exe"
    Pop $0 ; return error(1)/success(0)
FunctionEnd

Function .oninstsuccess
    # final install success, run qiperftray
    SetOutPath "$INSTDIR\"
    Exec "$INSTDIR\${APPNAME}"
FunctionEnd

; eof
