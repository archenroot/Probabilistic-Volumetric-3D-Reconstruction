/*
  awf@robots.ox.ac.uk
*/
#include "vgui_mfc.h"
#include <vgui/vgui_tag.h>
#include <vbl/vbl_trace.h>

vgui_tag(mfc) {
  vbl_trace;
  vgui_mfc::instance();
  vbl_trace;
  return 0;
}

