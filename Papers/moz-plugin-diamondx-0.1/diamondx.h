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

#ifndef DIAMONDX_H
#define DIAMONDX_H

#include <Xlib.h>
#include "gtk/gtk.h"

#define PLUGIN_NAME         "DiamondX Plugin"
#define MIME_TYPES_HANDLED  "application/x-diamondx:diamondx:Plugin to test browser XEmbed capabilities"
#define PLUGIN_DESCRIPTION  "Plugin that tests XEmbed capabilities"

typedef struct {

    uint16 mode;
    NPWindow *window;
    uint32 x, y;
    uint32 width, height;

    NPP instance;
    NPBool pluginsHidden;

    GtkWidget *container;
    GtkWidget *canvas;
    guchar *four_quads;
    gboolean dialog_up;

    gboolean windowless;

    /* when we have to go raw */
    XImage *ximage;

} PluginInstance;

NPError DiamondXSetWindow(NPP instance, NPWindow* window);
int16 DiamondXHandleEvent(NPP instance, void* event);

#endif /* DIAMONDX_H */
