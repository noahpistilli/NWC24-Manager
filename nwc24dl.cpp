//
// Created by Noah Pistilli on 2024-08-06.
//

#include "nwc24dl.h"
#include "utils.h"

constexpr const char CONFIG_PATH[] = "/shared2/wc24/nwc24dl.bin";

NWC24Dl::NWC24Dl()
{
  ReadDlList();
}

void NWC24Dl::ReadDlList()
{
  u32 readSize;
  void *fileBuffer = ISFS_GetFile(CONFIG_PATH, &readSize);
  m_data = *(reinterpret_cast<DLList*>(fileBuffer));
}

std::vector<std::string> NWC24Dl::GetDownloadURLs() const {
  std::vector<std::string> urls{};

  for (DLListEntry entry : m_data.entries) {
    if (entry.dl_url[0] != 0)
      urls.emplace_back(entry.dl_url);
  }

  return urls;
}

std::vector<std::string> NWC24Dl::GetGameIDs() const {
  std::vector<std::string> ids{};

  for (DLListEntry entry : m_data.entries) {
    if (entry.dl_url[0] != 0)
      ids.emplace_back(entry.high_title_id);
  }


  return ids;
}

std::vector<u16> NWC24Dl::GetIndexes() const {
  std::vector<u16> ids{};

  for (DLListEntry entry : m_data.entries) {
    if (entry.dl_url[0] != 0)
      ids.emplace_back(entry.index);
  }


  return ids;
}

u32 NWC24Dl::GetBitMask(u16 entry_index) {
  return m_data.entries[entry_index].subtask_bitmask;
}

u32 NWC24Dl::GetFlags(u16 entry_index) {
  return m_data.entries[entry_index].flags;
}