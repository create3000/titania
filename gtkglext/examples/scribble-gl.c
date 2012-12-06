/**************************************************************************
 * scribble-gl.c
 *
 * This is an OpenGL version of the scribble program from the Gtk library.
 * All the drawing here is obviously done using OpenGL.
 *
 * This program is in the public domain and you are using it at
 * your own risk.
 *
 * Alif Wahid, <awah005@users.sourceforge.net>
 * May 2003.
 **************************************************************************/

/**************************************************************************
 * Header file inclusions.
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <gtk/gtkgl.h>

#ifdef G_OS_WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#ifdef GDK_WINDOWING_QUARTZ
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

/**************************************************************************
 * The following section contains all the macro definitions.
 **************************************************************************/

/***
 *** Change these three macros to customise the
 *** default width and height of the drawing
 *** area, plus the default title of the window.
 ***/
#define DEFAULT_WIDTH  200
#define DEFAULT_HEIGHT 200
#define DEFAULT_TITLE  "Scribble-GL"


/**************************************************************************
 * Global variable declarations.
 **************************************************************************/

/* Structure for storing points of button clicks and drag motion. */
typedef struct _Point Point;
struct _Point
{
  gint x;
  gint y;
};

/* List of all the brush stroke points. */
static GList* brushStrokeList = NULL;

/* Thickness of the brush strokes. */
static gint thickness = 5;

/**************************************************************************
 * The following section contains the function prototype declarations.
 **************************************************************************/

static GdkGLConfig *configure_gl      (void);

static GtkWidget   *create_window     (GdkGLConfig *glconfig);


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
  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return;

  /*** Fill in the details here. ***/
  glClearColor(1.0,1.0,1.0,1.0);
  glClearDepth(1.0);

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
  GtkAllocation allocation;
  guint count;

  GLfloat w;
  GLfloat h;

  gtk_widget_get_allocation (widget, &allocation);
  w = allocation.width;
  h = allocation.height;

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return FALSE;

  /* Reallocate the brush stroke list. */
  for (count=0; count<g_list_length(brushStrokeList); count++)
    {
      g_free(g_list_nth_data(brushStrokeList, count));
    }
  g_list_free(brushStrokeList);
  brushStrokeList = NULL;

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, w, 0.0, h, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

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
      cairo_t   *cr,
      gpointer   data)
{
  guint count;
  Point* coord;

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return FALSE;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Set the foreground colour. */
  glColor3f(0.0,0.0,0.0);

  /* Draw the list of brush strokes. */
  for (count=0; count<g_list_length(brushStrokeList); count++)
    {
      coord = g_list_nth_data(brushStrokeList, count);
      glRecti(coord->x + thickness,
              coord->y - thickness,
              coord->x - thickness,
              coord->y + thickness);
    }

  gtk_widget_end_gl (widget, TRUE);
  /*** OpenGL END ***/

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
  Point* coord=NULL;

  if (event->state & GDK_BUTTON1_MASK)
    {
      gtk_widget_get_allocation (widget, &allocation);
      coord = g_malloc(sizeof(Point));
      coord->x = event->x;
      coord->y = allocation.height - event->y;

      brushStrokeList = g_list_append(brushStrokeList, coord);
      gdk_window_invalidate_rect (gtk_widget_get_window (widget), &allocation, FALSE);
      return TRUE;
    }

  return FALSE;
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
  GtkAllocation allocation;
  Point* coord=NULL;

  if (event->button == 1)
    {
      gtk_widget_get_allocation (widget, &allocation);
      coord = g_malloc(sizeof(Point));
      coord->x = event->x;
      coord->y = allocation.height - event->y;

      brushStrokeList = g_list_append(brushStrokeList, coord);
      gdk_window_invalidate_rect (gtk_widget_get_window (widget), &allocation, FALSE);
      return TRUE;
    }

  return FALSE;
}

static void
quit_cb (GtkWidget* widget, gpointer data)
{
  guint count;

  /* Free the resources occupied by brushStrokeList. */
  for (count=0; count<g_list_length(brushStrokeList); count++)
    {
      g_free(g_list_nth_data(brushStrokeList, count));
    }
  g_list_free(brushStrokeList);

  gtk_main_quit();
}

/**************************************************************************
 * The following section contains the GUI building function definitions.
 **************************************************************************/

/***
 *** Creates the simple application window with one
 *** drawing area that has an OpenGL-capable visual.
 ***/
static GtkWidget *
create_window (GdkGLConfig *glconfig)
{
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *drawing_area;
  GtkWidget *button;

  /*
   * Top-level window.
   */

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), DEFAULT_TITLE);

  /* Get automatically redrawn if any of their children changed allocation. */
  gtk_container_set_reallocate_redraws (GTK_CONTAINER (window), TRUE);

  /* Connect signal handlers to the window */
  g_signal_connect (G_OBJECT (window), "delete_event",
		    G_CALLBACK (quit_cb), NULL);

  /*
   * VBox.
   */

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_widget_show (vbox);

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
			 GDK_BUTTON_PRESS_MASK);

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

  gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);

  gtk_widget_show (drawing_area);

  /*
   * Simple quit button.
   */

  button = gtk_button_new_with_label ("Quit");

  g_signal_connect (G_OBJECT (button), "clicked",
                    G_CALLBACK (quit_cb), NULL);

  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

  gtk_widget_show (button);

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
