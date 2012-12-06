/* coolwave2.c
 *
 * Copyright (c) 2002 Alif Wahid <awah005@users.sourceforge.net>
 *
 * A program to demonstrate GtkGLExt. It's a modified version
 * of the old IrisGL demo 'newave', first ported to OpenGL and
 * Glut by Erik Larsen. Now I have modified it to use Gtk and GtkGLExt
 * comprehensively along with ofcourse OpenGL.
 *
 * This program is in the public domain and you are using it at
 * your own risk.
 *
 **************************************************************************/

/*
 * Modified by Naofumi Yasufuku  <naofumi@users.sourceforge.net>
 */

/* September, 2003.
 *
 * A slightly different version from the coolwave.c example program. Here
 * the primary goal is to test how well GtkGLExt works with GTK's
 * ability to display a window in fullscreen mode. It's quite nice to have
 * a 3D animation displayed in fullscreen mode!
 *
 * So just use the menu items to change between the fullscreen and normal
 * size mode.
 *
 * Alif Wahid.
 */

/**************************************************************************
 * Header file inclusions.
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <gtk/gtkgl.h>

#ifdef GDK_WINDOWING_QUARTZ
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#endif


/**************************************************************************
 * The following section contains all the macro definitions.
 **************************************************************************/

#define DEFAULT_WIDTH  300
#define DEFAULT_HEIGHT 200
#define DEFAULT_TITLE  "CoolWave2"

#define TIMEOUT_INTERVAL 10

#define MAXGRID 64
#define SQRTOFTWOINV (1.0 / 1.414213562)


/**************************************************************************
 * Global variable declarations.
 **************************************************************************/

static gboolean animate = TRUE;

static int grid = (MAXGRID/2);
static int beginX, beginY;

static float force[MAXGRID][MAXGRID];
static float veloc[MAXGRID][MAXGRID];
static float posit[MAXGRID][MAXGRID];

static float dt = 0.008;
static float sphi = 90.0;
static float stheta = 45.0;
static float sdepth = 5.0/4.0 * (MAXGRID/2);
static float zNear = (MAXGRID/2)/10.0;
static float zFar = (MAXGRID/2)*3.0;
static float aspect = 5.0/4.0;

static float lightPosition[4] = {0.0, 0.0, 1.0, 1.0};


/**************************************************************************
 * The following section contains the function prototype declarations.
 **************************************************************************/

static void         timeout_add       (GtkWidget   *widget);
static void         timeout_remove    (GtkWidget   *widget);

static void         toggle_animation  (GtkWidget   *widget);
static void         init_wireframe    (GtkWidget   *widget);

static GdkGLConfig *configure_gl      (void);

static GtkWidget   *create_popup_menu (GtkWidget   *window, GtkWidget   *drawing_area);
static GtkWidget   *create_window     (GdkGLConfig *glconfig);


/**************************************************************************
 * The waving functions.
 **************************************************************************/

void getforce (void)
{
  int i=0, j=0;
  float d;

  for(i=0;i<grid;i++)
    {
      for(j=0;j<grid;j++)
	{
	  force[i][j]=0.0;
	}
    }

  for(i=2;i<grid-2;i++)
    {
      for(j=2;j<grid-2;j++)
	{
	  d=posit[i][j]-posit[i][j-1];
	  force[i][j] -= d;
	  force[i][j-1] += d;

	  d=posit[i][j]-posit[i-1][j];
	  force[i][j] -= d;
	  force[i-1][j] += d;

	  d= (posit[i][j]-posit[i][j+1]);
	  force[i][j] -= d ;
	  force[i][j+1] += d;

	  d= (posit[i][j]-posit[i+1][j]);
	  force[i][j] -= d ;
	  force[i+1][j] += d;

	  d= (posit[i][j]-posit[i+1][j+1])*SQRTOFTWOINV;
	  force[i][j] -= d ;
	  force[i+1][j+1] += d;

	  d= (posit[i][j]-posit[i-1][j-1])*SQRTOFTWOINV;
	  force[i][j] -= d ;
	  force[i-1][j-1] += d;

	  d= (posit[i][j]-posit[i+1][j-1])*SQRTOFTWOINV;
	  force[i][j] -= d ;
	  force[i+1][j-1] += d;

	  d= (posit[i][j]-posit[i-1][j+1])*SQRTOFTWOINV;
	  force[i][j] -= d ;
	  force[i- 1][j+1] += d;
	}
    }
}

void getvelocity (void)
{
  int i=0, j=0;

  for(i=0;i<grid;i++)
    {
      for(j=0;j<grid;j++) veloc[i][j]+=force[i][j] * dt;
    }
}

void getposition (void)
{
  int i=0, j=0;

  for(i=0;i<grid;i++)
    {
      for(j=0;j<grid;j++) posit[i][j]+=veloc[i][j];
    }
}

void drawWireframe (void)
{
  int i=0, j=0;

  glColor3f(1.0, 1.0, 1.0);

  for(i=0;i<grid;i++)
    {
      glBegin(GL_LINE_STRIP);
      for(j=0;j<grid;j++) glVertex3f((float)i,(float)j,(float)posit[i][j]);
      glEnd();
    }

  for(i=0;i<grid;i++)
    {
      glBegin(GL_LINE_STRIP);
      for(j=0;j<grid;j++) glVertex3f((float)j,(float)i,(float)posit[j][i]);
      glEnd();
    }
}

void resetWireframe (void)
{
  int i=0, j=0;

  for(i=0;i<grid;i++)
    {
      for(j=0;j<grid;j++)
	{
	  force[i][j]=0.0;
	  veloc[i][j]=0.0;

	  posit[i][j]= 	(sin(G_PI*2 * ((float)i/(float)grid)) +
			 sin(G_PI*2 * ((float)j/(float)grid)))* grid/6.0;

	  if (i==0||j==0||i==grid-1||j==grid-1) posit[i][j]=0.0;
	}
    }
}


/**************************************************************************
 * The following section contains all the callback function definitions.
 **************************************************************************/

/***
 *** The "realize" signal handler. All the OpenGL initialization
 *** should be performed here, such as default background colour,
 *** certain states etc.
 ***/
static void
realize (GtkWidget *widget,
	 gpointer   data)
{
  void (APIENTRY *PolygonOffset)(GLfloat, GLfloat) = NULL;

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return;

  /* glPolygonOffsetEXT */
  PolygonOffset = (void (APIENTRY *)(GLfloat, GLfloat))gdk_gl_get_proc_address ("glPolygonOffsetEXT");
  if (PolygonOffset == NULL)
    {
      /* glPolygonOffset */
      PolygonOffset = (void (APIENTRY *)(GLfloat, GLfloat))gdk_gl_get_proc_address ("glPolygonOffset");
      if (PolygonOffset == NULL)
        {
          g_print ("Sorry, glPolygonOffset() is not supported by this renderer.\n");
          exit (1);
        }
    }

  glEnable (GL_DEPTH_TEST);
  glDepthFunc (GL_LEQUAL);
  glClearColor (0.0, 0.0, 0.0, 0.0);
  PolygonOffset (1.0, 1.0);
  glEnable (GL_CULL_FACE);
  glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glEnable (GL_COLOR_MATERIAL);
  glColorMaterial (GL_FRONT, GL_DIFFUSE);
  glLightfv (GL_LIGHT0, GL_POSITION, lightPosition);
  glEnable (GL_LIGHT0);
  glShadeModel (GL_FLAT);
  glDisable (GL_LIGHTING);

  resetWireframe ();

  gtk_widget_end_gl (widget, FALSE);
  /*** OpenGL END ***/

  return;
}

/***
 *** The "configure_event" signal handler. Any processing required when
 *** the OpenGL-capable drawing area is re-configured should be done here.
 *** Almost always it will be used to resize the OpenGL viewport when
 *** the window is resized.
 ***/
static gboolean
configure_event (GtkWidget         *widget,
                 GdkEventConfigure *event,
                 gpointer           data)
{
  GLfloat w;
  GLfloat h;

  w = event->width;
  h = event->height;

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return FALSE;

  aspect = (float)w/(float)h;
  glViewport (0, 0, w, h);

  gtk_widget_end_gl (widget, FALSE);
  /*** OpenGL END ***/

  return TRUE;
}

/***
 *** The "draw" signal handler. All the OpenGL re-drawing should
 *** be done here. This is repeatedly called as the painting routine
 *** every time the 'draw' event is signalled.
 ***/
static gboolean
draw (GtkWidget *widget,
      cairo_t   *event,
      gpointer   data)
{
  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl(widget))
    return FALSE;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (64.0, aspect, zNear, zFar);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glTranslatef (0.0,0.0,-sdepth);
  glRotatef (-stheta, 1.0, 0.0, 0.0);
  glRotatef (sphi, 0.0, 0.0, 1.0);
  glTranslatef (-(float)((grid+1)/2-1), -(float)((grid+1)/2-1), 0.0);

  drawWireframe ();

  gtk_widget_end_gl (widget, TRUE);
  /*** OpenGL END ***/

  return TRUE;
}

/***
 *** The timeout function. Often in animations,
 *** timeout functions are suitable for continous
 *** frame updates.
 ***/
static gboolean
timeout (GtkWidget *widget)
{
  GtkAllocation allocation;
  GdkWindow *window;
  getforce ();
  getvelocity ();
  getposition ();

  window = gtk_widget_get_window (widget);
  gtk_widget_get_allocation (widget, &allocation);

  /* Invalidate the whole window. */
  gdk_window_invalidate_rect (window, &allocation, FALSE);

  /* Update synchronously (fast). */
  gdk_window_process_updates (window, FALSE);

  return TRUE;
}

/***
 *** The "motion_notify_event" signal handler. Any processing required when
 *** the OpenGL-capable drawing area is under drag motion should be done here.
 ***/
static gboolean
motion_notify_event (GtkWidget      *widget,
		     GdkEventMotion *event,
		     gpointer        data)
{
  GtkAllocation allocation;
  gboolean redraw = FALSE;

  gtk_widget_get_allocation (widget, &allocation);

  if (event->state & GDK_BUTTON1_MASK)
    {
      sphi += (float)(event->x - beginX) / 4.0;
      stheta += (float)(beginY - event->y) / 4.0;

      redraw = TRUE;
    }

  if (event->state & GDK_BUTTON2_MASK)
    {
      sdepth -= ((event->y - beginY)/(allocation.height))*(MAXGRID/2);

      redraw = TRUE;
    }

  beginX = event->x;
  beginY = event->y;

  if (redraw && !animate)
    gdk_window_invalidate_rect (gtk_widget_get_window (widget), &allocation, FALSE);

  return TRUE;
}

/***
 *** The "button_press_event" signal handler. Any processing required when
 *** mouse buttons (only left and middle buttons) are pressed on the OpenGL-
 *** capable drawing area should be done here.
 ***/
static gboolean
button_press_event (GtkWidget      *widget,
		    GdkEventButton *event,
		    gpointer        data)
{
  if (event->button == 1)
    {
      beginX = event->x;
      beginY = event->y;
      return TRUE;
    }

  if (event->button == 2)
    {
      beginX = event->x;
      beginY = event->y;
      return TRUE;
    }

  return FALSE;
}

/* For popup menu. */
static gboolean
button_press_event_popup_menu (GtkWidget      *widget,
			       GdkEventButton *event,
			       gpointer        data)
{
  if (event->button == 3)
    {
      /* Popup menu. */
      gtk_menu_popup (GTK_MENU(widget), NULL, NULL, NULL, NULL,
		      event->button, event->time);
      return TRUE;
    }

  return FALSE;
}

/***
 *** The "key_press_event" signal handler. Any processing required when key
 *** presses occur should be done here.
 ***/

static gboolean
key_press_event (GtkWidget   *widget,
		 GdkEventKey *event,
		 gpointer     data)
{
  GtkAllocation allocation;

  switch (event->keyval)
    {
    case GDK_KEY_r:
      init_wireframe (widget);
      break;

    case GDK_KEY_a:
      toggle_animation (widget);
      break;

    case GDK_KEY_w:
      if (!animate)
	timeout (widget);
      break;

    case GDK_KEY_plus:
      sdepth -= 2.0;
      break;

    case GDK_KEY_minus:
      sdepth += 2.0;
      break;

    case GDK_KEY_Escape:
      gtk_main_quit ();
      break;

    default:
      return FALSE;
    }

  if (!animate)
    {
      gtk_widget_get_allocation (widget, &allocation);
      gdk_window_invalidate_rect (gtk_widget_get_window (widget), &allocation, FALSE);
    }

  return TRUE;
}

/***
 *** The "unrealize" signal handler. Any processing required when
 *** the OpenGL-capable window is unrealized should be done here.
 ***/
static void
unrealize (GtkWidget *widget,
	   gpointer   data)
{
  /*** Fill in the details here ***/

}


/**************************************************************************
 * The following section contains the timeout function management routines.
 **************************************************************************/

/***
 *** Helper functions to add or remove the timeout function.
 ***/

static guint timeout_id = 0;

static void
timeout_add (GtkWidget *widget)
{
  if (timeout_id == 0)
    {
      timeout_id = g_timeout_add (TIMEOUT_INTERVAL,
                                  (GSourceFunc) timeout,
                                  widget);
    }
}

static void
timeout_remove (GtkWidget *widget)
{
  if (timeout_id != 0)
    {
      g_source_remove (timeout_id);
      timeout_id = 0;
    }
}

/***
 *** The "map_event" signal handler. Any processing required when the
 *** OpenGL-capable drawing area is mapped should be done here.
 ***/
static gboolean
map_event (GtkWidget *widget,
	   GdkEvent  *event,
	   gpointer   data)
{
  if (animate)
    timeout_add (widget);

  return TRUE;
}

/***
 *** The "unmap_event" signal handler. Any processing required when the
 *** OpenGL-capable drawing area is unmapped should be done here.
 ***/
static gboolean
unmap_event (GtkWidget *widget,
	     GdkEvent  *event,
	     gpointer   data)
{
  timeout_remove (widget);

  return TRUE;
}

/***
 *** The "visibility_notify_event" signal handler. Any processing required
 *** when the OpenGL-capable drawing area is visually obscured should be
 *** done here.
 ***/
static gboolean
visibility_notify_event (GtkWidget          *widget,
			 GdkEventVisibility *event,
			 gpointer            data)
{
  if (animate)
    {
      if (event->state == GDK_VISIBILITY_FULLY_OBSCURED)
	timeout_remove (widget);
      else
	timeout_add (widget);
    }

  return TRUE;
}


/**************************************************************************
 * The following section contains some miscellaneous utility functions.
 **************************************************************************/

/***
 *** Toggle animation.
 ***/
static void
toggle_animation (GtkWidget *widget)
{
  GtkAllocation allocation;
  animate = !animate;

  if (animate)
    {
      timeout_add (widget);
    }
  else
    {
      timeout_remove (widget);
      gtk_widget_get_allocation (widget, &allocation);
      gdk_window_invalidate_rect (gtk_widget_get_window (widget), &allocation, FALSE);
    }
}

/***
 *** Init wireframe model.
 ***/
static void
init_wireframe (GtkWidget *widget)
{
  GtkAllocation allocation;

  gtk_widget_get_allocation (widget, &allocation);

  resetWireframe ();
  gdk_window_invalidate_rect (gtk_widget_get_window (widget), &allocation, FALSE);
}


/**************************************************************************
 * The following section contains the GUI building function definitions.
 **************************************************************************/

/* Two callbacks for the popup menu items Fullscreen and Unfullscreen.
 * These are just making sure that the user can't select the same menu
 * item among these, twice in a row by making the current item selected
 * insensitive. GTK+-2.2.3 on my WinXP box seems to ignore any calls to
 * unfullscreen if the user has selected fullscreen on a window that's
 * already fullscreen.
 */
static void fullscreen_window (GtkWidget *current, gpointer other)
{
  GtkWidget *window = g_object_get_data(G_OBJECT(current), "window");
  if (window)
    {
	  gtk_widget_set_sensitive(GTK_WIDGET(current), FALSE);
	  gtk_widget_set_sensitive(GTK_WIDGET(other), TRUE);
	  gtk_window_fullscreen(GTK_WINDOW(window));
    }
}

static void unfullscreen_window (GtkWidget *current, gpointer other)
{
  GtkWidget *window = g_object_get_data(G_OBJECT(current), "window");
  if (window)
    {
	  gtk_widget_set_sensitive(GTK_WIDGET(current), FALSE);
	  gtk_widget_set_sensitive(GTK_WIDGET(other), TRUE);
	  gtk_window_unfullscreen(GTK_WINDOW(window));
    }
}

/***
 *** Creates the popup menu to be displayed.
 ***/
static GtkWidget *
create_popup_menu (GtkWidget *window, GtkWidget *drawing_area)
{
  GtkWidget *menu;
  GtkWidget *menu_item;
  GtkWidget *full_item;
  GtkWidget *unfull_item;

  menu = gtk_menu_new ();

  /* Toggle animation */
  menu_item = gtk_menu_item_new_with_label ("Toggle Animation");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
  g_signal_connect_swapped (G_OBJECT (menu_item), "activate",
			    G_CALLBACK (toggle_animation), drawing_area);
  gtk_widget_show (menu_item);

  /* Init wireframe model */
  menu_item = gtk_menu_item_new_with_label ("Initialize");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
  g_signal_connect_swapped (G_OBJECT (menu_item), "activate",
			    G_CALLBACK (init_wireframe), drawing_area);
  gtk_widget_show (menu_item);

  /* Fullscreen window. */
  full_item = gtk_menu_item_new_with_label ("Fullscreen");
  g_object_set_data(G_OBJECT(full_item), "window", window);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), full_item);

  /* Un-fullscreen window. */
  unfull_item = gtk_menu_item_new_with_label ("Unfullscreen");
  g_object_set_data(G_OBJECT(unfull_item), "window", window);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), unfull_item);
  gtk_widget_set_sensitive(unfull_item, FALSE);

  /* Connect the fullscreen and unfullscreen callbacks. */
  g_signal_connect (G_OBJECT (full_item), "activate",
			    G_CALLBACK (fullscreen_window), unfull_item);
  g_signal_connect (G_OBJECT (unfull_item), "activate",
			    G_CALLBACK (unfullscreen_window), full_item);
  gtk_widget_show (full_item);
  gtk_widget_show (unfull_item);

  /* Quit */
  menu_item = gtk_menu_item_new_with_label ("Quit");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
		    G_CALLBACK (gtk_main_quit), NULL);
  gtk_widget_show (menu_item);

  return menu;
}

/***
 *** Creates the simple application window with one
 *** drawing area that has an OpenGL-capable visual.
 ***/
static GtkWidget *
create_window (GdkGLConfig *glconfig)
{
  GtkWidget *window;
  GtkWidget *drawing_area;
  GtkWidget *menu;

  /*
   * Top-level window.
   */

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), DEFAULT_TITLE);

  /* Get automatically redrawn if any of their children changed allocation. */
  gtk_container_set_reallocate_redraws (GTK_CONTAINER (window), TRUE);

  /* Connect signal handlers to the window */
  g_signal_connect (G_OBJECT (window), "delete_event",
		    G_CALLBACK (gtk_main_quit), NULL);

  /*
   * Drawing area to draw OpenGL scene.
   */

  drawing_area = gtk_drawing_area_new ();
  gtk_widget_set_size_request (drawing_area, DEFAULT_WIDTH, DEFAULT_HEIGHT);

  /* Set OpenGL-capability to the widget */
  gtk_widget_set_gl_capability (drawing_area,
				glconfig,
				NULL,
				TRUE,
				GDK_GL_RGBA_TYPE);

  gtk_widget_add_events (drawing_area,
			 GDK_BUTTON1_MOTION_MASK    |
			 GDK_BUTTON2_MOTION_MASK    |
			 GDK_BUTTON_PRESS_MASK      |
			 GDK_VISIBILITY_NOTIFY_MASK);

  /* Connect signal handlers to the drawing area */
  g_signal_connect_after (G_OBJECT (drawing_area), "realize",
                          G_CALLBACK (realize), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "configure_event",
		    G_CALLBACK (configure_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "draw",
		    G_CALLBACK (draw), NULL);

  g_signal_connect (G_OBJECT (drawing_area), "motion_notify_event",
		    G_CALLBACK (motion_notify_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "button_press_event",
		    G_CALLBACK (button_press_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "unrealize",
		    G_CALLBACK (unrealize), NULL);

  /* key_press_event handler for top-level window */
  g_signal_connect_swapped (G_OBJECT (window), "key_press_event",
			    G_CALLBACK (key_press_event), drawing_area);

  /* For timeout function. */
  g_signal_connect (G_OBJECT (drawing_area), "map_event",
		    G_CALLBACK (map_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "unmap_event",
		    G_CALLBACK (unmap_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "visibility_notify_event",
		    G_CALLBACK (visibility_notify_event), NULL);

  gtk_container_add (GTK_CONTAINER (window), drawing_area);
  gtk_widget_show (drawing_area);

  /*
   * Popup menu.
   */

  menu = create_popup_menu (window, drawing_area);

  g_signal_connect_swapped (G_OBJECT (drawing_area), "button_press_event",
			    G_CALLBACK (button_press_event_popup_menu), menu);

  return window;
}


/**************************************************************************
 * The following section contains utility function definitions.
 **************************************************************************/

/***
 *** Configure the OpenGL framebuffer.
 ***/
static GdkGLConfig *
configure_gl (void)
{
  GdkGLConfig *glconfig;

  /* Try double-buffered visual */
  glconfig = gdk_gl_config_new_by_mode (GDK_GL_MODE_RGB    |
					GDK_GL_MODE_DEPTH  |
					GDK_GL_MODE_DOUBLE);
  if (glconfig == NULL)
    {
      g_print ("\n*** Cannot find the double-buffered visual.\n");
      g_print ("\n*** Trying single-buffered visual.\n");

      /* Try single-buffered visual */
      glconfig = gdk_gl_config_new_by_mode (GDK_GL_MODE_RGB   |
					    GDK_GL_MODE_DEPTH);
      if (glconfig == NULL)
	{
	  g_print ("*** No appropriate OpenGL-capable visual found.\n");
	  exit (1);
	}
    }

  return glconfig;
}


/**************************************************************************
 * The main function is rather trivial.
 **************************************************************************/

int
main (int   argc,
      char *argv[])
{
  GtkWidget *window;
  GdkGLConfig *glconfig;

  /* Initialize GTK. */
  gtk_init (&argc, &argv);

  /* Initialize GtkGLExt. */
  gtk_gl_init (&argc, &argv);

  /* Configure OpenGL framebuffer. */
  glconfig = configure_gl ();

  /* Create and show the application window. */
  window = create_window (glconfig);
  gtk_widget_show (window);

  gtk_main ();

  return 0;
}


/**************************************************************************
 * End of file.
 **************************************************************************/
