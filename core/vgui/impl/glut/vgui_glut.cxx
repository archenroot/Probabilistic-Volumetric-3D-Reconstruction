/*
  fsm@robots.ox.ac.uk
*/
#include "vgui_glut.h"
#include "vgui_glut_window.h"
#include <vcl_cstdlib.h>
#include <vcl_cassert.h>
#include <vgui/vgui_glut.h>

//--------------------------------------------------------------------------------

int vgui_glut::count = 0;

vgui_glut::vgui_glut()
{
  ++count;
  assert(count == 1);
}

vgui_glut::~vgui_glut()
{
  --count;
  assert(count==0);
}

//--------------------------------------------------------------------------------

void vgui_glut::init(int &argc, char **argv)
{
//   cerr << __FILE__ " init() :" << endl;
//   for (unsigned i=0; i<argc; ++i)
//     cerr << i << ' ' << (void*)argv[i] << flush << ' ' << argv[i] << endl;
  glutInit(&argc, argv);
}

vcl_string vgui_glut::name() const
{
  return "glut";
}
  
vgui_window *vgui_glut::produce_window(int width, int height, 
				       vgui_menu const &menubar,
				       char const *title) 
{
  vgui_glut_window *win = new vgui_glut_window(title, width, height);
  win->menubar = menubar;
  return win;
}

vgui_window *vgui_glut::produce_window(int width, int height, 
				       char const *title) 
{
  return new vgui_glut_window(title, width, height);
}

//----------------------------------------------------------------------

#include <vcl_csetjmp.h>
#include <vcl_iostream.h>
#include <vgui/vgui_macro.h>

// Use setjmp()/longjmp() to circumvent GLUT event loop restrictions.
//
// It is essential never to longjmp() across C++ destructors. It
// happens to be safe to longjmp() out of glutMainLoop() from the
// GLUT idle callback, by inspection of the GLUT sources.
//
// main()
//  ...
//   vgui::run()
//    ...
//    vgui_glut::run()
//    ...
//     internal_run_till_idle();
//     ** setjmp()s **
//     ...
//      glutMainLoop()
//      ...
//       [user code]
//       ...

static int const internal_label = 1234;
static jmp_buf   internal_buf;

// This function is the idle callback used
// to longjmp() out of the GLUT event loop.
static void internal_longjmp_idler()
{
  longjmp(internal_buf, internal_label);
}

// This function lets the GLUT event loop run till it becomes
// idle and then returns to the caller. It is intended to be
// re-entrant, hence the saving and restoring of the jmp_buf.
static void internal_run_till_idle()
{
  // save the current jmp_buf;
  jmp_buf saved_buf;
  memcpy(&saved_buf, &internal_buf, sizeof internal_buf);
  
  // record current state/accept control after longjmp().
  int t = setjmp(internal_buf);
  
  // if we got back control after a longjmp(), restore
  // the previous jmp_buf and return to the caller now.
  if (t != 0) {
    assert(t == internal_label);
    memcpy(&internal_buf, &saved_buf, sizeof internal_buf);
    return;
  }
  
  // set idle function.
  glutIdleFunc(internal_longjmp_idler);
  
  // call GLUT event loop. when the event queue becomes empty, the idle callback
  // will be called and that then longjmp()s back.
  glutMainLoop();
  
  // If we get here, it means glutMainLoop() 
  // returned, which it should never do.
  vgui_macro_warning << "internal error in internal_run_till_idle_wrapped()" << vcl_endl;
  vgui_macro_warning << "please report to fsm@robots.ox.ac.uk" << vcl_endl;
  vcl_abort();
}

//--------------------------------------------------------------------------------

// When set, this flag indicates that the event
// loop should be terminated in the near future.
static bool internal_quit_flag = false;

void vgui_glut::run()
{
  internal_quit_flag = false;
  while (! internal_quit_flag)
    internal_run_till_idle();
  vgui_macro_warning << "end of vgui_glut event loop" << vcl_endl;
}

// This is (erroneously) called from vgui_glut_adaptor::post_destroy().
void vgui_glut_quit()
{
  internal_quit_flag = true;
}

void vgui_glut::quit()
{
  internal_quit_flag = true;
}

// This is actually run-a-few-events, sorry...
void vgui_glut::run_one_event()
{
  internal_run_till_idle();
}

void vgui_glut::run_till_idle()
{
  internal_run_till_idle();
}

//--------------------------------------------------------------------------------
