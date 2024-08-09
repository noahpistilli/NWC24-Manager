// Copyright 2022 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <memory>
#include <optional>
#include <string>
#include <gccore.h>
#include <vector>

class NWC24Dl final
{
public:
    explicit NWC24Dl();
    void ReadDlList();


    std::vector<std::string> GetDownloadURLs() const;
    std::vector<std::string> GetGameIDs() const;
    std::vector<u16> GetIndexes() const;
    u32 GetFlags(u16 entry_index);
    u32 GetBitMask(u16 entry_index);


    static constexpr u32 MAX_ENTRIES = 120;

private:
    static constexpr u32 DL_LIST_MAGIC = 0x5763446C;  // WcDl
    static constexpr u32 MAX_SUBENTRIES = 32;
    static constexpr u64 SECONDS_PER_MINUTE = 60;
    static constexpr u64 MINUTES_PER_DAY = 1440;

    enum EntryType : u8
    {
        SUBTASK = 1,
        MAIL,
        CHANNEL_CONTENT,
        UNUSED = 0xff
    };

#pragma pack(push, 1)
    // The following format is partially based off of https://wiibrew.org/wiki//dev/net/kd/request.
    struct DLListHeader final
    {
        u32 magic;    // 'WcDl' 0x5763446c
        u32 version;  // must be 1
        u32 unk1;
        u32 unk2;
        u16 max_subentries;  // Asserted to be less than max_entries
        u16 reserved_mailnum;
        u16 max_entries;
        u8 reserved[106];
    };

    struct DLListRecord final
    {
        u32 low_title_id;
        u32 next_dl_timestamp;
        u32 last_modified_timestamp;
        u8 flags;
        u8 padding[3];
    };

    struct DLListEntry final
    {
        u16 index;
        EntryType type;
        u8 record_flags;
        u32 flags;
        char high_title_id[4];
        u32 low_title_id;
        u32 unknown1;
        u16 group_id;
        u16 padding1;
        u16 remaining_downloads;
        u16 error_count;
        u16 dl_margin;
        u16 retry_frequency;
        s32 error_code;
        u8 subtask_id;
        u8 subtask_type;
        u16 subtask_flags;
        u32 subtask_bitmask;
        u32 server_dl_interval;
        u32 dl_timestamp;  // Last DL time
        u32 subtask_timestamps[32];
        char dl_url[236];
        char filename[64];
        u8 unk6[29];
        u8 should_use_rootca;
        u16 unknown3;
    };

    struct DLList final
    {
        DLListHeader header;
        DLListRecord records[MAX_ENTRIES];
        DLListEntry entries[MAX_ENTRIES];
    };
#pragma pack(pop)

    DLList m_data{};
};