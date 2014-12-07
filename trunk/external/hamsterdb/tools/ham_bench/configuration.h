/*
 * Copyright (C) 2005-2014 Christoph Rupp (chris@crupp.de).
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HAM_BENCH_CONFIGURATION_H
#define HAM_BENCH_CONFIGURATION_H

#include <string>
#include <cstdio>
#include <iostream>
#include <ham/hamsterdb.h>
#include <boost/thread.hpp>
#include <boost/cstdint.hpp> // MSVC 2008 does not have stdint
using namespace boost;

struct Configuration
{
  enum {
    kKeyBinary = 0,
    kKeyString,
    kKeyCustom,
    kKeyUint8,
    kKeyUint16,
    kKeyUint32,
    kKeyUint64,
    kKeyReal32,
    kKeyReal64
  };

  enum {
    kFullcheckDefault = 0,
    kFullcheckFind,
    kFullcheckReverse,
    kFullcheckNone
  };

  enum {
    kDistributionRandom = 0,
    kDistributionAscending,
    kDistributionDescending,
    kDistributionZipfian
  };

  enum {
    kDuplicateDisabled = 0,
    kDuplicateFirst,
    kDuplicateLast
  };

  enum {
    kMetricsNone,
    kMetricsDefault,
    kMetricsPng,
    kMetricsAll
  };

  enum {
    kDefaultKeysize = 16,
    kDefaultRecsize = 1024
  };

  Configuration()
    : profile(true), verbose(0), no_progress(false), reopen(false), open(false),
      quiet(false), key_type(kKeyBinary),
      rec_size_fixed(HAM_RECORD_SIZE_UNLIMITED), force_records_inline(false),
      distribution(kDistributionRandom), seed(0), limit_ops(0),
      limit_seconds(0), limit_bytes(0), key_size(kDefaultKeysize),
      key_is_fixed_size(false), rec_size(kDefaultRecsize),
      erase_pct(0), find_pct(0), table_scan_pct(0), use_encryption(false),
      use_remote(false), duplicate(kDuplicateDisabled), overwrite(false),
      transactions_nth(0), use_fsync(false), inmemory(false),
      use_recovery(false), use_transactions(false), no_mmap(false),
      cacheunlimited(false), cachesize(0), hints(0), pagesize(0),
      num_threads(1), use_cursors(false), direct_access(false),
      use_berkeleydb(false), use_hamsterdb(true), fullcheck(kFullcheckDefault),
      fullcheck_frequency(1000), metrics(kMetricsDefault),
      extkey_threshold(0), duptable_threshold(0), bulk_erase(false),
      flush_txn_immediately(false), disable_recovery(false),
      journal_compression(0), record_compression(0), key_compression(0),
      read_only(false), enable_crc32(false), record_number(false) {
  }

  void print() const {
    static const char *compressors[] = {
      "none",
      "zlib",
      "snappy",
      "lzf",
      "lzo",
      "bitmap"
    };
    std::cout << "Configuration: --seed=" << seed << " ";
    if (journal_compression)
      std::cout << "--journal-compression=" << compressors[journal_compression]
          << " ";
    if (record_compression)
      std::cout << "--record-compression=" << compressors[record_compression]
          << " ";
    if (key_compression)
      std::cout << "--key-compression=" << compressors[key_compression]
          << " ";
    if (use_encryption)
      std::cout << "--use-encryption ";
    if (use_remote)
      std::cout << "--use-remote ";
    if (use_fsync)
      std::cout << "--use-fsync ";
    if (use_recovery)
      std::cout << "--use-recovery ";
    if (disable_recovery)
      std::cout << "--disable-recovery ";
    if (use_cursors)
      std::cout << "--use-cursors ";
    if (duplicate == kDuplicateFirst)
      std::cout << "--duplicate=first ";
    else if (duplicate == kDuplicateLast)
      std::cout << "--duplicate=last ";
    if (overwrite)
      std::cout << "--overwrite ";
    if (inmemory)
      std::cout << "--inmemorydb ";
    if (no_mmap)
      std::cout << "--no-mmap ";
    if (cacheunlimited)
      std::cout << "--cache=unlimited ";
    if (cachesize)
      std::cout << "--cache=" << cachesize << " ";
    if (pagesize)
      std::cout << "--pagesize=" << pagesize << " ";
    if (num_threads > 1)
      std::cout << "--num-threads=" << num_threads << " ";
    if (direct_access)
      std::cout << "--direct-access ";
    if (use_berkeleydb)
      std::cout << "--use-berkeleydb ";
    if (!use_hamsterdb)
      std::cout << "--use-hamsterdb=false ";
    if (bulk_erase)
      std::cout << "--bulk-erase ";
    if (flush_txn_immediately)
      std::cout << "--flush-txn-immediately ";
    if (use_transactions) {
      if (!transactions_nth)
        std::cout << "--use-transactions=tmp ";
      else if (transactions_nth == 0xffffffffu)
        std::cout << "--use-transactions=all ";
      else
        std::cout << "--use-transactions=" << transactions_nth << " ";
    }
    if (hints & HAM_HINT_APPEND)
      std::cout << "--hints=HAM_HINT_APPEND ";
    else if (hints & HAM_HINT_PREPEND)
      std::cout << "--hints=HAM_HINT_PREPEND ";
    if (fullcheck == kFullcheckFind)
      std::cout << "--fullcheck=find ";
    if (fullcheck == kFullcheckReverse)
      std::cout << "--fullcheck=reverse ";
    if (fullcheck == kFullcheckNone)
      std::cout << "--fullcheck=none ";
    if (extkey_threshold)
      std::cout << "--extkey-threshold=" << extkey_threshold << " ";
    if (duptable_threshold)
      std::cout << "--duptable-threshold=" << duptable_threshold << " ";
    if (enable_crc32)
      std::cout << "--enable-crc32 ";
    if (record_number)
      std::cout << "--record-number ";
    if (!filename.empty())
      std::cout << filename;
    else {
      if (key_type == kKeyCustom)
        std::cout << "--key=custom ";
      else if (key_type == kKeyUint8)
        std::cout << "--key=uint8 ";
      else if (key_type == kKeyUint16)
        std::cout << "--key=uint16 ";
      else if (key_type == kKeyUint32)
        std::cout << "--key=uint32 ";
      else if (key_type == kKeyUint64)
        std::cout << "--key=uint64 ";
      else if (key_type == kKeyReal32)
        std::cout << "--key=real32 ";
      else if (key_type == kKeyReal64)
        std::cout << "--key=real64 ";
      if (key_size != kDefaultKeysize)
        std::cout << "--keysize=" << key_size << " ";
      if (key_is_fixed_size)
        std::cout << "--keysize-fixed ";
      if (rec_size_fixed != HAM_RECORD_SIZE_UNLIMITED)
        std::cout << "--recsize-fixed=" << rec_size_fixed << " ";
      if (force_records_inline)
        std::cout << "--force-records-inline ";
      std::cout << "--recsize=" << rec_size << " ";
      if (distribution == kDistributionRandom)
        std::cout << "--distribution=random ";
      if (distribution == kDistributionAscending)
        std::cout << "--distribution=ascending ";
      if (distribution == kDistributionDescending)
        std::cout << "--distribution=descending ";
      if (distribution == kDistributionZipfian)
        std::cout << "--distribution=zipfian ";
      if (limit_ops)
        std::cout << "--stop-ops=" << limit_ops << " ";
      if (limit_seconds)
        std::cout << "--stop-seconds=" << limit_seconds << " ";
      if (limit_bytes)
        std::cout << "--stop-bytes=" << limit_bytes << " ";
      if (erase_pct)
        std::cout << "--erase-pct=" << erase_pct << " ";
      if (find_pct)
        std::cout << "--find-pct=" << find_pct << " ";
      if (table_scan_pct)
        std::cout << "--table-scan-pct=" << table_scan_pct << " ";
      if (read_only)
        std::cout << "--read-only ";
      std::cout << std::endl;
    }
  }

  bool profile;
  unsigned verbose;
  bool no_progress;
  bool reopen;
  bool open;
  std::string filename;
  bool quiet;
  int key_type;
  unsigned rec_size_fixed;
  bool force_records_inline;
  int distribution;
  long seed;
  uint64_t limit_ops;
  uint64_t limit_seconds;
  uint64_t limit_bytes;
  int key_size;
  bool key_is_fixed_size;
  int rec_size;
  int erase_pct;
  int find_pct;
  int table_scan_pct;
  bool use_encryption;
  bool use_remote;
  int duplicate;
  bool overwrite;
  uint32_t transactions_nth;
  bool use_fsync;
  bool inmemory;
  bool use_recovery;
  bool use_transactions;
  bool no_mmap;
  bool cacheunlimited;
  int cachesize;
  int hints;
  int pagesize;
  int num_threads;
  bool use_cursors;
  bool direct_access;
  bool use_berkeleydb;
  bool use_hamsterdb;
  int fullcheck;
  int fullcheck_frequency;
  std::string tee_file;
  int metrics;
  int extkey_threshold;
  int duptable_threshold;
  bool bulk_erase;
  bool flush_txn_immediately;
  bool disable_recovery;
  int journal_compression;
  int record_compression;
  int key_compression;
  bool read_only;
  bool enable_crc32;
  bool record_number;
};

#endif /* HAM_BENCH_CONFIGURATION_H */
