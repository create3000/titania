/* -*- coding: utf-8 -*- */
/*
 * font-pangoft2-tex.c:
 * Simple example for text texture rendering with PangoFT2.
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

#define TIMEOUT_INTERVAL 10

#define FOVY_2 20.0
#define Z_NEAR 3.0

static const char *text = "This text is rendered with Παν語FT2.";

static PangoContext *ft2_context = NULL;

typedef struct _TextTexture {
  GLuint  name;
  GLsizei size;
  GLvoid *texels;
} TextTexture;

static TextTexture text_texture = {
  0, 0, NULL
};

static gboolean
gl_tex_create_texture (GLuint *texture)
{
  GLuint tex_name;
  GLint size;
  GLvoid *texels;

  glGetIntegerv (GL_MAX_TEXTURE_SIZE, &size);

  do
    {
      GLint width;

      glTexImage2D (GL_PROXY_TEXTURE_2D, 0, GL_RGBA,
                    size, size, 0,
                    GL_RGBA, GL_UNSIGNED_BYTE, NULL);

      glGetTexLevelParameteriv (GL_PROXY_TEXTURE_2D, 0,
                                GL_TEXTURE_WIDTH, &width);
      if (width != 0)
        break;

      size >>= 1;
    }
  while (size > 0);

  if (size == 0)
    {
      g_print ("There are not enough resources for the texture.\n");
      return FALSE;
    }

  texels = g_malloc (size * size * 4);
  memset (texels, 0, size * size * 4);

  glPixelStorei (GL_UNPACK_ALIGNMENT, 4);

  glGenTextures (1, &tex_name);

  glBindTexture (GL_TEXTURE_2D, tex_name);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA,
                size, size, 0,
                GL_RGBA, GL_UNSIGNED_BYTE,
                texels);

  text_texture.name = tex_name;
  text_texture.size = size;
  text_texture.texels = texels;

  *texture = text_texture.name;

  return TRUE;
}

static void
gl_tex_delete_texture (void)
{
  glDeleteTextures (1, &text_texture.name);
  g_free (text_texture.texels);
}

static gboolean
gl_tex_pango_ft2_render_layout (PangoLayout *layout,
                                GLuint *texture,
                                GLfloat *s0,
                                GLfloat *s1,
                                GLfloat *t0,
                                GLfloat *t1)
{
  PangoRectangle logical_rect;
  FT_Bitmap bitmap;
  guint32 *t;
  GLfloat color[4];
  guint32 rgb;
  GLfloat a;
  guint8 *row, *row_end;
  int i;

  if (text_texture.size == 0)
    return FALSE;

  pango_layout_get_extents (layout, NULL, &logical_rect);
  if (logical_rect.width == 0 || logical_rect.height == 0)
    return FALSE;

  bitmap.rows = PANGO_PIXELS (logical_rect.height);
  bitmap.width = PANGO_PIXELS (logical_rect.width);

  /* Ad hoc :-p */
  if (bitmap.width > text_texture.size || bitmap.rows > text_texture.size)
    return FALSE;

  bitmap.pitch = bitmap.width;
  bitmap.buffer = g_malloc (bitmap.rows * bitmap.width);
  bitmap.num_grays = 256;
  bitmap.pixel_mode = ft_pixel_mode_grays;

  memset (bitmap.buffer, 0, bitmap.rows * bitmap.width);
  pango_ft2_render_layout (&bitmap, layout,
                           PANGO_PIXELS (-logical_rect.x), 0);

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

  t = (guint32 *) text_texture.texels;

  if (a == 1.0)
    {
      do
        {
          row -= bitmap.width;
          for (i = 0; i < bitmap.width; i++)
#if !defined(GL_VERSION_1_2) && G_BYTE_ORDER == G_LITTLE_ENDIAN
            *t++ = rgb | (((guint32) row[i]) << 24);
#else
            *t++ = rgb | ((guint32) row[i]);
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
            *t++ = rgb | (((guint32) (a * row[i])) << 24);
#else
            *t++ = rgb | ((guint32) (a * row[i]));
#endif
        }
      while (row != row_end);
    }

  glPixelStorei (GL_UNPACK_ALIGNMENT, 4);

  glBindTexture (GL_TEXTURE_2D, text_texture.name);
#if !defined(GL_VERSION_1_2)
  glTexSubImage2D (GL_TEXTURE_2D, 0,
                   0, 0, bitmap.width, bitmap.rows,
                   GL_RGBA, GL_UNSIGNED_BYTE,
                   text_texture.texels);
#else
  glTexSubImage2D (GL_TEXTURE_2D, 0,
                   0, 0, bitmap.width, bitmap.rows,
                   GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
                   text_texture.texels);
#endif

  *texture = text_texture.name;

  *s0 = 0.0;
  *s1 = (GLfloat) bitmap.width / (GLfloat) text_texture.size;

  *t0 = 0.0;
  *t1 = (GLfloat) bitmap.rows / (GLfloat) text_texture.size;

  g_free (bitmap.buffer);

  return TRUE;
}

static void
realize (GtkWidget *widget,
         gpointer   data)
{
  GLUquadricObj *qobj;
  static GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};
  static GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};

  GLuint texture;

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

  /* Create texture. */
  gl_tex_create_texture (&texture);

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
                  2.5 * Z_NEAR);

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

#define ANGLE   30.0
/* tan (ANGLE * PI / 180.0) */
#define TANGENT 0.57735

#define TEXT_Z_NEAR  2.0
#define TEXT_Z_FAR  -5.0
#define TEXT_Z_DIFF  0.005

static GLfloat text_z = TEXT_Z_NEAR;

static gboolean animate = TRUE;

static gboolean
draw (GtkWidget *widget,
      cairo_t   *cr,
      gpointer   data)
{
  PangoContext *widget_context;
  PangoFontDescription *font_desc;
  PangoLayout *layout;

  GLuint texture;
  GLfloat s0, s1, t0, t1;
  gboolean ret;

  /* Font */
  widget_context = gtk_widget_get_pango_context (widget);
  font_desc = pango_context_get_font_description (widget_context);
  pango_font_description_set_size (font_desc, 24 * PANGO_SCALE);
  pango_context_set_font_description (ft2_context, font_desc);

  /* Text layout */
  layout = pango_layout_new (ft2_context);
  pango_layout_set_width (layout, PANGO_SCALE * 200);
  pango_layout_set_alignment (layout, PANGO_ALIGN_CENTER);
  pango_layout_set_text (layout, text, -1);

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return FALSE;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glCallList (1);

  /* Text color */
  glColor3f (1.0, 0.9, 0.0);

  /* Render text */
  ret = gl_tex_pango_ft2_render_layout (layout,
                                        &texture,
                                        &s0, &s1, &t0, &t1);
  if (ret)
    {
      glPushMatrix ();
        glTranslatef (0.0, -text_z * TANGENT, text_z + 2.0);
        glRotatef (ANGLE, 1.0, 0.0, 0.0);

        glEnable (GL_TEXTURE_2D);
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindTexture (GL_TEXTURE_2D, texture);
        glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glBegin (GL_QUADS);
          glTexCoord2f (s0, t0); glVertex3f (-1.0, 0.0,  1.0);
          glTexCoord2f (s0, t1); glVertex3f (-1.0, 0.0, -1.0);
          glTexCoord2f (s1, t1); glVertex3f ( 1.0, 0.0, -1.0);
          glTexCoord2f (s1, t0); glVertex3f ( 1.0, 0.0,  1.0);
        glEnd ();

        glDisable (GL_BLEND);
        glDisable (GL_TEXTURE_2D);

      glPopMatrix ();
    }

  gtk_widget_end_gl (widget, TRUE);
  /*** OpenGL END ***/

  g_object_unref (G_OBJECT (layout));

  return TRUE;
}

static void
unrealize (GtkWidget *widget,
	   gpointer   data)
{
  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return;

  gl_tex_delete_texture ();

  gtk_widget_end_gl (widget, FALSE);
  /*** OpenGL END ***/

  g_object_unref (G_OBJECT (ft2_context));
  pango_ft2_shutdown_display ();
}

static gboolean
timeout (GtkWidget *widget)
{
  GtkAllocation allocation;
  GdkWindow *window;

  window = gtk_widget_get_window (widget);
  gtk_widget_get_allocation (widget, &allocation);

  text_z -= TEXT_Z_DIFF;
  if (text_z <= TEXT_Z_FAR)
    text_z = TEXT_Z_NEAR;

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

  g_signal_connect (G_OBJECT (drawing_area), "map_event",
		    G_CALLBACK (map_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "unmap_event",
		    G_CALLBACK (unmap_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "visibility_notify_event",
		    G_CALLBACK (visibility_notify_event), NULL);

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
