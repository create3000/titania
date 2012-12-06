/*
 * logo-model.h:
 * Functions for drawing GtkGLExt logo models.
 *
 * written by Naofumi Yasufuku  <naofumi@users.sourceforge.net>
 */

#ifndef __LOGO_MODEL_H__
#define __LOGO_MODEL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Draw center cube.
 */
void logo_draw_cube (void);

/*
 * Draw "G", "T", "K" color planes.
 */
void logo_draw_g_plane (void);
void logo_draw_t_plane (void);
void logo_draw_k_plane (void);

/*
 * Draw "G", "T", "K" 3D characters.
 */
void logo_draw_g (void);
void logo_draw_t (void);
void logo_draw_k (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LOGO_MODEL_H__ */
