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

#include "3btree/btree_index.h"
#include "3page_manager/page_manager.h"
#include "4db/db_local.h"

namespace hamsterdb {

struct RecordNumberFixture {
  uint32_t m_flags;
  ham_db_t *m_db;
  ham_env_t *m_env;

  RecordNumberFixture(uint32_t flags = 0)
    : m_flags(flags) {
    REQUIRE(0 ==
        ham_env_create(&m_env, Utils::opath(".test"), m_flags, 0664, 0));
    REQUIRE(0 ==
        ham_env_create_db(m_env, &m_db, 1, HAM_RECORD_NUMBER, 0));
  }

  ~RecordNumberFixture() {
    teardown();
  }

  void teardown() {
    REQUIRE(0 == ham_env_close(m_env, HAM_AUTO_CLEANUP));
  }

  void reopen() {
    teardown();

    REQUIRE(0 ==
        ham_env_open(&m_env, Utils::opath(".test"), m_flags, 0));
    REQUIRE(0 ==
        ham_env_open_db(m_env, &m_db, 1, 0, 0));
  }

  void createCloseTest() {
    // nop
  }

  void createCloseOpenCloseTest() {
    reopen();
    REQUIRE((((LocalDatabase *)m_db)->get_rt_flags() & HAM_RECORD_NUMBER) != 0);
  }

  void createInsertCloseReopenTest() {
    ham_key_t key = {};
    ham_record_t rec = {};
    uint64_t recno, value = 1;

    key.flags = HAM_KEY_USER_ALLOC;
    key.data = &recno;
    key.size = sizeof(recno);

    rec.data = &value;
    rec.size = sizeof(value);

    for (int i = 0; i < 5; i++) {
      REQUIRE(0 == ham_db_insert(m_db, 0, &key, &rec, 0));
      REQUIRE(recno == (uint64_t)i + 1);
    }

    key.flags = HAM_KEY_USER_ALLOC;
    key.data = 0;
    REQUIRE(HAM_INV_PARAMETER == ham_db_insert(m_db, 0, &key, &rec, 0));
    key.data = &recno;
    key.size = 4;
    REQUIRE(HAM_INV_PARAMETER ==
        ham_db_insert(m_db, 0, &key, &rec, 0));
    key.size = sizeof(recno);

    key.flags = 0;
    key.size = 0;
    REQUIRE(HAM_INV_PARAMETER ==
        ham_db_insert(m_db, 0, &key, &rec, 0));
    key.size = 8;
    key.data = 0;
    REQUIRE(HAM_INV_PARAMETER ==
        ham_db_insert(m_db, 0, &key, &rec, 0));
    key.data = &recno;
    key.flags = HAM_KEY_USER_ALLOC;

    reopen();

    for (int i = 5; i < 10; i++) {
      REQUIRE(0 == ham_db_insert(m_db, 0, &key, &rec, 0));
      REQUIRE(recno == (uint64_t)i + 1);
    }
  }

  void createInsertCloseReopenCursorTest(void)
  {
    ham_key_t key = {};
    ham_record_t rec = {};
    ham_cursor_t *cursor;
    uint64_t recno, value = 1;

    key.flags = HAM_KEY_USER_ALLOC;
    key.data = &recno;
    key.size = sizeof(recno);

    rec.data = &value;
    rec.size = sizeof(value);

    REQUIRE(0 ==
        ham_cursor_create(&cursor, m_db, 0, 0));

    for (int i = 0; i < 5; i++) {
      REQUIRE(0 == ham_cursor_insert(cursor, &key, &rec, 0));
      REQUIRE(recno == (uint64_t)i + 1);
    }

    REQUIRE(0 == ham_cursor_close(cursor));
    reopen();
    REQUIRE(0 == ham_cursor_create(&cursor, m_db, 0, 0));

    for (int i = 5; i < 10; i++) {
      REQUIRE(0 == ham_cursor_insert(cursor, &key, &rec, 0));
      REQUIRE(recno == (uint64_t)i + 1);
    }

    REQUIRE(0 == ham_cursor_close(cursor));
  }

  void createInsertCloseTest() {
    ham_key_t key = {};
    ham_record_t rec = {};
    uint64_t recno, value = 1;

    key.flags = HAM_KEY_USER_ALLOC;
    key.data = &recno;
    key.size = sizeof(recno);

    rec.data = &value;
    rec.size = sizeof(value);

    for (int i = 0; i < 5; i++) {
      REQUIRE(0 == ham_db_insert(m_db, 0, &key, &rec, 0));
      REQUIRE(recno == (uint64_t)i + 1);
    }
  }

  void createInsertManyCloseTest() {
    ham_key_t key = {};
    ham_record_t rec = {};
    uint64_t recno, value = 1;

    key.flags = HAM_KEY_USER_ALLOC;
    key.data = &recno;
    key.size = sizeof(recno);

    rec.data = &value;
    rec.size = sizeof(value);

    for (int i = 0; i < 500; i++) {
      REQUIRE(0 == ham_db_insert(m_db, 0, &key, &rec, 0));
      REQUIRE(recno == (uint64_t)i + 1);
    }

    key.size = 4;
    REQUIRE(HAM_INV_PARAMETER == ham_db_find(m_db, 0, &key, &rec, 0));
    key.size = 0;
    key.data = &key;
    REQUIRE(HAM_INV_PARAMETER == ham_db_find(m_db, 0, &key, &rec, 0));

    for (int i = 0; i < 500; i++) {
      recno = i + 1;
      memset(&key, 0, sizeof(key));
      memset(&rec, 0, sizeof(rec));
      key.data = &recno;
      key.size = sizeof(recno);
      REQUIRE(0 == ham_db_find(m_db, 0, &key, &rec, 0));
    }
  }

  void createInsertCloseCursorTest(void) {
    ham_key_t key = {};
    ham_record_t rec = {};
    ham_cursor_t *cursor;
    uint64_t recno, value = 1;

    key.flags = HAM_KEY_USER_ALLOC;
    key.data = &recno;
    key.size = sizeof(recno);

    rec.data = &value;
    rec.size = sizeof(value);

    REQUIRE(0 == ham_cursor_create(&cursor, m_db, 0, 0));

    for (int i = 0; i < 5; i++) {
      REQUIRE(0 == ham_cursor_insert(cursor, &key, &rec, 0));
      REQUIRE(recno == (uint64_t)i + 1);
    }

    REQUIRE(0 == ham_cursor_close(cursor));
  }

  void createInsertCloseReopenTwiceTest() {
    ham_key_t key = {};
    ham_record_t rec = {};
    uint64_t recno, value = 1;

    key.flags = HAM_KEY_USER_ALLOC;
    key.data = &recno;
    key.size = sizeof(recno);

    rec.data = &value;
    rec.size = sizeof(value);

    for (int i = 0; i < 5; i++) {
      REQUIRE(0 == ham_db_insert(m_db, 0, &key, &rec, 0));
      REQUIRE(recno == (uint64_t)i + 1);
    }

    reopen();

    for (int i = 5; i < 10; i++) {
      REQUIRE(0 == ham_db_insert(m_db, 0, &key, &rec, 0));
      REQUIRE(recno == (uint64_t)i + 1);
    }

    reopen();

    for (int i = 10; i < 15; i++) {
      REQUIRE(0 == ham_db_insert(m_db, 0, &key, &rec, 0));
      REQUIRE(recno == (uint64_t)i + 1);
    }
  }

  void createInsertCloseReopenTwiceCursorTest() {
    ham_key_t key = {};
    ham_record_t rec = {};
    uint64_t recno, value = 1;
    ham_cursor_t *cursor;

    key.flags = HAM_KEY_USER_ALLOC;
    key.data = &recno;
    key.size = sizeof(recno);

    rec.data = &value;
    rec.size = sizeof(value);

    REQUIRE(0 == ham_cursor_create(&cursor, m_db, 0, 0));

    for (int i = 0; i < 5; i++) {
      REQUIRE(0 == ham_cursor_insert(cursor, &key, &rec, 0));
      REQUIRE(recno == (uint64_t)i + 1);
    }

    REQUIRE(0 == ham_cursor_close(cursor));

    reopen();

    REQUIRE(0 == ham_cursor_create(&cursor, m_db, 0, 0));

    for (int i = 5; i < 10; i++) {
      REQUIRE(0 == ham_cursor_insert(cursor, &key, &rec, 0));
      REQUIRE(recno == (uint64_t)i + 1);
    }

    REQUIRE(0 == ham_cursor_close(cursor));

    reopen();

    REQUIRE(0 ==
        ham_cursor_create(&cursor, m_db, 0, 0));

    for (int i = 10; i < 15; i++) {
      REQUIRE(0 == ham_cursor_insert(cursor, &key, &rec, 0));
      REQUIRE(recno == (uint64_t)i + 1);
    }

    REQUIRE(0 == ham_cursor_close(cursor));
  }

  void insertBadKeyTest() {
    ham_key_t key = {};
    ham_record_t rec = {};
    uint64_t recno;

    key.flags = 0;
    key.data = &recno;
    key.size = sizeof(recno);
    REQUIRE(HAM_INV_PARAMETER == ham_db_insert(m_db, 0, &key, &rec, 0));

    key.data = 0;
    key.size = 8;
    REQUIRE(HAM_INV_PARAMETER == ham_db_insert(m_db, 0, &key, &rec, 0));
    REQUIRE(HAM_INV_PARAMETER == ham_db_insert(m_db, 0, 0, &rec, 0));

    key.data = 0;
    key.size = 0;
    REQUIRE(0 == ham_db_insert(m_db, 0, &key, &rec, 0));
    REQUIRE((uint64_t)1ull == *(uint64_t *)key.data);
  }

  void insertBadKeyCursorTest() {
    ham_key_t key = {};
    ham_record_t rec = {};
    ham_cursor_t *cursor;
    uint64_t recno;

    REQUIRE(0 == ham_cursor_create(&cursor, m_db, 0, 0));

    key.flags = 0;
    key.data = &recno;
    key.size = sizeof(recno);
    REQUIRE(HAM_INV_PARAMETER ==
        ham_cursor_insert(cursor, &key, &rec, 0));

    key.data = 0;
    key.size = 8;
    REQUIRE(HAM_INV_PARAMETER ==
        ham_cursor_insert(cursor, &key, &rec, 0));

    REQUIRE(HAM_INV_PARAMETER ==
        ham_cursor_insert(cursor, 0, &rec, 0));

    key.data = 0;
    key.size = 0;
    REQUIRE(0 == ham_cursor_insert(cursor, &key, &rec, 0));
    REQUIRE((uint64_t)1ull == *(uint64_t *)key.data);

    REQUIRE(0 == ham_cursor_close(cursor));
  }

  void createBadKeysizeTest() {
    ham_parameter_t p[] = {
      { HAM_PARAM_KEYSIZE, 7 },
      { 0, 0 }
    };

    REQUIRE(HAM_INV_KEYSIZE ==
        ham_env_create_db(m_env, &m_db, 2, HAM_RECORD_NUMBER, &p[0]));

    p[0].value = 9;
    REQUIRE(0 ==
        ham_env_create_db(m_env, &m_db, 2, HAM_RECORD_NUMBER, &p[0]));
  }

  void envTest() {
    ham_key_t key = {};
    ham_record_t rec = {};
    uint64_t recno;

    key.data = &recno;
    key.size = sizeof(recno);
    key.flags = HAM_KEY_USER_ALLOC;

    teardown();

    REQUIRE(0 ==
        ham_env_create(&m_env, Utils::opath(".test"), m_flags, 0664, 0));
    REQUIRE(0 ==
        ham_env_create_db(m_env, &m_db, 1, HAM_RECORD_NUMBER, 0));
    REQUIRE(0 ==
        ham_db_insert(m_db, 0, &key, &rec, 0));
    REQUIRE((uint64_t)1ull == *(uint64_t *)key.data);

    if (!(m_flags & HAM_IN_MEMORY)) {
      reopen();

      REQUIRE(0 == ham_db_insert(m_db, 0, &key, &rec, 0));
      REQUIRE((uint64_t)2ull == *(uint64_t *)key.data);
    }
  }

  void overwriteTest() {
    ham_key_t key = {};
    ham_record_t rec = {};
    uint64_t recno, value;

    key.data = &recno;
    key.flags = HAM_KEY_USER_ALLOC;
    key.size = sizeof(recno);
    REQUIRE(0 == ham_db_insert(m_db, 0, &key, &rec, 0));

    value = 0x13ull;
    memset(&rec, 0, sizeof(rec));
    rec.data = &value;
    rec.size = sizeof(value);
    REQUIRE(0 == ham_db_insert(m_db, 0, &key, &rec, HAM_OVERWRITE));

    key.size = 4;
    REQUIRE(HAM_INV_PARAMETER ==
        ham_db_insert(m_db, 0, &key, &rec, HAM_OVERWRITE));
    key.size = 8;
    key.data = 0;
    REQUIRE(HAM_INV_PARAMETER ==
        ham_db_insert(m_db, 0, &key, &rec, HAM_OVERWRITE));
    key.data = &recno;

    memset(&rec, 0, sizeof(rec));
    REQUIRE(0 == ham_db_find(m_db, 0, &key, &rec, 0));

    REQUIRE(value == *(uint64_t *)rec.data);
  }

  void overwriteCursorTest() {
    ham_key_t key = {};
    ham_record_t rec = {};
    uint64_t recno, value;
    ham_cursor_t *cursor;

    REQUIRE(0 ==
        ham_cursor_create(&cursor, m_db, 0, 0));

    key.data = &recno;
    key.flags = HAM_KEY_USER_ALLOC;
    key.size = sizeof(recno);
    REQUIRE(0 == ham_cursor_insert(cursor, &key, &rec, 0));

    value = 0x13ull;
    memset(&rec, 0, sizeof(rec));
    rec.data = &value;
    rec.size = sizeof(value);
    REQUIRE(0 ==
        ham_cursor_insert(cursor, &key, &rec, HAM_OVERWRITE));

    memset(&rec, 0, sizeof(rec));
    REQUIRE(0 == ham_db_find(m_db, 0, &key, &rec, 0));

    REQUIRE(value == *(uint64_t *)rec.data);

    REQUIRE(0 == ham_cursor_close(cursor));
  }

  void eraseLastReopenTest() {
    ham_key_t key = {};
    ham_record_t rec = {};
    uint64_t recno;

    key.data = &recno;
    key.flags = HAM_KEY_USER_ALLOC;
    key.size = sizeof(recno);

    for (int i = 0; i < 5; i++) {
      REQUIRE(0 == ham_db_insert(m_db, 0, &key, &rec, 0));
      REQUIRE(recno == (uint64_t)i + 1);
    }

    REQUIRE(0 == ham_db_erase(m_db, 0, &key, 0));

    reopen();

    for (int i = 5; i < 10; i++) {
      REQUIRE(0 == ham_db_insert(m_db, 0, &key, &rec, 0));
      REQUIRE((uint64_t)i == recno);
    }
  }

  void uncoupleTest() {
    ham_key_t key = {};
    ham_record_t rec = {};
    uint64_t recno;
    ham_cursor_t *cursor, *c2;

    key.flags = HAM_KEY_USER_ALLOC;
    key.data = &recno;
    key.size = sizeof(recno);

    REQUIRE(0 == ham_cursor_create(&cursor, m_db, 0, 0));
    REQUIRE(0 == ham_cursor_create(&c2, m_db, 0, 0));

    for (int i = 0; i < 5; i++) {
      REQUIRE(0 == ham_cursor_insert(cursor, &key, &rec, 0));
      REQUIRE(recno == (uint64_t)i + 1);
    }

    LocalDatabase *db = (LocalDatabase *)m_db;
    BtreeIndex *be = db->get_btree_index();
    Page *page;
    PageManager *pm = db->get_local_env()->get_page_manager();
    REQUIRE((page = pm->fetch_page(db, be->get_root_address())) != 0);
    REQUIRE(page != 0);
    BtreeCursor::uncouple_all_cursors(page);

    for (int i = 0; i < 5; i++) {
      REQUIRE(0 == ham_cursor_move(c2, &key, &rec, HAM_CURSOR_NEXT));
      REQUIRE(recno == (uint64_t)i + 1);
    }
  }

  void splitTest() {
    ham_key_t key = {};
    ham_record_t rec = {};
    uint64_t recno;

    key.flags = HAM_KEY_USER_ALLOC;
    key.data = &recno;
    key.size = sizeof(recno);

    for (int i = 0; i < 4096; i++) {
      REQUIRE(0 == ham_db_insert(m_db, 0, &key, &rec, 0));
      REQUIRE(recno == (uint64_t)i + 1);
    }
  }
};

TEST_CASE("RecordNumber/createCloseTest", "")
{
  RecordNumberFixture f;
  f.createCloseTest();
}

TEST_CASE("RecordNumber/createCloseOpenCloseTest", "")
{
  RecordNumberFixture f;
  f.createCloseOpenCloseTest();
}

TEST_CASE("RecordNumber/createInsertCloseTest", "")
{
  RecordNumberFixture f;
  f.createInsertCloseTest();
}

TEST_CASE("RecordNumber/createInsertManyCloseTest", "")
{
  RecordNumberFixture f;
  f.createInsertManyCloseTest();
}

TEST_CASE("RecordNumber/createInsertCloseCursorTest", "")
{
  RecordNumberFixture f;
  f.createInsertCloseCursorTest();
}

TEST_CASE("RecordNumber/createInsertCloseReopenTest", "")
{
  RecordNumberFixture f;
  f.createInsertCloseReopenTest();
}

TEST_CASE("RecordNumber/createInsertCloseReopenCursorTest", "")
{
  RecordNumberFixture f;
  f.createInsertCloseReopenCursorTest();
}

TEST_CASE("RecordNumber/createInsertCloseReopenTwiceTest", "")
{
  RecordNumberFixture f;
  f.createInsertCloseReopenTwiceTest();
}

TEST_CASE("RecordNumber/createInsertCloseReopenTwiceCursorTest", "")
{
  RecordNumberFixture f;
  f.createInsertCloseReopenTwiceCursorTest();
}

TEST_CASE("RecordNumber/insertBadKeyTest", "")
{
  RecordNumberFixture f;
  f.insertBadKeyTest();
}

TEST_CASE("RecordNumber/insertBadKeyCursorTest", "")
{
  RecordNumberFixture f;
  f.insertBadKeyCursorTest();
}

TEST_CASE("RecordNumber/createBadKeysizeTest", "")
{
  RecordNumberFixture f;
  f.createBadKeysizeTest();
}

TEST_CASE("RecordNumber/envTest", "")
{
  RecordNumberFixture f;
  f.envTest();
}

TEST_CASE("RecordNumber/overwriteTest", "")
{
  RecordNumberFixture f;
  f.overwriteTest();
}

TEST_CASE("RecordNumber/overwriteCursorTest", "")
{
  RecordNumberFixture f;
  f.overwriteCursorTest();
}

TEST_CASE("RecordNumber/eraseLastReopenTest", "")
{
  RecordNumberFixture f;
  f.eraseLastReopenTest();
}

TEST_CASE("RecordNumber/uncoupleTest", "")
{
  RecordNumberFixture f;
  f.uncoupleTest();
}

TEST_CASE("RecordNumber/splitTest", "")
{
  RecordNumberFixture f;
  f.splitTest();
}


TEST_CASE("RecordNumber-inmem/createCloseTest", "")
{
  RecordNumberFixture f(HAM_IN_MEMORY);
  f.createCloseTest();
}

TEST_CASE("RecordNumber-inmem/createInsertCloseTest", "")
{
  RecordNumberFixture f(HAM_IN_MEMORY);
  f.createInsertCloseTest();
}

TEST_CASE("RecordNumber-inmem/createInsertManyCloseTest", "")
{
  RecordNumberFixture f(HAM_IN_MEMORY);
  f.createInsertManyCloseTest();
}

TEST_CASE("RecordNumber-inmem/createInsertCloseCursorTest", "")
{
  RecordNumberFixture f(HAM_IN_MEMORY);
  f.createInsertCloseCursorTest();
}

TEST_CASE("RecordNumber-inmem/insertBadKeyTest", "")
{
  RecordNumberFixture f(HAM_IN_MEMORY);
  f.insertBadKeyTest();
}

TEST_CASE("RecordNumber-inmem/insertBadKeyCursorTest", "")
{
  RecordNumberFixture f(HAM_IN_MEMORY);
  f.insertBadKeyCursorTest();
}

TEST_CASE("RecordNumber-inmem/createBadKeysizeTest", "")
{
  RecordNumberFixture f(HAM_IN_MEMORY);
  f.createBadKeysizeTest();
}

TEST_CASE("RecordNumber-inmem/envTest", "")
{
  RecordNumberFixture f(HAM_IN_MEMORY);
  f.envTest();
}

TEST_CASE("RecordNumber-inmem/overwriteTest", "")
{
  RecordNumberFixture f(HAM_IN_MEMORY);
  f.overwriteTest();
}

TEST_CASE("RecordNumber-inmem/overwriteCursorTest", "")
{
  RecordNumberFixture f(HAM_IN_MEMORY);
  f.overwriteCursorTest();
}

TEST_CASE("RecordNumber-inmem/uncoupleTest", "")
{
  RecordNumberFixture f(HAM_IN_MEMORY);
  f.uncoupleTest();
}

TEST_CASE("RecordNumber-inmem/splitTest", "")
{
  RecordNumberFixture f(HAM_IN_MEMORY);
  f.splitTest();
}

} // namespace RecordNumberFixture
