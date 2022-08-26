
Macro(SetHeaderVariables HAL_CORE_DIR)
set(HAL_CORE "${HAL_CORE_DIR}")


set(HAL_FEATURES
	${HAL_CORE}/features
	${HAL_CORE}/features/hist
	${HAL_CORE}/features/io
	${HAL_CORE}/features/io/html
	${HAL_CORE}/features/std
	${HAL_CORE}/features/minimizer
)

set(HAL_CUTS
    ${HAL_CORE}/cuts
    ${HAL_CORE}/cuts/eventcuts
    ${HAL_CORE}/cuts/trackcuts
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
	${HAL_CORE}/analysis/femto/imaging
	${HAL_CORE}/analysis/femto/base/source_models
	${HAL_CORE}/analysis/femto/base/corrfunc
	${HAL_CORE}/analysis/femto/base/shmath
	${HAL_CORE}/analysis/femto
)

EndMacro(SetHeaderVariables)


Macro(InstallMimes HAL_CORE)
	file(READ ${HAL_CORE}/hal/hal.mimes CONTENT_A)
	file(READ $ENV{ROOTSYS}/share/root/etc/root.mimes CONTENT_B)
	file(WRITE $ENV{HOME}/.root.mimes ${CONTENT_A})
	file(APPEND $ENV{HOME}/.root.mimes ${CONTENT_B})
EndMacro(InstallMimes)
