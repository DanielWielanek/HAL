
Macro(SetHeaderVariables HAL_CORE_DIR)
set(HAL_CORE "${HAL_CORE_DIR}")


set(HAL_FEATURES
	${HAL_CORE}/features
	${HAL_CORE}/features/hist
	${HAL_CORE}/features/io
	${HAL_CORE}/features/io/html
	${HAL_CORE}/features/std
	${HAL_CORE}/features/minimizer
	${HAL_CORE}/features/phys
)

set(HAL_CUTS
    ${HAL_CORE}/cuts
    ${HAL_CORE}/cuts/eventcuts
    ${HAL_CORE}/cuts/eventcuts/detector
    ${HAL_CORE}/cuts/eventcuts/general
    ${HAL_CORE}/cuts/trackcuts
    ${HAL_CORE}/cuts/trackcuts/mc
    ${HAL_CORE}/cuts/trackcuts/detector
    ${HAL_CORE}/cuts/trackcuts/kinematics
    ${HAL_CORE}/cuts/trackcuts/mc
    ${HAL_CORE}/cuts/trackcuts/resolution
    ${HAL_CORE}/cuts/trackcuts/monprop
    ${HAL_CORE}/cuts/twotrackcuts
    ${HAL_CORE}/cuts/twotrackcuts/detector
    ${HAL_CORE}/cuts/twotrackcuts/kinematics
    ${HAL_CORE}/cuts/twotrackcuts/mc
    ${HAL_CORE}/cuts/cutmonitors
)

set(HAL_DATA
    ${HAL_CORE}/data/
	${HAL_CORE}/data/dataformat
	${HAL_CORE}/data/dataformat/detector
	${HAL_CORE}/data/dataformat/compound
	${HAL_CORE}/data/dataformat/hiddeninfo	
	${HAL_CORE}/data/io	
)

set(HAL_GEN
	${HAL_CORE}/nicagenerators
	${HAL_CORE}/nicagenerators/readers
)

set(HAL_ANA
	${HAL_CORE}/analysis/base
	${HAL_CORE}/analysis/base/dump
	${HAL_CORE}/analysis/base/chains
	${HAL_CORE}/analysis/base/on_the_fly
	${HAL_CORE}/analysis/base/on_the_fly/smear_codes
)
set(HAL_HBT
	${HAL_CORE}/analysis/femto/ana
	${HAL_CORE}/analysis/femto/base
	${HAL_CORE}/analysis/femto/weights
	${HAL_CORE}/analysis/femto/corrfit
	${HAL_CORE}/analysis/femto/corrfit/mapgenerators
	${HAL_CORE}/analysis/femto/corrfit/mapgenerator_multidim
	${HAL_CORE}/analysis/femto/corrfit/fittingfunctions
	${HAL_CORE}/analysis/femto/corrfit/corrfitopt
	${HAL_CORE}/analysis/femto/imaging
	${HAL_CORE}/analysis/femto/base/source_models
	${HAL_CORE}/analysis/femto/base/corrfunc
	${HAL_CORE}/analysis/femto/base/shmath
	${HAL_CORE}/analysis/femto
	
)
set(HAL_QA
    ${HAL_CORE}/analysis/qa
)
set(HAL_SPECTRA
    ${HAL_CORE}/analysis/spectra
)
set(HAL_V0
    ${HAL_CORE}/analysis/v0s
)
set(HAL_FLUCTUATIONS
    ${HAL_CORE}/analysis/fluctuations
_)
if(DEFINED CIA)
    add_compile_definitions(__CIA__)
    Set(HAL_HBT ${HAL_HBT} ${HAL_CORE}/analysis/femto/top_secret)
endif()

EndMacro(SetHeaderVariables)

Macro(PrintConfig)
    Set(INFOFLAG_1 "")
    Set(INFOFLAG_2 "")
    if(NOT WIN32)
      string(ASCII 27 Esc)
      set(ColourReset "${Esc}[m")
      set(ColourBold  "${Esc}[1m")
      set(Red         "${Esc}[31m")
      set(Green       "${Esc}[32m")
      set(Yellow      "${Esc}[33m")
      set(Blue        "${Esc}[34m")
      set(Magenta     "${Esc}[35m")
      set(Cyan        "${Esc}[36m")
      set(White       "${Esc}[37m")
      set(BoldRed     "${Esc}[1;31m")
      set(BoldGreen   "${Esc}[1;32m")
      set(BoldYellow  "${Esc}[1;33m")
      set(BoldBlue    "${Esc}[1;34m")
      set(BoldMagenta "${Esc}[1;35m")
      set(BoldCyan    "${Esc}[1;36m")
      set(BoldWhite   "${Esc}[1;37m")
      
      Set(INFOFLAG_1 "${BoldCyan}")
      Set(INFOFLAG_2 "${ColourReset}")
    endif()
        message(STATUS "${INFOFLAG_1}Compilation with FairRoot: ${USE_FAIRROOT}${INFOFLAG_2}")
        message(STATUS "${INFOFLAG_1}Build examples:            ${USE_EXAMPLES}${INFOFLAG_2}")
        message(STATUS "${INFOFLAG_1}C++ standard:              ${CMAKE_CXX_STANDARD}${INFOFLAG_2}")
        message(STATUS "${INFOFLAG_1}Installation path:         ${CMAKE_INSTALL_PREFIX}${INFOFLAG_2}")

EndMacro()


Macro(InstallMimes HAL_CORE)
	file(READ ${HAL_CORE}/hal/hal.mimes CONTENT_A)
	file(READ $ENV{ROOTSYS}/share/root/etc/root.mimes CONTENT_B)
	file(WRITE $ENV{HOME}/.root.mimes ${CONTENT_A})
	file(APPEND $ENV{HOME}/.root.mimes ${CONTENT_B})
EndMacro(InstallMimes)
