/*
 * share-lists.c:
 * Simple display list sharing example using libglade.
 *
 * written by Naofumi Yasufuku  <naofumi@users.sourceforge.net>
 */

#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include <glade/glade.h>

#include <stdlib.h>

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

gboolean
on_window1_delete_event (GtkWidget *widget,
                         GdkEvent  *event,
                         gpointer   user_data)
{
  gtk_main_quit ();

  return FALSE;
}

static void
init_gl (GLfloat *light_diffuse)
{
  static GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};

  glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv (GL_LIGHT0, GL_POSITION, light_position);
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  glEnable (GL_DEPTH_TEST);

  glClearColor (1.0, 1.0, 1.0, 1.0);
  glClearDepth (1.0);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (40.0, 1.0, 1.0, 10.0);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  gluLookAt (0.0, 0.0, 3.0,
             0.0, 0.0, 0.0,
             0.0, 1.0, 0.0);
  glTranslatef (0.0, 0.0, -3.0);
}

void
on_drawingarea1_realize (GtkWidget *widget,
                         gpointer   user_data)
{
  GLUquadricObj *qobj;
  static GLfloat red[] = {1.0, 0.0, 0.0, 1.0};

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return;

  /* Create display list #1 */
  qobj = gluNewQuadric ();
  gluQuadricDrawStyle (qobj, GLU_FILL);
  glNewList (1, GL_COMPILE);
  gluSphere (qobj, 1.0, 20, 20);
  glEndList ();

  /* Initialize rendering context */
  init_gl (red);

  gtk_widget_end_gl (widget, FALSE);
  /*** OpenGL END ***/
}

gboolean
on_drawingarea1_configure_event (GtkWidget         *widget,
                                 GdkEventConfigure *event,
                                 gpointer           user_data)
{
  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return FALSE;

  glViewport (0, 0,
              widget->allocation.width, widget->allocation.height);

  gtk_widget_end_gl (widget, FALSE);
  /*** OpenGL END ***/

  return FALSE;
}

gboolean
on_drawingarea1_expose_event (GtkWidget       *widget,
                              GdkEventExpose  *event,
                              gpointer         user_data)
{
  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return FALSE;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glCallList (1);

  gtk_widget_end_gl (widget, TRUE);
  /*** OpenGL END ***/

  return FALSE;
}

void
on_drawingarea2_realize (GtkWidget *widget,
                         gpointer   user_data)
{
  static GLfloat blue[]    = {0.0, 0.0, 1.0, 1.0};

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return;

  /* Initialize rendering context */
  init_gl (blue);

  gtk_widget_end_gl (widget, FALSE);
  /*** OpenGL END ***/
}

gboolean
on_drawingarea2_configure_event (GtkWidget         *widget,
                                 GdkEventConfigure *event,
                                 gpointer           user_data)
{
  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return FALSE;

  glViewport (0, 0,
              widget->allocation.width, widget->allocation.height);

  gtk_widget_end_gl (widget, FALSE);
  /*** OpenGL END ***/

  return FALSE;
}

gboolean
on_drawingarea2_expose_event (GtkWidget       *widget,
                              GdkEventExpose  *event,
                              gpointer         user_data)
{
  /*** OpenGL BEGIN ***/
  if (!gtk_wiget_begin_gl (widget))
    return FALSE;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glCallList (1);

  gtk_widget_end_gl (widget, TRUE);
  /*** OpenGL END ***/

  return FALSE;
}

void
on_button1_clicked (GtkButton *button,
                    gpointer   user_data)
{
  gtk_main_quit ();
}

int
main (int   argc,
      char *argv[])
{
  GdkGLConfig *glconfig;
  GdkGLContext *glcontext;
  GladeXML *xml;
  GtkWidget *window;
  GtkWidget *drawingarea;

  /*
   * Init GTK+ and GtkGLExt.
   */

  gtk_init (&argc, &argv);
  gtk_gl_init (&argc, &argv);

  /*
   * Configure OpenGL-capable visual.
   */

  /* Try double-buffered visual */
  glconfig = gdk_gl_config_new_by_mode (GDK_GL_MODE_RGB    |
                                        GDK_GL_MODE_DEPTH  |
                                        GDK_GL_MODE_DOUBLE);
  if (glconfig == NULL)
    {
      g_print ("*** Cannot find the double-buffered visual.\n");
      g_print ("*** Trying single-buffered visual.\n");

      /* Try single-buffered visual */
      glconfig = gdk_gl_config_new_by_mode (GDK_GL_MODE_RGB   |
                                            GDK_GL_MODE_DEPTH);
      if (glconfig == NULL)
        {
          g_print ("*** No appropriate OpenGL-capable visual found.\n");
          exit (1);
        }
    }

  /*
   * Load the interface.
   */

  xml = glade_xml_new ("share-lists.glade", NULL, NULL);

  /*
   * Connect the signals in the interface.
   */

  glade_xml_signal_autoconnect (xml);

  /*
   * Top-level window.
   */

  window = glade_xml_get_widget (xml, "window1");

  gtk_container_set_reallocate_redraws (GTK_CONTAINER (window), TRUE);

  /*
   * Drawing area #1.
   */

  drawingarea = glade_xml_get_widget (xml, "drawingarea1");

  /* Add OpenGL-capability to drawingarea1. */
  gtk_widget_set_gl_capability (drawingarea,
                                glconfig,
                                NULL,
                                TRUE,
                                GDK_GL_RGBA_TYPE);

  /* Get GL context. */
  gtk_widget_realize (drawingarea);
  glcontext = gtk_widget_get_gl_context (drawingarea);

  /*
   * Drawing area #2.
   */

  drawingarea = glade_xml_get_widget (xml, "drawingarea2");

  /* Add OpenGL-capability to drawingarea2. */
  gtk_widget_set_gl_capability (drawingarea,
                                glconfig,
                                glcontext, /* share lists */
                                TRUE,
                                GDK_GL_RGBA_TYPE);

  /*
   * Show top-level window, then run.
   */

  gtk_widget_show (window);

  gtk_main ();

  return 0;
}
