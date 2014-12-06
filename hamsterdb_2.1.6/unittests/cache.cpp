/**
 * Copyright (C) 2005-2013 Christoph Rupp (chris@crupp.de).
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * See files COPYING.* for License information.
 */

#include "../src/config.h"

#include "3rdparty/catch/catch.hpp"

#include "globals.h"
#include "os.hpp"

#include "../src/page.h"
#include "../src/cache.h"
#include "../src/error.h"
#include "../src/env.h"
#include "../src/os.h"
#include "../src/page_manager.h"

using namespace hamsterdb;

struct CacheFixture {
  ham_db_t *m_db;
  ham_env_t *m_env;

  CacheFixture() {
    REQUIRE(0 ==
        ham_env_create(&m_env, Globals::opath(".test"), 0, 0644, 0));
    REQUIRE(0 ==
        ham_env_create_db(m_env, &m_db, 13, HAM_ENABLE_DUPLICATES, 0));
  }

  ~CacheFixture() {
    teardown();
  }

  void teardown() {
    if (m_env)
      REQUIRE(0 == ham_env_close(m_env, HAM_AUTO_CLEANUP));
  }
};

TEST_CASE("CacheTest/newDelete",
           "Tests the Cache")
{
  CacheFixture f;
  Cache *cache = new Cache((Environment *)f.m_env, 15);
  REQUIRE(cache != 0);
  delete cache;
}

TEST_CASE("CacheTest/putGet",
           "Tests the Cache")
{
  CacheFixture f;
  Page *page;
  PageData pers;
  memset(&pers, 0, sizeof(pers));
  Cache *cache = new Cache((Environment *)f.m_env, 15);
  page = new Page((Environment *)f.m_env);
  page->set_self(0x123ull);
  page->set_pers(&pers);
  page->set_flags(Page::NPERS_NO_HEADER);
  cache->put_page(page);
  cache->get_page(0x123ull, 0);
  delete cache;
  page->set_pers(0);
  delete page;
}

TEST_CASE("CacheTest/putGetRemoveGet",
           "Tests the Cache")
{
  CacheFixture f;
  Page *page;
  PageData pers;
  memset(&pers, 0, sizeof(pers));
  Cache *cache = new Cache((Environment *)f.m_env, 15);
  page = new Page((Environment *)f.m_env);
  page->set_flags(Page::NPERS_NO_HEADER);
  page->set_self(0x123ull);
  page->set_pers(&pers);
  cache->put_page(page);
  REQUIRE(page == cache->get_page(0x123ull, 0));
  cache->remove_page(page);
  REQUIRE((Page *)0 == cache->get_page(0x123ull));
  delete cache;
  page->set_pers(0);
  delete page;
}

TEST_CASE("CacheTest/putGetReplaceGet",
           "Tests the Cache")
{
  CacheFixture f;
  Page *page1, *page2;
  PageData pers1, pers2;
  memset(&pers1, 0, sizeof(pers1));
  memset(&pers2, 0, sizeof(pers2));
  Cache *cache = new Cache((Environment *)f.m_env, 15);
  page1 = new Page((Environment *)f.m_env);
  page1->set_flags(Page::NPERS_NO_HEADER);
  page1->set_self(0x123ull);
  page1->set_pers(&pers1);
  page2 = new Page((Environment *)f.m_env);
  page2->set_flags(Page::NPERS_NO_HEADER);
  page2->set_self(0x456ull);
  page2->set_pers(&pers2);
  cache->put_page(page1);
  cache->remove_page(page1);
  cache->put_page(page2);
  REQUIRE((Page *)0 == cache->get_page(0x123ull));
  REQUIRE(page2 == cache->get_page(0x456ull));
  delete cache;
  page1->set_pers(0);
  delete page1;
  page2->set_pers(0);
  delete page2;
}

TEST_CASE("CacheTest/multiplePut",
           "Tests the Cache")
{
  CacheFixture f;
  Page *page[20];
  PageData pers[20];
  Cache *cache = new Cache((Environment *)f.m_env, 15);

  for (int i = 0; i < 20; i++) {
    page[i] = new Page((Environment *)f.m_env);
    memset(&pers[i], 0, sizeof(pers[i]));
    page[i]->set_flags(Page::NPERS_NO_HEADER);
    page[i]->set_self((i + 1) * 1024);
    page[i]->set_pers(&pers[i]);
    cache->put_page(page[i]);
  }
  for (int i = 0; i < 20; i++)
    REQUIRE(page[i] == cache->get_page((i + 1) * 1024, 0));
  for (int i = 0; i < 20; i++)
    cache->remove_page(page[i]);
  for (int i = 0; i < 20; i++) {
    REQUIRE((Page *)0 == cache->get_page((i + 1) * 1024));
    page[i]->set_pers(0);
    delete page[i];
  }
  delete cache;
}

TEST_CASE("CacheTest/negativeGet",
           "Tests the Cache")
{
  CacheFixture f;
  Cache *cache = new Cache((Environment *)f.m_env, 15);
  for (int i = 0; i < 20; i++)
    REQUIRE((Page *)0 == cache->get_page(i * 1024 * 13));
  delete cache;
}

TEST_CASE("CacheTest/overflowTest",
           "Tests the Cache")
{
  CacheFixture f;
  Cache *cache = new Cache((Environment *)f.m_env, 15 * HAM_DEFAULT_PAGESIZE);
  PageData pers;
  memset(&pers, 0, sizeof(pers));
  std::vector<Page *> v;

  for (unsigned int i = 0; i < 15; i++) {
    Page *p = new Page((Environment *)f.m_env);
    p->set_flags(Page::NPERS_NO_HEADER | Page::NPERS_MALLOC);
    p->set_self((i + 1) * 1024);
    p->set_pers(&pers);
    v.push_back(p);
    cache->put_page(p);
    REQUIRE(false == cache->is_too_big());
  }

  for (unsigned int i = 0; i < 5; i++) {
    Page *p = new Page((Environment *)f.m_env);
    p->set_flags(Page::NPERS_NO_HEADER | Page::NPERS_MALLOC);
    p->set_self((i + 1) * 1024);
    p->set_pers(&pers);
    v.push_back(p);
    cache->put_page(p);
    REQUIRE(true == cache->is_too_big());
  }

  for (unsigned int i = 0; i < 5; i++) {
    Page *p;
    REQUIRE(true == cache->is_too_big());
    p = v.back();
    v.pop_back();
    cache->remove_page(p);
    p->set_pers(0);
    delete p;
  }

  for (unsigned int i = 0; i < 15; i++) {
    Page *p;
    p = v.back();
    v.pop_back();
    cache->remove_page(p);
    REQUIRE(false == cache->is_too_big());
    p->set_pers(0);
    delete p;
  }

  REQUIRE(false == cache->is_too_big());
  delete cache;
}

TEST_CASE("CacheTest/strict",
           "Tests the Cache")
{
  CacheFixture f;
  f.teardown();

  ham_parameter_t param[] = {
    { HAM_PARAM_PAGESIZE, 1024 * 128 },
    { 0, 0 }
  };

  Page *p[1024];

  REQUIRE(0 ==
      ham_env_create(&f.m_env, Globals::opath(".test"),  
          HAM_CACHE_STRICT | HAM_DISABLE_MMAP, 0644, &param[0]));
  REQUIRE(0 ==
      ham_env_create_db(f.m_env, &f.m_db, 13, 0, 0));

  Cache *cache = ((Environment *)f.m_env)->get_page_manager()->test_get_cache();

  REQUIRE(cache->get_capacity() == 1024 * 1024 * 2u);

  unsigned int max_pages = HAM_DEFAULT_CACHESIZE / (1024 * 128);
  unsigned int i;
  for (i = 0; i < max_pages; i++)
    REQUIRE(0 == ((Database *)f.m_db)->alloc_page(&p[i], 0, 0));

  REQUIRE(HAM_CACHE_FULL ==
      ((Database *)f.m_db)->alloc_page(&p[i], 0, 0));
  REQUIRE(0 == ((Environment *)f.m_env)->get_page_manager()->purge_cache());
  REQUIRE(0 == ((Database *)f.m_db)->alloc_page(&p[i], 0, 0));
}

TEST_CASE("CacheTest/setSizeEnvCreate",
           "Tests the Cache")
{
  CacheFixture f;
  f.teardown();

  ham_parameter_t param[] = {
    { HAM_PARAM_CACHESIZE, 100 * 1024 },
    { HAM_PARAM_PAGESIZE,  1024 },
    { 0, 0 }
  };

  REQUIRE(0 ==
      ham_env_create(&f.m_env, Globals::opath(".test"),  
          HAM_CACHE_STRICT, 0644, &param[0]));
  REQUIRE(0 ==
      ham_env_create_db(f.m_env, &f.m_db, 13, 0, 0));

  Cache *cache = ((Environment *)f.m_env)->get_page_manager()->test_get_cache();

  REQUIRE(102400ull == cache->get_capacity());
}

TEST_CASE("CacheTest/setSizeEnvOpen",
           "Tests the Cache")
{
  CacheFixture f;
  f.teardown();

  ham_parameter_t param[] = {
    { HAM_PARAM_CACHESIZE, 100 * 1024 },
    { 0, 0 }
  };

  REQUIRE(0 ==
      ham_env_open(&f.m_env, Globals::opath(".test"), 0, &param[0]));

  Cache *cache = ((Environment *)f.m_env)->get_page_manager()->test_get_cache();

  REQUIRE(102400ull == cache->get_capacity());
}

TEST_CASE("CacheTest/bigSize",
           "Tests the Cache")
{
  CacheFixture f;
  ham_u64_t size = 1024ull * 1024ull * 1024ull * 16ull;
  Cache *cache = new Cache((Environment *)f.m_env, size);
  REQUIRE(cache != 0);
  REQUIRE(size == cache->get_capacity());
  delete cache;
}

