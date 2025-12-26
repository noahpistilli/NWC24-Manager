#pragma once

#include <ogcsys.h>
#include <string>
#include <vector>

struct FSErr {
    std::string error;
    s32 error_code;
};

constexpr s32 ISFS_ENOENT = -106;

FSErr ISFS_GetFile(std::string_view path, std::vector<u8>* file);