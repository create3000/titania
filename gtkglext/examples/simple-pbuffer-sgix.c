/*
 * simple-pbuffer-sgix.c:
 * Simple GLX_SGIX_pbuffer example.
 *
 * written by Naofumi Yasufuku  <naofumi@users.sourceforge.net>
 */

#include <stdlib.h>
#include <stdio.h>

#include <gtk/gtk.h>

#include <gtk/gtkgl.h>
#include <gdk/x11/gdkglx.h>

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

static GLXFBConfigSGIX  (APIENTRY *GetFBConfigFromVisualSGIX) (Display*, XVisualInfo*);
static GLXPbufferSGIX   (APIENTRY *CreateGLXPbufferSGIX)      (Display*,
                                                               GLXFBConfigSGIX,
                                                               unsigned int,
                                                               unsigned int,
                                                               int*);
static void             (APIENTRY *DestroyGLXPbufferSGIX)     (Display*,
                                                               GLXPbufferSGIX);

static void
realize (GtkWidget *widget,
         gpointer   data)
{
  GtkAllocation allocation;

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

  gtk_widget_end_gl (widget, FALSE);
  /*** OpenGL END ***/
}

static gboolean
configure_event (GtkWidget         *widget,
                 GdkEventConfigure *event,
                 gpointer           data)
{
  GtkAllocation allocation;

  gtk_widget_get_allocation (widget, &allocation);

  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return FALSE;

  glViewport (0, 0,
              allocation.width, allocation.height);

  gtk_widget_end_gl (widget, FALSE);
  /*** OpenGL END ***/

  return TRUE;
}

static gboolean
draw (GtkWidget *widget,
      cairo_t   *cr,
      gpointer   data)
{
  /*** OpenGL BEGIN ***/
  if (!gtk_widget_begin_gl (widget))
    return FALSE;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glCallList (1);

  gtk_widget_end_gl (widget, TRUE);
  /*** OpenGL END ***/

  return TRUE;
}

/* ripped from pbdemo.c which comes with Mesa. */
static void
write_file(const char *filename,
           int width,
           int height)
{
  FILE *f;
  GLubyte *image;
  int i;

  image = malloc(width * height * 3 * sizeof(GLubyte));
  if (!image) {
    printf("Error: couldn't allocate image buffer\n");
    return;
  }

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);

  f = fopen(filename, "w");
  if (!f) {
    printf("Couldn't open image file: %s\n", filename);
    return;
  }
  fprintf(f,"P6\n");
  fprintf(f,"# ppm-file created by %s\n", "simple-pbuffer-sgix");
  fprintf(f,"%i %i\n", width, height);
  fprintf(f,"255\n");
  fclose(f);
  f = fopen(filename, "ab");  /* now append binary data */
  if (!f) {
    printf("Couldn't append to image file: %s\n", filename);
    return;
  }

  for (i=0;i<height;i++) {
    GLubyte *rowPtr;
    /* Remember, OpenGL images are bottom to top.  Have to reverse. */
    rowPtr = image + (height-1-i) * width*3;
    fwrite(rowPtr, 1, width*3, f);
  }

  fclose(f);
  free(image);

  printf("- Wrote %d by %d image file: %s\n", width, height, filename);
}

static void
render_to_file (GtkButton *button,
                GtkWidget *widget)
{
  GtkAllocation allocation;
  GdkGLConfig *glconfig;
  Display *xdisplay;
  XVisualInfo *xvinfo;
  GLXFBConfigSGIX fbconfig;

  GLXPbufferSGIX pbuffer;
  int pb_attrib_list[] = {
    GLX_LARGEST_PBUFFER_SGIX, True,
    GLX_PRESERVED_CONTENTS_SGIX, False,
    None
  };
  int width, height;

  GdkGLContext *glcontext;
  GLXContext glxcontext;

  g_print ("Render to PPM file...\n");

  glconfig = gtk_widget_get_gl_config (widget);
  xdisplay = gdk_x11_gl_config_get_xdisplay (glconfig);
  xvinfo = gdk_x11_gl_config_get_xvinfo (glconfig);

  /*
   * Get FBConfig.
   */

  g_print ("- get FBConfig\n");
  fbconfig = GetFBConfigFromVisualSGIX (xdisplay, xvinfo);
  if (!fbconfig)
    {
      g_print ("cannot get FBConfig\n");
      return;
    }

  /*
   * Create GLXPbuffer.
   */

  gtk_widget_get_allocation (widget, &allocation);
  width = allocation.width;
  height = allocation.height;

  g_print ("- create GLXPbuffer\n");
  pbuffer = CreateGLXPbufferSGIX (xdisplay, fbconfig,
                                  width, height,
                                  pb_attrib_list);
  if (!pbuffer)
    {
      g_print ("cannot create GLXPbuffer\n");
      return;
    }

  /*
   * Render.
   */

  glcontext = gtk_widget_get_gl_context (widget);
  glxcontext = gdk_x11_gl_context_get_glxcontext (glcontext);

  glXMakeCurrent (xdisplay, pbuffer, glxcontext);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glCallList (1);

  glFlush ();

  /*
   * Write to file.
   */

  write_file ("simple-pbuffer-sgix.ppm", width, height);

  /*
   * Destroy GLXPbuffer.
   */

  glXMakeCurrent (xdisplay, None, NULL);

  g_print ("- destroy GLXPbuffer\n");
  DestroyGLXPbufferSGIX (xdisplay, pbuffer);

  g_print ("Done.\n\n");
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
   * Query OpenGL extension version.
   */

  gdk_gl_query_version (&major, &minor);
  g_print ("\nOpenGL extension version - %d.%d\n\n",
           major, minor);

  /*
   * Configure OpenGL-capable visual.
   */

  /* Try double-buffered visual */
  glconfig = gdk_gl_config_new_by_mode (GDK_GL_MODE_RGB |
                                        GDK_GL_MODE_DEPTH |
                                        GDK_GL_MODE_DOUBLE);
  if (glconfig == NULL)
    {
      g_print ("*** Cannot find the double-buffered visual.\n");
      g_print ("*** Trying single-buffered visual.\n");

      /* Try single-buffered visual */
      glconfig = gdk_gl_config_new_by_mode (GDK_GL_MODE_RGB |
                                            GDK_GL_MODE_DEPTH);
      if (glconfig == NULL)
        {
          g_print ("*** No appropriate OpenGL-capable visual found.\n");
          exit (1);
        }
    }

  /*
   * GLX extensions.
   */

  if (!gdk_x11_gl_query_glx_extension (glconfig, "GLX_SGIX_fbconfig"))
    {
      g_print ("GLX_SGIX_fbconfig extension is not supported.\n");
      exit (1);
    }

  GetFBConfigFromVisualSGIX =
    (GLXFBConfigSGIX (APIENTRY *)(Display*, XVisualInfo*))
      gdk_gl_get_proc_address ("glXGetFBConfigFromVisualSGIX");

  if (!gdk_x11_gl_query_glx_extension (glconfig, "GLX_SGIX_pbuffer"))
    {
      g_print ("GLX_SGIX_pbuffer extension is not supported.\n");
      exit (1);
    }

  CreateGLXPbufferSGIX  =
    (GLXPbufferSGIX (APIENTRY *)(Display*, GLXFBConfigSGIX, unsigned int, unsigned int, int*))
      gdk_gl_get_proc_address ("glXCreateGLXPbufferSGIX");
  DestroyGLXPbufferSGIX =
    (void (APIENTRY *)(Display*, GLXPbufferSGIX))
      gdk_gl_get_proc_address ("glXDestroyGLXPbufferSGIX");

  /*
   * Top-level window.
   */

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "simple");

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

  gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);

  gtk_widget_set_events (drawing_area,
                         GDK_EXPOSURE_MASK |
                         GDK_BUTTON_PRESS_MASK);

  g_signal_connect_after (G_OBJECT (drawing_area), "realize",
                          G_CALLBACK (realize), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "configure_event",
		    G_CALLBACK (configure_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "draw",
		    G_CALLBACK (draw), NULL);

  gtk_widget_show (drawing_area);

  /*
   * Render button.
   */

  button = gtk_button_new_with_label ("Render to file");
  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

  g_signal_connect (G_OBJECT (button), "clicked",
                    G_CALLBACK (render_to_file), drawing_area);

  gtk_widget_show (button);

  /*
   * Simple quit button.
   */

  button = gtk_button_new_with_label ("Quit");
  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

  g_signal_connect (G_OBJECT (button), "clicked",
                    G_CALLBACK (gtk_main_quit), NULL);

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
