//
// Created by Noah Pistilli on 2024-08-06.
//

#include "nwc24dl.h"

#include <iostream>
#include <cstring>
#include <sys/unistd.h>

#include "utils.h"

constexpr char CONFIG_PATH[] = "/shared2/wc24/nwc24dl.bin";

NWC24Dl::NWC24Dl() {
  m_data = std::make_unique<DLList>();
}


bool NWC24Dl::ReadDlList()
{
  void* data = std::aligned_alloc(32, sizeof(DLList));

  FSErr err = ISFS_GetFile(CONFIG_PATH, data, sizeof(DLList));
  if (err.error_code != 0) {
    return false;
  }

  std::memcpy(m_data.get(), data, sizeof(DLList));
  std::free(data);
  return true;
}

int NWC24Dl::GetNumberOfEntries() const {
  int num{};

  for (DLListEntry entry : m_data->entries) {
    if (entry.type != UNUSED)
      num++;
  }

  return num;
}


std::vector<std::string> NWC24Dl::GetDownloadURLs() const {
  std::vector<std::string> urls{};

  for (DLListEntry entry : m_data->entries) {
    if (entry.type != UNUSED)
      urls.emplace_back(entry.dl_url);
  }

  return urls;
}

std::vector<std::string> NWC24Dl::GetGameIDs() const {
  std::vector<std::string> ids{};

  for (DLListEntry entry : m_data->entries) {
    if (entry.type != UNUSED)
      ids.emplace_back(entry.high_title_id);
  }

  return ids;
}

u16 NWC24Dl::GetIndex(int pos) const {
  u16 ret{};
  int i{};

  for (DLListEntry entry : m_data->entries) {
    if (entry.type != UNUSED) {
      if (pos == i) {
        ret = entry.index;
        break;
      }

      i++;
    }
  }


  return ret;
}

u32 NWC24Dl::GetBitMask(u16 entry_index) const {
  return m_data->entries[entry_index].subtask_bitmask;
}

u32 NWC24Dl::GetFlags(u16 entry_index) const {
  return m_data->entries[entry_index].flags;
}

NWC24Dl::EntryType NWC24Dl::GetEntryType(u16 entry_index) const {
  return m_data->entries[entry_index].type;
}

