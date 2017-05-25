//  Copyright (c) 2017-present, Intel Corporation.  All rights reserved.
//  This source code is licensed under the BSD-style license found in the
//  LICENSE file in the root directory of this source tree. An additional grant
//  of patent rights can be found in the PATENTS file in the same directory.

#ifndef _KV_DB_GCMANAGER_H_
#define _KV_DB_GCMANAGER_H_

#include <sys/types.h>
#include <mutex>

#include "Db_Structure.h"
#include "BlockDevice.h"
#include "hyperds/Options.h"
#include "IndexManager.h"
#include "SegmentManager.h"
#include "DataHandle.h"

namespace kvdb {
class GCSeg : public SegBuffer {
public:
    GCSeg();
    ~GCSeg();
    GCSeg(const GCSeg& toBeCopied);
    GCSeg& operator=(const GCSeg& toBeCopied);

    GCSeg(SegmentManager* sm, IndexManager* im, BlockDevice* bdev);

    bool UpdateToIndex();
private:
    IndexManager* idxMgr_;
};

class GcManager {
public:
    ~GcManager();
    GcManager(BlockDevice* bdev, IndexManager* im, SegmentManager* sm,
              Options &opt);

    bool ForeGC();
    void BackGC();
    void FullGC();

private:
    uint32_t doMerge(std::multimap<uint32_t, uint32_t> &cands_map);

    bool readSegment(uint64_t seg_offset);
    void loadSegKV(list<KVSlice*> &slice_list, uint32_t num_keys,
                   uint64_t phy_offset);

    bool loadKvList(uint32_t seg_id, std::list<KVSlice*> &slice_list);
    void cleanKvList(std::list<KVSlice*> &slice_list);

private:
    SegmentManager* segMgr_;
    IndexManager* idxMgr_;
    BlockDevice* bdev_;
    Options &options_;

    std::mutex gcMtx_;

    char *dataBuf_;
};

}//namespace kvdb

#endif //#ifndef _KV_DB_GCMANAGER_H_
