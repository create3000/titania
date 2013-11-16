/***********************************************************************

DiamondX plugin
Example XEmbed-aware Mozilla browser plugin by Adobe.

Copyright (c) 2007 Adobe Systems Incorporated

 

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

 

The above copyright notice and this permission notice shall be included in 
all copies or substantial portions of the Software.

 

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR

COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF 
OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE. *
***********************************************************************/

#include "npapi.h"
#include "diamondx.h"

static NPWindow windowlessWindow;

static void ContextMenuCallback1(PluginInstance *plugin)
{
    printf("   first menu item clicked\n");
}

static void ContextMenuCallback2(PluginInstance *plugin)
{
    printf("   second menu item clicked\n");
}

static gboolean CanvasEventCallback(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{

    gboolean handled = 0;
    PluginInstance *plugin = (PluginInstance *)user_data;

    GdkEventKey    *key;
    GdkEventFocus  *focus;
    GdkEventMotion *motion;
    GdkEventButton *button;
    GdkEventExpose *expose;

    /* only honor expose events if a dialog is up */
    if (plugin->dialog_up && event->type != GDK_EXPOSE)
        return 1;

    switch(event->type) {

    case GDK_EXPOSE:
        expose = (GdkEventExpose *)event;
        printf("  (canvas): expose event (x, y, w, h) = (%d, %d, %d, %d)\n",
            expose->area.x, expose->area.y, expose->area.width, expose->area.height);
        gdk_draw_rgb_image (
            plugin->canvas->window, 
            plugin->canvas->style->fg_gc[GTK_STATE_NORMAL],
            expose->area.x, 
            expose->area.y, 
            expose->area.width, 
            expose->area.height,
            GDK_RGB_DITHER_MAX, 
            plugin->four_quads + (expose->area.y * plugin->width + expose->area.x) * 3, 
            plugin->width * 3);
        handled = 1;
        break;

    case GDK_FOCUS_CHANGE:
        focus = (GdkEventFocus *)event;
        printf("  (canvas): focus change event (%s)\n", (focus->in) ? "in" : "out");
        handled = 1;
        break;

    case GDK_ENTER_NOTIFY:
        printf("  (canvas): enter event\n");
        handled = 1;
        break;

    case GDK_LEAVE_NOTIFY:
        printf("  (canvas): leave event\n");
        handled = 1;
        break;

    case GDK_MOTION_NOTIFY:
        motion = (GdkEventMotion *)event;
        printf("  (canvas): mouse motion -> (%f, %f)\n", motion->x, motion->y);
        handled = 1;
        break;

    case GDK_BUTTON_PRESS:
        button = (GdkEventButton *)event;
        printf("  (canvas): button %d pressed @ (%f, %f)\n", button->button, button->x, button->y);
        if (button->button == 1)
        {
            /* take the focus into our own hands */
            gtk_widget_grab_focus(plugin->canvas);

            /* handle first quadrant */
            if (((uint32)button->x <= (plugin->width  / 2)) &&
                ((uint32)button->y <= (plugin->height / 2)))
            {
                printf("  first quadrant clicked\n");

                /* make a generic modal dialog */
                GtkWidget *dialog = gtk_dialog_new_with_buttons(
                    "Hey!", 
                    GTK_WINDOW(plugin->container), 
                    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_STOCK_OK,
                    GTK_RESPONSE_OK,
                    NULL);
                plugin->dialog_up = 1;
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(GTK_WIDGET(dialog));
                plugin->dialog_up = 0;
            }
            else
            if (((uint32)button->x >  (plugin->width  / 2)) &&
                ((uint32)button->y <= (plugin->height / 2)))
            {
                printf("  second quadrant clicked\n");

                /* make a GTK file selection dialog */
                GtkWidget *dialog = gtk_file_chooser_dialog_new(
                    "Open a file...",
                    GTK_WINDOW(plugin->container),
                    GTK_FILE_CHOOSER_ACTION_OPEN,
                    GTK_STOCK_OK,
                    GTK_RESPONSE_OK,
                    GTK_STOCK_CANCEL,
                    GTK_RESPONSE_CANCEL,
                    NULL);
                gtk_window_set_destroy_with_parent(GTK_WINDOW(dialog), 1);
                plugin->dialog_up = 1;
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(GTK_WIDGET(dialog));
                plugin->dialog_up = 0;
            }
            else
            if (((uint32)button->x <= (plugin->width  / 2)) &&
                ((uint32)button->y >  (plugin->height / 2)))
            {
                printf("  third quadrant clicked\n");
            }
            else
            {
                printf("  fourth quadrant clicked\n");
            }
        }
        else
        if (button->button == 3)
        {
            /* context menu */
            GtkWidget *menu;
            GtkWidget *menu_item;

            menu = gtk_menu_new();

            menu_item = gtk_check_menu_item_new_with_label ("Hey, is this thing on?");
            gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (menu_item), 1);
            gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
            g_signal_connect_swapped(G_OBJECT(menu_item), "activate",
                G_CALLBACK(ContextMenuCallback1), plugin);
            gtk_widget_show (menu_item);

            menu_item = gtk_separator_menu_item_new();
            gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
            gtk_widget_show (menu_item);

            menu_item = gtk_menu_item_new_with_label ("You suck! Get off the stage!");
            gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
            g_signal_connect_swapped(G_OBJECT(menu_item), "activate",
                G_CALLBACK(ContextMenuCallback2), plugin);
            gtk_widget_show (menu_item);

            gtk_menu_popup (GTK_MENU (menu), NULL, NULL, NULL, NULL,
                0, gtk_get_current_event_time());
        }
        handled = 1;
        break;

    case GDK_BUTTON_RELEASE:
        button = (GdkEventButton *)event;
        printf("  (canvas): button %d released @ (%f, %f)\n", button->button, button->x, button->y);
        handled = 1;
        break;

    case GDK_KEY_PRESS:
    case GDK_KEY_RELEASE:
        key = (GdkEventKey *)event;
        printf("  (canvas): key %s, code = %d (0x%X)\n", 
            (event->type == GDK_KEY_PRESS) ? "pressed" : "released", key->keyval, key->keyval);
        handled = 1;
        break;

    case GDK_SCROLL:
        printf("  (canvas): scroll event\n");
        handled = 0;
        break;

    default:
        printf("  (canvas): unhandled event %d\n", event->type);
        break;

    }

    return handled;
}

NPError DiamondXSetWindow(NPP instance, NPWindow* window)
{
    PluginInstance* This;
    NPSetWindowCallbackStruct *ws_info;

    int quads, x, y;
    guchar *row, *index;
    guchar r1, g1, b1;
    guchar r2, g2, b2;
    int split;
    int half_width, half_height;
    gboolean positive_triangle;
    int xembedSupported = 0;

    if (instance == NULL)
        return NPERR_INVALID_INSTANCE_ERROR;

    This = (PluginInstance*) instance->pdata;

    if (This == NULL)
        return NPERR_INVALID_INSTANCE_ERROR;

    ws_info = (NPSetWindowCallbackStruct *)window->ws_info;

    /* Mozilla likes to re-run its greatest hits */
    if ((window == This->window) &&
        (window->x == This->x) &&
        (window->y == This->y) &&
        (window->width == This->width) &&
        (window->height == This->height)) {
        printf("  (window re-run; returning)\n");
        return NPERR_NO_ERROR;
    }

    NPN_GetValue(instance, NPNVSupportsXEmbedBool, &xembedSupported);
    if (!xembedSupported)
    {
        printf("DiamondX: XEmbed not supported\n");
        return NPERR_GENERIC_ERROR;
    }

    This->window = window;
    This->x = window->x;
    This->y = window->y;
    This->width = window->width;
    This->height = window->height;

    half_width = This->width / 2;
    half_height = This->height / 2;

    /* the following conditional block is for handling the experimental 
     * windowless mode */
    if (This->windowless)
    {
        Display *disp = This->window->window;
        Drawable drawable = (Drawable)This->window->ws_info;
        GC gc = DefaultGC(disp, DefaultScreen(disp));

        if (!disp || ! drawable)
            return NPERR_GENERIC_ERROR;

        if (!This->ximage)
        {
            This->ximage = XCreateImage(
                disp,  /* actually an X display */
                DefaultVisual(disp, DefaultScreen(disp)),  /* default visual */
                32,  /* image depth */
                ZPixmap,
                0,
                NULL,
                This->width,
                This->height,
                BitmapPad(disp),  /* scanline pad */
                0
            );

            if (This->ximage)
            {
                This->ximage->data = NPN_MemAlloc(This->ximage->height * This->ximage->bytes_per_line);
                if (!This->ximage->data)
                {
                    XDestroyImage(This->ximage);
                    This->ximage = NULL;
                }
            }
        }

        XPutImage(
            disp,
            drawable,
            gc,
            This->ximage,
            0,  /* source x */
            0,  /* source y */
            0,  /* dest x */
            0,  /* dest y */
            This->width,
            This->height);

        return NPERR_NO_ERROR;
    }

    This->four_quads = (guchar *)NPN_MemAlloc(This->width * This->height * 3);
    if (!This->four_quads)
        return NPERR_GENERIC_ERROR;

    /* draw the diamond */
    for (quads = 0; quads < 4; quads++) {
        switch (quads) {
        case 0:
            row = This->four_quads;
            r1 = 0xFF; g1 = 0x00; b1 = 0x00;
            r2 = 0xFF; g2 = 0xFF; b2 = 0xFF;
            positive_triangle = TRUE;
            break;

        case 1:
            row = This->four_quads + (This->width / 2) * 3;
            r1 = 0xFF; g1 = 0xFF; b1 = 0xFF;
            r2 = 0x00; g2 = 0xFF; b2 = 0x00;
            positive_triangle = FALSE;
            break;

        case 2:
            row = This->four_quads + (This->height / 2) * (This->width * 3);
            r1 = 0x00; g1 = 0x00; b1 = 0xFF;
            r2 = 0xFF; g2 = 0xFF; b2 = 0xFF;
            positive_triangle = FALSE;
            break;

        case 3:
            row = This->four_quads + ((This->height / 2) * (This->width * 3)) + ((This->width / 2) * 3);
            r1 = 0xFF; g1 = 0xFF; b1 = 0xFF;
            r2 = 0x80; g2 = 0x80; b2 = 0x80;
            positive_triangle = TRUE;
            break;

        }

        for (y = 0; y < half_height; y++)
        {
            index = row;
            if (positive_triangle)
                split = (half_height - y) * half_width / half_height;
            else
                split = y * half_width / half_height;

            for (x = 0; x < half_width; x++)
            {
                if (x >= split)
                {
                    *index++ = r1;
                    *index++ = g1;
                    *index++ = b1;
                }
                else
                {
                    *index++ = r2;
                    *index++ = g2;
                    *index++ = b2;
                }
            }
            row += This->width * 3;
        }
    }

    /* create a GtkPlug container and plot a drawing canvas inside */
    This->container = gtk_plug_new((GdkNativeWindow)window->window);

    This->canvas = gtk_drawing_area_new();

    /* make sure the canvas is capable of receiving focus */
    GTK_WIDGET_SET_FLAGS (GTK_WIDGET(This->canvas), GTK_CAN_FOCUS);

    /* all the events that our canvas wants to receive */
    gtk_widget_add_events(
        This->canvas,
        GDK_BUTTON_PRESS_MASK | 
        GDK_BUTTON_RELEASE_MASK |
        GDK_KEY_PRESS_MASK | 
        GDK_KEY_RELEASE_MASK | 
        GDK_POINTER_MOTION_MASK |
        GDK_SCROLL_MASK |
        GDK_EXPOSURE_MASK |
        GDK_VISIBILITY_NOTIFY_MASK |
        GDK_ENTER_NOTIFY_MASK |
        GDK_LEAVE_NOTIFY_MASK |
        GDK_FOCUS_CHANGE_MASK
    );
    g_signal_connect(G_OBJECT(This->canvas), "event", G_CALLBACK(CanvasEventCallback), This);

    gtk_widget_show(This->canvas);

    gtk_container_add(GTK_CONTAINER(This->container), This->canvas);

    gtk_widget_show(This->container);

    gdk_draw_rgb_image (This->canvas->window, This->canvas->style->fg_gc[GTK_STATE_NORMAL],
        0, 0, This->width, This->height,
        GDK_RGB_DITHER_MAX, This->four_quads, This->width * 3);

    return NPERR_NO_ERROR;
}

int16 DiamondXHandleEvent(NPP instance, void* event)
{
    XGraphicsExposeEvent exposeEvent;
    XEvent *nsEvent;

    nsEvent = (XEvent *)event;
    exposeEvent = nsEvent->xgraphicsexpose;
printf(" event: x, y, w, h = %d, %d, %d, %d; display @ %p, window/drawable = %d\n",
  exposeEvent.x,
  exposeEvent.y,
  exposeEvent.width,
  exposeEvent.height,
  exposeEvent.display,
  exposeEvent.drawable);

    windowlessWindow.window = exposeEvent.display;
    windowlessWindow.x = exposeEvent.x;
    windowlessWindow.y = exposeEvent.y;
    windowlessWindow.width = exposeEvent.width;
    windowlessWindow.height = exposeEvent.height;
    windowlessWindow.ws_info = (void *)exposeEvent.drawable;

    NPP_SetWindow(instance, &windowlessWindow);

    return 0;
}
