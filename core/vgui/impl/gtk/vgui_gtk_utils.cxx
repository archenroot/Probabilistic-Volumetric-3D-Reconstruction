//-*- c++ -*-------------------------------------------------------------------
#ifdef __GNUC__
#pragma implementation
#endif
//
// Class: vgui_gtk_utils
// Author: Philip C. Pritchett, RRG, University of Oxford
// Created: 19 Dec 99
//
//-----------------------------------------------------------------------------

#include "vgui_gtk_utils.h"

#include <vcl_iostream.h>

#include <vgui/vgui_gl.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>

#include <vgui/vgui_command.h>
#include <vgui/vgui_menu.h>

static bool debug = false;
GtkAccelGroup *vgui_gtk_utils::accel_group = NULL;

vgui_button vgui_gtk_utils::translate_button(int button) {
  if (button == 1)
    return vgui_LEFT;
  else if (button == 2)
    return vgui_MIDDLE;
  else if (button == 3)
    return vgui_RIGHT;

  abort();
  return vgui_BUTTON_NULL;
}

vgui_key vgui_gtk_utils::translate_key(GdkEventKey const *gev) {
  if (gev->length == 1)
    return vgui_key( *(gev->string) );

  switch(gev->keyval) {
  case GDK_Page_Up:
    return vgui_PAGE_UP;
  case GDK_Page_Down:
    return vgui_PAGE_DOWN;
  }

  return vgui_KEY_NULL;
}
  
void vgui_gtk_utils::set_coordinates(vgui_event &e, const gdouble x, const gdouble y) {
  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  // FIXME : the size of the current glViewport settings is not
  // really what we want -- we want the size of the _window_.
  e.wx = (int)x;
  e.wy = vp[3]-1-(int)y;
}

void vgui_gtk_utils::set_modifiers(vgui_event &e, const guint state) {
  e.modifier = vgui_modifier( ((state & 4)?vgui_CTRL:0) |
			      ((state & 8)?vgui_ALT:0) |
			      ((state & 1)?vgui_SHIFT:0) );
}

  
bool vgui_gtk_utils::is_modifier(GdkEvent const *gev) {

  if (gev->type == GDK_KEY_PRESS || 
      gev->type == GDK_KEY_RELEASE) {
    
    GdkEventKey *e = (GdkEventKey*)gev;
    
    // -- This code would only return 'true' if any of the modifier is solely
    // pressed. However we want to return 'true' so long as any of the 
    // modifiers is pressed simultaneously with some other key. This is a must
    // if want to allow menu accelerator keys. -- u97mb

    //if (e->keyval & GDK_Shift_L || 
    //e->keyval & GDK_Shift_R || 
    //e->keyval & GDK_Control_L || 
    //e->keyval & GDK_Control_R || 
    //e->keyval & GDK_Meta_L || 
    //e->keyval & GDK_Meta_R ||
    //e->keyval & GDK_Alt_L || 
    //e->keyval & GDK_Alt_R) 
    
    // -- u97mb
    // GDK_MOD1_MASK corresponds to META key(at least on Sun Solaris)
    if(e->state & GDK_CONTROL_MASK ||
       e->state & GDK_SHIFT_MASK ||
       e->state & GDK_MOD1_MASK)
      return true;
  }
  return false;
}



static void execute_command(GtkWidget*, gpointer c) {
  vgui_command *cmnd = static_cast<vgui_command*>(c);
  cmnd->execute();
}


void vgui_gtk_utils::add_submenu(GtkWidget *widget, const vgui_menu& menu) {

  for (unsigned i=0;i<menu.size();i++) {

    if (menu[i].is_separator()) {
      if (debug) vcl_cerr << " <separator> " << vcl_endl;
      GtkWidget* item = gtk_menu_item_new();
      gtk_menu_append(GTK_MENU(widget), item);
      gtk_widget_show(item);
      
    }

    else if (menu[i].is_command()) {
      if (debug) vcl_cerr << " <command>" << vcl_endl;
      GtkWidget* item = gtk_menu_item_new_with_label(menu[i].name.c_str());
      gtk_signal_connect(GTK_OBJECT(item), "activate",
			 GTK_SIGNAL_FUNC(execute_command), 
			 (void*)menu[i].cmnd.as_pointer());
      
      gtk_menu_append(GTK_MENU(widget), item);
      
      gtk_widget_show(item);  
      if(menu[i].short_cut.mod!=vgui_MODIFIER_NULL || 
	 menu[i].short_cut.key!=vgui_KEY_NULL)
	{
	  gint mask = 0;
	  // -- Health warning -- It seems that GDK_MOD1_MASK corresponds
	  // to META on Solaris and ALT has no correspondance
	  if(menu[i].short_cut.mod & vgui_CTRL)
	    mask|=GDK_CONTROL_MASK;
	  if(menu[i].short_cut.mod & vgui_META)
	    mask|=GDK_MOD1_MASK;
	  if(menu[i].short_cut.mod & vgui_SHIFT)
	    mask|=GDK_SHIFT_MASK;
	  if(menu[i].short_cut.mod & vgui_ALT)
	    mask|=GDK_MOD2_MASK;
	  gtk_widget_add_accelerator (item,
				      "activate",
				      accel_group,
				      char(menu[i].short_cut.key),
				      mask,
				      GtkAccelFlags(GTK_ACCEL_VISIBLE|GTK_ACCEL_LOCKED));
	}
    
    }
    else if (menu[i].is_submenu()) {
      if (debug) vcl_cerr << " <submenu>" << vcl_endl;
      GtkWidget* item = gtk_menu_item_new_with_label(menu[i].name.c_str());
      GtkWidget* submenu = gtk_menu_new();

      gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), submenu);
      gtk_menu_append(GTK_MENU(widget), item);
      gtk_widget_show(item);

      add_submenu(submenu, *menu[i].menu);

    }
  }   
}

void vgui_gtk_utils::set_menu(GtkWidget *widget, const vgui_menu& menu, bool is_menubar) {

  for (unsigned i=0;i<menu.size();i++) {
    
    if (menu[i].is_separator()) {
      if (debug) vcl_cerr << " <separator> " << vcl_endl;
      GtkWidget* item = gtk_menu_item_new();
      if (!is_menubar)
	gtk_menu_append(GTK_MENU(widget), item);
      gtk_widget_show(item);
      
    }
    else if (menu[i].is_command()) {
      if (debug) vcl_cerr << " <command> " << menu[i].name << vcl_endl;
      GtkWidget* item = gtk_menu_item_new_with_label(menu[i].name.c_str());

      if (is_menubar)
	gtk_menu_bar_append(GTK_MENU_BAR(widget), item);
      else {
	if(menu[i].short_cut.mod!=vgui_MODIFIER_NULL || 
	   menu[i].short_cut.key!=vgui_KEY_NULL)
	  {
	    gint mask = 0;
	    // -- Health warning -- It seems that GDK_MOD1_MASK corresponds
	    // to META on Solaris and ALT has no correspondance
	    if(menu[i].short_cut.mod & vgui_CTRL)
	      mask|=GDK_CONTROL_MASK;
	    if(menu[i].short_cut.mod & vgui_META)
	      mask|=GDK_MOD1_MASK;
	    if(menu[i].short_cut.mod & vgui_SHIFT)
	      mask|=GDK_SHIFT_MASK;
	    if(menu[i].short_cut.mod & vgui_ALT)
	      mask|=GDK_MOD2_MASK;
	    gtk_widget_add_accelerator (item,
					"activate",
					accel_group,
					char(menu[i].short_cut.key),
					mask,
					GtkAccelFlags(GTK_ACCEL_VISIBLE|GTK_ACCEL_LOCKED));
	  }
	gtk_menu_append(GTK_MENU(widget), item);
      }
      
      gtk_signal_connect(GTK_OBJECT(item), "activate",
			 GTK_SIGNAL_FUNC(execute_command), 
			 (void*)menu[i].cmnd.as_pointer());

      gtk_widget_show(item);


    }
    else if (menu[i].is_submenu()) {
      if (debug) vcl_cerr << " <submenu> " << menu[i].name << vcl_endl;

      GtkWidget* item = gtk_menu_item_new_with_label(menu[i].name.c_str());
      GtkWidget* submenu = gtk_menu_new();

      gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), submenu);


      if (is_menubar)
	gtk_menu_bar_append(GTK_MENU_BAR(widget), item);
      else 
	gtk_menu_append(GTK_MENU(widget), item);


      gtk_widget_show(item);

      add_submenu(submenu, *menu[i].menu);


    }
  }
}
