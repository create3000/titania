/*
 * button.c:
 * Simple toggle button example.
 *
 * written by Naofumi Yasufuku  <naofumi@users.sourceforge.net>
 */

#include <stdlib.h>
#include <math.h>

#include <gtk/gtk.h>

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

#include "drawshapes.h"

#define TIMEOUT_INTERVAL 10

static gboolean animate = TRUE;

static GLfloat angle = 0.0;
static GLfloat pos_y = 0.0;

static void
realize (GtkWidget *widget,
         gpointer   data)
{
  static GLfloat ambient[]  = { 0.0, 0.0, 0.0, 1.0 };
  static GLfloat diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
  static GLfloat position[] = { 1.0, 1.0, 1.0, 0.0 };
  static GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
  static GLfloat local_view[] = {0.0};

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return;

  glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv (GL_LIGHT0, GL_POSITION, position);
  glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glLightModelfv (GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  glEnable (GL_DEPTH_TEST);

  glClearColor (1.0, 1.0, 1.0, 1.0);
  glClearDepth (1.0);

  gtk_widget_end_gl (widget, FALSE);
  /*** OpenGL END ***/
}

static gboolean
configure_event (GtkWidget         *widget,
                 GdkEventConfigure *event,
                 gpointer           data)
{
  GtkAllocation allocation;

  GLfloat w;
  GLfloat h;
  GLfloat aspect;

  gtk_widget_get_allocation (widget, &allocation);
  w = allocation.width;
  h = allocation.height;

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return FALSE;

  glViewport (0, 0, w, h);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  if (w > h)
    {
      aspect = w / h;
      glFrustum (-aspect, aspect, -1.0, 1.0, 5.0, 60.0);
    }
  else
    {
      aspect = h / w;
      glFrustum (-1.0, 1.0, -aspect, aspect, 5.0, 60.0);
    }

  glMatrixMode (GL_MODELVIEW);

  gtk_widget_end_gl (widget, FALSE);
  /*** OpenGL END ***/

  return TRUE;
}

static gboolean
draw (GtkWidget *widget,
      cairo_t   *cr,
      gpointer   data)
{
  /* brass */
  static GLfloat ambient[4]  = { 0.329412, 0.223529, 0.027451, 1.0 };
  static GLfloat diffuse[4]  = { 0.780392, 0.568627, 0.113725, 1.0 };
  static GLfloat specular[4] = { 0.992157, 0.941176, 0.807843, 1.0 };
  static GLfloat shininess   = 0.21794872 * 128.0;

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return FALSE;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity ();
  glTranslatef (0.0, 0.0, -10.0);

  glPushMatrix ();
    glTranslatef (0.0, pos_y, 0.0);
    glRotatef (angle, 0.0, 1.0, 0.0);
    glMaterialfv (GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv (GL_FRONT, GL_SPECULAR, specular);
    glMaterialf (GL_FRONT, GL_SHININESS, shininess);
    draw_torus (TRUE, 0.3, 0.6, 30, 30);
  glPopMatrix ();

  gtk_widget_end_gl (widget, TRUE);
  /*** OpenGL END ***/

  return TRUE;
}

static gboolean
timeout (GtkWidget *widget)
{
  GtkAllocation allocation;
  GdkWindow *window;
  GLfloat t;

  window = gtk_widget_get_window (widget);
  gtk_widget_get_allocation (widget, &allocation);

  angle += 3.0;
  if (angle >= 360.0)
    angle -= 360.0;

  t = angle * G_PI / 180.0;
  if (t > G_PI)
    t = 2.0 * G_PI - t;

  pos_y = 2.0 * (sin (t) + 0.4 * sin (3.0*t)) - 1.0;

  /* Invalidate the whole window. */
  gdk_window_invalidate_rect (window, &allocation, FALSE);

  /* Update synchronously (fast). */
  gdk_window_process_updates (window, FALSE);

  return TRUE;
}

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

static void
unrealize (GtkWidget *widget,
	   gpointer   data)
{
  timeout_remove (widget);
}

static gboolean
map_event (GtkWidget *widget,
	   GdkEvent  *event,
	   gpointer   data)
{
  if (animate)
    timeout_add (widget);

  return TRUE;
}

static gboolean
unmap_event (GtkWidget *widget,
	     GdkEvent  *event,
	     gpointer   data)
{
  timeout_remove (widget);

  return TRUE;
}

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

static GtkWidget *
create_gl_toggle_button (GdkGLConfig *glconfig)
{
  GtkWidget *vbox;
  GtkWidget *drawing_area;
  GtkWidget *label;
  GtkWidget *button;

  /*
   * VBox.
   */

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);

  /*
   * Drawing area for drawing OpenGL scene.
   */

  drawing_area = gtk_drawing_area_new ();
  gtk_widget_set_size_request (drawing_area, 200, 200);

  /* Set OpenGL-capability to the widget. */
  gtk_widget_set_gl_capability (drawing_area,
                                glconfig,
                                NULL,
                                TRUE,
                                GDK_GL_RGBA_TYPE);

  g_signal_connect_after (G_OBJECT (drawing_area), "realize",
                          G_CALLBACK (realize), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "configure_event",
		    G_CALLBACK (configure_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "draw",
		    G_CALLBACK (draw), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "unrealize",
		    G_CALLBACK (unrealize), NULL);

  g_signal_connect (G_OBJECT (drawing_area), "map_event",
		    G_CALLBACK (map_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "unmap_event",
		    G_CALLBACK (unmap_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "visibility_notify_event",
		    G_CALLBACK (visibility_notify_event), NULL);

  gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);
  gtk_widget_show (drawing_area);

  /*
   * Label.
   */

  label = gtk_label_new ("Toggle Animation");
  gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 10);
  gtk_widget_show (label);

  /*
   * Toggle button.
   */

  button = gtk_toggle_button_new ();

  g_signal_connect_swapped (G_OBJECT (button), "toggled",
                            G_CALLBACK (toggle_animation), drawing_area);

  /* Add VBox. */
  gtk_widget_show (vbox);
  gtk_container_add (GTK_CONTAINER (button), vbox);

  return button;
}

int
main (int   argc,
      char *argv[])
{
  GdkGLConfig *glconfig;
  GtkWidget *window;
  GtkWidget *button;

  /*
   * Init GTK.
   */

  gtk_init (&argc, &argv);

  /*
   * Init GtkGLExt.
   */

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
   * Top-level window.
   */

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "button");

  /* Get automatically redrawn if any of their children changed allocation. */
  gtk_container_set_reallocate_redraws (GTK_CONTAINER (window), TRUE);
  /* Set border width. */
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  g_signal_connect (G_OBJECT (window), "delete_event",
                    G_CALLBACK (gtk_main_quit), NULL);

  /*
   * Toggle button which contains an OpenGL scene.
   */

  button = create_gl_toggle_button (glconfig);
  gtk_widget_show (button);
  gtk_container_add (GTK_CONTAINER (window), button);

  /*
   * Show window.
   */

  gtk_widget_show (window);

  /*
   * Main loop.
   */

  gtk_main ();

  return 0;
}
