/*
 * shapes.c:
 * shapes demo.
 *
 * written by Naofumi Yasufuku  <naofumi@users.sourceforge.net>
 */

#include <stdlib.h>
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

#include "trackball.h"
#include "drawshapes.h"

#define DIG_2_RAD (G_PI / 180.0)
#define RAD_2_DIG (180.0 / G_PI)

#define ANIMATE_THRESHOLD 25.0

#define VIEW_SCALE_MAX 2.0
#define VIEW_SCALE_MIN 0.5

#define NUM_SHAPES 9

static const GLuint shape_cube         = 0;
static const GLuint shape_sphere       = 1;
static const GLuint shape_cone         = 2;
static const GLuint shape_torus        = 3;
static const GLuint shape_tetrahedron  = 4;
static const GLuint shape_octahedron   = 5;
static const GLuint shape_dodecahedron = 6;
static const GLuint shape_icosahedron  = 7;
static const GLuint shape_teapot       = 8;

static GLuint shape_list_base = 0;
static GLuint shape_current = 8;

typedef struct _MaterialProp
{
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess;
} MaterialProp;

static MaterialProp mat_emerald = {
  {0.0215, 0.1745, 0.0215, 1.0},
  {0.07568, 0.61424, 0.07568, 1.0},
  {0.633, 0.727811, 0.633, 1.0},
  0.6
};

static MaterialProp mat_jade = {
  {0.135, 0.2225, 0.1575, 1.0},
  {0.54, 0.89, 0.63, 1.0},
  {0.316228, 0.316228, 0.316228, 1.0},
  0.1
};

static MaterialProp mat_obsidian = {
  {0.05375, 0.05, 0.06625, 1.0},
  {0.18275, 0.17, 0.22525, 1.0},
  {0.332741, 0.328634, 0.346435, 1.0},
  0.3
};

static MaterialProp mat_pearl = {
  {0.25, 0.20725, 0.20725, 1.0},
  {1.0, 0.829, 0.829, 1.0},
  {0.296648, 0.296648, 0.296648, 1.0},
  0.088
};

static MaterialProp mat_ruby = {
  {0.1745, 0.01175, 0.01175, 1.0},
  {0.61424, 0.04136, 0.04136, 1.0},
  {0.727811, 0.626959, 0.626959, 1.0},
  0.6
};

static MaterialProp mat_turquoise = {
  {0.1, 0.18725, 0.1745, 1.0},
  {0.396, 0.74151, 0.69102, 1.0},
  {0.297254, 0.30829, 0.306678, 1.0},
  0.1
};

static MaterialProp mat_brass = {
  {0.329412, 0.223529, 0.027451, 1.0},
  {0.780392, 0.568627, 0.113725, 1.0},
  {0.992157, 0.941176, 0.807843, 1.0},
  0.21794872
};

static MaterialProp mat_bronze = {
  {0.2125, 0.1275, 0.054, 1.0},
  {0.714, 0.4284, 0.18144, 1.0},
  {0.393548, 0.271906, 0.166721, 1.0},
  0.2
};

static MaterialProp mat_chrome = {
  {0.25, 0.25, 0.25, 1.0},
  {0.4, 0.4, 0.4, 1.0},
  {0.774597, 0.774597, 0.774597, 1.0},
  0.6
};

static MaterialProp mat_copper = {
  {0.19125, 0.0735, 0.0225, 1.0},
  {0.7038, 0.27048, 0.0828, 1.0},
  {0.256777, 0.137622, 0.086014, 1.0},
  0.1
};

static MaterialProp mat_gold = {
  {0.24725, 0.1995, 0.0745, 1.0},
  {0.75164, 0.60648, 0.22648, 1.0},
  {0.628281, 0.555802, 0.366065, 1.0},
  0.4
};

static MaterialProp mat_silver = {
  {0.19225, 0.19225, 0.19225, 1.0},
  {0.50754, 0.50754, 0.50754, 1.0},
  {0.508273, 0.508273, 0.508273, 1.0},
  0.4
};

static MaterialProp *mat_current = &mat_silver;

static float view_quat_diff[4] = { 0.0, 0.0, 0.0, 1.0 };
static float view_quat[4] = { 0.0, 0.0, 0.0, 1.0 };
static float view_scale = 1.0;

static gboolean animate = FALSE;

static void toggle_animation (GtkWidget *widget);

static void
init_view (void)
{
  view_quat[0] = view_quat_diff[0] = 0.0;
  view_quat[1] = view_quat_diff[1] = 0.0;
  view_quat[2] = view_quat_diff[2] = 0.0;
  view_quat[3] = view_quat_diff[3] = 1.0;
  view_scale = 1.0;
}

static void
realize (GtkWidget *widget,
         gpointer   data)
{
  GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
  GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat position[] = {0.0, 3.0, 3.0, 0.0};

  GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
  GLfloat local_view[] = {0.0};

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return;

  glClearColor (0.5, 0.5, 0.8, 1.0);
  glClearDepth (1.0);

  glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv (GL_LIGHT0, GL_POSITION, position);
  glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glLightModelfv (GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

  glFrontFace (GL_CW);
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  glEnable (GL_AUTO_NORMAL);
  glEnable (GL_NORMALIZE);
  glEnable (GL_DEPTH_TEST);
  glDepthFunc (GL_LESS);

  /* Shape display lists */
  shape_list_base = glGenLists (NUM_SHAPES);

  /* Cube */
  glNewList (shape_list_base + shape_cube, GL_COMPILE);
    draw_cube (TRUE, 1.5);
  glEndList ();

  /* Sphere */
  glNewList (shape_list_base + shape_sphere, GL_COMPILE);
    draw_sphere (TRUE, 1.0, 30, 30);
  glEndList ();

  /* Cone */
  glNewList (shape_list_base + shape_cone, GL_COMPILE);
    glPushMatrix ();
      glTranslatef (0.0, 0.0, -1.0);
      draw_cone (TRUE, 1.0, 2.0, 30, 30);
    glPopMatrix ();
  glEndList ();

  /* Torus */
  glNewList (shape_list_base + shape_torus, GL_COMPILE);
    draw_torus (TRUE, 0.4, 0.8, 30, 30);
  glEndList ();

  /* Tetrahedron */
  glNewList (shape_list_base + shape_tetrahedron, GL_COMPILE);
    glPushMatrix ();
      glScalef (1.2, 1.2, 1.2);
      draw_tetrahedron (TRUE);
    glPopMatrix ();
  glEndList ();

  /* Octahedron */
  glNewList (shape_list_base + shape_octahedron, GL_COMPILE);
    glPushMatrix ();
      glScalef (1.2, 1.2, 1.2);
      draw_octahedron (TRUE);
    glPopMatrix ();
  glEndList ();

  /* Dodecahedron */
  glNewList (shape_list_base + shape_dodecahedron, GL_COMPILE);
    glPushMatrix ();
      glScalef (0.7, 0.7, 0.7);
      draw_dodecahedron (TRUE);
    glPopMatrix ();
  glEndList ();

  /* Icosahedron */
  glNewList (shape_list_base + shape_icosahedron, GL_COMPILE);
    glPushMatrix ();
      glScalef (1.2, 1.2, 1.2);
      draw_icosahedron (TRUE);
    glPopMatrix ();
  glEndList ();

  /* Teapot */
  glNewList (shape_list_base + shape_teapot, GL_COMPILE);
    draw_teapot (TRUE, 1.0);
  glEndList ();

  gtk_widget_end_gl (widget, FALSE);
  /*** OpenGL END ***/

  return;
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
  float m[4][4];

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return FALSE;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity ();

  /* View transformation. */
  glTranslatef (0.0, 0.0, -10.0);
  glScalef (view_scale, view_scale, view_scale);
  add_quats (view_quat_diff, view_quat, view_quat);
  build_rotmatrix (m, view_quat);
  glMultMatrixf (&m[0][0]);

  /* Render shape */
  glMaterialfv (GL_FRONT, GL_AMBIENT, mat_current->ambient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_current->diffuse);
  glMaterialfv (GL_FRONT, GL_SPECULAR, mat_current->specular);
  glMaterialf (GL_FRONT, GL_SHININESS, mat_current->shininess * 128.0);
  glCallList (shape_list_base + shape_current);

  gtk_widget_end_gl (widget, TRUE);
  /*** OpenGL END ***/

  return TRUE;
}

static float begin_x = 0.0;
static float begin_y = 0.0;

static float dx = 0.0;
static float dy = 0.0;

static gboolean
button_press_event (GtkWidget      *widget,
		    GdkEventButton *event,
		    gpointer        data)
{
  if (animate)
    {
      if (event->button == 1)
        toggle_animation (widget);
    }
  else
    {
      view_quat_diff[0] = 0.0;
      view_quat_diff[1] = 0.0;
      view_quat_diff[2] = 0.0;
      view_quat_diff[3] = 1.0;
    }

  begin_x = event->x;
  begin_y = event->y;

  return FALSE;
}

static gboolean
button_release_event (GtkWidget      *widget,
                      GdkEventButton *event,
                      gpointer        data)
{
  if (!animate)
    {
      if (event->button == 1 &&
          ((dx*dx + dy*dy) > ANIMATE_THRESHOLD))
        toggle_animation (widget);
    }

  dx = 0.0;
  dy = 0.0;

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
  gboolean redraw = FALSE;

  gtk_widget_get_allocation (widget, &allocation);
  w = allocation.width;
  h = allocation.height;

  /* Rotation. */
  if (event->state & GDK_BUTTON1_MASK)
    {
      trackball (view_quat_diff,
		 (2.0 * begin_x - w) / w,
		 (h - 2.0 * begin_y) / h,
		 (2.0 * x - w) / w,
		 (h - 2.0 * y) / h);

      dx = x - begin_x;
      dy = y - begin_y;

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
    case GDK_KEY_Escape:
      gtk_main_quit ();
      break;

    default:
      return FALSE;
    }

  return TRUE;
}

static gboolean
idle (GtkWidget *widget)
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
map_event (GtkWidget *widget,
	   GdkEvent  *event,
	   gpointer   data)
{
  if (animate)
    idle_add (widget);

  return TRUE;
}

static gboolean
unmap_event (GtkWidget *widget,
	     GdkEvent  *event,
	     gpointer   data)
{
  idle_remove (widget);

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
	idle_remove (widget);
      else
	idle_add (widget);
    }

  return TRUE;
}

/* Toggle animation.*/
static void
toggle_animation (GtkWidget *widget)
{
  GtkAllocation allocation;
  animate = !animate;

  if (animate)
    {
      idle_add (widget);
    }
  else
    {
      idle_remove (widget);
      view_quat_diff[0] = 0.0;
      view_quat_diff[1] = 0.0;
      view_quat_diff[2] = 0.0;
      view_quat_diff[3] = 1.0;
      gtk_widget_get_allocation (widget, &allocation);
      gdk_window_invalidate_rect (gtk_widget_get_window (widget), &allocation, FALSE);
    }
}

static void
change_shape (GtkMenuItem  *menuitem,
              const GLuint *shape)
{
  shape_current = *shape;
  init_view ();
}

static void
change_material (GtkMenuItem  *menuitem,
                 MaterialProp *mat)
{
  mat_current = mat;
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
  GtkWidget *shapes_menu;
  GtkWidget *materials_menu;
  GtkWidget *menu;
  GtkWidget *menu_item;

  /*
   * Shapes submenu.
   */

  shapes_menu = gtk_menu_new ();

  /* Cube */
  menu_item = gtk_menu_item_new_with_label ("Cube");
  gtk_menu_shell_append (GTK_MENU_SHELL (shapes_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_shape), (gpointer) &shape_cube);
  gtk_widget_show (menu_item);

  /* Sphere */
  menu_item = gtk_menu_item_new_with_label ("Sphere");
  gtk_menu_shell_append (GTK_MENU_SHELL (shapes_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_shape), (gpointer) &shape_sphere);
  gtk_widget_show (menu_item);

  /* Cone */
  menu_item = gtk_menu_item_new_with_label ("Cone");
  gtk_menu_shell_append (GTK_MENU_SHELL (shapes_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_shape), (gpointer) &shape_cone);
  gtk_widget_show (menu_item);

  /* Torus */
  menu_item = gtk_menu_item_new_with_label ("Torus");
  gtk_menu_shell_append (GTK_MENU_SHELL (shapes_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_shape), (gpointer) &shape_torus);
  gtk_widget_show (menu_item);

  /* Tetrahedron */
  menu_item = gtk_menu_item_new_with_label ("Tetrahedron");
  gtk_menu_shell_append (GTK_MENU_SHELL (shapes_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_shape), (gpointer) &shape_tetrahedron);
  gtk_widget_show (menu_item);

  /* Octahedron */
  menu_item = gtk_menu_item_new_with_label ("Octahedron");
  gtk_menu_shell_append (GTK_MENU_SHELL (shapes_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_shape), (gpointer) &shape_octahedron);
  gtk_widget_show (menu_item);

  /* Dodecahedron */
  menu_item = gtk_menu_item_new_with_label ("Dodecahedron");
  gtk_menu_shell_append (GTK_MENU_SHELL (shapes_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_shape), (gpointer) &shape_dodecahedron);
  gtk_widget_show (menu_item);

  /* Icosahedron */
  menu_item = gtk_menu_item_new_with_label ("Icosahedron");
  gtk_menu_shell_append (GTK_MENU_SHELL (shapes_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_shape), (gpointer) &shape_icosahedron);
  gtk_widget_show (menu_item);

  /* Teapot */
  menu_item = gtk_menu_item_new_with_label ("Teapot");
  gtk_menu_shell_append (GTK_MENU_SHELL (shapes_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_shape), (gpointer) &shape_teapot);
  gtk_widget_show (menu_item);

  /*
   * Materials submenu.
   */

  materials_menu = gtk_menu_new ();

  /* Emerald */
  menu_item = gtk_menu_item_new_with_label ("Emerald");
  gtk_menu_shell_append (GTK_MENU_SHELL (materials_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_material), &mat_emerald);
  gtk_widget_show (menu_item);

  /* Jade */
  menu_item = gtk_menu_item_new_with_label ("Jade");
  gtk_menu_shell_append (GTK_MENU_SHELL (materials_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_material), &mat_jade);
  gtk_widget_show (menu_item);

  /* Obsidian */
  menu_item = gtk_menu_item_new_with_label ("Obsidian");
  gtk_menu_shell_append (GTK_MENU_SHELL (materials_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_material), &mat_obsidian);
  gtk_widget_show (menu_item);

  /* Pearl */
  menu_item = gtk_menu_item_new_with_label ("Pearl");
  gtk_menu_shell_append (GTK_MENU_SHELL (materials_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_material), &mat_pearl);
  gtk_widget_show (menu_item);

  /* Ruby */
  menu_item = gtk_menu_item_new_with_label ("Ruby");
  gtk_menu_shell_append (GTK_MENU_SHELL (materials_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_material), &mat_ruby);
  gtk_widget_show (menu_item);

  /* Turquoise */
  menu_item = gtk_menu_item_new_with_label ("Turquoise");
  gtk_menu_shell_append (GTK_MENU_SHELL (materials_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_material), &mat_turquoise);
  gtk_widget_show (menu_item);

  /* Brass */
  menu_item = gtk_menu_item_new_with_label ("Brass");
  gtk_menu_shell_append (GTK_MENU_SHELL (materials_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_material), &mat_brass);
  gtk_widget_show (menu_item);

  /* Bronze */
  menu_item = gtk_menu_item_new_with_label ("Bronze");
  gtk_menu_shell_append (GTK_MENU_SHELL (materials_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_material), &mat_bronze);
  gtk_widget_show (menu_item);

  /* Chrome */
  menu_item = gtk_menu_item_new_with_label ("Chrome");
  gtk_menu_shell_append (GTK_MENU_SHELL (materials_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_material), &mat_chrome);
  gtk_widget_show (menu_item);

  /* Copper */
  menu_item = gtk_menu_item_new_with_label ("Copper");
  gtk_menu_shell_append (GTK_MENU_SHELL (materials_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_material), &mat_copper);
  gtk_widget_show (menu_item);

  /* Gold */
  menu_item = gtk_menu_item_new_with_label ("Gold");
  gtk_menu_shell_append (GTK_MENU_SHELL (materials_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_material), &mat_gold);
  gtk_widget_show (menu_item);

  /* Silver */
  menu_item = gtk_menu_item_new_with_label ("Silver");
  gtk_menu_shell_append (GTK_MENU_SHELL (materials_menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
                    G_CALLBACK (change_material), &mat_silver);
  gtk_widget_show (menu_item);

  /*
   * Root popup menu.
   */

  menu = gtk_menu_new ();

  /* Shapes */
  menu_item = gtk_menu_item_new_with_label ("Shapes");
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_item), shapes_menu);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
  gtk_widget_show (menu_item);

  /* Materials */
  menu_item = gtk_menu_item_new_with_label ("Materials");
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_item), materials_menu);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
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
main (int   argc,
      char *argv[])
{
  GdkGLConfig *glconfig;
  gint major, minor;

  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *drawing_area;
  GtkWidget *menu;
  GtkWidget *button;

  /* Initialize GTK. */
  gtk_init (&argc, &argv);

  /* Initialize GtkGLExt. */
  gtk_gl_init (&argc, &argv);

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
  gtk_window_set_title (GTK_WINDOW (window), "shapes");

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
                         GDK_BUTTON_RELEASE_MASK    |
			 GDK_VISIBILITY_NOTIFY_MASK);

  g_signal_connect_after (G_OBJECT (drawing_area), "realize",
                          G_CALLBACK (realize), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "configure_event",
		    G_CALLBACK (configure_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "draw",
		    G_CALLBACK (draw), NULL);

  g_signal_connect (G_OBJECT (drawing_area), "button_press_event",
		    G_CALLBACK (button_press_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "button_release_event",
		    G_CALLBACK (button_release_event), NULL);
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
