#ifndef _VGUI_QT_H_
#define _VGUI_QT_H_

// .NAME vgui_qt - QT implementation of vgui
// .HEADER vxl package
// .LIBRARY vgui-qt
// .INCLUDE vgui/impl/qt/vgui_qt.h
// .FILE vgui_qt.cxx

// .SECTION Description
// vgui_qt is a QT implementation of vgui.
// Provides functions for controlling the event loop.

// .SECTION Author
// Joris Schouteden

// .SECTION Modifications:
// 24.03.2000 JS  Initial Version, adapted from vgui_gtk

#include <vcl_vector.h>
#include <vgui/vgui_toolkit.h>

class vgui_qt_adaptor;
class vgui_qt_window;

class vgui_qt : public vgui_toolkit
{
public:
   static    vgui_qt* instance();
   virtual   vcl_string name() const { return "qt"; };
   virtual   void     run();
   virtual   vgui_window* produce_window(int width, int height, 
                                              const vgui_menu& menubar,
					      const char* title="vgui qt window");
   virtual   vgui_window* produce_window(int width, int height,
					      const char* title="vgui qt popup");
   virtual   vgui_dialog_impl* produce_dialog(const char* name);

protected:
   vgui_qt() { };
   void      init(int &, char **);
   static    vgui_qt* instance_; 
};

#endif // _VGUI_QT_H_
