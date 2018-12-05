//
// "Id"
//
// My3DView class definitions for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2002 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#ifndef My3DView_H
#define My3DView_H 1
#include <config.h>
#include <FL/Fl.H>
#include "itkMesh.h"

#if HAVE_GL
#  include <FL/Fl_Gl_Window.H>
#  include <FL/gl.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#else
#  include <FL/Fl_Box.H>
#endif /* HAVE_GL */

#include <stdlib.h>

#if HAVE_GL
class My3DView : public Fl_Gl_Window {
#else
class My3DView : public Fl_Box {
#endif /* HAVE_GL */

public:
    // this value determines the scaling factor used to draw the cube.
    double size;

    typedef itk::Mesh< float, 3 > meshType;
    typedef meshType::Pointer meshPointer;

    My3DView(int x,int y,int w,int h,const char *l=0);

    /* Set the rotation about the vertical (y ) axis.
     *
     * This function is called by the horizontal roller in My3DViewUI and the
     * initialize button in My3DViewUI.
     */
    void v_angle(float angle){vAng=angle;};
    
    // Return the rotation about the vertical (y ) axis.
    float v_angle(){return vAng;};

    /* Set the rotation about the horizontal (x ) axis.
     *
     * This function is called by the vertical roller in My3DViewUI and the
     * initialize button in My3DViewUI.
     */

    void h_angle(float angle){hAng=angle;};

    // the rotation about the horizontal (x ) axis.
    float h_angle(){return hAng;};

    /* Sets the x shift of the cube view camera.
     *
     * This function is called by the slider in My3DViewUI and the
     * initialize button in My3DViewUI.
     */
    void panx(float x){xshift=x;};
    /* Sets the y shift of the cube view camera.
     *
     * This function is called by the slider in My3DViewUI and the
     * initialize button in My3DViewUI.
     */
    void pany(float y){yshift=y;};

#if HAVE_GL
    /*The widget class draw() override.
     *
     *The draw() function initialize Gl for another round o f drawing
     * then calls specialized functions for drawing each of the
     * entities displayed in the cube view.
     *
     */
    void draw();    
#endif /* HAVE_GL */

#if HAVE_GL
    void loadObject(meshPointer objectmesh);
#else
    void loadObject() { }
#endif /* HAVE_GL */

#if HAVE_GL
    void loadNormals(meshPointer objectnormal);
#else
    void loadNormals() { }
#endif /* HAVE_GL */

private:

    /*  Draw the cube boundaries
     *
     *Draw the faces of the cube using the boxv[] vertices, using
     * GL_LINE_LOOP for the faces. The color is \#defined by CUBECOLOR.
     */
#if HAVE_GL
    void drawObject();
#else
    void drawObject() { }
#endif /* HAVE_GL */

#if HAVE_GL
    void setup();
#else
    void setup() { }
#endif /* HAVE_GL */

        
    float vAng,hAng;
    float xshift,yshift;

    meshPointer m_mesh;
    meshPointer m_meshnormals;

};
#endif

//
// End of "Id".
//