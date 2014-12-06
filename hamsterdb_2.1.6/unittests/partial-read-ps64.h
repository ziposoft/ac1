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

#define TEST_PREFIX "-ps64"
#define TEST_PAGESIZE 1024 * 64
#define TEST_INMEMORY false
#define TEST_FLAGS 0

namespace read_ps64 {

/* read at offset 0, partial size 50, record size 50 (no gaps) */
TEST_CASE("PartialRead" TEST_PREFIX "/simpleFindTest", "")
{
  PartialReadFixture f(TEST_PAGESIZE, TEST_INMEMORY, TEST_FLAGS);
  f.simpleFindTest();
}

/* read at offset 0, partial size 50, record size 100 (gap at end) */
TEST_CASE("PartialRead" TEST_PREFIX "/findGapsAtEndTestSmall", "")
{
  PartialReadFixture f(TEST_PAGESIZE, TEST_INMEMORY, TEST_FLAGS);
  f.findGapsAtEndTestSmall();
}

/* read at offset 0, partial size 500, record size 1000 (gap at end) */
TEST_CASE("PartialRead" TEST_PREFIX "/findGapsAtEndTestBig", "")
{
  PartialReadFixture f(TEST_PAGESIZE, TEST_INMEMORY, TEST_FLAGS);
  f.findGapsAtEndTestBig();
}

/* read at offset 0, partial size 5000, record size 10000 (gap at end) */
TEST_CASE("PartialRead" TEST_PREFIX "/findGapsAtEndTestBigger", "")
{
  PartialReadFixture f(TEST_PAGESIZE, TEST_INMEMORY, TEST_FLAGS);
  f.findGapsAtEndTestBigger();
}

/* read at offset 0, partial size 50000, record size 100000 (gap at end) */
TEST_CASE("PartialRead" TEST_PREFIX "/findGapsAtEndTestBiggest", "")
{
  PartialReadFixture f(TEST_PAGESIZE, TEST_INMEMORY, TEST_FLAGS);
  f.findGapsAtEndTestBiggest();
}

/* read at offset 0, partial size 500000, record size 1000000 (gap at end) */
TEST_CASE("PartialRead" TEST_PREFIX "/findGapsAtEndTestSuperbig", "")
{
  PartialReadFixture f(TEST_PAGESIZE, TEST_INMEMORY, TEST_FLAGS);
  f.findGapsAtEndTestSuperbig();
}

/* read at offset 50, partial size 50, record size 100 (gap at end) */
TEST_CASE("PartialRead" TEST_PREFIX "/findGapsAtBeginningTestSmall", "")
{
  PartialReadFixture f(TEST_PAGESIZE, TEST_INMEMORY, TEST_FLAGS);
  f.findGapsAtBeginningTestSmall();
}

/* read at offset 500, partial size 500, record size 1000 (gap at end) */
TEST_CASE("PartialRead" TEST_PREFIX "/findGapsAtBeginningTestBig", "")
{
  PartialReadFixture f(TEST_PAGESIZE, TEST_INMEMORY, TEST_FLAGS);
  f.findGapsAtBeginningTestBig();
}

/* read at offset 5000, partial size 5000, record size 10000 (gap at end) */
TEST_CASE("PartialRead" TEST_PREFIX "/findGapsAtBeginningTestBigger", "")
{
  PartialReadFixture f(TEST_PAGESIZE, TEST_INMEMORY, TEST_FLAGS);
  f.findGapsAtBeginningTestBigger();
}

/* read at offset 50000, partial size 50000, record size 100000 (gap at end) */
TEST_CASE("PartialRead" TEST_PREFIX "/findGapsAtBeginningTestBiggest", "")
{
  PartialReadFixture f(TEST_PAGESIZE, TEST_INMEMORY, TEST_FLAGS);
  f.findGapsAtBeginningTestBiggest();
}

/* read at offset 500000, partial size 500000, record size 1000000 (gap at end) */
TEST_CASE("PartialRead" TEST_PREFIX "/findGapsAtBeginningTestSuperbig", "")
{
  PartialReadFixture f(TEST_PAGESIZE, TEST_INMEMORY, TEST_FLAGS);
  f.findGapsAtBeginningTestSuperbig();
}

/* read at offset 50, partial size 50, record size 200 (gap
* at beginning and end) */
TEST_CASE("PartialRead" TEST_PREFIX "/findGapsTestSmall", "")
{
  PartialReadFixture f(TEST_PAGESIZE, TEST_INMEMORY, TEST_FLAGS);
  f.findGapsTestSmall();
}

/* read at offset 500, partial size 500, record size 2000 (gap
* at beginning and end) */
TEST_CASE("PartialRead" TEST_PREFIX "/findGapsTestBig", "")
{
  PartialReadFixture f(TEST_PAGESIZE, TEST_INMEMORY, TEST_FLAGS);
  f.findGapsTestBig();
}

/* read at offset 5000, partial size 5000, record size 20000 (gap
* at beginning and end) */
TEST_CASE("PartialRead" TEST_PREFIX "/findGapsTestBigger", "")
{
  PartialReadFixture f(TEST_PAGESIZE, TEST_INMEMORY, TEST_FLAGS);
  f.findGapsTestBigger();
}

/* read at offset 50000, partial size 50000, record size 200000 (gap
* at beginning and end) */
TEST_CASE("PartialRead" TEST_PREFIX "/findGapsTestBiggest", "")
{
  PartialReadFixture f(TEST_PAGESIZE, TEST_INMEMORY, TEST_FLAGS);
  f.findGapsTestBiggest();
}

/* read at offset 500000, partial size 500000, record size 2000000 (gap
* at beginning and end) */
TEST_CASE("PartialRead" TEST_PREFIX "/findGapsTestSuperbig", "")
{
  PartialReadFixture f(TEST_PAGESIZE, TEST_INMEMORY, TEST_FLAGS);
  f.findGapsTestSuperbig();
}

} // namespace

#undef TEST_PREFIX
#undef TEST_PAGESIZE
#undef TEST_INMEMORY
#undef TEST_FLAGS
