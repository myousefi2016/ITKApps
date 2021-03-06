/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    KmeansMSClassifierValidationApp.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _KmeansMSClassifierValidationApp_txx
#define _KmeansMSClassifierValidationApp_txx

#include "KmeansMSClassifierValidationApp.h"

namespace itk
{

template <typename TImage, typename TMaskImage>
KmeansMSClassifierValidationApp<TImage,TMaskImage>
::KmeansMSClassifierValidationApp()
{ 
  m_ImageDirectoryName = "";
  m_BrainSegmentationDirectoryName = "";
  m_PatientID = "";
  m_StartSliceNumber = 1;
  m_NumberOfSlices = 1;
  m_StartSegSliceNumber = 1;
  m_NumberOfSegSlices = 1;
  m_NumberOfChannels = 1;

  m_ParameterFileName = "";
  m_OutputFileName = "";
  m_AppendOutputFile = true;
  m_FileExtensions.resize(0);
}


template <typename TImage, typename TMaskImage>
void
KmeansMSClassifierValidationApp<TImage,TMaskImage>
::InitializeParser()
{
  this->m_Parser->SetImageDirectoryName( m_ImageDirectoryName.c_str() );
  this->m_Parser->SetFileExtensions( m_FileExtensions );
  this->m_Parser->SetBrainSegmentationDirectoryName( m_BrainSegmentationDirectoryName.c_str() );

  this->m_Parser->SetPatientID( m_PatientID.c_str() );
  this->m_Parser->SetStartSliceNumber( m_StartSliceNumber );
  this->m_Parser->SetNumberOfSlices( m_NumberOfSlices );
  this->m_Parser->SetStartSegSliceNumber( m_StartSegSliceNumber );
  this->m_Parser->SetNumberOfSegSlices( m_NumberOfSegSlices );
  this->m_Parser->SetNumberOfChannels( m_NumberOfChannels );

  this->m_Parser->SetParameterFileName( m_ParameterFileName.c_str() );
}


template <typename TImage, typename TMaskImage>
void
KmeansMSClassifierValidationApp<TImage,TMaskImage>
::InitializeMSClassifier()
{
  typedef TImage VectorInputImageType;

  typename VectorInputImageType::Pointer testImage = this->m_Parser->GetVectorInputImage();

  this->m_MSClassifier->SetVectorInputImage( this->m_Parser->GetVectorInputImage() );
  this->m_MSClassifier->SetMaskInputImage( this->m_Parser->GetMaskImage() );
  this->m_MSClassifier->SetNumberOfClasses( m_NumberOfClasses );  
  this->m_MSClassifier->SetNumberOfChannels( this->m_Parser->GetNumberOfChannels() );  
 
}


template <typename TImage, typename TMaskImage>
void
KmeansMSClassifierValidationApp<TImage,TMaskImage>
::InitializeOutputGenerator()
{
  this->m_OutputGenerator->SetPatientID( m_PatientID.c_str() );

  this->m_OutputGenerator->SetClassifiedImage( this->m_MSClassifier->GetClassifiedImage() );
//  this->m_OutputGenerator->SetTruthImage( this->m_Parser->GetTruthImage() );

//  this->m_OutputGenerator->SetStartSegSliceNumber( this->m_Parser->GetStartSegSliceNumber() );
  this->m_OutputGenerator->SetNumberOfSegSlices( this->m_Parser->GetNumberOfSegSlices() );

  this->m_OutputGenerator->SetTruthLabels( m_TruthLabels );

  this->m_OutputGenerator->SetOutputFileName( m_OutputFileName.c_str() );
  this->m_OutputGenerator->SetAppendOutputFile( m_AppendOutputFile );
} 


} // namespace itk

#endif
