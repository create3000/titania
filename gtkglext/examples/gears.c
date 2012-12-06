/*
 * 3-D gear wheels.  This program is in the public domain.
 *
 * Brian Paul
 */

/* Conversion to GLUT by Mark J. Kilgard */

/* Conversion to GtkGLExt by Naofumi Yasufuku */

#include <stdlib.h>
#include <string.h>
#include <math.h>

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

/*
 * Draw a gear wheel.  You'll probably want to call this function when
 * building a display list since we do a lot of trig here.
 *
 * Input:  inner_radius - radius of hole at center
 * outer_radius - radius at center of teeth
 * width - width of gear
 * teeth - number of teeth
 * tooth_depth - depth of tooth
 */

static void
gear(GLfloat inner_radius,
     GLfloat outer_radius,
     GLfloat width,
     GLint   teeth,
     GLfloat tooth_depth)
{
  GLint i;
  GLfloat r0, r1, r2;
  GLfloat angle, da;
  GLfloat u, v, len;

  r0 = inner_radius;
  r1 = outer_radius - tooth_depth / 2.0;
  r2 = outer_radius + tooth_depth / 2.0;

  da = 2.0 * G_PI / teeth / 4.0;

  glShadeModel(GL_FLAT);

  glNormal3f(0.0, 0.0, 1.0);

  /* draw front face */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;
    glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
    glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
    if (i < teeth) {
      glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
    }
  }
  glEnd();

  /* draw front sides of teeth */
  glBegin(GL_QUADS);
  da = 2.0 * G_PI / teeth / 4.0;
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;

    glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
  }
  glEnd();

  glNormal3f(0.0, 0.0, -1.0);

  /* draw back face */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;
    glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
    glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
    if (i < teeth) {
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
      glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
    }
  }
  glEnd();

  /* draw back sides of teeth */
  glBegin(GL_QUADS);
  da = 2.0 * G_PI / teeth / 4.0;
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;

    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
    glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
  }
  glEnd();

  /* draw outward faces of teeth */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;

    glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
    glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
    u = r2 * cos(angle + da) - r1 * cos(angle);
    v = r2 * sin(angle + da) - r1 * sin(angle);
    len = sqrt(u * u + v * v);
    u /= len;
    v /= len;
    glNormal3f(v, -u, 0.0);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
    glNormal3f(cos(angle), sin(angle), 0.0);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
    u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
    v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
    glNormal3f(v, -u, 0.0);
    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
    glNormal3f(cos(angle), sin(angle), 0.0);
  }

  glVertex3f(r1 * cos(0), r1 * sin(0), width * 0.5);
  glVertex3f(r1 * cos(0), r1 * sin(0), -width * 0.5);

  glEnd();

  glShadeModel(GL_SMOOTH);

  /* draw inside radius cylinder */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;
    glNormal3f(-cos(angle), -sin(angle), 0.0);
    glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
    glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
  }
  glEnd();

}

static GLfloat view_rotx = 20.0, view_roty = 30.0, view_rotz = 0.0;
static GLint gear1, gear2, gear3;
static GLfloat angle = 0.0;

static GTimer *timer = NULL;
static gint frames = 0;

static gboolean is_sync = TRUE;

static gboolean
draw (GtkWidget *widget,
      cairo_t   *cr,
      gpointer   data)
{
  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return FALSE;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix ();
    glRotatef (view_rotx, 1.0, 0.0, 0.0);
    glRotatef (view_roty, 0.0, 1.0, 0.0);
    glRotatef (view_rotz, 0.0, 0.0, 1.0);

    glPushMatrix ();
      glTranslatef (-3.0, -2.0, 0.0);
      glRotatef (angle, 0.0, 0.0, 1.0);
      glCallList (gear1);
    glPopMatrix ();

    glPushMatrix ();
      glTranslatef (3.1, -2.0, 0.0);
      glRotatef (-2.0 * angle - 9.0, 0.0, 0.0, 1.0);
      glCallList (gear2);
    glPopMatrix ();

    glPushMatrix ();
      glTranslatef (-3.1, 4.2, 0.0);
      glRotatef (-2.0 * angle - 25.0, 0.0, 0.0, 1.0);
      glCallList (gear3);
    glPopMatrix ();

  glPopMatrix ();

  gtk_widget_end_gl (widget, TRUE);
  /*** OpenGL END ***/

  frames++;

  {
    gdouble seconds = g_timer_elapsed (timer, NULL);
    if (seconds >= 5.0) {
      gdouble fps = frames / seconds;
      g_print ("%d frames in %6.3f seconds = %6.3f FPS\n", frames, seconds, fps);
      g_timer_reset (timer);
      frames = 0;
    }
  }

  return TRUE;
}

/* new window size or exposure */
static gboolean
reshape (GtkWidget         *widget,
	 GdkEventConfigure *event,
	 gpointer           data)
{
  GtkAllocation allocation;

  GLfloat h;

  gtk_widget_get_allocation (widget, &allocation);
  h = (GLfloat) (allocation.height) / (GLfloat) (allocation.width);

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return FALSE;

  glViewport (0, 0, allocation.width, allocation.height);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glFrustum (-1.0, 1.0, -h, h, 5.0, 60.0);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  glTranslatef (0.0, 0.0, -40.0);

  gtk_widget_end_gl (widget, FALSE);
  /*** OpenGL END ***/

  return TRUE;
}

static void
init(GtkWidget *widget,
     gpointer   data)
{
  static GLfloat pos[4] = {5.0, 5.0, 10.0, 0.0};
  static GLfloat red[4] = {0.8, 0.1, 0.0, 1.0};
  static GLfloat green[4] = {0.0, 0.8, 0.2, 1.0};
  static GLfloat blue[4] = {0.2, 0.2, 1.0, 1.0};

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return;

  glLightfv (GL_LIGHT0, GL_POSITION, pos);
  glEnable (GL_CULL_FACE);
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  glEnable (GL_DEPTH_TEST);

  /* make the gears */
  gear1 = glGenLists (1);
  glNewList (gear1, GL_COMPILE);
    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
    gear (1.0, 4.0, 1.0, 20, 0.7);
  glEndList ();

  gear2 = glGenLists (1);
  glNewList (gear2, GL_COMPILE);
    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
    gear (0.5, 2.0, 2.0, 10, 0.7);
  glEndList ();

  gear3 = glGenLists (1);
  glNewList (gear3, GL_COMPILE);
    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    gear (1.3, 2.0, 0.5, 10, 0.7);
  glEndList ();

  glEnable (GL_NORMALIZE);

  g_print ("\n");
  g_print ("GL_RENDERER   = %s\n", (char *) glGetString (GL_RENDERER));
  g_print ("GL_VERSION    = %s\n", (char *) glGetString (GL_VERSION));
  g_print ("GL_VENDOR     = %s\n", (char *) glGetString (GL_VENDOR));
  g_print ("GL_EXTENSIONS = %s\n", (char *) glGetString (GL_EXTENSIONS));
  g_print ("\n");

  gtk_widget_end_gl (widget, FALSE);
  /*** OpenGL END ***/

  /* create timer */
  if (timer == NULL)
    timer = g_timer_new ();

  g_timer_start (timer);
}

static gboolean
idle (GtkWidget *widget)
{
  GtkAllocation allocation;
  GdkWindow *window;
  angle += 2.0;

  window = gtk_widget_get_window (widget);
  gtk_widget_get_allocation (widget, &allocation);

  /* Invalidate the whole window. */
  gdk_window_invalidate_rect (window, &allocation, FALSE);

  /* Update synchronously (fast). */
  if (is_sync)
    gdk_window_process_updates (window, FALSE);

  return TRUE;
}

static guint idle_id = 0;

static void
idle_add (GtkWidget *widget)
{
  if (idle_id == 0)
    {
      idle_id = g_idle_add_full (GDK_PRIORITY_REDRAW,
                                 (GSourceFunc) idle,
                                 widget,
                                 NULL);
    }
}

static void
idle_remove (GtkWidget *widget)
{
  if (idle_id != 0)
    {
      g_source_remove (idle_id);
      idle_id = 0;
    }
}

static gboolean
map (GtkWidget   *widget,
     GdkEventAny *event,
     gpointer     data)
{
  idle_add (widget);

  return TRUE;
}

static gboolean
unmap (GtkWidget   *widget,
       GdkEventAny *event,
       gpointer     data)
{
  idle_remove (widget);

  return TRUE;
}

static gboolean
visible (GtkWidget          *widget,
	 GdkEventVisibility *event,
	 gpointer            data)
{
  if (event->state == GDK_VISIBILITY_FULLY_OBSCURED)
    idle_remove (widget);
  else
    idle_add (widget);

  return TRUE;
}

/* change view angle, exit upon ESC */
static gboolean
key (GtkWidget   *widget,
     GdkEventKey *event,
     gpointer     data)
{
  GtkAllocation allocation;

  switch (event->keyval)
    {
    case GDK_KEY_z:
      view_rotz += 5.0;
      break;
    case GDK_KEY_Z:
      view_rotz -= 5.0;
      break;
    case GDK_KEY_Up:
      view_rotx += 5.0;
      break;
    case GDK_KEY_Down:
      view_rotx -= 5.0;
      break;
    case GDK_KEY_Left:
      view_roty += 5.0;
      break;
    case GDK_KEY_Right:
      view_roty -= 5.0;
      break;
    case GDK_KEY_Escape:
      gtk_main_quit ();
      break;
    default:
      return FALSE;
    }

  gtk_widget_get_allocation (widget, &allocation);
  gdk_window_invalidate_rect (gtk_widget_get_window (widget), &allocation, FALSE);

  return TRUE;
}

int
main (int   argc,
      char *argv[])
{
  GdkGLConfig *glconfig;
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *drawing_area;
  GtkWidget *button;
  int i;

  /*
   * Init GTK.
   */

  gtk_init (&argc, &argv);

  /*
   * Init GtkGLExt.
   */

  gtk_gl_init (&argc, &argv);

  /*
   * Command line options.
   */

  for (i = 0; i < argc; i++)
    {
      if (strcmp (argv[i], "--async") == 0)
        is_sync = FALSE;
    }

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
  gtk_window_set_title (GTK_WINDOW (window), "gears");

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
  gtk_widget_set_size_request (drawing_area, 300, 300);

  /* Set OpenGL-capability to the widget. */
  gtk_widget_set_gl_capability (drawing_area,
				glconfig,
				NULL,
				TRUE,
				GDK_GL_RGBA_TYPE);

  gtk_widget_add_events (drawing_area,
			 GDK_VISIBILITY_NOTIFY_MASK);

  g_signal_connect_after (G_OBJECT (drawing_area), "realize",
                          G_CALLBACK (init), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "configure_event",
		    G_CALLBACK (reshape), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "draw",
		    G_CALLBACK (draw), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "map_event",
		    G_CALLBACK (map), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "unmap_event",
		    G_CALLBACK (unmap), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "visibility_notify_event",
		    G_CALLBACK (visible), NULL);

  g_signal_connect_swapped (G_OBJECT (window), "key_press_event",
			    G_CALLBACK (key), drawing_area);

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

  /*
   * Main loop.
   */
//idle_add (drawing_area);
  gtk_main ();

  return 0;
}
