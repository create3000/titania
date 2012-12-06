/*
 * logo.c:
 * GtkGLExt logo demo.
 *
 * written by Naofumi Yasufuku  <naofumi@users.sourceforge.net>
 */

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

#include "logo-model.h"
#include "trackball.h"

#define DIG_2_RAD (G_PI / 180.0)
#define RAD_2_DIG (180.0 / G_PI)

#define TIMEOUT_INTERVAL 10

#define DEFAULT_ROT_COUNT 100

#define VIEW_INIT_AXIS_X 1.0
#define VIEW_INIT_AXIS_Y 0.0
#define VIEW_INIT_AXIS_Z 0.0
#define VIEW_INIT_ANGLE  20.0

#define VIEW_SCALE_MAX 2.0
#define VIEW_SCALE_MIN 0.5

#define LOGO_CUBE       1
#define LOGO_G_FORWARD  2
#define LOGO_G_BACKWARD 3
#define LOGO_T_FORWARD  4
#define LOGO_T_BACKWARD 5
#define LOGO_K_FORWARD  6
#define LOGO_K_BACKWARD 7

static float view_quat[4] = { 0.0, 0.0, 0.0, 1.0 };
static float view_scale = 1.0;

static float logo_quat[4] = { 0.0, 0.0, 0.0, 1.0 };

static gboolean animate = TRUE;

static void toggle_animation (GtkWidget *widget);
static void init_logo_view   (GtkWidget *widget);

static void
init_view (void)
{
  float sine = sin (0.5 * VIEW_INIT_ANGLE * DIG_2_RAD);
  view_quat[0] = VIEW_INIT_AXIS_X * sine;
  view_quat[1] = VIEW_INIT_AXIS_Y * sine;
  view_quat[2] = VIEW_INIT_AXIS_Z * sine;
  view_quat[3] = cos (0.5 * VIEW_INIT_ANGLE * DIG_2_RAD);
  view_scale = 1.0;
}

static void
init_logo_quat (void)
{
  logo_quat[0] = 0.0;
  logo_quat[1] = 0.0;
  logo_quat[2] = 0.0;
  logo_quat[3] = 1.0;
}

static void
realize (GtkWidget *widget,
	 gpointer   data)
{
  static GLfloat light0_position[] = { 0.0, 0.0, 30.0, 0.0 };
  static GLfloat light0_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
  static GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };

  static GLfloat mat_specular[]  = { 0.5, 0.5, 0.5, 1.0 };
  static GLfloat mat_shininess[] = { 10.0 };
  static GLfloat mat_black[]     = { 0.0, 0.0, 0.0, 1.0 };
  static GLfloat mat_red[]       = { 1.0, 0.0, 0.0, 1.0 };
  static GLfloat mat_green[]     = { 0.0, 1.0, 0.0, 1.0 };
  static GLfloat mat_blue[]      = { 0.0, 0.0, 1.0, 1.0 };

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return;

  glClearColor (0.5, 0.5, 0.8, 1.0);
  glClearDepth (1.0);

  glLightfv (GL_LIGHT0, GL_POSITION, light0_position);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv (GL_LIGHT0, GL_SPECULAR, light0_specular);

  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  glEnable (GL_DEPTH_TEST);

  glEnable (GL_CULL_FACE);

  glShadeModel (GL_SMOOTH);

  glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv (GL_FRONT, GL_SHININESS, mat_shininess);

  /* Center black cube. */
  glNewList (LOGO_CUBE, GL_COMPILE);
    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_black);
    logo_draw_cube ();
  glEndList ();

  /* Forward "G". */
  glNewList (LOGO_G_FORWARD, GL_COMPILE);
    glDisable (GL_CULL_FACE);
    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_blue);
    logo_draw_g_plane ();
    glEnable (GL_CULL_FACE);

    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_black);
    logo_draw_g ();
  glEndList ();

  /* Backward "G". */
  glNewList (LOGO_G_BACKWARD, GL_COMPILE);
    glPushMatrix ();
      glRotatef (180.0, 1.0, 0.0, 0.0);

      glDisable (GL_CULL_FACE);
      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_blue);
      logo_draw_g_plane ();
      glEnable (GL_CULL_FACE);

      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_black);
      logo_draw_g ();

    glPopMatrix ();
  glEndList ();

  /* Forward "T". */
  glNewList (LOGO_T_FORWARD, GL_COMPILE);
    glDisable (GL_CULL_FACE);
    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_red);
    logo_draw_t_plane ();
    glEnable (GL_CULL_FACE);

    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_black);
    logo_draw_t ();
  glEndList ();

  /* Backward "T". */
  glNewList (LOGO_T_BACKWARD, GL_COMPILE);
    glPushMatrix ();
      glRotatef (180.0, 1.0, 0.0, 0.0);

      glDisable (GL_CULL_FACE);
      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_red);
      logo_draw_t_plane ();
      glEnable (GL_CULL_FACE);

      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_black);
      logo_draw_t ();

    glPopMatrix ();
  glEndList ();

  /* Forward "K". */
  glNewList (LOGO_K_FORWARD, GL_COMPILE);
    glDisable (GL_CULL_FACE);
    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_green);
    logo_draw_k_plane ();
    glEnable (GL_CULL_FACE);

    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_black);
    logo_draw_k ();
  glEndList ();

  /* Backward "K". */
  glNewList (LOGO_K_BACKWARD, GL_COMPILE);
    glPushMatrix ();
      glRotatef (180.0, 0.0, 0.0, 1.0);

      glDisable (GL_CULL_FACE);
      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_green);
      logo_draw_k_plane ();
      glEnable (GL_CULL_FACE);

      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_black);
      logo_draw_k ();
    glPopMatrix ();
  glEndList ();

  glEnable (GL_NORMALIZE);

  /* Init logo orientation. */
  init_logo_quat ();

  /* Init view. */
  init_view ();

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
      glFrustum (-aspect, aspect, -1.0, 1.0, 2.0, 60.0);
    }
  else
    {
      aspect = h / w;
      glFrustum (-1.0, 1.0, -aspect, aspect, 2.0, 60.0);
    }

  glMatrixMode (GL_MODELVIEW);

  gtk_widget_end_gl (widget, FALSE);
  /*** OpenGL END ***/

  return TRUE;
}

static float axis_x[3] = { 1.0, 0.0, 0.0 };
static float axis_y[3] = { 0.0, 1.0, 0.0 };
static float axis_z[3] = { 0.0, 0.0, 1.0 };

/* Logo rotation mode. */
typedef struct _RotMode
{
  float *axis;
  float sign;
} RotMode;

static RotMode rot_mode[] = {
  { axis_x,  1.0 },
  { axis_y,  1.0 },
  { axis_x,  1.0 },
  { axis_z,  1.0 },
  { axis_x,  1.0 },
  { axis_y, -1.0 },
  { axis_x,  1.0 },
  { axis_z, -1.0 },
  { NULL,    0.0 }  /* terminator */
};

static int rot_count = DEFAULT_ROT_COUNT;
static int mode = 0;
static int counter = 0;

static gboolean
draw (GtkWidget *widget,
	    cairo_t   *cr,
	    gpointer   data)
{
  float d_quat[4];
  float m[4][4];

  if (animate)
    {
      if (counter == rot_count)
	      {
          if (rot_mode[++mode].axis == NULL)
	          mode = 0;
          counter = 0;
	      }

      axis_to_quat (rot_mode[mode].axis,
		    rot_mode[mode].sign * G_PI_2 / rot_count,
		    d_quat);
      add_quats (d_quat, logo_quat, logo_quat);

      counter++;

    }

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return FALSE;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity ();

  /* View transformation. */
  glTranslatef (0.0, 0.0, -30.0);
  glScalef (view_scale, view_scale, view_scale);
  build_rotmatrix (m, view_quat);
  glMultMatrixf (&m[0][0]);

  /* Logo model. */
  glPushMatrix ();
    build_rotmatrix (m, logo_quat);
    glMultMatrixf (&m[0][0]);

    glRotatef (90.0, 1.0, 0.0, 0.0);
    glCallList (LOGO_CUBE);
    glCallList (LOGO_G_FORWARD);
    glCallList (LOGO_G_BACKWARD);
    glCallList (LOGO_T_FORWARD);
    glCallList (LOGO_T_BACKWARD);
    glCallList (LOGO_K_FORWARD);
    glCallList (LOGO_K_BACKWARD);
  glPopMatrix ();

  gtk_widget_end_gl (widget, TRUE);
  /*** OpenGL END ***/

  return TRUE;
}

static float begin_x = 0.0;
static float begin_y = 0.0;

static gboolean
button_press_event (GtkWidget      *widget,
		    GdkEventButton *event,
		    GtkWidget      *menu)
{
  begin_x = event->x;
  begin_y = event->y;

  return FALSE;
}

static gboolean
motion_notify_event (GtkWidget      *widget,
		     GdkEventMotion *event,
		     gpointer        data)
{
  GtkAllocation allocation;
  float w;
  float h;
  float x = event->x;
  float y = event->y;
  float d_quat[4];
  gboolean redraw = FALSE;

  gtk_widget_get_allocation (widget, &allocation);
  w = allocation.width;
  h = allocation.height;

  /* Rotation. */
  if (event->state & GDK_BUTTON1_MASK)
    {
      trackball (d_quat,
		 (2.0 * begin_x - w) / w,
		 (h - 2.0 * begin_y) / h,
		 (2.0 * x - w) / w,
		 (h - 2.0 * y) / h);
      add_quats (d_quat, view_quat, view_quat);
      redraw = TRUE;
    }

  /* Scaling. */
  if (event->state & GDK_BUTTON2_MASK)
    {
      view_scale = view_scale * (1.0 + (y - begin_y) / h);
      if (view_scale > VIEW_SCALE_MAX)
	view_scale = VIEW_SCALE_MAX;
      else if (view_scale < VIEW_SCALE_MIN)
	view_scale = VIEW_SCALE_MIN;
      redraw = TRUE;
    }

  begin_x = x;
  begin_y = y;

  if (redraw && !animate)
    gdk_window_invalidate_rect (gtk_widget_get_window (widget), &allocation, FALSE);

  return TRUE;
}

static gboolean
key_press_event (GtkWidget   *widget,
		 GdkEventKey *event,
		 gpointer     data)
{
  switch (event->keyval)
    {
    case GDK_KEY_a:
      toggle_animation (widget);
      break;
    case GDK_KEY_i:
      init_logo_view (widget);
      break;
    case GDK_KEY_Escape:
      gtk_main_quit ();
      break;
    default:
      return TRUE;
    }

  return TRUE;
}

static gboolean
timeout (GtkWidget *widget)
{
  GtkAllocation allocation;
  GdkWindow *window;

  window = gtk_widget_get_window (widget);
  gtk_widget_get_allocation (widget, &allocation);

  /* Invalidate the whole window. */
  gdk_window_invalidate_rect (window, &allocation, FALSE);

  /* Update synchronously. */
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

static gboolean
map_event (GtkWidget   *widget,
	   GdkEventAny *event,
	   gpointer     data)
{
  if (animate)
    timeout_add (widget);

  return TRUE;
}

static gboolean
unmap_event (GtkWidget   *widget,
	     GdkEventAny *event,
	     gpointer     data)
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

static void
init_logo_view (GtkWidget *widget)
{
  GtkAllocation allocation;
  init_logo_quat ();
  init_view ();
  mode = 0;
  counter = 0;

  if (!animate)
    {
      gtk_widget_get_allocation (widget, &allocation);
      gdk_window_invalidate_rect (gtk_widget_get_window (widget), &allocation, FALSE);
    }
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
      gtk_menu_popup (GTK_MENU (widget), NULL, NULL, NULL, NULL,
		      event->button, event->time);
      return TRUE;
    }

  return FALSE;
}

/* Creates the popup menu.*/
static GtkWidget *
create_popup_menu (GtkWidget *drawing_area)
{
  GtkWidget *menu;
  GtkWidget *menu_item;

  menu = gtk_menu_new ();

  /* Toggle animation */
  menu_item = gtk_menu_item_new_with_label ("Toggle Animation");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
  g_signal_connect_swapped (G_OBJECT (menu_item), "activate",
			    G_CALLBACK (toggle_animation), drawing_area);
  gtk_widget_show (menu_item);

  /* Init orientation */
  menu_item = gtk_menu_item_new_with_label ("Initialize");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
  g_signal_connect_swapped (G_OBJECT (menu_item), "activate",
			    G_CALLBACK (init_logo_view), drawing_area);
  gtk_widget_show (menu_item);

  /* Quit */
  menu_item = gtk_menu_item_new_with_label ("Quit");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
		    G_CALLBACK (gtk_main_quit), NULL);
  gtk_widget_show (menu_item);

  return menu;
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
main(int   argc,
     char *argv[])
{
  GdkGLConfig *glconfig;
  gint major, minor;

  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *drawing_area;
  GtkWidget *menu;
  GtkWidget *button;

  int i;
  gboolean arg_count = FALSE;

  /*
   * Init GTK.
   */

  gtk_init (&argc, &argv);

  /*
   * Init GtkGLExt.
   */

  gtk_gl_init (&argc, &argv);

  /*
   * Parse arguments.
   */

  for (i = 1; i < argc; i++)
    {
      if (arg_count)
	rot_count = atoi (argv[i]);

      if (strcmp (argv[i], "--help") == 0 ||
	  strcmp (argv[i], "-h") == 0)
	{
	  g_print ("Usage: %s [--count num] [--no-anim] [--help]\n", argv[0]);
	  exit (0);
	}

      if (strcmp (argv[i], "--count") == 0)
	arg_count = TRUE;

      if (strcmp (argv[i], "--no-anim") == 0)
	animate = FALSE;
    }

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

  examine_gl_config_attrib (glconfig);

  /*
   * Top-level window.
   */

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "logo");

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
			 GDK_BUTTON1_MOTION_MASK    |
			 GDK_BUTTON2_MOTION_MASK    |
			 GDK_BUTTON_PRESS_MASK      |
			 GDK_VISIBILITY_NOTIFY_MASK);

  g_signal_connect_after (G_OBJECT (drawing_area), "realize",
                          G_CALLBACK (realize), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "configure_event",
		    G_CALLBACK (configure_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "draw",
		    G_CALLBACK (draw), NULL);

  g_signal_connect (G_OBJECT (drawing_area), "button_press_event",
		    G_CALLBACK (button_press_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "motion_notify_event",
		    G_CALLBACK (motion_notify_event), NULL);

  g_signal_connect (G_OBJECT (drawing_area), "map_event",
		    G_CALLBACK (map_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "unmap_event",
		    G_CALLBACK (unmap_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "visibility_notify_event",
		    G_CALLBACK (visibility_notify_event), NULL);

  g_signal_connect_swapped (G_OBJECT (window), "key_press_event",
			    G_CALLBACK (key_press_event), drawing_area);

  gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);

  gtk_widget_show (drawing_area);

  /*
   * Popup menu.
   */

  menu = create_popup_menu (drawing_area);

  /* Signal handler */
  g_signal_connect_swapped (G_OBJECT (drawing_area), "button_press_event",
                            G_CALLBACK (button_press_event_popup_menu), menu);

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

  gtk_main ();

  return 0;
}
