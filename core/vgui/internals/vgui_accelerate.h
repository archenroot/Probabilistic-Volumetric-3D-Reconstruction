#ifndef vgui_accelerate_h_
#define vgui_accelerate_h_
#ifdef __GNUC__
#pragma interface
#endif
// 
// .NAME vgui_accelerate
// .LIBRARY vgui
// .HEADER vxl Package
// .INCLUDE vgui/internals/vgui_accelerate.h
// .FILE internals/vgui_accelerate.cxx
//
// .SECTION Author
//              David Capel
//              Robotics Research Group, University of Oxford
//
// .SECTION Modifications
//   01-APR-2000   initial version
//   10-JUL-2000   Marko Bacic, Oxford RRG - Speeded up rendering on Mesa by using GL_RGB instead of GL_RGBA
//

#include <vgui/vgui_gl.h>

// (Potentially) Accelerated functions

class vgui_accelerate {
public:
  static bool vgui_no_acceleration;
  static bool vgui_mfc_ogl_acceleration;
  static bool vgui_mfc_acceleration;

  static vgui_accelerate* instance();
  
  virtual ~vgui_accelerate();

  virtual bool vgui_glClear( GLbitfield mask );
  virtual bool vgui_glDrawPixels( GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels );
  
  virtual bool vgui_choose_cache_format( GLenum* format, GLenum* type);

  virtual bool vgui_copy_back_to_aux();
  virtual bool vgui_copy_aux_to_back();

  static void register_accelerator (vgui_accelerate* p, int level);
  // -- u97mb hack. Used by vgui_mfc_adaptor when in mfc-accel mode
  static int image_width_;
  static int image_height_;
protected:
  vgui_accelerate();
};

#endif // vgui_accelerate_h_
