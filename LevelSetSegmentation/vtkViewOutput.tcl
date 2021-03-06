package require vtk
package require vtkinteraction

set X 216
set Y 179
set MAG 2

# A reader for the original image
vtkPNGReader sReader
sReader SetFileName "brainslice.png"

# A reader for the level set image
vtkImageReader lsReader
lsReader SetFileName "output.raw"
lsReader SetDataExtent 0 $X 0 $Y 0 0
lsReader SetFileDimensionality 2
lsReader SetDataScalarTypeToFloat
lsReader SetDataByteOrderToBigEndian
lsReader FileLowerLeftOn

# A reader for the feature image
vtkImageReader fReader
fReader SetFileName "features.raw"
fReader SetDataExtent 0 $X 0 $Y 0 0
fReader SetFileDimensionality 2
fReader SetDataScalarTypeToFloat
fReader SetDataByteOrderToBigEndian
fReader FileLowerLeftOn

# Resamplers for all images
vtkImageMagnify fResampler
fResampler SetMagnificationFactors $MAG $MAG $MAG
fResampler InterpolateOn
fResampler SetInput [fReader GetOutput]

vtkImageMagnify lsResampler
lsResampler SetMagnificationFactors $MAG $MAG $MAG
lsResampler InterpolateOn
lsResampler SetInput [lsReader GetOutput]

vtkImageMagnify sResampler
sResampler SetMagnificationFactors $MAG $MAG $MAG
sResampler InterpolateOn
sResampler SetInput [sReader GetOutput]

# A color lookup table for the level sets
vtkLookupTable lsLookup
lsLookup SetNumberOfTableValues 2
lsLookup SetNumberOfColors 2
lsLookup SetTableValue 1 1.0 1.0 0.0 1.0
lsLookup SetTableValue 0 0.0 0.0 0.0 0.0

# A color lookup table for the image features
vtkColorTransferFunction fLookup
fLookup AddRGBPoint 0.0001 1 0 0   ;# Red are positive INSIDE values
fLookup AddRGBPoint -0.0001 0 1 0  ;# Green are negative OUTSIDE values

# A threshold filter to approximate zero crossings of level sets
vtkImageThreshold lsThreshold
lsThreshold ThresholdBetween -0.55 0.55
lsThreshold ReplaceInOn
lsThreshold ReplaceOutOn
lsThreshold SetInValue 1
lsThreshold SetOutValue 0
lsThreshold SetOutputScalarTypeToUnsignedChar
lsThreshold SetInput [lsResampler GetOutput]

# A color mapper for the level sets
vtkImageMapToColors lsMapper
lsMapper SetLookupTable lsLookup
lsMapper SetInput [lsThreshold GetOutput]

# A color mapper for the features
vtkImageMapToColors fMapper
fMapper SetLookupTable fLookup
fMapper SetInput [fResampler GetOutput]

# A viewer
vtkImageViewer viewer
viewer SetColorWindow 100
viewer SetColorLevel  200
viewer SetInput [sResampler GetOutput]
#viewer SetInput [fMapper GetOutput]
#viewer SetInput [lsMapper GetOutput]

#An image mapper for the level sets
vtkImageMapper lsImageMapper
lsImageMapper SetColorLevel 127.5
lsImageMapper SetColorWindow 255
lsImageMapper SetInput [lsMapper GetOutput]

# An overlay
vtkActor2D overlay
overlay SetMapper lsImageMapper
[viewer GetRenderer] AddActor2D overlay

# A widget for the viewer
vtkTkImageViewerWidget .viewer_widget -iv viewer -height [expr $Y * $MAG] \
    -width [expr $X * $MAG]
::vtk::bind_tk_imageviewer_widget .viewer_widget
pack .viewer_widget

# A button for toggling views
checkbutton .toggle_view -text "Show Features" -variable view_features \
    -command ToggleFeatureView -onvalue 1 -offvalue 0
set view_features 0
pack .toggle_view

proc ToggleFeatureView {} {
    global view_features
    if { $view_features == 1 } {
        viewer SetInput [fMapper GetOutput]
    } else {
        viewer SetInput [sResampler GetOutput]
    }
    viewer Render
}
