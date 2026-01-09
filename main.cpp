#include <iostream>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <unistd.h>

#include "download.h"
#include "select.h"
#include "nwc24.h"
#include "nwc24dl.h"

extern "C" {
#include <libpatcher/libpatcher.h>
}

static void* xfb = nullptr;
static GXRModeObj* rmode = nullptr;


int main() {
  VIDEO_Init();

  rmode = VIDEO_GetPreferredMode(nullptr);
  xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
  VIDEO_ClearFrameBuffer(rmode, xfb, COLOR_BLACK);
  VIDEO_Configure(rmode);
  VIDEO_SetNextFramebuffer(xfb);
  VIDEO_SetBlack(FALSE);
  VIDEO_Flush();
  VIDEO_WaitVSync();
  if (rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();
  console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);

  bool success = apply_patches();
  if (!success) {
    std::cout << "Failed to apply patches!" << std::endl;
    sleep(5);
    WII_ReturnToMenu();
  }

  CONF_Init();
  ISFS_Initialize();
  WPAD_Init();

  NWC24::Init();
  DownloadModal::Init();

  auto modal = SelectModal();
  modal.Start();

  return 0;
}

