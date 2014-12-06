/**
 * Copyright (C) 2005-2013 Christoph Rupp (chris@crupp.de).
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * See files COPYING.* for License information.
 *
 *
 * A simple example which connects to a hamsterdb server (see server1.c),
 * creates a database, inserts some values, looks them up and erases them.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h> /* for exit() */
#include <ham/hamsterdb.h>

#define LOOP 10

void
error(const char *foo, ham_status_t st) {
  printf("%s() returned error %d: %s\n", foo, st, ham_strerror(st));
  exit(-1);
}

int
main(int argc, char **argv) {
  int i;
  ham_status_t st;      /* status variable */
  ham_env_t *env;       /* hamsterdb Environment object */
  ham_db_t *db;         /* hamsterdb Database object */
  ham_key_t key = {0};     /* the structure for a key */
  ham_record_t record = {0};   /* the structure for a record */

  /*
   * Connect to the server which should listen at 8080. The server is
   * implemented in server1.c.
   */
  st = ham_env_create(&env, "http://localhost:8080/env1.db", 0, 0, 0);
  if (st != HAM_SUCCESS)
    error("ham_env_create", st);

  /* now open a Database in this Environment */
  st = ham_env_open_db(env, &db, 13, 0, 0);
  if (st != HAM_SUCCESS)
    error("ham_env_open_db", st);

  /* now we can insert, delete or lookup values in the database */
  for (i = 0; i < LOOP; i++) {
    key.data = &i;
    key.size = sizeof(i);

    record.size = key.size;
    record.data = key.data;

    st = ham_db_insert(db, 0, &key, &record, 0);
    if (st != HAM_SUCCESS)
      error("ham_db_insert", st);
  }

  /* now lookup all values */
  for (i = 0; i < LOOP; i++) {
    key.data = &i;
    key.size = sizeof(i);

    st = ham_db_find(db, 0, &key, &record, 0);
    if (st != HAM_SUCCESS)
      error("ham_db_find", st);

    /* check if the value is ok */
    if (*(int *)record.data != i) {
      printf("ham_db_find() ok, but returned bad value\n");
      return (-1);
    }
  }

  /* erase everything */
  for (i = 0; i < LOOP; i++) {
    key.data = &i;
    key.size = sizeof(i);

    st = ham_db_erase(db, 0, &key, 0);
    if (st != HAM_SUCCESS)
      error("ham_db_erase", st);
  }

  /* and make sure that the database is empty */
  for (i = 0; i < LOOP; i++) {
    key.data = &i;
    key.size = sizeof(i);

    st = ham_db_find(db, 0, &key, &record, 0);
    if (st != HAM_KEY_NOT_FOUND)
      error("ham_db_find", st);
  }

  /* close the database handle */
  st = ham_db_close(db, 0);
  if (st != HAM_SUCCESS)
    error("ham_db_close", st);

  printf("success!\n");
  return (0);
}

