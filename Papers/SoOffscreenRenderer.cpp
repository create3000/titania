    1 /**************************************************************************\
    2  *
    3  *  This file is part of the Coin 3D visualization library.
    4  *  Copyright (C) by Kongsberg Oil & Gas Technologies.
    5  *
    6  *  This library is free software; you can redistribute it and/or
    7  *  modify it under the terms of the GNU General Public License
    8  *  ("GPL") version 2 as published by the Free Software Foundation.
    9  *  See the file LICENSE.GPL at the root directory of this source
   10  *  distribution for additional information about the GNU GPL.
   11  *
   12  *  For using Coin with software that can not be combined with the GNU
   13  *  GPL, and for taking advantage of the additional benefits of our
   14  *  support services, please contact Kongsberg Oil & Gas Technologies
   15  *  about acquiring a Coin Professional Edition License.
   16  *
   17  *  See http://www.coin3d.org/ for more information.
   18  *
   19  *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
   20  *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
   21  *
   22 \**************************************************************************/
   23 
  234 // As first mentioned to me by kyrah, the functionality of this class
  235 // should really have been outside the core Coin library, seeing how
  236 // it makes heavy use of window-system specifics. To be SGI Inventor
  237 // compatible we need it to be part of the Coin API, though.
  238 //
  239 // mortene.
  240 
  241 // *************************************************************************
  242 
  243 // FIXME: we don't set up and render to RGBA-capable OpenGL-contexts,
  244 // even when the requested format from the app-programmer is
  245 // RGBA.
  246 //
  247 // I think this is what we should do:
  248 //
  249 //        1) first, try to get hold of a p-buffer with destination
  250 //        alpha (p-buffers are faster to render into, as they can take
  251 //        advantage of hardware acceleration)
  252 //
  253 //        2) failing that, try to make WGL/GLX/AGL/CGL set up a buffer
  254 //        with destination alpha
  255 //
  256 //        3) failing that, get hold of either a p-buffer or a straight
  257 //        WGL buffer with only RGB (no destination alpha -- this
  258 //        should never fail), and do post-processing on the rendered
  259 //        scene pixel-by-pixel to convert it into an RGBA texture
  260 //
  261 // 20020604 mortene.
  262 //
  263 // UPDATE 20041111 mortene: TGS Inventor has a new set of classes,
  264 // e.g. "SoGLGraphicConfigTemplate", which makes it possible to set up
  265 // wanted attributes with GL contexts. Audit their interface and
  266 // implement, if well designed.
  267 
  268 // *************************************************************************
  269 
  270 #include <Inventor/SoOffscreenRenderer.h>
  271 
  272 #ifdef HAVE_CONFIG_H
  273 #include "config.h"
  274 #endif // HAVE_CONFIG_H
  275 
  276 #include <assert.h>
  277 #include <string.h> // memset(), memcpy()
  278 #include <math.h> // for ceil()
  279 #include <limits.h> // SHRT_MAX
  280 
  281 #include <Inventor/C/glue/gl.h>
  282 #include <Inventor/C/tidbits.h>
  283 #include <Inventor/SbMatrix.h>
  284 #include <Inventor/SbVec2f.h>
  285 #include <Inventor/SbViewportRegion.h>
  286 #include <Inventor/SoPath.h>
  287 #include <Inventor/actions/SoGLRenderAction.h>
  288 #include <Inventor/elements/SoCullElement.h>
  289 #include <Inventor/elements/SoGLCacheContextElement.h>
  290 #include <Inventor/elements/SoModelMatrixElement.h>
  291 #include <Inventor/elements/SoProjectionMatrixElement.h>
  292 #include <Inventor/elements/SoViewVolumeElement.h>
  293 #include <Inventor/elements/SoViewingMatrixElement.h>
  294 #include <Inventor/elements/SoCacheElement.h>
  295 #include <Inventor/errors/SoDebugError.h>
  296 #include <Inventor/misc/SoContextHandler.h>
  297 #include <Inventor/misc/SoGLBigImage.h>
  298 #include <Inventor/nodes/SoCallback.h>
  299 #include <Inventor/nodes/SoCamera.h>
  300 #include <Inventor/nodes/SoNode.h>
  301 #include <Inventor/system/gl.h>
  302 #include <Inventor/SbTime.h>
  303 
  304 #include "glue/simage_wrapper.h"
  305 #include "tidbitsp.h"
  306 #include "coindefs.h" // COIN_STUB()
  307 
  308 // *************************************************************************
  309 
  310 #include "CoinOffscreenGLCanvas.h"
  311 
  312 #ifdef HAVE_GLX
  313 #include "SoOffscreenGLXData.h"
  314 #endif // HAVE_GLX
  315 
  316 #ifdef COIN_MACOS_10
  317 #include "SoOffscreenCGData.h"
  318 #endif // COIN_MACOS_10
  319 
  320 #ifdef HAVE_WGL
  321 #include "SoOffscreenWGLData.h"
  322 #endif // HAVE_WGL
  323 
  324 // *************************************************************************
  325 
  334 // *************************************************************************
  335 
  336 class SoOffscreenRendererP {
  337 public:
  338   SoOffscreenRendererP(SoOffscreenRenderer * masterptr,
  339                        const SbViewportRegion & vpr,
  340                        SoGLRenderAction * glrenderaction = NULL)
  341   {
  342     this->master = masterptr;
  343     this->didreadbuffer = TRUE;
  344 
  345     this->backgroundcolor.setValue(0,0,0);
  346     this->components = SoOffscreenRenderer::RGB;
  347     this->buffer = NULL;
  348     this->bufferbytesize = 0;
  349     this->lastnodewasacamera = FALSE;
  350 
  351     if (glrenderaction) {
  352       this->renderaction = glrenderaction;
  353     }
  354     else {
  355       this->renderaction = new SoGLRenderAction(vpr);
  356       this->renderaction->setCacheContext(SoGLCacheContextElement::getUniqueCacheContext());
  357       this->renderaction->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);
  358     }
  359 
  360     this->didallocation = glrenderaction ? FALSE : TRUE;
  361     this->viewport = vpr;
  362   }
  363 
  364   ~SoOffscreenRendererP()
  365   {
  366     if (this->didallocation) { delete this->renderaction; }
  367   }
  368 
  369   static SbBool offscreenContextsNotSupported(void);
  370 
  371   static const char * debugTileOutputPrefix(void);
  372 
  373   static SoGLRenderAction::AbortCode GLRenderAbortCallback(void *userData);
  374   SbBool renderFromBase(SoBase * base);
  375 
  376   void setCameraViewvolForTile(SoCamera * cam);
  377 
  378   static SbBool writeToRGB(FILE * fp, unsigned int w, unsigned int h,
  379                            unsigned int nrcomponents, const uint8_t * imgbuf);
  380 
  381   SbViewportRegion viewport;
  382   SbColor backgroundcolor;
  383   SoOffscreenRenderer::Components components;
  384   SoGLRenderAction * renderaction;
  385   SbBool didallocation;
  386 
  387   unsigned char * buffer;
  388   size_t bufferbytesize;
  389 
  390   CoinOffscreenGLCanvas glcanvas;
  391   unsigned int glcanvassize[2];
  392 
  393   int numsubscreens[2];
  394   // The subscreen size of the current tile. (Less than max if it's a
  395   // right- or bottom-border tile.)
  396   unsigned int subsize[2];
  397   // Keeps track of the current tile to be rendered.
  398   SbVec2s currenttile;
  399 
  400   SbBool lastnodewasacamera;
  401   SoCamera * visitedcamera;
  402 
  403   // used for lazy readPixels()
  404   SbBool didreadbuffer;
  405 private:
  406   SoOffscreenRenderer * master;
  407 };
  408 
  409 #define PRIVATE(p) (p->pimpl)
  410 #define PUBLIC(p) (p->master)
  411 
  412 // *************************************************************************
  413 
  414 // Set the environment variable below to get the individual tiles
  415 // written out for debugging purposes. E.g.
  416 //
  417 //   $ export COIN_DEBUG_SOOFFSCREENRENDERER_TILEPREFIX="/tmp/offscreentile_"
  418 //
  419 // Tile X and Y position, plus the ".rgb" suffix, will be added when
  420 // writing.
  421 const char *
  422 SoOffscreenRendererP::debugTileOutputPrefix(void)
  423 {
  424   return coin_getenv("COIN_DEBUG_SOOFFSCREENRENDERER_TILEPREFIX");
  425 }
  426 
  427 // *************************************************************************
  428 
  433 SoOffscreenRenderer::SoOffscreenRenderer(const SbViewportRegion & viewportregion)
  434 {
  435   PRIVATE(this) = new SoOffscreenRendererP(this, viewportregion);
  436 }
  437 
  443 SoOffscreenRenderer::SoOffscreenRenderer(SoGLRenderAction * action)
  444 {
  445   PRIVATE(this) = new SoOffscreenRendererP(this, action->getViewportRegion(),
  446                                            action);
  447 }
  448 
  452 SoOffscreenRenderer::~SoOffscreenRenderer()
  453 {
  454   delete[] PRIVATE(this)->buffer;
  455   delete PRIVATE(this);
  456 }
  457 
  461 float
  462 SoOffscreenRenderer::getScreenPixelsPerInch(void)
  463 {
  464   SbVec2f pixmmres(72.0f / 25.4f, 72.0f / 25.4f);
  465 #ifdef HAVE_GLX
  466   pixmmres = SoOffscreenGLXData::getResolution();
  467 #elif defined(HAVE_WGL)
  468   pixmmres = SoOffscreenWGLData::getResolution();
  469 #elif defined(COIN_MACOS_10)
  470   pixmmres = SoOffscreenCGData::getResolution();
  471 #endif // COIN_MACOS_10
  472 
  473   // The API-signature of this method is not what it should be: it
  474   // assumes the same resolution in the vertical and horizontal
  475   // directions.
  476   float pixprmm = (pixmmres[0] + pixmmres[1]) / 2.0f; // find average
  477 
  478   return pixprmm * 25.4f; // an inch is 25.4 mm.
  479 }
  480 
  492 SbVec2s
  493 SoOffscreenRenderer::getMaximumResolution(void)
  494 {
  495   return SbVec2s(SHRT_MAX, SHRT_MAX);
  496 }
  497 
  513 void
  514 SoOffscreenRenderer::setComponents(const Components components)
  515 {
  516   PRIVATE(this)->components = components;
  517 }
  518 
  524 SoOffscreenRenderer::Components
  525 SoOffscreenRenderer::getComponents(void) const
  526 {
  527   return PRIVATE(this)->components;
  528 
  529 }
  530 
  538 void
  539 SoOffscreenRenderer::setViewportRegion(const SbViewportRegion & region)
  540 {
  541   PRIVATE(this)->viewport = region;
  542 }
  543 
  547 const SbViewportRegion &
  548 SoOffscreenRenderer::getViewportRegion(void) const
  549 {
  550   return PRIVATE(this)->viewport;
  551 }
  552 
  557 void
  558 SoOffscreenRenderer::setBackgroundColor(const SbColor & color)
  559 {
  560   PRIVATE(this)->backgroundcolor = color;
  561 }
  562 
  566 const SbColor &
  567 SoOffscreenRenderer::getBackgroundColor(void) const
  568 {
  569   return PRIVATE(this)->backgroundcolor;
  570 }
  571 
  575 void
  576 SoOffscreenRenderer::setGLRenderAction(SoGLRenderAction * action)
  577 {
  578   if (action == PRIVATE(this)->renderaction) { return; }
  579 
  580   if (PRIVATE(this)->didallocation) { delete PRIVATE(this)->renderaction; }
  581   PRIVATE(this)->renderaction = action;
  582   PRIVATE(this)->didallocation = FALSE;
  583 }
  584 
  588 SoGLRenderAction *
  589 SoOffscreenRenderer::getGLRenderAction(void) const
  590 {
  591   return PRIVATE(this)->renderaction;
  592 }
  593 
  594 // *************************************************************************
  595 
  596 static void
  597 pre_render_cb(void * userdata, SoGLRenderAction * action)
  598 {
  599   glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
  600   action->setRenderingIsRemote(FALSE);
  601 }
  602 
  603 // *************************************************************************
  604 
  605 // Callback when rendering scenegraph to subscreens. Detects when a
  606 // camera has just been traversed, and then invokes the method which
  607 // narrows the camera viewport according to the current tile we're
  608 // rendering to.
  609 //
  610 // FIXME: if possible, it would be better to pick up from the state
  611 // whatever data we're now grabbing directly from the SoCamera nodes.
  612 // It'd be more robust, I believe, as the elements set by SoCamera can
  613 // in principle also be set from other code. 20041006 mortene.
  614 //
  615 // UPDATE 20050711 mortene: on how to fix this properly, see item #121
  616 // in Coin/BUGS.txt.
  617 SoGLRenderAction::AbortCode
  618 SoOffscreenRendererP::GLRenderAbortCallback(void *userData)
  619 {
  620   SoOffscreenRendererP * thisp = (SoOffscreenRendererP *) userData;
  621   const SoFullPath * path = (const SoFullPath*) thisp->renderaction->getCurPath();
  622   SoNode * node = path->getTail();
  623   assert(node);
  624 
  625   if (thisp->lastnodewasacamera) {
  626     thisp->setCameraViewvolForTile(thisp->visitedcamera);
  627     thisp->lastnodewasacamera = FALSE;
  628   }
  629 
  630   if (node->isOfType(SoCamera::getClassTypeId())) {
  631     thisp->visitedcamera = (SoCamera *) node;
  632     thisp->lastnodewasacamera = TRUE;
  633 
  634     // FIXME: this is not really entirely sufficient. If a camera is
  635     // already within a cached list upon the first invocation of a
  636     // render pass, we'll never get a callback upon encountering it.
  637     //
  638     // This would be a fairly obscure case, though, as the glcache
  639     // would have to be set up in another context, compatible for
  640     // sharing GL data with the one set up internally by the
  641     // SoOffscreenRenderer -- which is very unlikely.
  642     //
  643     // 20050512 mortene.
  644     //
  645     // UPDATE 20050711 mortene: on how to fix this properly, see item
  646     // #121 in Coin/BUGS.txt. (The tile number should be in an
  647     // element, which the SoCamera would query (and thereby also make
  648     // the cache dependent on)).
  649     SoCacheElement::invalidate(thisp->renderaction->getState());
  650   }
  651 
  652   return SoGLRenderAction::CONTINUE;
  653 }
  654 
  655 // Collects common code from the two render() functions.
  656 SbBool
  657 SoOffscreenRendererP::renderFromBase(SoBase * base)
  658 {
  659   if (SoOffscreenRendererP::offscreenContextsNotSupported()) {
  660     static SbBool first = TRUE;
  661     if (first) {
  662       SoDebugError::post("SoOffscreenRenderer::renderFromBase",
  663                          "SoOffscreenRenderer not compiled against any "
  664                          "window-system binding, it is defunct for this build.");
  665       first = FALSE;
  666     }
  667     return FALSE;
  668   }
  669 
  670   const SbVec2s fullsize = this->viewport.getViewportSizePixels();
  671   this->glcanvas.setWantedSize(fullsize);
  672 
  673   // check if no possible canvas size was found
  674   if (this->glcanvas.getActualSize() == SbVec2s(0, 0)) { return FALSE; }
  675 
  676   const uint32_t newcontext = this->glcanvas.activateGLContext();
  677   if (newcontext == 0) {
  678     SoDebugError::postWarning("SoOffscreenRenderer::renderFromBase",
  679                               "Could not set up an offscreen OpenGL context.");
  680     return FALSE;
  681   }
  682 
  683   const SbVec2s glsize = this->glcanvas.getActualSize();
  684 
  685   // We need to know the actual GL viewport size for tiled rendering,
  686   // in calculations when narrowing the camera view volume -- so we
  687   // store away this value for the "found a camera"-callback.
  688   //
  689   // FIXME: seems unnecessary now, should be able to just query
  690   // glcanvas.getActualSize() XXX
  691   this->glcanvassize[0] = glsize[0];
  692   this->glcanvassize[1] = glsize[1];
  693 
  694   if (CoinOffscreenGLCanvas::debug()) {
  695     SoDebugError::postInfo("SoOffscreenRendererP::renderFromBase",
  696                            "fullsize==<%d, %d>, glsize==<%d, %d>",
  697                            fullsize[0], fullsize[1], glsize[0], glsize[1]);
  698   }
  699 
  700   // oldcontext is used to restore the previous context id, in case
  701   // the render action is not allocated by us.
  702   const uint32_t oldcontext = this->renderaction->getCacheContext();
  703   this->renderaction->setCacheContext(newcontext);
  704 
  705   if (CoinOffscreenGLCanvas::debug()) {
  706     GLint colbits[4];
  707     glGetIntegerv(GL_RED_BITS, &colbits[0]);
  708     glGetIntegerv(GL_GREEN_BITS, &colbits[1]);
  709     glGetIntegerv(GL_BLUE_BITS, &colbits[2]);
  710     glGetIntegerv(GL_ALPHA_BITS, &colbits[3]);
  711     SoDebugError::postInfo("SoOffscreenRenderer::renderFromBase",
  712                            "GL context GL_[RED|GREEN|BLUE|ALPHA]_BITS=="
  713                            "[%d, %d, %d, %d]",
  714                            colbits[0], colbits[1], colbits[2], colbits[3]);
  715   }
  716 
  717   glEnable(GL_DEPTH_TEST);
  718   glClearColor(this->backgroundcolor[0],
  719                this->backgroundcolor[1],
  720                this->backgroundcolor[2],
  721                0.0f);
  722 
  723   // Make this large to get best possible quality on any "big-image"
  724   // textures (from using SoTextureScalePolicy).
  725   //
  726   // FIXME: this doesn't seem to be working, according to a report by
  727   // Colin Dunlop. See bug item #108. 20050509 mortene.
  728   //
  729   // UPDATE 20050711 mortene: the bug report referred to above may not
  730   // be correct. We should anyway fix this in a more appropriate
  731   // manner, for instance by setting up a new element with a boolean
  732   // value to indicate whether or not stuff should be rendered in
  733   // maximum quality. That would be generally useful for having better
  734   // control from the offscreenrenderer.
  735   const int bigimagechangelimit = SoGLBigImage::setChangeLimit(INT_MAX);
  736 
  737   // Deallocate old and allocate new target buffer, if necessary.
  738   //
  739   // If we need more space:
  740   const size_t bufsize =
  741     fullsize[0] * fullsize[1] * PUBLIC(this)->getComponents();
  742   SbBool alloc = (bufsize > this->bufferbytesize);
  743   // or if old buffer was much larger, free up the memory by fitting
  744   // to smaller size:
  745   alloc = alloc || (bufsize <= (this->bufferbytesize / 8));
  746 
  747   if (alloc) {
  748     delete[] this->buffer;
  749     this->buffer = new unsigned char[bufsize];
  750     this->bufferbytesize = bufsize;
  751   }
  752 
  753   if (SoOffscreenRendererP::debugTileOutputPrefix()) {
  754     (void)memset(this->buffer, 0x00, bufsize);
  755   }
  756 
  757   // needed to clear viewport after glViewport() is called from
  758   // SoGLRenderAction
  759   this->renderaction->addPreRenderCallback(pre_render_cb, NULL);
  760 
  761   // For debugging purposes, it has been made possible to use an
  762   // envvar to *force* tiled rendering even when it can be done in a
  763   // single chunk.
  764   //
  765   // (Note: don't use this envvar when using SoExtSelection nodes, for
  766   // the reason noted below.)
  767   static int forcetiled = -1;
  768   if (forcetiled == -1) {
  769     const char * env = coin_getenv("COIN_FORCE_TILED_OFFSCREENRENDERING");
  770     forcetiled = (env && (atoi(env) > 0)) ? 1 : 0;
  771     if (forcetiled) {
  772       SoDebugError::postInfo("SoOffscreenRendererP::renderFromBase",
  773                              "Forcing tiled rendering.");
  774     }
  775   }
  776 
  777   // FIXME: tiled rendering should be decided on the exact same
  778   // criteria as is used in SoExtSelection to decide which size to use
  779   // for its offscreen-buffer, as that node fails in VISIBLE_SHAPE
  780   // mode with tiled rendering. This is a weakness with SoExtSelection
  781   // which should be improved upon, if possible (i.e. fix
  782   // SoExtSelection, rather than adding some kind of "semi-private"
  783   // API to let SoExtSelection find out whether or not tiled rendering
  784   // is used). 20041028 mortene.
  785   const SbBool tiledrendering =
  786     forcetiled || (fullsize[0] > glsize[0]) || (fullsize[1] > glsize[1]);
  787 
  788   // Shall we use subscreen rendering or regular one-screen renderer?
  789   if (tiledrendering) {
  790     // we need to copy from GL to system memory if we're doing tiled rendering
  791     this->didreadbuffer = TRUE;
  792 
  793     for (int i=0; i < 2; i++) {
  794       this->numsubscreens[i] = (fullsize[i] + (glsize[i] - 1)) / glsize[i];
  795     }
  796 
  797     // We have to grab cameras using this callback during rendering
  798     this->visitedcamera = NULL;
  799     this->renderaction->setAbortCallback(SoOffscreenRendererP::GLRenderAbortCallback, this);
  800 
  801     // Render entire scenegraph for each subscreen.
  802     for (int y=0; y < this->numsubscreens[1]; y++) {
  803       for (int x=0; x < this->numsubscreens[0]; x++) {
  804         this->currenttile = SbVec2s(x, y);
  805 
  806         // Find current "active" tilesize.
  807         this->subsize[0] = glsize[0];
  808         this->subsize[1] = glsize[1];
  809         if (x == (this->numsubscreens[0] - 1)) {
  810           this->subsize[0] = fullsize[0] % glsize[0];
  811           if (this->subsize[0] == 0) { this->subsize[0] = glsize[0]; }
  812         }
  813         if (y == (this->numsubscreens[1] - 1)) {
  814           this->subsize[1] = fullsize[1] % glsize[1];
  815           if (this->subsize[1] == 0) { this->subsize[1] = glsize[1]; }
  816         }
  817 
  818         SbViewportRegion subviewport = SbViewportRegion(SbVec2s(this->subsize[0], this->subsize[1]));
  819         this->renderaction->setViewportRegion(subviewport);
  820 
  821         if (base->isOfType(SoNode::getClassTypeId()))
  822           this->renderaction->apply((SoNode *)base);
  823         else if (base->isOfType(SoPath::getClassTypeId()))
  824           this->renderaction->apply((SoPath *)base);
  825         else {
  826           assert(FALSE && "Cannot apply to anything else than an SoNode or an SoPath");
  827         }
  828 
  829         const unsigned int nrcomp = PUBLIC(this)->getComponents();
  830 
  831         const int MAINBUF_OFFSET =
  832           (glsize[1] * y * fullsize[0] + glsize[0] * x) * nrcomp;
  833 
  834         const SbVec2s vpsize = subviewport.getViewportSizePixels();
  835         this->glcanvas.readPixels(this->buffer + MAINBUF_OFFSET,
  836                                   vpsize, fullsize[0], nrcomp);
  837 
  838         // Debug option to dump the (full) buffer after each
  839         // iteration.
  840         if (SoOffscreenRendererP::debugTileOutputPrefix()) {
  841           SbString s;
  842           s.sprintf("%s_%03d_%03d.rgb",
  843                     SoOffscreenRendererP::debugTileOutputPrefix(), x, y);
  844 
  845           FILE * f = fopen(s.getString(), "wb");
  846           assert(f);
  847           SbBool w = SoOffscreenRendererP::writeToRGB(f, fullsize[0], fullsize[1],
  848                                                       nrcomp, this->buffer);
  849           assert(w);
  850           const int r = fclose(f);
  851           assert(r == 0);
  852 
  853           // This is sometimes useful to enable during debugging to
  854           // see the exact order and position of the tiles. Not
  855           // enabled by default because it makes the final buffer
  856           // completely blank.
  857 #if 0 // debug
  858           (void)memset(this->buffer, 0x00, bufsize);
  859 #endif // debug
  860         }
  861       }
  862     }
  863 
  864     this->renderaction->setAbortCallback(NULL, this);
  865 
  866     if (!this->visitedcamera) {
  867       SoDebugError::postWarning("SoOffscreenRenderer::renderFromBase",
  868                                 "No camera node found in scenegraph while rendering offscreen image. "
  869                                 "The result will most likely be incorrect.");
  870     }
  871 
  872   }
  873   // Regular, non-tiled rendering.
  874   else {
  875     // do lazy buffer read (GL context is read in getBuffer())
  876     this->didreadbuffer = FALSE;
  877     this->renderaction->setViewportRegion(this->viewport);
  878 
  879     SbTime t = SbTime::getTimeOfDay(); // for profiling
  880 
  881     if (base->isOfType(SoNode::getClassTypeId()))
  882       this->renderaction->apply((SoNode *)base);
  883     else if (base->isOfType(SoPath::getClassTypeId()))
  884       this->renderaction->apply((SoPath *)base);
  885     else  {
  886       assert(FALSE && "Cannot apply to anything else than an SoNode or an SoPath");
  887     }
  888 
  889     if (CoinOffscreenGLCanvas::debug()) {
  890       SoDebugError::postInfo("SoOffscreenRendererP::renderFromBase",
  891                              "*TIMING* SoGLRenderAction::apply() took %f msecs",
  892                              (SbTime::getTimeOfDay() - t).getValue() * 1000);
  893       t = SbTime::getTimeOfDay();
  894     }
  895 
  896     if (CoinOffscreenGLCanvas::debug()) {
  897       SoDebugError::postInfo("SoOffscreenRendererP::renderFromBase",
  898                              "*TIMING* glcanvas.readPixels() took %f msecs",
  899                              (SbTime::getTimeOfDay() - t).getValue() * 1000);
  900     }
  901   }
  902 
  903   this->renderaction->removePreRenderCallback(pre_render_cb, NULL);
  904 
  905   // Restore old value.
  906   (void)SoGLBigImage::setChangeLimit(bigimagechangelimit);
  907 
  908   this->glcanvas.deactivateGLContext();
  909   this->renderaction->setCacheContext(oldcontext); // restore old
  910 
  911   return TRUE;
  912 }
  913 
  956 SbBool
  957 SoOffscreenRenderer::render(SoNode * scene)
  958 {
  959   return PRIVATE(this)->renderFromBase(scene);
  960 }
  961 
  965 SbBool
  966 SoOffscreenRenderer::render(SoPath * scene)
  967 {
  968   return PRIVATE(this)->renderFromBase(scene);
  969 }
  970 
  971 // *************************************************************************
  972 
  976 unsigned char *
  977 SoOffscreenRenderer::getBuffer(void) const
  978 {
  979   if (!PRIVATE(this)->didreadbuffer) {
  980     const SbVec2s dims = this->getViewportRegion().getViewportSizePixels();
  981     //fprintf(stderr,"reading pixels: %d %d\n", dims[0], dims[1]);
  982 
  983     PRIVATE(this)->glcanvas.activateGLContext();
  984     PRIVATE(this)->glcanvas.readPixels(PRIVATE(this)->buffer, dims, dims[0],
  985                                        (unsigned int) this->getComponents());
  986     PRIVATE(this)->glcanvas.deactivateGLContext();
  987     PRIVATE(this)->didreadbuffer = TRUE;
  988   }
  989   return PRIVATE(this)->buffer;
  990 }
  991 
 1020 const void * const &
 1021 SoOffscreenRenderer::getDC(void) const
 1022 {
 1023   return PRIVATE(this)->glcanvas.getHDC();
 1024 }
 1025 
 1026 // *************************************************************************
 1027 
 1028 //
 1029 // avoid endian problems (little endian sucks, right? :)
 1030 //
 1031 static size_t
 1032 write_short(FILE * fp, unsigned short val)
 1033 {
 1034   unsigned char tmp[2];
 1035   tmp[0] = (unsigned char)(val >> 8);
 1036   tmp[1] = (unsigned char)(val & 0xff);
 1037   return fwrite(&tmp, 2, 1, fp);
 1038 }
 1039 
 1040 SbBool
 1041 SoOffscreenRendererP::writeToRGB(FILE * fp, unsigned int w, unsigned int h,
 1042                                  unsigned int nrcomponents,
 1043                                  const uint8_t * imgbuf)
 1044 {
 1045   // FIXME: add code to rle rows, pederb 2000-01-10
 1046 
 1047   (void)write_short(fp, 0x01da); // imagic
 1048   (void)write_short(fp, 0x0001); // raw (no rle yet)
 1049 
 1050   if (nrcomponents == 1)
 1051     (void)write_short(fp, 0x0002); // 2 dimensions (heightmap)
 1052   else
 1053     (void)write_short(fp, 0x0003); // 3 dimensions
 1054 
 1055   (void)write_short(fp, (unsigned short) w);
 1056   (void)write_short(fp, (unsigned short) h);
 1057   (void)write_short(fp, (unsigned short) nrcomponents);
 1058 
 1059   const size_t BUFSIZE = 500;
 1060   unsigned char buf[BUFSIZE];
 1061   (void)memset(buf, 0, BUFSIZE);
 1062   buf[7] = 255; // set maximum pixel value to 255
 1063   strcpy((char *)buf+8, "http://www.coin3d.org");
 1064   const size_t wrote = fwrite(buf, 1, BUFSIZE, fp);
 1065   assert(wrote == BUFSIZE);
 1066 
 1067   unsigned char * tmpbuf = new unsigned char[w];
 1068 
 1069   SbBool writeok = TRUE;
 1070   for (unsigned int c = 0; c < nrcomponents; c++) {
 1071     for (unsigned int y = 0; y < h; y++) {
 1072       for (unsigned int x = 0; x < w; x++) {
 1073         tmpbuf[x] = imgbuf[(x + y * w) * nrcomponents + c];
 1074       }
 1075       writeok = writeok && (fwrite(tmpbuf, 1, w, fp) == w);
 1076     }
 1077   }
 1078 
 1079   if (!writeok) {
 1080     SoDebugError::postWarning("SoOffscreenRendererP::writeToRGB",
 1081                               "error when writing RGB file");
 1082   }
 1083 
 1084   delete [] tmpbuf;
 1085   return writeok;
 1086 }
 1087 
 1088 
 1098 SbBool
 1099 SoOffscreenRenderer::writeToRGB(FILE * fp) const
 1100 {
 1101   if (SoOffscreenRendererP::offscreenContextsNotSupported()) { return FALSE; }
 1102 
 1103   SbVec2s size = PRIVATE(this)->viewport.getViewportSizePixels();
 1104 
 1105   return SoOffscreenRendererP::writeToRGB(fp, size[0], size[1],
 1106                                           this->getComponents(),
 1107                                           this->getBuffer());
 1108 }
 1109 
 1117 SbBool
 1118 SoOffscreenRenderer::writeToRGB(const char * filename) const
 1119 {
 1120   FILE * rgbfp = fopen(filename, "wb");
 1121   if (!rgbfp) {
 1122     SoDebugError::postWarning("SoOffscreenRenderer::writeToRGB",
 1123                               "couldn't open file '%s'", filename);
 1124     return FALSE;
 1125   }
 1126   SbBool result = this->writeToRGB(rgbfp);
 1127   (void)fclose(rgbfp);
 1128   return result;
 1129 }
 1130 
 1140 SbBool
 1141 SoOffscreenRenderer::writeToPostScript(FILE * fp) const
 1142 {
 1143   // just choose a page size of 8.5 x 11 inches (A4)
 1144   return this->writeToPostScript(fp, SbVec2f(8.5f, 11.0f));
 1145 }
 1146 
 1154 SbBool
 1155 SoOffscreenRenderer::writeToPostScript(const char * filename) const
 1156 {
 1157   FILE * psfp = fopen(filename, "wb");
 1158   if (!psfp) {
 1159     SoDebugError::postWarning("SoOffscreenRenderer::writeToPostScript",
 1160                               "couldn't open file '%s'", filename);
 1161     return FALSE;
 1162   }
 1163   SbBool result = this->writeToPostScript(psfp);
 1164   (void)fclose(psfp);
 1165   return result;
 1166 }
 1167 
 1177 SbBool
 1178 SoOffscreenRenderer::writeToPostScript(FILE * fp,
 1179                                        const SbVec2f & printsize) const
 1180 {
 1181   if (SoOffscreenRendererP::offscreenContextsNotSupported()) { return FALSE;}
 1182 
 1183   const SbVec2s size = PRIVATE(this)->viewport.getViewportSizePixels();
 1184   const int nc = this->getComponents();
 1185   const float defaultdpi = 72.0f; // we scale against this value
 1186   const float dpi = this->getScreenPixelsPerInch();
 1187   const SbVec2s pixelsize((short)(printsize[0]*defaultdpi),
 1188                           (short)(printsize[1]*defaultdpi));
 1189 
 1190   const unsigned char * src = this->getBuffer();
 1191   const int chan = nc <= 2 ? 1 : 3;
 1192   const SbVec2s scaledsize((short) ceil(size[0]*defaultdpi/dpi),
 1193                            (short) ceil(size[1]*defaultdpi/dpi));
 1194 
 1195   cc_string storedlocale;
 1196   SbBool changed = coin_locale_set_portable(&storedlocale);
 1197 
 1198   fprintf(fp, "%%!PS-Adobe-2.0 EPSF-1.2\n");
 1199   fprintf(fp, "%%%%BoundingBox: 0 %d %d %d\n",
 1200           pixelsize[1]-scaledsize[1],
 1201           scaledsize[0],
 1202           pixelsize[1]);
 1203   fprintf(fp, "%%%%Creator: Coin <http://www.coin3d.org>\n");
 1204   fprintf(fp, "%%%%EndComments\n");
 1205 
 1206   fprintf(fp, "\n");
 1207   fprintf(fp, "/origstate save def\n");
 1208   fprintf(fp, "\n");
 1209   fprintf(fp, "%% workaround for bug in some PS interpreters\n");
 1210   fprintf(fp, "%% which doesn't skip the ASCII85 EOD marker.\n");
 1211   fprintf(fp, "/~ {currentfile read pop pop} def\n\n");
 1212   fprintf(fp, "/image_wd %d def\n", size[0]);
 1213   fprintf(fp, "/image_ht %d def\n", size[1]);
 1214   fprintf(fp, "/pos_wd %d def\n", size[0]);
 1215   fprintf(fp, "/pos_ht %d def\n", size[1]);
 1216   fprintf(fp, "/image_dpi %g def\n", dpi);
 1217   fprintf(fp, "/image_scale %g image_dpi div def\n", defaultdpi);
 1218   fprintf(fp, "/image_chan %d def\n", chan);
 1219   fprintf(fp, "/xpos_offset 0 image_scale mul def\n");
 1220   fprintf(fp, "/ypos_offset 0 image_scale mul def\n");
 1221   fprintf(fp, "/pix_buf_size %d def\n\n", size[0]*chan);
 1222   fprintf(fp, "/page_ht %g %g mul def\n", printsize[1], defaultdpi);
 1223   fprintf(fp, "/page_wd %g %g mul def\n", printsize[0], defaultdpi);
 1224   fprintf(fp, "/image_xpos 0 def\n");
 1225   fprintf(fp, "/image_ypos page_ht pos_ht image_scale mul sub def\n");
 1226   fprintf(fp, "image_xpos xpos_offset add image_ypos ypos_offset add translate\n");
 1227   fprintf(fp, "\n");
 1228   fprintf(fp, "/pix pix_buf_size string def\n");
 1229   fprintf(fp, "image_wd image_scale mul image_ht image_scale mul scale\n");
 1230   fprintf(fp, "\n");
 1231   fprintf(fp, "image_wd image_ht 8\n");
 1232   fprintf(fp, "[image_wd 0 0 image_ht 0 0]\n");
 1233   fprintf(fp, "currentfile\n");
 1234   fprintf(fp, "/ASCII85Decode filter\n");
 1235   // fprintf(fp, "/RunLengthDecode filter\n"); // FIXME: add later. 2003???? pederb.
 1236   if (chan == 3) fprintf(fp, "false 3\ncolorimage\n");
 1237   else fprintf(fp,"image\n");
 1238 
 1239   const int rowlen = 72;
 1240   int num = size[0] * size[1];
 1241   unsigned char tuple[4];
 1242   unsigned char linebuf[rowlen+5];
 1243   int tuplecnt = 0;
 1244   int linecnt = 0;
 1245   int cnt = 0;
 1246   while (cnt < num) {
 1247     switch (nc) {
 1248     default: // avoid warning
 1249     case 1:
 1250       coin_output_ascii85(fp, src[cnt], tuple, linebuf, &tuplecnt, &linecnt, rowlen, FALSE);
 1251       break;
 1252     case 2:
 1253       coin_output_ascii85(fp, src[cnt*2], tuple, linebuf, &tuplecnt, &linecnt, rowlen, FALSE);
 1254       break;
 1255     case 3:
 1256       coin_output_ascii85(fp, src[cnt*3], tuple, linebuf, &tuplecnt, &linecnt, rowlen, FALSE);
 1257       coin_output_ascii85(fp, src[cnt*3+1], tuple, linebuf, &tuplecnt, &linecnt, rowlen, FALSE);
 1258       coin_output_ascii85(fp, src[cnt*3+2], tuple, linebuf, &tuplecnt, &linecnt, rowlen, FALSE);
 1259       break;
 1260     case 4:
 1261       coin_output_ascii85(fp, src[cnt*4], tuple, linebuf, &tuplecnt, &linecnt, rowlen, FALSE);
 1262       coin_output_ascii85(fp, src[cnt*4+1], tuple, linebuf, &tuplecnt, &linecnt,rowlen, FALSE);
 1263       coin_output_ascii85(fp, src[cnt*4+2], tuple, linebuf, &tuplecnt, &linecnt, rowlen, FALSE);
 1264       break;
 1265     }
 1266     cnt++;
 1267   }
 1268 
 1269   // flush data in ascii85 encoder
 1270   coin_flush_ascii85(fp, tuple, linebuf, &tuplecnt, &linecnt, rowlen);
 1271 
 1272   fprintf(fp, "~>\n\n"); // ASCII85 EOD marker
 1273   fprintf(fp, "origstate restore\n");
 1274   fprintf(fp, "\n");
 1275   fprintf(fp, "%%%%Trailer\n");
 1276   fprintf(fp, "\n");
 1277   fprintf(fp, "%%%%EOF\n");
 1278 
 1279   if (changed) { coin_locale_reset(&storedlocale); }
 1280 
 1281   return (SbBool) (ferror(fp) == 0);
 1282 }
 1283 
 1292 SbBool
 1293 SoOffscreenRenderer::writeToPostScript(const char * filename,
 1294                                        const SbVec2f & printsize) const
 1295 {
 1296   FILE * psfp = fopen(filename, "wb");
 1297   if (!psfp) {
 1298     SoDebugError::postWarning("SoOffscreenRenderer::writeToPostScript",
 1299                               "couldn't open file '%s'", filename);
 1300     return FALSE;
 1301   }
 1302   SbBool result = this->writeToPostScript(psfp, printsize);
 1303   (void)fclose(psfp);
 1304   return result;
 1305 }
 1306 
 1307 // FIXME: the file format support checking could have been done
 1308 // better, for instance by using MIME types. Consider fixing the API
 1309 // for later major releases. 20020206 mortene.
 1310 //
 1311 // UPDATE 20050711 mortene: it seems like TGS has extended their API
 1312 // in an even worse way; by adding separate writeToJPEG(),
 1313 // writeToPNG(), etc etc functions.
 1314 
 1355 SbBool
 1356 SoOffscreenRenderer::isWriteSupported(const SbName & filetypeextension) const
 1357 {
 1358   if (!simage_wrapper()->versionMatchesAtLeast(1,1,0)) {
 1359     if (CoinOffscreenGLCanvas::debug()) {
 1360       if (!simage_wrapper()->available) {
 1361         SoDebugError::postInfo("SoOffscreenRenderer::isWriteSupported",
 1362                                "simage library not available.");
 1363       } else {
 1364         SoDebugError::postInfo("SoOffscreenRenderer::isWriteSupported",
 1365                                "You need simage v1.1 for this functionality.");
 1366       }
 1367     }
 1368     return FALSE;
 1369   }
 1370   int ret = simage_wrapper()->simage_check_save_supported(filetypeextension.getString());
 1371   return ret ? TRUE : FALSE;
 1372 }
 1373 
 1389 int
 1390 SoOffscreenRenderer::getNumWriteFiletypes(void) const
 1391 {
 1392   if (!simage_wrapper()->versionMatchesAtLeast(1,1,0)) {
 1393 #if COIN_DEBUG
 1394     SoDebugError::postInfo("SoOffscreenRenderer::getNumWriteFiletypes",
 1395                            "You need simage v1.1 for this functionality.");
 1396 #endif // COIN_DEBUG
 1397     return 0;
 1398   }
 1399   return simage_wrapper()->simage_get_num_savers();
 1400 }
 1401 
 1461 void
 1462 SoOffscreenRenderer::getWriteFiletypeInfo(const int idx,
 1463                                           SbPList & extlist,
 1464                                           SbString & fullname,
 1465                                           SbString & description)
 1466 {
 1467   if (!simage_wrapper()->versionMatchesAtLeast(1,1,0)) {
 1468 #if COIN_DEBUG
 1469     SoDebugError::postInfo("SoOffscreenRenderer::getNumWriteFiletypes",
 1470                            "You need simage v1.1 for this functionality.");
 1471 #endif // COIN_DEBUG
 1472     return;
 1473   }
 1474   extlist.truncate(0);
 1475   assert(idx >= 0 && idx < this->getNumWriteFiletypes());
 1476   void * saver = simage_wrapper()->simage_get_saver_handle(idx);
 1477   SbString allext(simage_wrapper()->simage_get_saver_extensions(saver));
 1478   const char * start = allext.getString();
 1479   const char * curr = start;
 1480   const char * end = strchr(curr, ',');
 1481   while (end) {
 1482     const ptrdiff_t offset_start = curr - start;
 1483     const ptrdiff_t offset_end = end - start - 1;
 1484     SbString ext = allext.getSubString((int)offset_start, (int)offset_end);
 1485     SbName extname(ext.getString());
 1486     extlist.append((void*)extname.getString());
 1487     curr = end+1;
 1488     end = strchr(curr, ',');
 1489   }
 1490   const ptrdiff_t offset = curr - start;
 1491   SbString ext = allext.getSubString((int)offset);
 1492   SbName extname(ext.getString());
 1493   extlist.append((void*)extname.getString());
 1494   const char * fullname_s = simage_wrapper()->simage_get_saver_fullname(saver);
 1495   const char * desc_s = simage_wrapper()->simage_get_saver_description(saver);
 1496   fullname = fullname_s ? SbString(fullname_s) : SbString("");
 1497   description = desc_s ? SbString(desc_s) : SbString("");
 1498 }
 1499 
 1513 SbBool
 1514 SoOffscreenRenderer::writeToFile(const SbString & filename, const SbName & filetypeextension) const
 1515 {
 1516   // FIXME: shouldn't there be warnings on these two? 20050510 mortene.
 1517   if (!simage_wrapper()->versionMatchesAtLeast(1,1,0)) { return FALSE; }
 1518   if (SoOffscreenRendererP::offscreenContextsNotSupported()) { return FALSE; }
 1519 
 1520   SbVec2s size = PRIVATE(this)->viewport.getViewportSizePixels();
 1521   int comp = (int) this->getComponents();
 1522   unsigned char * bytes = this->getBuffer();
 1523   int ret = simage_wrapper()->simage_save_image(filename.getString(),
 1524                                                 bytes,
 1525                                                 int(size[0]), int(size[1]), comp,
 1526                                                 filetypeextension.getString());
 1527   return ret ? TRUE : FALSE;
 1528 }
 1529 
 1530 // *************************************************************************
 1531 
 1547 void
 1548 SoOffscreenRenderer::setPbufferEnable(SbBool enable)
 1549 {
 1550   // FIXME: change the semantics of this function from just ignoring
 1551   // the input argument, to using it for shutting off pbuffers if
 1552   // FALSE?
 1553   //
 1554   // not sure there's really any good reason to do that, however.
 1555   //
 1556   // mortene.
 1557 }
 1558 
 1564 SbBool
 1565 SoOffscreenRenderer::getPbufferEnable(void) const
 1566 {
 1567   // FIXME: should perhaps return a flag indicating whether or not the
 1568   // system can use pbuffers. this depends on the GL context, however,
 1569   // so the design of this Mercury Inventor API function is inherently
 1570   // flawed.
 1571   //
 1572   // hardly any GL driver these days does *not* provide pbuffers,
 1573   // though, so this is unlikely to be an important issue.
 1574   //
 1575   // mortene.
 1576 
 1577   return TRUE;
 1578 }
 1579 
 1580 // *************************************************************************
 1581 
 1582 // FIXME: this should really be done by SoCamera, on the basis of data
 1583 // from an "SoTileRenderingElement". See BUGS.txt, item #121. 20050712 mortene.
 1584 void
 1585 SoOffscreenRendererP::setCameraViewvolForTile(SoCamera * cam)
 1586 {
 1587   SoState * state = (PUBLIC(this)->getGLRenderAction())->getState();
 1588 
 1589   // A small trick to change the aspect ratio without changing the
 1590   // scenegraph camera.
 1591   SbViewVolume vv;
 1592   const float aspectratio = this->viewport.getViewportAspectRatio();
 1593   const SbVec2s vporigin = this->viewport.getViewportOriginPixels();
 1594 
 1595   switch(cam->viewportMapping.getValue()) {
 1596   case SoCamera::CROP_VIEWPORT_FILL_FRAME:
 1597   case SoCamera::CROP_VIEWPORT_LINE_FRAME:
 1598   case SoCamera::CROP_VIEWPORT_NO_FRAME:
 1599     vv = cam->getViewVolume(0.0f);
 1600 
 1601     { // FIXME: should really fix this bug, not just warn that it is
 1602       // there. See item #191 in Coin/BUGS.txt for more information.
 1603       // 20050714 mortene.
 1604       static SbBool first = TRUE;
 1605       if (first) {
 1606         SbString s;
 1607         cam->viewportMapping.get(s);
 1608         SoDebugError::postWarning("SoOffscreenRendererP::setCameraViewvolForTile",
 1609                                   "The SoOffscreenRenderer does not yet work "
 1610                                   "properly with the SoCamera::viewportMapping "
 1611                                   "field set to '%s'", s.getString());
 1612         first = FALSE;
 1613       }
 1614     }
 1615     break;
 1616   case SoCamera::ADJUST_CAMERA:
 1617     vv = cam->getViewVolume(aspectratio);
 1618     if (aspectratio < 1.0f) vv.scale(1.0f / aspectratio);
 1619     break;
 1620   case SoCamera::LEAVE_ALONE:
 1621     vv = cam->getViewVolume(0.0f);
 1622     break;
 1623   default:
 1624     assert(0 && "unknown viewport mapping");
 1625     break;
 1626   }
 1627 
 1628   const int LEFTINTPOS = (this->currenttile[0] * this->glcanvassize[0]) - vporigin[0];
 1629   const int RIGHTINTPOS = LEFTINTPOS + this->subsize[0];
 1630   const int TOPINTPOS = (this->currenttile[1] * this->glcanvassize[1]) - vporigin[1];
 1631   const int BOTTOMINTPOS = TOPINTPOS + this->subsize[1];
 1632 
 1633   const SbVec2s fullsize = this->viewport.getViewportSizePixels();
 1634   const float left = float(LEFTINTPOS) / float(fullsize[0]);
 1635   const float right = float(RIGHTINTPOS) / float(fullsize[0]);
 1636   // Swap top / bottom, to flip the coordinate system for the Y axis
 1637   // the way we want it.
 1638   const float top = float(BOTTOMINTPOS) / float(fullsize[1]);
 1639   const float bottom = float(TOPINTPOS) / float(fullsize[1]);
 1640 
 1641   if (CoinOffscreenGLCanvas::debug()) {
 1642     SoDebugError::postInfo("SoOffscreenRendererP::setCameraViewvolForTile",
 1643                            "narrowing for tile <%d, %d>: <%f, %f> - <%f, %f>",
 1644                            this->currenttile[0], this->currenttile[1],
 1645                            left, bottom, right, top);
 1646   }
 1647 
 1648   // Reshape view volume
 1649   vv = vv.narrow(left, bottom, right, top);
 1650 
 1651   SbMatrix proj, affine;
 1652   vv.getMatrices(affine, proj);
 1653 
 1654   // Support antialiasing if renderpasses > 1
 1655   if (renderaction->getNumPasses() > 1) {
 1656     SbVec3f jittervec;
 1657     SbMatrix m;
 1658     const int vpsize[2] = { this->glcanvassize[0], this->glcanvassize[1] };
 1659     coin_viewvolume_jitter(renderaction->getNumPasses(), renderaction->getCurPass(),
 1660                            vpsize, (float *)jittervec.getValue());
 1661     m.setTranslate(jittervec);
 1662     proj.multRight(m);
 1663   }
 1664 
 1665   SoCullElement::setViewVolume(state, vv);
 1666   SoViewVolumeElement::set(state, cam, vv);
 1667   SoProjectionMatrixElement::set(state, cam, proj);
 1668   SoViewingMatrixElement::set(state, cam, affine);
 1669 }
 1670 
 1678 void
 1679 SoOffscreenRenderer::getWriteFiletypeInfo(const int idx,
 1680                                           SbList <SbName> & extlist,
 1681                                           SbString & fullname,
 1682                                           SbString & description)
 1683 {
 1684   SoDebugError::postWarning("SoOffscreenRenderer::getWriteFiletypeInfo",
 1685                             "Obsoleted function. Use instead the overloaded "
 1686                             "method with an SbPList for the second argument.");
 1687 
 1688   if (!simage_wrapper()->versionMatchesAtLeast(1,1,0)) {
 1689 #if COIN_DEBUG
 1690     SoDebugError::postInfo("SoOffscreenRenderer::getWriteFiletypeInfo",
 1691                            "You need simage v1.1 for this functionality.");
 1692 #endif // COIN_DEBUG
 1693     return;
 1694   }
 1695   extlist.truncate(0);
 1696   assert(idx >= 0 && idx < this->getNumWriteFiletypes());
 1697   void * saver = simage_wrapper()->simage_get_saver_handle(idx);
 1698   SbString allext(simage_wrapper()->simage_get_saver_extensions(saver));
 1699   const char * start = allext.getString();
 1700   const char * curr = start;
 1701   const char * end = strchr(curr, ',');
 1702   while (end) {
 1703     const ptrdiff_t offset_start = curr - start;
 1704     const ptrdiff_t offset_end = end - start - 1;
 1705     SbString ext = allext.getSubString((int)offset_start, (int)offset_end);
 1706     extlist.append(SbName(ext.getString()));
 1707     curr = end+1;
 1708     end = strchr(curr, ',');
 1709   }
 1710   const ptrdiff_t offset = curr - start;
 1711   SbString ext = allext.getSubString((int)offset);
 1712   extlist.append(SbName(ext.getString()));
 1713   const char * fullname_s = simage_wrapper()->simage_get_saver_fullname(saver);
 1714   const char * desc_s = simage_wrapper()->simage_get_saver_description(saver);
 1715   fullname = fullname_s ? SbString(fullname_s) : SbString("");
 1716   description = desc_s ? SbString(desc_s) : SbString("");
 1717 }
 1718 
 1719 // *************************************************************************
 1720 
 1721 SbBool
 1722 SoOffscreenRendererP::offscreenContextsNotSupported(void)
 1723 {
 1724   // Returning FALSE means that offscreen rendering seems to be
 1725   // generally supported on the system.
 1726   //
 1727   // (It is however important to be robust and handle cases where it
 1728   // still fails, as this can happen due to e.g. lack of resources or
 1729   // other causes that may change during run-time.)
 1730 
 1731 #ifdef HAVE_GLX
 1732   return FALSE;
 1733 #elif defined(HAVE_WGL)
 1734   return FALSE;
 1735 #elif defined(COIN_MACOS_10)
 1736   return FALSE;
 1737 #endif
 1738 
 1739   // No win-system GL binding was found, so we're sure that offscreen
 1740   // rendering can *not* be done.
 1741   return TRUE;
 1742 }
 1743 
 1744 // *************************************************************************
 1745 
 1746 #undef PRIVATE
 1747 #undef PUBLIC
