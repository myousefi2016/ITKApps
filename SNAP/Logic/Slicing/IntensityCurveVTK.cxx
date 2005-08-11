/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    IntensityCurveVTK.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2003 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#include "IntensityCurveVTK.h"

#include "SNAPCommon.h"
#include <assert.h>

using namespace std;

IntensityCurveVTK
::IntensityCurveVTK()
{
  m_Spline = vtkKochanekSpline::New();
  m_Spline->SetLeftConstraint(2);
  m_Spline->SetRightConstraint(2);
  m_Spline->SetDefaultContinuity(-1);
}

IntensityCurveVTK
::~IntensityCurveVTK()
{
  m_Spline->Delete();
}

void 
IntensityCurveVTK
::Initialize(unsigned int nControlPoints) 
{   
  // Set up the intervals for the control points
  float interval = 1.0 / (nControlPoints - 1);
  float t = 0;

  // Initialize the control points
  m_ControlPoints.clear();
  m_Spline->RemoveAllPoints();

  for(unsigned int i=0;i<nControlPoints;i++,t+=interval) 
    {
    ControlPoint c = {t,t};
    m_ControlPoints.push_back(c);
    m_Spline->AddPoint(t,t);
    }

  m_Spline->Compute();
}

void 
IntensityCurveVTK
::GetControlPoint(unsigned int iControlPoint,float &t,float &x)  const
{
  assert(iControlPoint < m_ControlPoints.size());
  t = m_ControlPoints[iControlPoint].t;
  x = m_ControlPoints[iControlPoint].x;
}

void 
IntensityCurveVTK
::UpdateControlPoint(unsigned int iControlPoint, float t, float x) 
{
  assert(iControlPoint < m_ControlPoints.size());

  // Update the control point of interest
  m_ControlPoints[iControlPoint].t = t;
  m_ControlPoints[iControlPoint].x = x;

  // Oops, we have to do this the hard way
  m_Spline->RemoveAllPoints();
  for(IteratorType it = m_ControlPoints.begin();it!=m_ControlPoints.end();it++)
    {
    m_Spline->AddPoint(it->t,it->x);
    }

  m_Spline->Compute();
}

bool 
IntensityCurveVTK
::IsMonotonic() const
{
  // The simple thing is to check each interval a bunch of
  // times.  There ought to be an upper limit on the length of non-monotonic
  // regions.
  unsigned int nRegions = 8;

  for(unsigned int i=0;i<m_ControlPoints.size()-1;i++) {
    float t =  m_ControlPoints[i].t;
    double tStep = (m_ControlPoints[i+1].t - t) / (nRegions - 1);
    float t1 = t + tStep;

    for(unsigned int j=0;j<nRegions-1;j++) {
      if(m_Spline->Evaluate(t) >= m_Spline->Evaluate(t1))
        return false;
      t = t1;
      t1 += tStep;
    }
  }

  return true;
}

void 
IntensityCurveVTK
::ScaleControlPointsToWindow(float tMin, float tMax)
{
  assert(tMin < tMax);

  // Get the current range
  float t1 = m_ControlPoints.front().t;
  float tn = m_ControlPoints.back().t;

  // Compute coefficients of the mapping t' = b t + k
  float b = (tMax - tMin) / (tn - t1);
  float k = tMin - b * t1;

  m_Spline->RemoveAllPoints();

  for(IteratorType it = m_ControlPoints.begin();it != m_ControlPoints.end();it++)
    {
    it->t = it->t * b + k;
    m_Spline->AddPoint(it->t,it->x);
    }

  m_Spline->Compute();
}

void
IntensityCurveVTK
::PrintSelf(std::ostream &os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Spline: "
    << m_Spline
    << std::endl;
}

void
IntensityCurveVTK
::LoadFromRegistry(Registry &registry)
{
  // Read the number of control points
  size_t nPoints = registry["NumberOfControlPoints"][3];
  this->Initialize(nPoints);

  // Load each of the control point
  for(size_t iPoint = 0; iPoint < nPoints; iPoint++)
    {
    // Get the default values, just in case
    float t0, x0;
    this->GetControlPoint(iPoint, t0, x0);

    // Read the registry
    Registry &folder = registry.Folder(registry.Key("ControlPoint[%d]",iPoint));
    float t = (float) folder["tValue"][(double) t0];
    float x = (float) folder["xValue"][(double) x0];

    // Set the control point
    this->UpdateControlPoint(iPoint, t, x);
    }
}

void
IntensityCurveVTK
::SaveToRegistry(Registry &registry) const
{
  // Store the number of control points
  registry["NumberOfControlPoints"] << this->GetControlPointCount();

  // Save each control point
  for(size_t iPoint = 0; iPoint < this->GetControlPointCount(); iPoint++)
    {
    // Get the current values, just in case
    float t, x;
    this->GetControlPoint(iPoint, t, x);

    // Create a folder in the registry
    string key = registry.Key("ControlPoint[%d]",iPoint);
    Registry &folder = registry.Folder(key);
    folder["tValue"] << (double) t;
    folder["xValue"] << (double) x;
    }
}
