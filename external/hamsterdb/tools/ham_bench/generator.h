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

#ifndef HAM_BENCH_GENERATOR_H
#define HAM_BENCH_GENERATOR_H

#include "database.h"
#include "graph.h"

struct Configuration;

//
// base class for generating test data
//
class Generator
{
  public:
    enum {
      kCommandInsert = 0,
      kCommandFind,
      kCommandErase,
      kCommandCommitTransaction,
      kCommandTablescan,
      kCommandCreate,
      kCommandOpen,
      kCommandClose,
      kCommandBeginTransaction,
      kCommandAbortTransaction,
      kCommandFlush,
      kCommandNop,
      kCommandFullcheck = 999999 // avoid conflicts with ham_status_t
    };

    // constructor
    Generator(int id, Configuration *conf, Database *db)
      : m_id(id), m_config(conf), m_db(db), m_last_status(0), m_graph(0) {
      memset(&m_record, 0, sizeof(m_record));
      memset(&m_opspersec, 0, sizeof(m_opspersec));

      // only create graph output for the first hamsterdb thread!
      if (conf->metrics >= Configuration::kMetricsPng
           && !strcmp(db->get_name(), "hamsterdb")
           && id == 0) {
        m_graph = new Graph("hamsterdb");
      }
    }

    // destructor
    virtual ~Generator() {
      if (m_graph) {
        delete m_graph;
        m_graph = 0;
      }
    }

    // Returns the database
    Database *get_db() {
      return (m_db);
    }
    
    // Returns the status of the last database operation
    ham_status_t get_status() const {
      return (m_last_status);
    }

    // executes the next generated statement; returns true if more statements
    // will follow, false when the test is done
    virtual bool execute() = 0;

    // opens the Environment; used for 'reopen'
    virtual void open() = 0;

    // closes the Environment; used for 'reopen'
    virtual void close() = 0;

    // returns true if the test was successful
    virtual bool was_successful() const = 0;

    // returns the collected metrics/statistics
    virtual void get_metrics(Metrics *metrics) = 0;

    // Returns the previously retrieved record
    const ham_record_t *get_record() const {
      return (&m_record);
    }

    // "tee"s the generated test data to a file (and/or to stdout 
    // if 'verbose' is enabled)
    virtual void tee(const char *foo, const ham_key_t *key = 0,
                    const ham_record_t *record = 0) {
    }

  protected:
    // unique ID - used to create the database
    int m_id;

    // the configuration settings
    Configuration *m_config;

    // the Database
    Database *m_db;

    // the status of the previous database operation
    ham_status_t m_last_status;

    // A record to store data from the last call to find() and
    // cursor_find(); used by the caller to compare results from
    // different databases
    ham_record_t m_record;

    // the performance graphs
    Graph *m_graph;

    // accumulating operations-per-seconds for the graphs
    uint32_t m_opspersec[4];
};

#endif /* HAM_BENCH_GENERATOR_H */
