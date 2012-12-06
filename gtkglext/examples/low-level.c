/*
 * low-level.c:
 * Low-level GdkGLExt example. This program uses only gdkgl stuff.
 *
 * written by Naofumi Yasufuku  <naofumi@users.sourceforge.net>
 */

#include <stdlib.h>

#include <gtk/gtk.h>

#include <gdk/gdkgl.h>

#ifdef G_OS_WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#ifdef GDK_WINDOWING_QUARTZ
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

static GdkGLConfig *glconfig = NULL;
static GdkGLWindow *glwindow = NULL;
static GdkGLContext *glcontext = NULL;

static const int config_attributes[] = {
  GDK_GL_DOUBLEBUFFER,
  GDK_GL_RGBA,
  GDK_GL_RED_SIZE,        1,
  GDK_GL_GREEN_SIZE,      1,
  GDK_GL_BLUE_SIZE,       1,
  GDK_GL_DEPTH_SIZE,      12,
};

static void
realize (GtkWidget *widget,
         gpointer   data)
{
  GtkAllocation allocation;
  GdkWindow *window;
  GLUquadricObj *qobj;
  static GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};
  static GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};

  window = gtk_widget_get_window (widget);

  /*
   * Create GdkGLWindow for widget->window.
   */

  glwindow = gdk_gl_window_new (glconfig,
                                window,
                                NULL);

  /* Set a background of "None" on window to avoid AIX X server crash */
  gdk_window_set_background_pattern (window, NULL);

  /*
   * Create OpenGL rendering context.
   */

  if (glcontext == NULL)
    {
      glcontext = gdk_gl_context_new (GDK_GL_DRAWABLE (glwindow),
                                      NULL,
                                      TRUE,
                                      GDK_GL_RGBA_TYPE);
      if (glcontext == NULL)
        {
          g_print ("Connot create the OpenGL rendering context\n");
          exit (1);
        }

      g_print ("The OpenGL rendering context is created\n");
    }

  /*** OpenGL BEGIN ***/

  if (!gdk_gl_context_make_current (glcontext,
                                    GDK_GL_DRAWABLE (glwindow),
                                    GDK_GL_DRAWABLE (glwindow)))
    return;

  qobj = gluNewQuadric ();
  gluQuadricDrawStyle (qobj, GLU_FILL);
  glNewList (1, GL_COMPILE);
  gluSphere (qobj, 1.0, 20, 20);
  glEndList ();

  glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv (GL_LIGHT0, GL_POSITION, light_position);
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  glEnable (GL_DEPTH_TEST);

  glClearColor (1.0, 1.0, 1.0, 1.0);
  glClearDepth (1.0);

  gtk_widget_get_allocation (widget, &allocation);
  glViewport (0, 0,
	      allocation.width, allocation.height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (40.0, 1.0, 1.0, 10.0);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  gluLookAt (0.0, 0.0, 3.0,
	     0.0, 0.0, 0.0,
	     0.0, 1.0, 0.0);
  glTranslatef (0.0, 0.0, -3.0);

  gdk_gl_context_release_current();

  /*** OpenGL END ***/
}

static void
size_allocate (GtkWidget     *widget,
               GtkAllocation *allocation,
               gpointer       data)
{
  /*  Synchronize OpenGL rendering pipeline on resizing X window. */
  if (glwindow != NULL)
    gdk_gl_drawable_wait_gdk (GDK_GL_DRAWABLE (glwindow));
}

static gboolean
configure_event (GtkWidget         *widget,
                 GdkEventConfigure *event,
                 gpointer           data)
{
  GtkAllocation allocation;

  /* gtk_drawing_area sends configure_event when it is realized. */
  if (glwindow == NULL)
    return FALSE;

  /*** OpenGL BEGIN ***/

  if (!gdk_gl_context_make_current (glcontext,
                                    GDK_GL_DRAWABLE (glwindow),
                                    GDK_GL_DRAWABLE (glwindow)))
    return FALSE;

  gtk_widget_get_allocation (widget, &allocation);
  glViewport (0, 0,
	      allocation.width, allocation.height);

  gdk_gl_context_release_current();

  /*** OpenGL END ***/

  return TRUE;
}

static gboolean
draw (GtkWidget *widget,
      cairo_t   *cr,
      gpointer   data)
{
  GdkGLDrawable *gldrawable = GDK_GL_DRAWABLE (glwindow);

  /*** OpenGL BEGIN ***/

  if (!gdk_gl_context_make_current (glcontext,
                                    gldrawable,
                                    gldrawable))
    return FALSE;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glCallList (1);

  if (gdk_gl_drawable_is_double_buffered (gldrawable))
    gdk_gl_drawable_swap_buffers (gldrawable);
  else
    glFlush ();

  gdk_gl_context_release_current();

  /*** OpenGL END ***/

  return TRUE;
}

static void
unrealize (GtkWidget *widget,
           gpointer   data)
{
  if (glcontext != NULL)
    {
      g_object_unref (G_OBJECT (glcontext));
      glcontext = NULL;
    }

  if (glwindow != NULL)
    {
      g_object_unref (G_OBJECT (glwindow));
      glwindow = NULL;
    }

  if (glconfig != NULL)
    {
      g_object_unref (G_OBJECT (glconfig));
      glconfig = NULL;
    }
}

static void
print_gl_config_attrib (GdkGLConfig *glconfig,
                        const gchar *attrib_str,
                        int          attrib,
                        gboolean     is_boolean)
{
  int value;

  g_print ("%s = ", attrib_str);
  if (gdk_gl_config_get_attrib (glconfig, attrib, &value))
    {
      if (is_boolean)
        g_print ("%s\n", value == TRUE ? "TRUE" : "FALSE");
      else
        g_print ("%d\n", value);
    }
  else
    g_print ("*** Cannot get %s attribute value\n", attrib_str);
}

static void
examine_gl_config_attrib (GdkGLConfig *glconfig)
{
  g_print ("\nOpenGL visual configurations :\n\n");

  g_print ("gdk_gl_config_is_rgba (glconfig) = %s\n",
           gdk_gl_config_is_rgba (glconfig) ? "TRUE" : "FALSE");
  g_print ("gdk_gl_config_is_double_buffered (glconfig) = %s\n",
           gdk_gl_config_is_double_buffered (glconfig) ? "TRUE" : "FALSE");
  g_print ("gdk_gl_config_is_stereo (glconfig) = %s\n",
           gdk_gl_config_is_stereo (glconfig) ? "TRUE" : "FALSE");
  g_print ("gdk_gl_config_has_alpha (glconfig) = %s\n",
           gdk_gl_config_has_alpha (glconfig) ? "TRUE" : "FALSE");
  g_print ("gdk_gl_config_has_depth_buffer (glconfig) = %s\n",
           gdk_gl_config_has_depth_buffer (glconfig) ? "TRUE" : "FALSE");
  g_print ("gdk_gl_config_has_stencil_buffer (glconfig) = %s\n",
           gdk_gl_config_has_stencil_buffer (glconfig) ? "TRUE" : "FALSE");
  g_print ("gdk_gl_config_has_accum_buffer (glconfig) = %s\n",
           gdk_gl_config_has_accum_buffer (glconfig) ? "TRUE" : "FALSE");

  g_print ("\n");

  print_gl_config_attrib (glconfig, "GDK_GL_USE_GL",           GDK_GL_USE_GL,           TRUE);
  print_gl_config_attrib (glconfig, "GDK_GL_BUFFER_SIZE",      GDK_GL_BUFFER_SIZE,      FALSE);
  print_gl_config_attrib (glconfig, "GDK_GL_LEVEL",            GDK_GL_LEVEL,            FALSE);
  print_gl_config_attrib (glconfig, "GDK_GL_RGBA",             GDK_GL_RGBA,             TRUE);
  print_gl_config_attrib (glconfig, "GDK_GL_DOUBLEBUFFER",     GDK_GL_DOUBLEBUFFER,     TRUE);
  print_gl_config_attrib (glconfig, "GDK_GL_STEREO",           GDK_GL_STEREO,           TRUE);
  print_gl_config_attrib (glconfig, "GDK_GL_AUX_BUFFERS",      GDK_GL_AUX_BUFFERS,      FALSE);
  print_gl_config_attrib (glconfig, "GDK_GL_RED_SIZE",         GDK_GL_RED_SIZE,         FALSE);
  print_gl_config_attrib (glconfig, "GDK_GL_GREEN_SIZE",       GDK_GL_GREEN_SIZE,       FALSE);
  print_gl_config_attrib (glconfig, "GDK_GL_BLUE_SIZE",        GDK_GL_BLUE_SIZE,        FALSE);
  print_gl_config_attrib (glconfig, "GDK_GL_ALPHA_SIZE",       GDK_GL_ALPHA_SIZE,       FALSE);
  print_gl_config_attrib (glconfig, "GDK_GL_DEPTH_SIZE",       GDK_GL_DEPTH_SIZE,       FALSE);
  print_gl_config_attrib (glconfig, "GDK_GL_STENCIL_SIZE",     GDK_GL_STENCIL_SIZE,     FALSE);
  print_gl_config_attrib (glconfig, "GDK_GL_ACCUM_RED_SIZE",   GDK_GL_ACCUM_RED_SIZE,   FALSE);
  print_gl_config_attrib (glconfig, "GDK_GL_ACCUM_GREEN_SIZE", GDK_GL_ACCUM_GREEN_SIZE, FALSE);
  print_gl_config_attrib (glconfig, "GDK_GL_ACCUM_BLUE_SIZE",  GDK_GL_ACCUM_BLUE_SIZE,  FALSE);
  print_gl_config_attrib (glconfig, "GDK_GL_ACCUM_ALPHA_SIZE", GDK_GL_ACCUM_ALPHA_SIZE, FALSE);

  g_print ("\n");
}

int
main (int   argc,
      char *argv[])
{
  gint major, minor;

  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *drawing_area;
  GtkWidget *button;

  /*
   * Init GTK.
   */

  gtk_init (&argc, &argv);

  /*
   * Init GdkGLExt.
   */

  gdk_gl_init (&argc, &argv);

  /*
   * Query OpenGL extension version.
   */

  gdk_gl_query_version (&major, &minor);
  g_print ("\nOpenGL extension version - %d.%d\n",
           major, minor);

  /*
   * Configure OpenGL-capable visual.
   */

  /* Try double-buffered visual */
  glconfig = gdk_gl_config_new (&config_attributes[0], G_N_ELEMENTS(config_attributes));
  if (glconfig == NULL)
    {
      g_print ("*** Cannot find the double-buffered visual.\n");
      g_print ("*** Trying single-buffered visual.\n");

      /* Try single-buffered visual */
      glconfig = gdk_gl_config_new (&config_attributes[1], G_N_ELEMENTS(config_attributes)-1);
      if (glconfig == NULL)
        {
          g_print ("*** No appropriate OpenGL-capable visual found.\n");
          exit (1);
        }
    }

  examine_gl_config_attrib (glconfig);

  /*
   * Top-level window.
   */

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "low-level");

  /*
   * If window manager doesn't watch the WM_COLORMAP_WINDOWS property on
   * the top-level window, we have to set OpenGL window's visual to the
   * top-level window.
   */
  gtk_widget_set_visual (window, gdk_gl_config_get_visual (glconfig));

  /* Get automatically redrawn if any of their children changed allocation. */
  gtk_container_set_reallocate_redraws (GTK_CONTAINER (window), TRUE);

  g_signal_connect (G_OBJECT (window), "delete_event",
                    G_CALLBACK (gtk_main_quit), NULL);

  /*
   * VBox.
   */

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_widget_show (vbox);

  /*
   * Drawing area for drawing OpenGL scene.
   */

  drawing_area = gtk_drawing_area_new ();
  gtk_widget_set_size_request (drawing_area, 200, 200);

  /* Set OpenGL-capable visual. */
  gtk_widget_set_visual (drawing_area, gdk_gl_config_get_visual (glconfig));

  /* Disable backing store feature of the widget. */
  gtk_widget_set_double_buffered (drawing_area, FALSE);

  g_signal_connect_after (G_OBJECT (drawing_area), "realize",
                          G_CALLBACK (realize), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "size_allocate",
                    G_CALLBACK (size_allocate), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "configure-event",
		    G_CALLBACK (configure_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "draw",
		    G_CALLBACK (draw), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "unrealize",
		    G_CALLBACK (unrealize), NULL);

  gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);

  gtk_widget_show (drawing_area);

  /*
   * Simple quit button.
   */

  button = gtk_button_new_with_label ("Quit");

  g_signal_connect (G_OBJECT (button), "clicked",
                    G_CALLBACK (gtk_main_quit), NULL);

  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

  gtk_widget_show (button);

  /*
   * Show window.
   */

  gtk_widget_show (window);

  gtk_main ();

  return 0;
}
