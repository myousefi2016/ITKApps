SET( VTKITKCommon_SRCS
vtkITKAntiAliasBinaryImageFilter.h
vtkITKBilateralImageFilter.h
vtkITKConfidenceConnectedImageFilter.h
vtkITKConnectedThresholdImageFilter.h
vtkITKCurvatureAnisotropicDiffusionImageFilter.h
vtkITKCurvatureAnisotropicDiffusionImageFilterSF.h
vtkITKDanielssonDistanceMapImageFilter.h
vtkITKDiscreteGaussianImageFilter.h
vtkITKGradientAnisotropicDiffusionImageFilter.h
vtkITKGradientMagnitudeImageFilter.h
vtkITKGrayscaleFillholeImageFilter.h
vtkITKImageToImageFilter.h
vtkITKImageToImageFilter2DFF.h
vtkITKImageToImageFilterFF.h
vtkITKImageToImageFilterFUL.h
vtkITKImageToImageFilterULUL.h
vtkITKImageToImageFilterSF.h
vtkITKImageToImageFilterSUL.h
vtkITKImageToImageFilterUSUL.h
vtkITKImageToImageFilterUSUS.h
vtkITKImageToImageFilterSS.h
vtkITKIsolatedConnectedImageFilter.h
vtkITKLaplacianSegmentationLevelSetImageFilter.h
vtkITKMutualInformationTransform.cxx
vtkITKNeighborhoodConnectedImageFilter.h
vtkITKNormalizeImageFilterSF.h
vtkITKNormalizeImageFilter.h
vtkITKOtsuThresholdImageFilter.h
vtkITKPDEDeformableRegistrationFilter.h
vtkITKStatisticsImageFilterULUL.h
vtkITKSmoothingRecursiveGaussianImageFilterSS.h
vtkITKGradientMagnitudeRecursiveGaussianImageFilterSS.h
vtkITKGradientMagnitudeRecursiveGaussianImageFilterFF.h
vtkITKThresholdSegmentationLevelSetImageFilter.h
vtkITKTobogganImageFilter.h
vtkITKWatershedImageFilter.h
vtkITKWatershedImageFilterSUL.h
vtkITKRelabelComponentImageFilter.h
vtkITKArchetypeImageSeriesReader.cxx
)

SET_SOURCE_FILES_PROPERTIES (
vtkITKImageToImageFilter.h
vtkITKImageToImageFilter2DFF.h
vtkITKImageToImageFilterFF.h
vtkITKImageToImageFilterF2F.h
vtkITKImageToImageFilterF2F2.h
vtkITKImageToImageFilterF3F3.h
vtkITKImageToImageFilterFUL.h
vtkITKImageToImageFilterULUL.h
vtkITKImageToImageFilterSF.h
vtkITKImageToImageFilterSUL.h
vtkITKImageToImageFilterUSUL.h
vtkITKImageToImageFilterUSUS.h
vtkITKImageToImageFilterSS.h
ABSTRACT
)

# with -fPIC
IF(UNIX AND NOT WIN32)
  FIND_PROGRAM(CMAKE_UNAME uname /bin /usr/bin /usr/local/bin )
  IF(CMAKE_UNAME)
    EXEC_PROGRAM(uname ARGS -m OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR)
    SET(CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR} CACHE INTERNAL "processor type (i386 and x86_64)")
    IF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
      ADD_DEFINITIONS(-fPIC)
    ENDIF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
  ENDIF(CMAKE_UNAME)
ENDIF(UNIX AND NOT WIN32)

ADD_LIBRARY(vtkVTKITKCommon  ${VTKITKCommon_SRCS} )
INSTALL_TARGETS(/lib vtkVTKITKCommon)

# Configure Tcl wrappers.
VTK_WRAP_TCL2(vtkVTKITKCommonTCL VTKITKCommonTCL_SRCS ${VTKITKCommon_SRCS})
ADD_LIBRARY(vtkVTKITKCommonTCL  ${VTKITKCommonTCL_SRCS} )
INSTALL_TARGETS(/lib vtkVTKITKCommonTCL)
TARGET_LINK_LIBRARIES(vtkVTKITKCommonTCL vtkVTKITKCommon vtkCommonTCL vtkFilteringTCL vtkImagingTCL )

# Configure Python wrappers.
IF (VTK_WRAP_PYTHON)
  VTK_WRAP_PYTHON2(vtkVTKITKCommonPython VTKITKCommonPython_SRCS
                   ${VTKITKCommon_SRCS})
  ADD_LIBRARY(vtkVTKITKCommonPython  SHARED ${VTKITKCommonPython_SRCS})
  INSTALL_TARGETS(/lib vtkVTKITKCommonPython)
  TARGET_LINK_LIBRARIES (vtkVTKITKCommonPython
                         vtkVTKITKCommon
                         vtkFilteringPython
                         vtkCommonPython)
ENDIF (VTK_WRAP_PYTHON)

IF(BORLAND)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w-8004")
ENDIF(BORLAND)

IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-cast-qual -Wno-unused")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

TARGET_LINK_LIBRARIES(vtkVTKITKCommon vtkImaging
                      ${ITK_LIBRARIES})

