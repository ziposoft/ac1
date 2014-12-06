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
 * This sample demonstrates the use of duplicate items. Every line is
 * split into words, and each word is inserted with its line number.
 * Then a cursor is used to print all words in a sorted order, with the
 * lines in which the word occurred.
 */

#include <stdio.h>
#include <string.h>
#include <ham/hamsterdb.h>

#define DATABASE_NAME     1

char* words[]=
{
	"fred","larry","ant","tod","1234","3445","1" 
};

int
main(int argc, char **argv) {
  ham_status_t st;          /* status variable */
  ham_env_t *env;           /* hamsterdb environment object */
  ham_db_t *db;             /* hamsterdb database object */
  ham_cursor_t *cursor;     /* a database cursor */
  ham_key_t key;
  ham_record_t record;
  int i;

  memset(&key, 0, sizeof(key));
  memset(&record, 0, sizeof(record));

  printf("This sample uses hamsterdb and duplicate keys to list all words "
      "in the\noriginal order, together with their line number.\n");
  printf("Reading from stdin...\n");

  /* Create a new Database with support for duplicate keys */
  st = ham_env_create(&env, 0, HAM_IN_MEMORY, 0664, 0);
  if (st != HAM_SUCCESS) {
    printf("ham_env_create() failed with error %d\n", st);
    return (-1);
  }
  st = ham_env_create_db(env, &db, DATABASE_NAME,
      0,    
	  //HAM_ENABLE_EXTENDED_KEYS | HAM_ENABLE_DUPLICATES,
		  0);
  if (st != HAM_SUCCESS) {
    printf("ham_env_create_db() failed with error %d\n", st);
    return (-1);
  }

	for(i=0;i<sizeof(words)/sizeof(char*);i++)
	{
      key.data = words[i];
      key.size = (ham_size_t)strlen(words[i]) + 1; /* also store the terminating
                                             * 0-byte */
      record.data = &i;
      record.size = sizeof(i);

      st = ham_db_insert(db, 0, &key, &record, 0 /* HAM_DUPLICATE*/);
      if (st != HAM_SUCCESS) {
        printf("ham_db_insert() failed with error %d\n", st);
        return (-1);
      }

    
  }

  /* Create a cursor */
  st = ham_cursor_create(&cursor, db, 0, 0);
  if (st != HAM_SUCCESS) {
    printf("ham_cursor_create() failed with error %d\n", st);
    return (-1);
  }

  /* Iterate over all items and print them */
  while (1) {
    st = ham_cursor_move(cursor, &key, &record, HAM_CURSOR_NEXT);
    if (st != HAM_SUCCESS) {
      /* reached end of the database? */
      if (st == HAM_KEY_NOT_FOUND)
        break;
      else {
        printf("ham_cursor_next() failed with error %d\n", st);
        return (-1);
      }
    }

    /* print the word and the line number */
    printf("%s: appeared in line %u\n", (const char *)key.data,
        *(unsigned *)record.data);
  }

  /*
   * Then close the handles; the flag HAM_AUTO_CLEANUP will automatically
   * close all cursors and we do not need to call ham_cursor_close and
   * ham_db_close
   */
  st = ham_env_close(env, HAM_AUTO_CLEANUP);
  if (st != HAM_SUCCESS) {
    printf("ham_env_close() failed with error %d\n", st);
    return (-1);
  }

  /* success! */
  return (0);
}

