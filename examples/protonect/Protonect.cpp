/*
 * This file is part of the OpenKinect Project. http://www.openkinect.org
 *
 * Copyright (c) 2011 individual OpenKinect contributors. See the CONTRIB file
 * for details.
 *
 * This code is licensed to you under the terms of the Apache License, version
 * 2.0, or, at your option, the terms of the GNU General Public License,
 * version 2.0. See the APACHE20 and GPL2 files for the text of the licenses,
 * or the following URLs:
 * http://www.apache.org/licenses/LICENSE-2.0
 * http://www.gnu.org/licenses/gpl-2.0.txt
 *
 * If you redistribute this file in source form, modified or unmodified, you
 * may:
 *   1) Leave this header intact and distribute it under the same terms,
 *      accompanying it with the APACHE20 and GPL20 files, or
 *   2) Delete the Apache 2.0 clause and accompany it with the GPL2 file, or
 *   3) Delete the GPL v2 clause and accompany it with the APACHE20 file
 * In all cases you must keep the copyright notice intact and include a copy
 * of the CONTRIB file.
 *
 * Binary distributions must follow the binary distribution requirements of
 * either License.
 */

#include <iostream>
#include <signal.h>

#include <opencv2/opencv.hpp>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/threading.h>

#include "protonect_view.h"

static libfreenect2::SyncMultiFrameListener *p_Listener = NULL;
static libfreenect2::FrameMap frames;

extern "C" {

void pn_event(void) {
  printf("%s\n", __func__);

  if (!p_Listener) {
    puts("skip");
    return;
  }

  p_Listener->waitForNewFrame(frames);
  libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
  libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
  libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];

  printf("rgb=<%d %d> ir=<%d %d> depth=<%d %d>\n", rgb->width, rgb->height,
         ir->width, ir->height, depth->width, depth->height);

  struct protonect_texdata data = {
	  .data = {
		  [PROTONECT_TEX_RGB] = {
			  .data = rgb->data,
			  .width = rgb->width,
			  .height = rgb->height,
		  },
		  [PROTONECT_TEX_DEPTH] = {
			  .data = depth->data,
			  .width = depth->width,
			  .height = depth->height,
			  .is_float = true,
		  },
		  [PROTONECT_TEX_IR] = {
			  .data = ir->data,
			  .width = ir->width,
			  .height = ir->height,
			  .is_float = true,
		  },
	  },
  };

  UpdateGLTexture(&data);

  p_Listener->release(frames);
}
}

int main(int argc, char *argv[]) {
  std::string program_path(argv[0]);
  size_t executable_name_idx = program_path.rfind("Protonect");

  std::string binpath = "/";

  if (executable_name_idx != std::string::npos) {
    binpath = program_path.substr(0, executable_name_idx);
  }

  libfreenect2::Freenect2 freenect2;
  libfreenect2::Freenect2Device *dev = freenect2.openDefaultDevice();

  if (dev == 0) {
    std::cout << "no device connected or failure opening the default one!"
              << std::endl;
    return -1;
  }

  libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color |
                                                libfreenect2::Frame::Ir |
                                                libfreenect2::Frame::Depth);
  p_Listener = &listener;

  dev->setColorFrameListener(&listener);
  dev->setIrAndDepthFrameListener(&listener);
  dev->start();

  std::cout << "device serial: " << dev->getSerialNumber() << std::endl;
  std::cout << "device firmware: " << dev->getFirmwareVersion() << std::endl;

  InitGL();

  // TODO: restarting ir stream doesn't work!
  // TODO: bad things will happen, if frame listeners are freed before
  // dev->stop() :(
  dev->stop();
  dev->close();

  return 0;
}
