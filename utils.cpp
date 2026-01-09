#include "utils.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>
#include <gccore.h>

FSErr ISFS_GetFile(std::string_view path, void* file, size_t size) {
  FSErr err{};

  s32 fd = ISFS_Open(path.data(), ISFS_OPEN_READ);
  if (fd < 0) {
    err.error = std::format("ISFS_GetFile: unable to open file {} (error {})\n", path, fd);
    err.error_code = fd;
    return err;
  }

  // For all the files we are dealing with here, we are guarenteed to know the file size beforehand.
  s32 tmp_size = ISFS_Read(fd, file, size);
  if (tmp_size != size) {
    // If positive, the file could not be fully read.
    // If negative, it is most likely an underlying /dev/fs
    // error.
    if (tmp_size >= 0) {
      err.error = std::format("ISFS_GetFile: only able to read {} out of {} bytes!", tmp_size, size);
      err.error_code = tmp_size;
    } else if (tmp_size == ISFS_ENOENT) {
      err.error = std::format("ISFS_GetFile: file not found (error {})", tmp_size);
      err.error_code = tmp_size;
    } else {
      err.error = std::format("ISFS_GetFile: ISFS_Open failed! (error {})", tmp_size);
      err.error_code = tmp_size;
    }
  }
  ISFS_Close(fd);

  return err;
}
