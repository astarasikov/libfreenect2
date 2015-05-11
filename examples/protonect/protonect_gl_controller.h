#ifndef __PROTONECT_GL_CONTROLLER__H__
#define __PROTONECT_GL_CONTROLLER__H__

#import "protonect_view.h"

@interface GLController : NSWindow
-(void)createGLView;

@property(nonatomic, readwrite, retain) ProtonectView *glView;
@end

#endif //__PROTONECT_GL_CONTROLLER__H__
