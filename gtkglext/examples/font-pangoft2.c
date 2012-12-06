/* -*- coding: utf-8 -*- */
/*
 * font-pangoft2.c:
 * Simple example for text rendering with PangoFT2.
 *
 * written by Naofumi Yasufuku  <naofumi@users.sourceforge.net>
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <gtk/gtk.h>
#include <pango/pangoft2.h>

#include <gtk/gtkgl.h>

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

#define FOVY_2 20.0
#define Z_NEAR 3.0

static const char *text = "This text is rendered with Παν語FT2.";

static PangoContext *ft2_context = NULL;

static void
gl_pango_ft2_render_layout (PangoLayout *layout)
{
  PangoRectangle logical_rect;
  FT_Bitmap bitmap;
  GLvoid *pixels;
  guint32 *p;
  GLfloat color[4];
  guint32 rgb;
  GLfloat a;
  guint8 *row, *row_end;
  int i;

  pango_layout_get_extents (layout, NULL, &logical_rect);
  if (logical_rect.width == 0 || logical_rect.height == 0)
    return;

  bitmap.rows = PANGO_PIXELS (logical_rect.height);
  bitmap.width = PANGO_PIXELS (logical_rect.width);
  bitmap.pitch = bitmap.width;
  bitmap.buffer = g_malloc (bitmap.rows * bitmap.width);
  bitmap.num_grays = 256;
  bitmap.pixel_mode = ft_pixel_mode_grays;

  memset (bitmap.buffer, 0, bitmap.rows * bitmap.width);
  pango_ft2_render_layout (&bitmap, layout,
                           PANGO_PIXELS (-logical_rect.x), 0);

  pixels = g_malloc (bitmap.rows * bitmap.width * 4);
  p = (guint32 *) pixels;

  glGetFloatv (GL_CURRENT_COLOR, color);
#if !defined(GL_VERSION_1_2) && G_BYTE_ORDER == G_LITTLE_ENDIAN
  rgb =  ((guint32) (color[0] * 255.0))        |
        (((guint32) (color[1] * 255.0)) << 8)  |
        (((guint32) (color[2] * 255.0)) << 16);
#else
  rgb = (((guint32) (color[0] * 255.0)) << 24) |
        (((guint32) (color[1] * 255.0)) << 16) |
        (((guint32) (color[2] * 255.0)) << 8);
#endif
  a = color[3];

  row = bitmap.buffer + bitmap.rows * bitmap.width; /* past-the-end */
  row_end = bitmap.buffer;      /* beginning */

  if (a == 1.0)
    {
      do
        {
          row -= bitmap.width;
          for (i = 0; i < bitmap.width; i++)
#if !defined(GL_VERSION_1_2) && G_BYTE_ORDER == G_LITTLE_ENDIAN
            *p++ = rgb | (((guint32) row[i]) << 24);
#else
            *p++ = rgb | ((guint32) row[i]);
#endif
        }
      while (row != row_end);
    }
  else
    {
      do
        {
          row -= bitmap.width;
          for (i = 0; i < bitmap.width; i++)
#if !defined(GL_VERSION_1_2) && G_BYTE_ORDER == G_LITTLE_ENDIAN
            *p++ = rgb | (((guint32) (a * row[i])) << 24);
#else
            *p++ = rgb | ((guint32) (a * row[i]));
#endif
        }
      while (row != row_end);
    }

  glPixelStorei (GL_UNPACK_ALIGNMENT, 4);

  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if !defined(GL_VERSION_1_2)
  glDrawPixels (bitmap.width, bitmap.rows,
                GL_RGBA, GL_UNSIGNED_BYTE,
                pixels);
#else
  glDrawPixels (bitmap.width, bitmap.rows,
                GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
                pixels);
#endif

  glDisable (GL_BLEND);

  g_free (bitmap.buffer);
  g_free (pixels);
}

static void
realize (GtkWidget *widget,
         gpointer   data)
{
  GLUquadricObj *qobj;
  static GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};
  static GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
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

  glClearColor (0.0, 0.0, 0.0, 0.0);
  glClearDepth (1.0);

  gtk_widget_end_gl (widget, FALSE);
  /*** OpenGL END ***/

  /* Get PangoFT2 context. */
  ft2_context = pango_ft2_get_context (72, 72);
}

static gboolean
configure_event (GtkWidget         *widget,
                 GdkEventConfigure *event,
                 gpointer           data)
{
  GtkAllocation allocation;
  GLsizei w;
  GLsizei h;

  gtk_widget_get_allocation (widget, &allocation);
  w = allocation.width;
  h = allocation.height;

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return FALSE;

  glViewport (0, 0, w, h);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (2.0 * FOVY_2,
                  (GLfloat) w / (GLfloat) h,
                  Z_NEAR,
                  2.0 * Z_NEAR);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  gluLookAt (0.0, 0.0, Z_NEAR,
             0.0, 0.0, 0.0,
             0.0, 1.0, 0.0);
  glTranslatef (0.0, 0.0, -Z_NEAR);

  gtk_widget_end_gl (widget, FALSE);
  /*** OpenGL END ***/

  return TRUE;
}

static gboolean
draw (GtkWidget *widget,
      cairo_t   *cr,
      gpointer   data)
{
  GtkAllocation allocation;

  PangoContext *widget_context;
  PangoFontDescription *font_desc;
  PangoLayout *layout;
  PangoRectangle logical_rect;
  GLfloat text_w, text_h;
  GLfloat tangent_h;

  gtk_widget_get_allocation (widget, &allocation);

  /* Font */
  widget_context = gtk_widget_get_pango_context (widget);
  font_desc = pango_context_get_font_description (widget_context);
  pango_font_description_set_size (font_desc, 24 * PANGO_SCALE);
  pango_context_set_font_description (ft2_context, font_desc);

  /* Text layout */
  layout = pango_layout_new (ft2_context);
  pango_layout_set_width (layout, PANGO_SCALE * allocation.width);
  pango_layout_set_alignment (layout, PANGO_ALIGN_CENTER);
  pango_layout_set_text (layout, text, -1);

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return FALSE;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glCallList (1);

  /* Text color */
  glColor3f (1.0, 0.9, 0.0);

  /* Text position */
  pango_layout_get_extents (layout, NULL, &logical_rect);
  text_w = PANGO_PIXELS (logical_rect.width);
  text_h = PANGO_PIXELS (logical_rect.height);
  /*
   * tangent = Z_NEAR * tan (FOVY_2 * G_PI / 180.0)
   * w = allocation.width
   * h = allocation.height
   *
   * x = -1.0 * (text_w/w) * tangent * (w/h) = -text_w * tangent / h
   * y = -1.0 * (text_h/h) * tangent         = -text_h * tangent / h
   * z = Z_NEAR
   */
  tangent_h = Z_NEAR * tan (FOVY_2 * G_PI / 180.0);
  tangent_h /= allocation.height;
  glRasterPos3f (-text_w * tangent_h,
                 -text_h * tangent_h,
                 Z_NEAR);

  /* Render text */
  gl_pango_ft2_render_layout (layout);

  gtk_widget_end_gl (widget, TRUE);
  /*** OpenGL END ***/

  g_object_unref (G_OBJECT (layout));

  return TRUE;
}

static void
unrealize (GtkWidget *widget,
	   gpointer   data)
{
  g_object_unref (G_OBJECT (ft2_context));
  pango_ft2_shutdown_display ();
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
  gtk_window_set_title (GTK_WINDOW (window), "font-pangoft2");

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

  gtk_main ();

  return 0;
}
