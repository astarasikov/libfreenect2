#ifndef __FFMPEG_VIEW__H__
#define __FFMPEG_VIEW__H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

struct texdata {
	void *data;
	unsigned width;
	unsigned height;
	int is_float;
};

enum protonect_texidx {
	PROTONECT_TEX_RGB,
	PROTONECT_TEX_DEPTH,
	PROTONECT_TEX_IR,

	PROTONECT_TEX_COUNT = PROTONECT_TEX_IR + 1
};

struct protonect_texdata {
	struct texdata data[PROTONECT_TEX_COUNT];
};

void InitGL(void);
void UpdateGLTexture(struct protonect_texdata *data);

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __OBJC__

#import "opengl_view.h"

@interface ProtonectView : MyOpenGLViewBase
-(void)renderForTime:(CVTimeStamp)time;
-(void)setTexture:(struct protonect_texdata*)data;
@end
#endif //__OBJC

#endif //__FFMPEG_VIEW__H__
