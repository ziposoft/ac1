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
#include "2device/device.h"
#include "4db/db.h"
#include "4env/env_local.h"
#include "4txn/txn.h"

using namespace hamsterdb;

struct PageFixture {
  ham_db_t *m_db;
  ham_env_t *m_env;
  bool m_inmemory;
  bool m_usemmap;

  PageFixture(bool inmemorydb = false, bool mmap = true)
    : m_db(0), m_inmemory(inmemorydb), m_usemmap(mmap) {
    uint32_t flags = 0;

    if (m_inmemory)
      flags |= HAM_IN_MEMORY;
    if (!m_usemmap)
      flags |= HAM_DISABLE_MMAP;

    REQUIRE(0 ==
        ham_env_create(&m_env, Utils::opath(".test"), flags, 0644, 0));
    REQUIRE(0 ==
        ham_env_create_db(m_env, &m_db, 1, 0, 0));
  }

  ~PageFixture() {
    REQUIRE(0 == ham_env_close(m_env, HAM_AUTO_CLEANUP));
  }

  void newDeleteTest() {
    Page *page;
    page = new Page(((LocalEnvironment *)m_env)->get_device());
    REQUIRE(page);
    delete page;
  }

  void allocFreeTest() {
    Page *page;
    page = new Page(((LocalEnvironment *)m_env)->get_device());
    page->allocate(0, 1024);
    delete page;
  }

  void multipleAllocFreeTest() {
    int i;
    Page *page;
    uint32_t ps = ((LocalEnvironment *)m_env)->get_page_size();

    for (i = 0; i < 10; i++) {
      page = new Page(((LocalEnvironment *)m_env)->get_device());
      page->allocate(0, ps);
      /* i+2 since we need 1 page for the header page and one page
       * for the root page */
      if (!m_inmemory)
        REQUIRE(page->get_address() == (i + 2) * ps);
      delete page;
    }
  }

  void fetchFlushTest() {
    Page *page, *temp;
    uint32_t ps = ((LocalEnvironment *)m_env)->get_page_size();

    page = new Page(((LocalEnvironment *)m_env)->get_device());
    temp = new Page(((LocalEnvironment *)m_env)->get_device());
    page->allocate(0, ps);
    REQUIRE(page->get_address() == ps * 2);

    page->fetch(page->get_address());

    // patch the size, otherwise we run into asserts

    memset(page->get_payload(), 0x13, ps - Page::kSizeofPersistentHeader);
    page->set_dirty(true);
    page->flush();

    REQUIRE(false == page->is_dirty());
    temp->fetch(ps * 2);
    REQUIRE(0 == memcmp(page->get_data(), temp->get_data(), ps));

    delete temp;
    delete page;
  }
};

TEST_CASE("Page/newDelete", "")
{
  PageFixture f;
  f.newDeleteTest();
}

TEST_CASE("Page/allocFree", "")
{
  PageFixture f;
  f.allocFreeTest();
}

TEST_CASE("Page/multipleAllocFree", "")
{
  PageFixture f;
  f.multipleAllocFreeTest();
}

TEST_CASE("Page/fetchFlush", "")
{
  PageFixture f;
  f.fetchFlushTest();
}

TEST_CASE("Page-nommap/newDelete", "")
{
  PageFixture f(false, false);
  f.newDeleteTest();
}

TEST_CASE("Page-nommap/allocFree", "")
{
  PageFixture f(false, false);
  f.allocFreeTest();
}

TEST_CASE("Page-nommap/multipleAllocFree", "")
{
  PageFixture f(false, false);
  f.multipleAllocFreeTest();
}

TEST_CASE("Page-nommap/fetchFlush", "")
{
  PageFixture f(false, false);
  f.fetchFlushTest();
}


TEST_CASE("Page-inmem/newDelete", "")
{
  PageFixture f(true);
  f.newDeleteTest();
}

TEST_CASE("Page-inmem/allocFree", "")
{
  PageFixture f(true);
  f.allocFreeTest();
}

TEST_CASE("Page-inmem/multipleAllocFree", "")
{
  PageFixture f(true);
  f.multipleAllocFreeTest();
}

