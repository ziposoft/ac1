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

#include "3rdparty/catch/catch.hpp"

#include "utils.h"
#include "os.hpp"

#include "2page/page.h"
#include "4db/db_local.h"
#include "4env/env.h"
#include "4env/env_header.h"
#include "4env/env_local.h"

using namespace hamsterdb;

struct EnvFixture {
  uint32_t m_flags;

  EnvFixture(uint32_t flags = 0)
    : m_flags(flags) {
    os::unlink(Utils::opath(".test"));
  }

  void createCloseTest() {
    ham_env_t *env;

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));
    REQUIRE(HAM_INV_PARAMETER == ham_env_close(0, 0));
    REQUIRE(0 == ham_env_close(env, 0));
  }

  void createCloseOpenCloseTest() {
    ham_env_t *env;

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));
    REQUIRE(0 == ham_env_close(env, 0));

    if (!(m_flags & HAM_IN_MEMORY)) {
      REQUIRE(0 == ham_env_open(&env, Utils::opath(".test"), 0, 0));
      REQUIRE(0 == ham_env_close(env, 0));
    }
  }

  void createCloseOpenCloseWithDatabasesTest() {
    ham_env_t *env;
    ham_db_t *db, *db2;

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));
    REQUIRE(HAM_INV_PARAMETER ==
        ham_env_create_db(0, &db, 333, 0, 0));
    REQUIRE(HAM_INV_PARAMETER ==
        ham_env_create_db(env, 0, 333, 0, 0));
    REQUIRE(0 == ham_env_create_db(env, &db, 333, 0, 0));
    REQUIRE(HAM_DATABASE_ALREADY_EXISTS ==
        ham_env_create_db(env, &db2, 333, 0, 0));
    REQUIRE(0 == ham_db_close(db, 0));

    REQUIRE(HAM_INV_PARAMETER ==
        ham_env_open_db(0, &db, 333, 0, 0));
    REQUIRE(HAM_INV_PARAMETER ==
        ham_env_open_db(env, 0, 333, 0, 0));

    if (!(m_flags & HAM_IN_MEMORY)) {
      REQUIRE(0 == ham_env_open_db(env, &db, 333, 0, 0));
      REQUIRE(HAM_DATABASE_ALREADY_OPEN ==
          ham_env_open_db(env, &db, 333, 0, 0));
      REQUIRE(0 == ham_env_close(env, HAM_AUTO_CLEANUP));

      REQUIRE(0 == ham_env_open(&env, Utils::opath(".test"), 0, 0));

      REQUIRE(0 == ham_env_open_db(env, &db, 333, 0, 0));
      REQUIRE(0 == ham_db_close(db, 0));
    }
    REQUIRE(0 == ham_env_close(env, HAM_AUTO_CLEANUP));
  }

  /*
   * Create and ENV using EXtended parameters, than close it.
   *
   * Open the ENV again and check the parameters before and after
   * creating the first database.
   */
  void createCloseEmptyOpenCloseWithDatabasesTest() {
    ham_env_t *env;
    ham_db_t *db[128], *dbx;
    int i;
    const ham_parameter_t parameters[] = {
       { HAM_PARAM_CACHESIZE, 128 * 1024 },
       { HAM_PARAM_PAGESIZE, 64 * 1024 },
       { 0, 0 }
    };
    const ham_parameter_t parameters2[] = {
       { HAM_PARAM_CACHESIZE, 128 * 1024 },
       { 0, 0 }
    };

    ham_parameter_t ps[]={
       { HAM_PARAM_CACHESIZE,0},
       { HAM_PARAM_PAGESIZE, 0},
       { HAM_PARAM_MAX_DATABASES, 0},
       { 0, 0 }
    };

    REQUIRE(0 ==
      ham_env_create(&env, Utils::opath(".test"),
        m_flags, 0664, parameters));
    REQUIRE(0 == ham_env_get_parameters(env, ps));
    REQUIRE((uint64_t)(128 * 1024u) == ps[0].value);
    REQUIRE((uint64_t)(64 * 1024u) == ps[1].value);
    REQUIRE((uint64_t)2724u == ps[2].value);

    /* close and re-open the ENV */
    if (!(m_flags & HAM_IN_MEMORY)) {
      REQUIRE(0 == ham_env_close(env, 0));

      REQUIRE(0 ==
        ham_env_open(&env, Utils::opath(".test"), m_flags, parameters2));
    }

    REQUIRE(0 == ham_env_get_parameters(env, ps));
    REQUIRE((uint64_t)(128 * 1024u) == ps[0].value);
    REQUIRE((uint64_t)(1024 * 64u) == ps[1].value);
    REQUIRE(2724ull == ps[2].value);

    /* now create 128 DBs; we said we would, anyway, when creating the
     * ENV ! */
    for (i = 0; i < 128; i++) {
      int j;

      REQUIRE(0 ==
          ham_env_create_db(env, &db[i], i + 100, 0, 0));
      REQUIRE(HAM_DATABASE_ALREADY_EXISTS ==
          ham_env_create_db(env, &dbx, i + 100, 0, 0));
      REQUIRE(0 == ham_db_close(db[i], 0));
      REQUIRE(0 ==
          ham_env_open_db(env, &db[i], i + 100, 0, 0));

      for (j = 0; ps[j].name; j++)
        ps[j].value = 0;
    }

    for (i = 0; i < 128; i++)
      REQUIRE(0 == ham_db_close(db[i], 0));

    REQUIRE(0 == ham_env_close(env, 0));
  }

  void autoCleanupTest(void)
  {
    ham_env_t *env;
    ham_db_t *db[3];
    ham_cursor_t *c[5];

    REQUIRE(0 ==
      ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));
    for (int i = 0; i < 3; i++)
      REQUIRE(0 == ham_env_create_db(env, &db[i], i+1, 0, 0));
    for (int i = 0; i < 5; i++)
      REQUIRE(0 == ham_cursor_create(&c[i], db[0], 0, 0));

    REQUIRE(0 == ham_env_close(env, HAM_AUTO_CLEANUP));
  }

  void autoCleanup2Test() {
    ham_env_t *env;
    ham_db_t *db;

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));
    REQUIRE(0 == ham_env_create_db(env, &db, 1, 0, 0));

    REQUIRE(0 == ham_env_close(env, HAM_AUTO_CLEANUP));
  }

  void readOnlyTest() {
    ham_db_t *db, *db2;
    ham_env_t *env;
    ham_key_t key;
    ham_record_t rec;
    ham_cursor_t *cursor;
    ::memset(&key, 0, sizeof(key));
    ::memset(&rec, 0, sizeof(rec));

    REQUIRE(0 == ham_env_create(&env, Utils::opath(".test"), 0, 0664, 0));
    REQUIRE(0 == ham_env_create_db(env, &db, 333, 0, 0));
    REQUIRE(0 == ham_db_close(db, 0));
    REQUIRE(0 == ham_env_close(env, 0));

    REQUIRE(0 == ham_env_open(&env, Utils::opath(".test"), HAM_READ_ONLY, 0));
    REQUIRE(0 == ham_env_open_db(env, &db, 333, 0, 0));

    REQUIRE(0 == ham_cursor_create(&cursor, db, 0, 0));
    REQUIRE(HAM_DATABASE_ALREADY_OPEN ==
        ham_env_open_db(env, &db2, 333, 0, 0));
    REQUIRE(HAM_WRITE_PROTECTED ==
        ham_env_create_db(env, &db2, 444, 0, 0));

    REQUIRE(HAM_WRITE_PROTECTED ==
        ham_db_insert(db, 0, &key, &rec, 0));
    REQUIRE(HAM_WRITE_PROTECTED ==
        ham_db_erase(db, 0, &key, 0));
    REQUIRE(HAM_WRITE_PROTECTED ==
        ham_cursor_overwrite(cursor, &rec, 0));
    REQUIRE(HAM_WRITE_PROTECTED ==
        ham_cursor_insert(cursor, &key, &rec, 0));
    REQUIRE(HAM_WRITE_PROTECTED ==
        ham_cursor_erase(cursor, 0));

    REQUIRE(0 == ham_cursor_close(cursor));
    REQUIRE(0 == ham_db_close(db, 0));
    REQUIRE(0 == ham_env_close(env, 0));
  }

  void createPagesizeReopenTest() {
    ham_env_t *env;
    ham_parameter_t ps[] = { { HAM_PARAM_PAGESIZE, 1024 * 128 }, { 0, 0 } };

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0644, &ps[0]));
    if (!(m_flags & HAM_IN_MEMORY)) {
      REQUIRE(0 == ham_env_close(env, 0));
      REQUIRE(0 ==
          ham_env_open(&env, Utils::opath(".test"), m_flags, 0));
    }
    REQUIRE(0 == ham_env_close(env, 0));
  }

  void openFailCloseTest() {
    ham_env_t *env;

    REQUIRE(HAM_FILE_NOT_FOUND ==
        ham_env_open(&env, "xxxxxx...", 0, 0));
    REQUIRE((ham_env_t *)0 == env);
  }

  void openWithKeysizeTest() {
    ham_env_t *env;

    REQUIRE(HAM_INV_PARAMETER ==
        ham_env_open(0, Utils::opath(".test"), m_flags, 0));
    REQUIRE(HAM_FILE_NOT_FOUND ==
        ham_env_open(&env, Utils::opath(".test"), m_flags, 0));
  }

  void createDbWithKeysizeTest() {
    ham_env_t *env;
    ham_db_t *db;
    ham_parameter_t parameters2[] = {
       { HAM_PARAM_KEYSIZE, (uint64_t)64 },
       { 0, 0ull }
    };

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0644, 0));

    REQUIRE(0 ==
        ham_env_create_db(env, &db, 333, 0, parameters2));
    REQUIRE((uint16_t)64 == ((LocalDatabase *)db)->get_config().key_size);
    REQUIRE(0 == ham_db_close(db, 0));
    REQUIRE(0 == ham_env_close(env, 0));
  }

  // check to make sure both create and open_ex support accessing more
  // than DB_MAX_INDICES DBs in one env:
  void createAndOpenMultiDbTest() {
#undef MAX
#define MAX 256
    ham_env_t *env;
    ham_db_t *db[MAX];
    int i;
    ham_key_t key;
    ham_record_t rec;
    ham_parameter_t parameters[] = {
       { HAM_PARAM_KEYSIZE, 20 },
       { 0, 0 }
    };

    ham_parameter_t parameters2[] = {
       { HAM_PARAM_CACHESIZE, 1024 * 128 },
       { HAM_PARAM_PAGESIZE, 1024 * 4 },
       { 0, 0 }
    };

    ham_parameter_t parameters3[] = {
       { HAM_PARAM_CACHESIZE, 1024 * 128 },
       { 0, 0 }
    };

    if (m_flags & HAM_IN_MEMORY) {
      REQUIRE(HAM_INV_PARAMETER ==
        ham_env_create(&env, Utils::opath(".test"),
            m_flags, 0644, parameters2));
      parameters2[1].value = 0; // page_size := 0
    }
    else {
      REQUIRE(HAM_INV_PARAMETER ==
        ham_env_create(&env, Utils::opath(".test"),
          m_flags | HAM_CACHE_UNLIMITED, 0644, parameters2));
      parameters2[1].value = 65536; // page_size := 64K
    }

    if (m_flags & HAM_IN_MEMORY)
      parameters2[0].value = 0; // cache_size := 0

    REQUIRE(0 ==
      ham_env_create(&env, Utils::opath(".test"), m_flags, 0644, parameters2));

    char buffer[20] = {0};

    // create DBs
    for (i = 0; i < MAX; i++) {
      REQUIRE(0 ==
          ham_env_create_db(env, &db[i], i + 1, 0, parameters));
      memset(&key, 0, sizeof(key));
      memset(&rec, 0, sizeof(rec));
      *(int *)&buffer[0] = i;
      key.data = &buffer[0];
      key.size = sizeof(buffer);
      rec.data = &buffer[0];
      rec.size = sizeof(buffer);
      REQUIRE(0 == ham_db_insert(db[i], 0, &key, &rec, 0));
      if (!(m_flags & HAM_IN_MEMORY))
        REQUIRE(0 == ham_db_close(db[i], 0));
    }

    if (!(m_flags & HAM_IN_MEMORY)) {
      REQUIRE(0 == ham_env_close(env, 0));

      // open DBs
      // page_size param not allowed
      REQUIRE(HAM_INV_PARAMETER ==
        ham_env_open(&env, Utils::opath(".test"), m_flags, parameters2));
      REQUIRE(0 ==
        ham_env_open(&env, Utils::opath(".test"), m_flags, parameters3));
      // key_size param not allowed
      REQUIRE(HAM_INV_PARAMETER ==
        ham_env_open_db(env, &db[0], 1, 0, parameters));
    }

    for (i = 0; i < MAX; i++) {
      if (!(m_flags & HAM_IN_MEMORY)) {
        REQUIRE(0 ==
            ham_env_open_db(env, &db[i], i + 1, 0, 0));
      }
      memset(&key, 0, sizeof(key));
      memset(&rec, 0, sizeof(rec));
      *(int *)&buffer[0] = i;
      key.data = &buffer[0];
      key.size = sizeof(buffer);
      REQUIRE(0 == ham_db_find(db[i], 0, &key, &rec, 0));
      REQUIRE(*(int *)key.data == i);
      REQUIRE(*(int *)rec.data == i);
      REQUIRE(0 == ham_db_close(db[i], 0));
    }

    REQUIRE(0 == ham_env_close(env, 0));
  }

  void multiDbTest() {
    int i;
    ham_env_t *env;
    ham_db_t *db[10];

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));

    for (i = 0; i < 10; i++) {
      REQUIRE(0 == ham_env_create_db(env, &db[i],
            (uint16_t)i + 1, 0, 0));
      REQUIRE(0 == ham_db_close(db[i], 0));
      REQUIRE(0 == ham_env_open_db(env, &db[i],
            (uint16_t)i + 1, 0, 0));
      REQUIRE(0 == ham_db_close(db[i], 0));
    }

    for (i = 0; i < 10; i++) {
      REQUIRE(0 == ham_env_open_db(env, &db[i],
            (uint16_t)i + 1, 0, 0));
      REQUIRE(0 == ham_db_close(db[i], 0));
    }

    REQUIRE(0 == ham_env_close(env, 0));
  }

  void multiDbTest2() {
    int i;
    ham_env_t *env;
    ham_db_t *db[10];

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));

    for (i = 0; i < 10; i++)
      REQUIRE(0 == ham_env_create_db(env, &db[i],
            (uint16_t)i + 1, 0, 0));

    for (i = 0; i < 10; i++)
      REQUIRE(0 == ham_db_close(db[i], 0));

    if (!(m_flags & HAM_IN_MEMORY)) {
      for (i = 0; i < 10; i++) {
        REQUIRE(0 == ham_env_open_db(env, &db[i],
              (uint16_t)i + 1, 0, 0));
        REQUIRE(0 == ham_db_close(db[i], 0));
      }
    }

    REQUIRE(0 == ham_env_close(env, 0));
  }

  void multiDbInsertFindTest() {
    int i;
    const int MAX_DB = 5;
    const int MAX_ITEMS = 300;
    ham_env_t *env;
    ham_db_t *db[MAX_DB];
    ham_record_t rec;
    ham_key_t key;

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));

    for (i = 0; i < MAX_DB; i++) {
      REQUIRE(0 == ham_env_create_db(env, &db[i],
            (uint16_t)i + 1, 0, 0));

      for (int j = 0; j < MAX_ITEMS; j++) {
        int value = j * (i + 1);
        memset(&key, 0, sizeof(key));
        memset(&rec, 0, sizeof(rec));
        key.data = &value;
        key.size = sizeof(value);
        rec.data = &value;
        rec.size = sizeof(value);

        REQUIRE(0 == ham_db_insert(db[i], 0, &key, &rec, 0));
      }
    }

    for (i = 0; i < MAX_DB; i++) {
      for (int j = 0; j < MAX_ITEMS; j++) {
        int value = j * (i + 1);
        memset(&key, 0, sizeof(key));
        memset(&rec, 0, sizeof(rec));
        key.data = (void *)&value;
        key.size = sizeof(value);

        REQUIRE(0 == ham_db_find(db[i], 0, &key, &rec, 0));
        REQUIRE(value == *(int *)key.data);
        REQUIRE((uint16_t)sizeof(value) == key.size);
      }
    }

    if (!(m_flags & HAM_IN_MEMORY)) {
      for (i = 0; i < MAX_DB; i++) {
        REQUIRE(0 == ham_db_close(db[i], 0));
        REQUIRE(0 == ham_env_open_db(env, &db[i],
              (uint16_t)i + 1, 0, 0));
        for (int j = 0; j < MAX_ITEMS; j++) {
          int value = j * (i + 1);
          memset(&key, 0, sizeof(key));
          memset(&rec, 0, sizeof(rec));
          key.data = (void *)&value;
          key.size = sizeof(value);

          REQUIRE(0 == ham_db_find(db[i], 0, &key, &rec, 0));
          REQUIRE(value == *(int *)key.data);
          REQUIRE((uint16_t)sizeof(value) == key.size);
        }
      }
    }

    for (i = 0; i < MAX_DB; i++)
      REQUIRE(0 == ham_db_close(db[i], 0));

    REQUIRE(0 == ham_env_close(env, 0));
  }

  void multiDbInsertFindExtendedTest() {
    int i;
    const int MAX_DB = 5;
    const int MAX_ITEMS = 300;
    ham_env_t *env;
    ham_db_t *db[MAX_DB];
    ham_record_t rec;
    ham_key_t key;
    char buffer[512];

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));

    for (i = 0; i < MAX_DB; i++) {
      REQUIRE(0 == ham_env_create_db(env, &db[i], (uint16_t)i + 1, 0, 0));

      for (int j = 0; j < MAX_ITEMS; j++) {
        int value = j * (i + 1);
        memset(&key, 0, sizeof(key));
        memset(&rec, 0, sizeof(rec));
        memset(buffer, (char)value, sizeof(buffer));
        key.data = buffer;
        key.size = sizeof(buffer);
        rec.data = buffer;
        rec.size = sizeof(buffer);
        sprintf(buffer, "%08x%08x", j, i + 1);

        REQUIRE(0 == ham_db_insert(db[i], 0, &key, &rec, 0));
      }
    }

    for (i = 0; i < MAX_DB; i++) {
      for (int j = 0; j < MAX_ITEMS; j++) {
        int value = j * (i + 1);
        memset(&key, 0, sizeof(key));
        memset(&rec, 0, sizeof(rec));
        memset(buffer, (char)value, sizeof(buffer));
        key.data = buffer;
        key.size = sizeof(buffer);
        sprintf(buffer, "%08x%08x", j, i+1);

        REQUIRE(0 == ham_db_find(db[i], 0, &key, &rec, 0));
        REQUIRE((uint32_t)sizeof(buffer) == rec.size);
        REQUIRE(0 == memcmp(buffer, rec.data, rec.size));
      }
    }

    if (!(m_flags & HAM_IN_MEMORY)) {
      for (i = 0; i < MAX_DB; i++) {
        REQUIRE(0 == ham_db_close(db[i], 0));
        REQUIRE(0 == ham_env_open_db(env, &db[i],
              (uint16_t)i + 1, 0, 0));
        for (int j = 0; j < MAX_ITEMS; j++) {
          int value = j * (i + 1);
          memset(&key, 0, sizeof(key));
          memset(&rec, 0, sizeof(rec));
          memset(buffer, (char)value, sizeof(buffer));
          key.data = buffer;
          key.size = sizeof(buffer);
          sprintf(buffer, "%08x%08x", j, i + 1);

          REQUIRE(0 == ham_db_find(db[i], 0, &key, &rec, 0));
          REQUIRE((uint32_t)sizeof(buffer) == rec.size);
          REQUIRE(0 == memcmp(buffer, rec.data, rec.size));
        }
      }
    }

    for (i = 0; i < MAX_DB; i++)
      REQUIRE(0 == ham_db_close(db[i], 0));
    REQUIRE(0 == ham_env_close(env, 0));
  }

  void multiDbInsertFindExtendedEraseTest() {
    int i;
    const int MAX_DB = 5;
    const int MAX_ITEMS = 300;
    ham_env_t *env;
    ham_db_t *db[MAX_DB];
    ham_record_t rec;
    ham_key_t key;
    char buffer[512];

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));

    for (i = 0; i < MAX_DB; i++) {
      REQUIRE(0 == ham_env_create_db(env, &db[i], (uint16_t)i + 1, 0, 0));

      for (int j = 0; j < MAX_ITEMS; j++) {
        int value = j * (i + 1);
        memset(&key, 0, sizeof(key));
        memset(&rec, 0, sizeof(rec));
        memset(buffer, (char)value, sizeof(buffer));
        key.data = buffer;
        key.size = sizeof(buffer);
        rec.data = buffer;
        rec.size = sizeof(buffer);
        sprintf(buffer, "%08x%08x", j, i+1);

        REQUIRE(0 == ham_db_insert(db[i], 0, &key, &rec, 0));
      }
    }

    for (i = 0; i < MAX_DB; i++) {
      for (int j = 0; j < MAX_ITEMS; j++) {
        int value = j * (i + 1);
        memset(&key, 0, sizeof(key));
        memset(&rec, 0, sizeof(rec));
        memset(buffer, (char)value, sizeof(buffer));
        key.data = buffer;
        key.size = sizeof(buffer);
        sprintf(buffer, "%08x%08x", j, i+1);

        REQUIRE(0 == ham_db_find(db[i], 0, &key, &rec, 0));
        REQUIRE((uint32_t)sizeof(buffer) == rec.size);
        REQUIRE(0 == memcmp(buffer, rec.data, rec.size));
      }
    }

    for (i = 0; i < MAX_DB; i++) {
      for (int j = 0; j < MAX_ITEMS; j += 2) { // delete every 2nd entry
        int value = j * (i + 1);
        memset(&key, 0, sizeof(key));
        memset(&rec, 0, sizeof(rec));
        memset(buffer, (char)value, sizeof(buffer));
        key.data = buffer;
        key.size = sizeof(buffer);
        sprintf(buffer, "%08x%08x", j, i+1);

        REQUIRE(0 == ham_db_erase(db[i], 0, &key, 0));
      }
    }

    if (!(m_flags & HAM_IN_MEMORY)) {
      for (i = 0; i < MAX_DB; i++) {
        REQUIRE(0 == ham_db_close(db[i], 0));
        REQUIRE(0 == ham_env_open_db(env, &db[i],
              (uint16_t)i + 1, 0, 0));
        for (int j = 0; j < MAX_ITEMS; j++) {
          int value = j * (i + 1);
          memset(&key, 0, sizeof(key));
          memset(&rec, 0, sizeof(rec));
          memset(buffer, (char)value, sizeof(buffer));
          key.data = buffer;
          key.size = sizeof(buffer);
          sprintf(buffer, "%08x%08x", j, i+1);

          if (j & 1) { // must exist
            REQUIRE(0 ==
                ham_db_find(db[i], 0, &key, &rec, 0));
            REQUIRE((uint32_t)sizeof(buffer) == rec.size);
            REQUIRE(0 == memcmp(buffer, rec.data, rec.size));
          }
          else { // was deleted
            REQUIRE(HAM_KEY_NOT_FOUND ==
                ham_db_find(db[i], 0, &key, &rec, 0));
          }
        }
      }
    }

    for (i = 0; i < MAX_DB; i++)
      REQUIRE(0 == ham_db_close(db[i], 0));

    REQUIRE(0 == ham_env_close(env, 0));
  }

  void multiDbInsertCursorTest() {
    int i;
    const int MAX_DB = 5;
    const int MAX_ITEMS = 300;
    ham_env_t *env;
    ham_db_t *db[MAX_DB];
    ham_cursor_t *cursor[MAX_DB];
    ham_record_t rec;
    ham_key_t key;
    char buffer[512];

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));

    for (i = 0; i < MAX_DB; i++) {
      REQUIRE(0 == ham_env_create_db(env, &db[i],
            (uint16_t)i + 1, 0, 0));
      REQUIRE(0 == ham_cursor_create(&cursor[i], db[i], 0, 0));

      for (int j = 0; j < MAX_ITEMS; j++) {
        memset(&key, 0, sizeof(key));
        memset(&rec, 0, sizeof(rec));
        sprintf(buffer, "%08x%08x", j, i+1);
        key.data = buffer;
        key.size = (uint16_t)strlen(buffer) + 1;
        rec.data = buffer;
        rec.size = (uint16_t)strlen(buffer) + 1;

        REQUIRE(0 == ham_cursor_insert(cursor[i], &key, &rec, 0));
      }
    }

    for (i = 0; i < MAX_DB; i++) {
      memset(&key, 0, sizeof(key));
      memset(&rec, 0, sizeof(rec));

      REQUIRE(0 == ham_cursor_move(cursor[i], &key, &rec, HAM_CURSOR_FIRST));
      sprintf(buffer, "%08x%08x", 0, i+1);
      REQUIRE((uint32_t)(strlen(buffer) + 1) == rec.size);
      REQUIRE(0 == strcmp(buffer, (char *)rec.data));

      for (int j = 1; j < MAX_ITEMS; j++) {
        REQUIRE(0 == ham_cursor_move(cursor[i], &key, &rec, HAM_CURSOR_NEXT));
        sprintf(buffer, "%08x%08x", j, i+1);
        REQUIRE((uint32_t)(strlen(buffer) + 1) == rec.size);
        REQUIRE(0 == strcmp(buffer, (char *)rec.data));
      }
    }

    for (i = 0; i < MAX_DB; i++) {
      for (int j = 0; j < MAX_ITEMS; j += 2) { // delete every 2nd entry
        memset(&key, 0, sizeof(key));
        memset(&rec, 0, sizeof(rec));
        sprintf(buffer, "%08x%08x", j, i + 1);
        key.data = buffer;
        key.size = (uint16_t)strlen(buffer) + 1;

        REQUIRE(0 == ham_cursor_find(cursor[i], &key, 0, 0));
        REQUIRE(0 == ham_cursor_erase(cursor[i], 0));
      }
    }

    if (!(m_flags & HAM_IN_MEMORY)) {
      for (i = 0; i < MAX_DB; i++) {
        REQUIRE(0 == ham_cursor_close(cursor[i]));
        REQUIRE(0 == ham_db_close(db[i], 0));
        REQUIRE(0 == ham_env_open_db(env, &db[i],
              (uint16_t)i + 1, 0, 0));
        REQUIRE(0 == ham_cursor_create(&cursor[i], db[i], 0, 0));
        for (int j = 0; j < MAX_ITEMS; j++) {
          memset(&key, 0, sizeof(key));
          memset(&rec, 0, sizeof(rec));
          sprintf(buffer, "%08x%08x", j, i + 1);
          key.data = buffer;
          key.size = (uint16_t)strlen(buffer) + 1;

          if (j & 1) { // must exist
            REQUIRE(0 ==
                ham_cursor_find(cursor[i], &key, 0, 0));
            REQUIRE(0 ==
                ham_cursor_move(cursor[i], 0, &rec, 0));
            REQUIRE((uint32_t)(strlen(buffer) + 1) == rec.size);
            REQUIRE(0 == strcmp(buffer, (char *)rec.data));
          }
          else { // was deleted
            REQUIRE(HAM_KEY_NOT_FOUND ==
                ham_cursor_find(cursor[i], &key, 0, 0));
          }
        }
      }
    }

    for (i = 0; i < MAX_DB; i++) {
      REQUIRE(0 == ham_cursor_close(cursor[i]));
      REQUIRE(0 == ham_db_close(db[i], 0));
    }

    REQUIRE(0 == ham_env_close(env, 0));
  }

  void multiDbInsertFindExtendedCloseReopenTest() {
    int i;
    const int MAX_DB = 5;
    const int MAX_ITEMS = 300;
    ham_env_t *env;
    ham_db_t *db[MAX_DB];
    ham_record_t rec;
    ham_key_t key;
    char buffer[512];

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));

    for (i = 0; i < MAX_DB; i++)
      REQUIRE(0 == ham_env_create_db(env, &db[i], (uint16_t)i + 1, 0, 0));

    for (i = 0; i < MAX_DB; i++) {
      for (int j = 0; j < MAX_ITEMS; j++) {
        int value = j * (i + 1);
        memset(&key, 0, sizeof(key));
        memset(&rec, 0, sizeof(rec));
        memset(buffer, (char)value, sizeof(buffer));
        key.data = buffer;
        key.size = sizeof(buffer);
        rec.data = buffer;
        rec.size = sizeof(buffer);
        sprintf(buffer, "%08x%08x", j, i+1);

        REQUIRE(0 == ham_db_insert(db[i], 0, &key, &rec, 0));
      }
      if (!(m_flags & HAM_IN_MEMORY))
        REQUIRE(0 == ham_db_close(db[i], 0));
    }

    if (!(m_flags & HAM_IN_MEMORY)) {
      REQUIRE(0 == ham_env_close(env, 0));
      REQUIRE(0 == ham_env_open(&env, Utils::opath(".test"), m_flags, 0));
    }

    for (i = 0; i < MAX_DB; i++) {
      if (!(m_flags & HAM_IN_MEMORY)) {
        REQUIRE(0 == ham_env_open_db(env, &db[i],
              (uint16_t)i + 1, 0, 0));
      }
      for (int j = 0; j < MAX_ITEMS; j++) {
        int value = j * (i + 1);
        memset(&key, 0, sizeof(key));
        memset(&rec, 0, sizeof(rec));
        memset(buffer, (char)value, sizeof(buffer));
        key.data = buffer;
        key.size = sizeof(buffer);
        sprintf(buffer, "%08x%08x", j, i+1);

		REQUIRE(0 == ham_db_find(db[i], 0, &key, &rec, 0));
        REQUIRE((uint32_t)sizeof(buffer) == rec.size);
        REQUIRE(0 == memcmp(buffer, rec.data, rec.size));
      }
    }

    for (i = 0; i < MAX_DB; i++)
      REQUIRE(0 == ham_db_close(db[i], 0));

    REQUIRE(0 == ham_env_close(env, 0));
  }

  void renameOpenDatabases() {
    int i;
    const int MAX_DB = 10;
    ham_env_t *env;
    ham_db_t *db[MAX_DB];

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));

    for (i = 0; i < MAX_DB; i++)
      REQUIRE(0 == ham_env_create_db(env, &db[i],
            (uint16_t)i+1, 0, 0));

    REQUIRE(HAM_INV_PARAMETER ==
        ham_env_rename_db(0, 1, 2, 0));
    REQUIRE(HAM_INV_PARAMETER ==
        ham_env_rename_db(env, 0, 2, 0));
    REQUIRE(HAM_INV_PARAMETER ==
        ham_env_rename_db(env, 1, 0, 0));
    REQUIRE(HAM_INV_PARAMETER ==
        ham_env_rename_db(env, 1, 0xffff, 0));
    REQUIRE(0 ==
        ham_env_rename_db(env, 1, 1, 0));
    REQUIRE(HAM_DATABASE_ALREADY_EXISTS ==
        ham_env_rename_db(env, 1, 5, 0));
    REQUIRE(HAM_DATABASE_NOT_FOUND ==
        ham_env_rename_db(env, 1000, 20, 0));

    for (i = 0; i < MAX_DB; i++) {
      REQUIRE(0 == ham_env_rename_db(env,
            (uint16_t)i + 1, (uint16_t)i + 1000, 0));
      REQUIRE(0 == ham_db_close(db[i], 0));
    }

    if (!(m_flags & HAM_IN_MEMORY)) {
      for (i = 0; i < MAX_DB; i++) {
        REQUIRE(0 == ham_env_open_db(env, &db[i],
              (uint16_t)i+1000, 0, 0));
      }

      for (i = 0; i < MAX_DB; i++)
        REQUIRE(0 == ham_db_close(db[i], 0));
    }

    REQUIRE(0 == ham_env_close(env, 0));
  }

  void renameClosedDatabases() {
    int i;
    const int MAX_DB = 10;
    ham_env_t *env;
    ham_db_t *db[MAX_DB];

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));

    for (i = 0; i < MAX_DB; i++) {
      REQUIRE(0 == ham_env_create_db(env, &db[i],
            (uint16_t)i + 1, 0, 0));
      REQUIRE(0 == ham_db_close(db[i], 0));
    }

    for (i = 0; i < MAX_DB; i++) {
      REQUIRE(0 == ham_env_rename_db(env,
            (uint16_t)i + 1, (uint16_t)i + 1000, 0));
    }

    for (i = 0; i < MAX_DB; i++) {
      REQUIRE(0 == ham_env_open_db(env, &db[i],
            (uint16_t)i + 1000, 0, 0));
      REQUIRE(0 == ham_db_close(db[i], 0));
    }

    REQUIRE(0 == ham_env_close(env, 0));
  }

  void eraseOpenDatabases() {
    int i;
    const int MAX_DB = 1;
    ham_env_t *env;
    ham_db_t *db[MAX_DB];

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));

    for (i = 0; i < MAX_DB; i++) {
      REQUIRE(0 == ham_env_create_db(env, &db[i],
            (uint16_t)i + 1, 0, 0));
    }

    REQUIRE(HAM_INV_PARAMETER ==
            ham_env_erase_db(0, (uint16_t)i + 1, 0));
    REQUIRE(HAM_INV_PARAMETER ==
            ham_env_erase_db(env, 0, 0));

    for (i = 0; i < MAX_DB; i++) {
      REQUIRE(HAM_DATABASE_ALREADY_OPEN ==
              ham_env_erase_db(env, (uint16_t)i + 1, 0));
      REQUIRE(0 == ham_db_close(db[i], 0));
      if (m_flags & HAM_IN_MEMORY) {
        REQUIRE(HAM_DATABASE_NOT_FOUND ==
            ham_env_erase_db(env, (uint16_t)i + 1, 0));
      }
      else {
        REQUIRE(0 ==
            ham_env_erase_db(env, (uint16_t)i + 1, 0));
      }
    }

    REQUIRE(0 == ham_env_close(env, 0));
  }

  void eraseUnknownDatabases() {
    int i;
    const int MAX_DB = 1;
    ham_env_t *env;
    ham_db_t *db[MAX_DB];

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));

    for (i = 0; i < MAX_DB; i++) {
      REQUIRE(0 == ham_env_create_db(env, &db[i],
            (uint16_t)i + 1, 0, 0));
    }

    for (i = 0; i < MAX_DB; i++) {
      REQUIRE(HAM_DATABASE_NOT_FOUND ==
              ham_env_erase_db(env, (uint16_t)i + 1000, 0));
      REQUIRE(0 == ham_db_close(db[i], 0));
      REQUIRE(HAM_DATABASE_NOT_FOUND ==
              ham_env_erase_db(env, (uint16_t)i + 1000, 0));
    }

    REQUIRE(0 == ham_env_close(env, 0));
  }

  void eraseMultipleDatabases() {
    int i, j;
    const int MAX_DB = 13;
    const int MAX_ITEMS = 300;
    ham_env_t *env;
    ham_db_t *db[MAX_DB];
    ham_record_t rec;
    ham_key_t key;
    char buffer[512];
    ham_parameter_t ps[] = {
      { HAM_PARAM_PAGESIZE, 1024 * 6 },
      { 0, 0 }
    };
    ham_parameter_t ps2[] = {
      { HAM_PARAM_KEYSIZE, sizeof(buffer) },
      { 0, 0 }
    };

    REQUIRE(0 ==
      ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, ps));

    for (i = 0; i < MAX_DB; i++) {
      REQUIRE(0 ==
        ham_env_create_db(env, &db[i], (uint16_t)i + 1, 0, ps2));
      for (j = 0; j < MAX_ITEMS; j++) {
        memset(&key, 0, sizeof(key));
        memset(&rec, 0, sizeof(rec));
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%08x%08x", j, i+1);
        key.data = buffer;
        key.size = sizeof(buffer);
        key.flags = HAM_KEY_USER_ALLOC;
        rec.data = buffer;
        rec.size = sizeof(buffer);
        rec.flags = HAM_RECORD_USER_ALLOC;

        REQUIRE(0 == ham_db_insert(db[i], 0, &key, &rec, 0));
      }
      REQUIRE(0 == ham_db_close(db[i], 0));
    }

    for (i = 0; i < MAX_DB; i++) {
      REQUIRE(((m_flags & HAM_IN_MEMORY)
                ? HAM_DATABASE_NOT_FOUND
                : 0) ==
        ham_env_erase_db(env, (uint16_t)i + 1, 0));
    }

    for (i = 0; i < 10; i++) {
      REQUIRE(((m_flags & HAM_IN_MEMORY)
                ? HAM_INV_PARAMETER
                : HAM_DATABASE_NOT_FOUND) ==
        ham_env_open_db(env, &db[i], (uint16_t)i + 1, 0, 0));
    }

    REQUIRE(0 == ham_env_close(env, 0));
  }

  void eraseMultipleDatabasesReopenEnv() {
    int i, j;
    const int MAX_DB = 13;
    const int MAX_ITEMS = 300;
    ham_env_t *env;
    ham_db_t *db[MAX_DB];
    ham_record_t rec;
    ham_key_t key;
    char buffer[512];

    REQUIRE(0 ==
      ham_env_create(&env, Utils::opath(".test"),
              m_flags | HAM_DISABLE_RECLAIM_INTERNAL, 0664, 0));

    for (i = 0; i < MAX_DB; i++) {
      REQUIRE(0 ==
        ham_env_create_db(env, &db[i], (uint16_t)i + 1, 0, 0));
      for (j = 0; j < MAX_ITEMS; j++) {
        memset(&key, 0, sizeof(key));
        memset(&rec, 0, sizeof(rec));
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%08x%08x", j, i+1);
        key.data = buffer;
        key.size = sizeof(buffer);
        key.flags = HAM_KEY_USER_ALLOC;
        rec.data = buffer;
        rec.size = sizeof(buffer);
        rec.flags = HAM_RECORD_USER_ALLOC;

        REQUIRE(0 == ham_db_insert(db[i], 0, &key, &rec, 0));
      }
      REQUIRE(0 == ham_db_close(db[i], 0));
    }

    REQUIRE(0 == ham_env_close(env, 0));
    REQUIRE(0 == ham_env_open(&env, Utils::opath(".test"), m_flags, 0));

    for (i = 0; i < MAX_DB; i++) {
      REQUIRE(0 == ham_env_erase_db(env, (uint16_t)i + 1, 0));
    }

    for (i = 0; i < 10; i++) {
      REQUIRE(HAM_DATABASE_NOT_FOUND ==
        ham_env_open_db(env, &db[i], (uint16_t)i + 1, 0, 0));
    }

    REQUIRE(0 == ham_env_close(env, 0));
  }

  void limitsReachedTest() {
    int i;
    const int MAX_DB = 676 + 1;
    ham_env_t *env;
    ham_db_t *db[MAX_DB];

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));

    for (i = 0; i < MAX_DB - 1; i++)
      REQUIRE(0 == ham_env_create_db(env, &db[i],
            (uint16_t)i + 1, 0, 0));

    REQUIRE(HAM_LIMITS_REACHED ==
        ham_env_create_db(env, &db[i], (uint16_t)i + 1, 0, 0));

    for (i = 0; i < MAX_DB - 1; i++)
      REQUIRE(0 == ham_db_close(db[i], 0));
    REQUIRE(0 == ham_env_close(env, 0));
  }

  void getDatabaseNamesTest() {
    ham_env_t *env;
    ham_db_t *db1, *db2, *db3;
    uint16_t names[5];
    uint32_t names_size = 0;

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));

    REQUIRE(HAM_INV_PARAMETER ==
        ham_env_get_database_names(0, names, &names_size));
    REQUIRE(HAM_INV_PARAMETER ==
        ham_env_get_database_names(env, 0, &names_size));
    REQUIRE(HAM_INV_PARAMETER ==
        ham_env_get_database_names(env, names, 0));

    names_size = 1;
    REQUIRE(0 ==
        ham_env_get_database_names(env, names, &names_size));
    REQUIRE((uint32_t)0 == names_size);

    REQUIRE(0 ==
        ham_env_create_db(env, &db1, 111, 0, 0));
    names_size = 0;
    REQUIRE(HAM_LIMITS_REACHED ==
        ham_env_get_database_names(env, names, &names_size));

    names_size = 1;
    REQUIRE(0 ==
        ham_env_get_database_names(env, names, &names_size));
    REQUIRE((uint32_t)1 == names_size);
    REQUIRE((uint16_t)111 == names[0]);

    REQUIRE(0 ==
        ham_env_create_db(env, &db2, 222, 0, 0));
    names_size = 1;
    REQUIRE(HAM_LIMITS_REACHED ==
        ham_env_get_database_names(env, names, &names_size));

    REQUIRE(0 ==
        ham_env_create_db(env, &db3, 333, 0, 0));
    names_size = 5;
    REQUIRE(0 ==
        ham_env_get_database_names(env, names, &names_size));
    REQUIRE((uint32_t)3 == names_size);
    REQUIRE((uint16_t)111 == names[0]);
    REQUIRE((uint16_t)222 == names[1]);
    REQUIRE((uint16_t)333 == names[2]);

    REQUIRE(0 == ham_db_close(db2, 0));
    if (!(m_flags & HAM_IN_MEMORY)) {
      REQUIRE(0 == ham_env_erase_db(env, 222, 0));
      names_size = 5;
      REQUIRE(0 ==
          ham_env_get_database_names(env, names, &names_size));
      REQUIRE((uint32_t)2 == names_size);
      REQUIRE((uint16_t)111 == names[0]);
      REQUIRE((uint16_t)333 == names[1]);
    }

    REQUIRE(0 == ham_db_close(db1, 0));
    REQUIRE(0 == ham_db_close(db3, 0));
    REQUIRE(0 == ham_env_close(env, 0));
  }

  void createOpenEmptyTest() {
    ham_env_t *env;
    ham_db_t *db[10];

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));
    for (int i = 0; i < 10; i++)
      REQUIRE(0 ==
          ham_env_create_db(env, &db[i], 333+i, 0, 0));
    if (!(m_flags & HAM_IN_MEMORY)) {
      REQUIRE(0 == ham_env_close(env, HAM_AUTO_CLEANUP));

      REQUIRE(0 ==
          ham_env_open(&env, Utils::opath(".test"), m_flags, 0));
      for (int i = 0; i < 10; i++) {
        REQUIRE(0 ==
          ham_env_open_db(env, &db[i], 333+i, 0, 0));
      }
    }
    REQUIRE(0 == ham_env_close(env, HAM_AUTO_CLEANUP));
  }

  void memoryDbTest() {
    int i;
    ham_env_t *env;
    ham_db_t *db[10];

    REQUIRE(0 ==
        ham_env_create(&env, Utils::opath(".test"), m_flags, 0664, 0));

    for (i = 0; i < 10; i++)
      REQUIRE(0 == ham_env_create_db(env, &db[i],
            (uint16_t)i + 1, 0, 0));

    for (i = 0; i < 10; i++)
      REQUIRE(0 == ham_db_close(db[i], 0));

    REQUIRE(0 == ham_env_close(env, 0));
  }
};

TEST_CASE("Env/createCloseTest", "")
{
  EnvFixture f;
  f.createCloseTest();
}

TEST_CASE("Env/createCloseOpenCloseTest", "")
{
  EnvFixture f;
  f.createCloseOpenCloseTest();
}

TEST_CASE("Env/createCloseOpenCloseWithDatabasesTest", "")
{
  EnvFixture f;
  f.createCloseOpenCloseWithDatabasesTest();
}

TEST_CASE("Env/createCloseEmptyOpenCloseWithDatabasesTest", "")
{
  EnvFixture f;
  f.createCloseEmptyOpenCloseWithDatabasesTest();
}

TEST_CASE("Env/autoCleanupTest", "")
{
  EnvFixture f;
  f.autoCleanupTest();
}

TEST_CASE("Env/autoCleanup2Test", "")
{
  EnvFixture f;
  f.autoCleanup2Test();
}

TEST_CASE("Env/readOnlyTest", "")
{
  EnvFixture f;
  f.readOnlyTest();
}

TEST_CASE("Env/createPagesizeReopenTest", "")
{
  EnvFixture f;
  f.createPagesizeReopenTest();
}

TEST_CASE("Env/openFailCloseTest", "")
{
  EnvFixture f;
  f.openFailCloseTest();
}

TEST_CASE("Env/openWithKeysizeTest", "")
{
  EnvFixture f;
  f.openWithKeysizeTest();
}

TEST_CASE("Env/createDbWithKeysizeTest", "")
{
  EnvFixture f;
  f.createDbWithKeysizeTest();
}

TEST_CASE("Env/createAndOpenMultiDbTest", "")
{
  EnvFixture f;
  f.createAndOpenMultiDbTest();
}

TEST_CASE("Env/multiDbTest", "")
{
  EnvFixture f;
  f.multiDbTest();
}

TEST_CASE("Env/multiDbTest2", "")
{
  EnvFixture f;
  f.multiDbTest2();
}

TEST_CASE("Env/multiDbInsertFindTest", "")
{
  EnvFixture f;
  f.multiDbInsertFindTest();
}

TEST_CASE("Env/multiDbInsertFindExtendedTest", "")
{
  EnvFixture f;
  f.multiDbInsertFindExtendedTest();
}

TEST_CASE("Env/multiDbInsertFindExtendedEraseTest", "")
{
  EnvFixture f;
  f.multiDbInsertFindExtendedEraseTest();
}

TEST_CASE("Env/multiDbInsertCursorTest", "")
{
  EnvFixture f;
  f.multiDbInsertCursorTest();
}

TEST_CASE("Env/multiDbInsertFindExtendedCloseReopenTest", "")
{
  EnvFixture f;
  f.multiDbInsertFindExtendedCloseReopenTest();
}

TEST_CASE("Env/renameOpenDatabases", "")
{
  EnvFixture f;
  f.renameOpenDatabases();
}

TEST_CASE("Env/renameClosedDatabases", "")
{
  EnvFixture f;
  f.renameClosedDatabases();
}

TEST_CASE("Env/eraseOpenDatabases", "")
{
  EnvFixture f;
  f.eraseOpenDatabases();
}

TEST_CASE("Env/eraseUnknownDatabases", "")
{
  EnvFixture f;
  f.eraseUnknownDatabases();
}

TEST_CASE("Env/eraseMultipleDatabases", "")
{
  EnvFixture f;
  f.eraseMultipleDatabases();
}

TEST_CASE("Env/eraseMultipleDatabasesReopenEnv", "")
{
  EnvFixture f;
  f.eraseMultipleDatabasesReopenEnv();
}

TEST_CASE("Env/limitsReachedTest", "")
{
  EnvFixture f;
  f.limitsReachedTest();
}

TEST_CASE("Env/getDatabaseNamesTest", "")
{
  EnvFixture f;
  f.getDatabaseNamesTest();
}

TEST_CASE("Env/createOpenEmptyTest", "")
{
  EnvFixture f;
  f.createOpenEmptyTest();
}


TEST_CASE("Env-inmem/createCloseTest", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.createCloseTest();
}

TEST_CASE("Env-inmem/createCloseOpenCloseTest", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.createCloseOpenCloseTest();
}

TEST_CASE("Env-inmem/createCloseOpenCloseWithDatabasesTest", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.createCloseOpenCloseWithDatabasesTest();
}

TEST_CASE("Env-inmem/createPagesizeReopenTest", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.createPagesizeReopenTest();
}

TEST_CASE("Env-inmem/createDbWithKeysizeTest", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.createDbWithKeysizeTest();
}

TEST_CASE("Env-inmem/createAndOpenMultiDbTest", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.createAndOpenMultiDbTest();
}

TEST_CASE("Env-inmem/autoCleanupTest", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.autoCleanupTest();
}

TEST_CASE("Env-inmem/autoCleanup2Test", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.autoCleanup2Test();
}

TEST_CASE("Env-inmem/memoryDbTest", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.memoryDbTest();
}

TEST_CASE("Env-inmem/multiDbTest2", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.multiDbTest2();
}

TEST_CASE("Env-inmem/multiDbInsertFindTest", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.multiDbInsertFindTest();
}

TEST_CASE("Env-inmem/multiDbInsertFindExtendedTest", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.multiDbInsertFindExtendedTest();
}

TEST_CASE("Env-inmem/multiDbInsertFindExtendedEraseTest", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.multiDbInsertFindExtendedEraseTest();
}

TEST_CASE("Env-inmem/multiDbInsertCursorTest", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.multiDbInsertCursorTest();
}

TEST_CASE("Env-inmem/multiDbInsertFindExtendedCloseReopenTest", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.multiDbInsertFindExtendedCloseReopenTest();
}

TEST_CASE("Env-inmem/renameOpenDatabases", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.renameOpenDatabases();
}

TEST_CASE("Env-inmem/eraseOpenDatabases", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.eraseOpenDatabases();
}

TEST_CASE("Env-inmem/eraseUnknownDatabases", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.eraseUnknownDatabases();
}

TEST_CASE("Env-inmem/limitsReachedTest", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.limitsReachedTest();
}

TEST_CASE("Env-inmem/getDatabaseNamesTest", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.getDatabaseNamesTest();
}

TEST_CASE("Env-inmem/createOpenEmptyTest", "")
{
  EnvFixture f(HAM_IN_MEMORY);
  f.createOpenEmptyTest();
}

