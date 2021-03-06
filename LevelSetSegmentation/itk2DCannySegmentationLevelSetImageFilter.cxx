#include "itkPNGImageIO.h"
#include "itkRawImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCannySegmentationLevelSetImageFilter.h"
#include "itkCastImageFilter.h"
#include <iostream>

extern "C" {
#include <stdio.h>
}

int main(int argc, char *argv[]) {

  // Test for command line arguments.
  if (argc != 14)
    {
      std::cerr << "Usage: " << argv[0]
                << " input_image input_model output_model output_features iterations max_rms_change threshold variance propagation_scaling advection_scaling curvature_scaling isovalue negative_features"
                << std::endl;
      return 1;
    }

  // Grab the number of iterations and conductance term
  int iterations;
  int negative_features=0;
  float max_rms_change, threshold, variance;
  float propagation_scaling, advection_scaling, curvature_scaling, isovalue;
  ::sscanf(argv[5], "%d", &iterations);
  ::sscanf(argv[6], "%f", &max_rms_change);
  ::sscanf(argv[7], "%f", &threshold);
  ::sscanf(argv[8], "%f", &variance);
  ::sscanf(argv[9], "%f", &propagation_scaling);
    ::sscanf(argv[10], "%f", &advection_scaling);
  ::sscanf(argv[11], "%f", &curvature_scaling);
  ::sscanf(argv[12], "%f", &isovalue);
  ::sscanf(argv[13], "%d", &negative_features);

  // Convenient typedefs
  typedef itk::Image<float, 2> RealImageType;
  typedef itk::Image<unsigned char, 2>  CharImageType;
  typedef itk::CastImageFilter<CharImageType, RealImageType> CastToRealFilterType;
    typedef itk::CannySegmentationLevelSetImageFilter<CharImageType, RealImageType>
      SegmentationFilterType;
    typedef SegmentationFilterType::VectorImageType VectorImageType;

  // Create a file IO object for reading PNG.
  itk::PNGImageIO::Pointer io = itk::PNGImageIO::New();
  io->SetNumberOfDimensions(2);

  // Create a file IO object for writing raw floats 
  itk::RawImageIO<float, 2>::Pointer raw_io = itk::RawImageIO<float,2>::New();
  raw_io->SetFileDimensionality(2);
  raw_io->SetByteOrderToBigEndian();
  
  // Create two file readers
  itk::ImageFileReader<CharImageType>::Pointer seed_reader
    = itk::ImageFileReader<CharImageType>::New();
  itk::ImageFileReader<CharImageType>::Pointer feature_reader
    = itk::ImageFileReader<CharImageType>::New();
  seed_reader->SetImageIO(io);
  seed_reader->SetFileName(argv[2]);
  feature_reader->SetImageIO(io);
  feature_reader->SetFileName(argv[1]);

  // Create two file writers
  itk::ImageFileWriter<RealImageType>::Pointer model_writer
    = itk::ImageFileWriter<RealImageType>::New();
  itk::ImageFileWriter<RealImageType>::Pointer feature_writer
    = itk::ImageFileWriter<RealImageType>::New();
  feature_writer->SetImageIO(raw_io);
  feature_writer->SetFileName(argv[4]);
  model_writer->SetImageIO(raw_io);
  model_writer->SetFileName(argv[3]);

  // Create the casting filters
  CastToRealFilterType::Pointer to_real = CastToRealFilterType::New();

  // Create the segmentation filter
  SegmentationFilterType::Pointer segmentation = SegmentationFilterType::New();
  segmentation->SetMaximumIterations(iterations);
  segmentation->SetMaximumRMSError(max_rms_change);
  segmentation->SetPropagationScaling(propagation_scaling);
  segmentation->SetAdvectionScaling(advection_scaling);
  segmentation->SetCurvatureScaling(curvature_scaling);
  segmentation->SetIsoSurfaceValue(isovalue);
  segmentation->SetThreshold(threshold);
  segmentation->SetVariance(variance);
  if (negative_features != 0)
    {
      segmentation->SetUseNegativeFeatures(true);
    }
  // Connect the pipeline
  to_real->SetInput(feature_reader->GetOutput());
  segmentation->SetInput(seed_reader->GetOutput());
  model_writer->SetInput(segmentation->GetOutput());

  // Execute the pipeline
  try {
    //    to_real->Update(); // In the future, the feature image will be a proper
                       // input to the segmenation filter and this step will
                       // not be necessary.

    segmentation->SetFeatureImage(to_real->GetOutput());
    
    model_writer->Write();
    
        feature_writer->SetInput(segmentation->GetSpeedImage());
        feature_writer->Write();
  }
  catch(itk::ExceptionObject &e)
    {
      std::cerr << "Caught ITK exception: " << e << std::endl;
      return 1;
    }

  return 0;
}
