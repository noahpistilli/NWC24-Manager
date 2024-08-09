#include "nwc24.h"
#include <unistd.h>
#include <iostream>

NWC24* NWC24::s_instance;

NWC24::NWC24() {
  m_fd = IOS_Open("/dev/net/kd/request", IPC_OPEN_READ);
  if (m_fd < 0) {
    std::cout << "Failed to init WiiConnect24" << std::endl;
    sleep(5);
    WII_ReturnToMenu();
  }
}

void NWC24::Init() {
  s_instance = new NWC24;
}

s32 NWC24::DoIoctl(Command command, s32 in_size, void *in_buf, s32 io_size, void *io_buf) const {
  return IOS_Ioctl(m_fd, static_cast<s32>(command), in_buf, in_size, io_buf, io_size);
}

s32 NWC24::GetLastError() const
{
  void *io_buf = std::malloc(256);
  s32 ret = DoIoctl(Command::GetSchedulerStat, 0, nullptr, 256, io_buf);
  if (ret < 0) {
    return -1;
  }

  return reinterpret_cast<s32 *>(io_buf)[2];
}