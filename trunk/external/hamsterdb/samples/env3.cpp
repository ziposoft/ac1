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

/**
 * This sample does the same as env2, but uses the C++ API.
 */

#include <iostream>
#include <stdlib.h> /* for exit() */
#include <ham/hamsterdb.hpp>

#define MAX_DBS           3

#define DBNAME_CUSTOMER   1
#define DBNAME_ORDER      2
#define DBNAME_C2O        3   /* C2O: Customer To Order */

#define DBIDX_CUSTOMER    0
#define DBIDX_ORDER       1
#define DBIDX_C2O         2

#define MAX_CUSTOMERS     4
#define MAX_ORDERS        8

/* A structure for the "customer" database */
typedef struct {
  uint32_t id;         /* customer id; will be the key of the customer table */
  char name[32];        /* customer name */
  /* ... additional information could follow here */
} customer_t;

/* A structure for the "orders" database */
typedef struct {
  uint32_t id;         /* order id; will be the key of the order table */
  uint32_t customer_id;/* customer id */
  char assignee[32];    /* assigned to whom? */
  /* ... additional information could follow here */
} order_t;

int
run_demo() {
  int i;
  hamsterdb::env env;       /* hamsterdb environment */
  hamsterdb::db db[MAX_DBS];  /* hamsterdb database objects */
  hamsterdb::cursor cursor[MAX_DBS]; /* a cursor for each database */
  hamsterdb::key key, cust_key, ord_key, c2o_key;
  hamsterdb::record record, cust_record, ord_record, c2o_record;

  customer_t customers[MAX_CUSTOMERS] = {
    { 1, "Alan Antonov Corp." },
    { 2, "Barry Broke Inc." },
    { 3, "Carl Caesar Lat." },
    { 4, "Doris Dove Brd." }
  };

  order_t orders[MAX_ORDERS] = {
    { 1, 1, "Joe" },
    { 2, 1, "Tom" },
    { 3, 3, "Joe" },
    { 4, 4, "Tom" },
    { 5, 3, "Ben" },
    { 6, 3, "Ben" },
    { 7, 4, "Chris" },
    { 8, 1, "Ben" }
  };

  /* Create a new hamsterdb environment */
  env.create("test.db");

  /*
   * Then create the two Databases in this Environment; each Database
   * has a name - the first is our "customer" Database, the second
   * is for the "orders"; the third manages our 1:n relation and
   * therefore needs to enable duplicate keys
   *
   * All database keys are uint32 types.
   */
  ham_parameter_t params[] = {
    {HAM_PARAM_KEY_TYPE, HAM_TYPE_UINT32},
    {0, }
  };

  /*
   * The "mapping" between customers and orders stores uint32 customer IDs
   * as a key and uint32 order IDs as a record
   */
  ham_parameter_t c2o_params[] = {
    {HAM_PARAM_KEY_TYPE, HAM_TYPE_UINT32},
    {HAM_PARAM_RECORD_SIZE, sizeof(uint32_t)},
    {0, }
  };

  db[DBIDX_CUSTOMER] = env.create_db(DBNAME_CUSTOMER, 0, &params[0]);
  db[DBIDX_ORDER]  = env.create_db(DBNAME_ORDER, 0, &params[0]);
  db[DBIDX_C2O]    = env.create_db(DBNAME_C2O,
                  HAM_ENABLE_DUPLICATE_KEYS, &c2o_params[0]);

  /* Create a cursor for each database */
  for (i = 0; i < MAX_DBS; i++)
    cursor[i].create(&db[i]);

  /*
   * Insert the customers in the customer table
   *
   * INSERT INTO customers VALUES (1, "Alan Antonov Corp.");
   * INSERT INTO customers VALUES (2, "Barry Broke Inc.");
   * etc
   */
  for (i = 0; i < MAX_CUSTOMERS; i++) {
    key.set_size(sizeof(int));
    key.set_data(&customers[i].id);

    record.set_size(sizeof(customer_t));
    record.set_data(&customers[i]);

    db[0].insert(&key, &record);
  }

  /*
   * And now the orders in the second database; contrary to env1,
   * we only store the assignee, not the whole structure
   *
   * INSERT INTO orders VALUES (1, "Joe");
   * INSERT INTO orders VALUES (2, "Tom");
   */
  for (i = 0; i < MAX_ORDERS; i++) {
    key.set_size(sizeof(int));
    key.set_data(&orders[i].id);

    record.set_size(sizeof(orders[i].assignee));
    record.set_data(orders[i].assignee);

    db[1].insert(&key, &record);
  }

  /*
   * And now the 1:n relationships; the flag HAM_DUPLICATE creates
   * a duplicate key, if the key already exists
   *
   * INSERT INTO c2o VALUES (1, 1);
   * INSERT INTO c2o VALUES (2, 1);
   * etc
   */
  for (i = 0; i < MAX_ORDERS; i++) {
    key.set_size(sizeof(int));
    key.set_data(&orders[i].customer_id);

    record.set_size(sizeof(int));
    record.set_data(&orders[i].id);

    db[2].insert(&key, &record, HAM_DUPLICATE);
  }

  /*
   * Now start the query - we want to dump each customer with his
   * orders
   *
   * loop over the customer; for each customer, loop over the 1:n table
   * and pick those orders with the customer id. then load the order
   * and print it
   *
   * the outer loop is similar to
   * SELECT * FROM customers WHERE 1;
   */
  while (1) {
    customer_t *customer;

    try {
      cursor[0].move_next(&cust_key, &cust_record);
    }
    catch (hamsterdb::error &e) {
      /* reached end of the database? */
      if (e.get_errno() == HAM_KEY_NOT_FOUND)
        break;
      else {
        std::cerr << "cursor.move_next() failed: " << e.get_string()
              << std::endl;
        return (-1);
      }
    }

    customer = (customer_t *)cust_record.get_data();

    /* print the customer id and name */
    std::cout << "customer " << customer->id << " ('"
          << customer->name << "')" << std::endl;

    /*
     * Loop over the 1:n table
     *
     * before we start the loop, we move the cursor to the
     * first duplicate key
     *
     * SELECT * FROM customers, orders, c2o
     *   WHERE c2o.customer_id=customers.id AND
     *    c2o.order_id=orders.id;
     */
    c2o_key.set_data(&customer->id);
    c2o_key.set_size(sizeof(int));

    try {
      cursor[2].find(&c2o_key);
    }
    catch (hamsterdb::error &e) {
      if (e.get_errno() == HAM_KEY_NOT_FOUND)
        continue;
      else {
        std::cerr << "cursor.find() failed: " << e.get_string()
              << std::endl;
        return (-1);
      }
    }

    /* get the record of this database entry */
    cursor[2].move(0, &c2o_record);

    do {
      int order_id;

      order_id = *(int *)c2o_record.get_data();
      ord_key.set_data(&order_id);
      ord_key.set_size(sizeof(int));

      /*
       * Load the order
       * SELECT * FROM orders WHERE id = order_id;
       */
      ord_record = db[1].find(&ord_key);

      std::cout << "  order: " << order_id << " (assigned to "
            << (char *)ord_record.get_data() << ")" << std::endl;

      /*
       * the flag HAM_ONLY_DUPLICATES restricts the cursor
       * movement to the duplicate list.
       */
      try {
        cursor[2].move(&c2o_key, &c2o_record,
              HAM_CURSOR_NEXT | HAM_ONLY_DUPLICATES);
      }
      catch (hamsterdb::error &e) {
        /* reached end of the database? */
        if (e.get_errno() == HAM_KEY_NOT_FOUND)
          break;
        else {
          std::cerr << "cursor.move() failed: " << e.get_string()
              << std::endl;
          return (-1);
        }
      }

    } while (1);
  }

  /*
   * we're done! no need to cleanup, the destructors will prevent memory
   * leaks
   */
  std::cout << "success!" << std::endl;
  return (0);
}

int
main(int argc, char **argv)
{
  try {
    return (run_demo());
  }
  catch (hamsterdb::error &e) {
    std::cerr << "run_demo() failed with unexpected error "
          << e.get_errno() << " ('"
          << e.get_string() << "')" << std::endl;
    return (-1);
  }
}
