
Macro(SetHeaderVariables HAL_CORE_DIR)
set(HAL_CORE "${HAL_CORE_DIR}")

#do not forget that last directory must be "core" directory with lindef ile
set(HAL_FEATURES
	${HAL_CORE}/features/hist
	${HAL_CORE}/features/hist/styles
	${HAL_CORE}/features/io
	${HAL_CORE}/features/io/html
	${HAL_CORE}/features/std
	${HAL_CORE}/features/minimizer
	${HAL_CORE}/features/phys
	${HAL_CORE}/features
)

set(HAL_CUTS
    ${HAL_CORE}/cuts/eventcuts
    ${HAL_CORE}/cuts/eventcuts/detector
    ${HAL_CORE}/cuts/eventcuts/general
    ${HAL_CORE}/cuts/eventcuts/mc
    ${HAL_CORE}/cuts/trackcuts
    ${HAL_CORE}/cuts/trackcuts/mc
    ${HAL_CORE}/cuts/trackcuts/detector
    ${HAL_CORE}/cuts/trackcuts/kinematics
    ${HAL_CORE}/cuts/trackcuts/mc
    ${HAL_CORE}/cuts/trackcuts/resolution
    ${HAL_CORE}/cuts/trackcuts/monprop
    ${HAL_CORE}/cuts/trackcuts/v0
    ${HAL_CORE}/cuts/twotrackcuts
    ${HAL_CORE}/cuts/twotrackcuts/detector
    ${HAL_CORE}/cuts/twotrackcuts/kinematics
    ${HAL_CORE}/cuts/twotrackcuts/mc
    ${HAL_CORE}/cuts/twotrackcuts/compound    
    ${HAL_CORE}/cuts/cutmonitors
    ${HAL_CORE}/cuts/cutmonitors/customized
    ${HAL_CORE}/cuts
)

set(HAL_DATA
	${HAL_CORE}/data/dataformat
	${HAL_CORE}/data/dataformat/detector
	${HAL_CORE}/data/dataformat/compound
	${HAL_CORE}/data/dataformat/hiddeninfo	
	${HAL_CORE}/data/io	
    ${HAL_CORE}/data/decays
    ${HAL_CORE}/data
)

set(HAL_ANA
	${HAL_CORE}/analysis/base/dump
	${HAL_CORE}/analysis/base/chains
	${HAL_CORE}/analysis/base/on_the_fly
	${HAL_CORE}/analysis/base/on_the_fly/smear_codes
    ${HAL_CORE}/analysis/base
)
set(HAL_HBT
	${HAL_CORE}/analysis/femto/ana
	${HAL_CORE}/analysis/femto/base
	${HAL_CORE}/analysis/femto/weights
	${HAL_CORE}/analysis/femto/corrfit
	${HAL_CORE}/analysis/femto/corrfit/corrfitopt
	${HAL_CORE}/analysis/femto/corrfit/fittingfunctions  
	${HAL_CORE}/analysis/femto/corrfit/mapgenerator_multidim
	${HAL_CORE}/analysis/femto/corrfit/mapgenerators
	${HAL_CORE}/analysis/femto/corrfit/pairgenerators
	${HAL_CORE}/analysis/femto/corrfit/serialization
    ${HAL_CORE}/analysis/femto/corrfit/roco
    ${HAL_CORE}/analysis/femto/painters
	${HAL_CORE}/analysis/femto/imaging
	${HAL_CORE}/analysis/femto/base/source_models
	${HAL_CORE}/analysis/femto/base/corrfunc
	${HAL_CORE}/analysis/femto/base/shmath
	${HAL_CORE}/analysis/femto/base/funcs
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
    Set(INFOFLAG_3 "")
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
      Set(INFOFLAG_3 "${BoldMagenta}")
    endif()
        message(STATUS "${INFOFLAG_1}Build examples:            ${USE_EXAMPLES}${INFOFLAG_2}")
        message(STATUS "${INFOFLAG_1}C++ standard:              ${CMAKE_CXX_STANDARD}${INFOFLAG_2}")
        message(STATUS "${INFOFLAG_1}Installation path:         ${CMAKE_INSTALL_PREFIX}${INFOFLAG_2}")
    if(DEFINED CIA)
    	message(STATUS "${INFOFLAG_3}Magic flag enabled${INFOFLAG_2}")
    endif()
EndMacro()


Macro(InstallMimes HAL_CORE)
	file(READ ${HAL_CORE}/hal/hal.mimes CONTENT_A)
	file(READ $ENV{ROOTSYS}/share/root/etc/root.mimes CONTENT_B)
	file(WRITE $ENV{HOME}/.root.mimes ${CONTENT_A})
	file(APPEND $ENV{HOME}/.root.mimes ${CONTENT_B})
EndMacro(InstallMimes)

if(NOT DEFINED INCLUDE_HAL_SUBDIR)
  set(INCLUDE_HAL_SUBDIR FALSE)
endif()

