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

/*
 * A operating-system dependent mutex
 *
 * @exception_safe: nothrow
 * @thread_safe: yes
 */

#ifndef HAM_MUTEX_H
#define HAM_MUTEX_H

#include "0root/root.h"
#if 1
namespace hamsterdb {
	class Mutex  {
	public:

		void lock() {
		}

		void unlock() {
		}
	};
	class ScopedLock : public Mutex  {
	public:
		ScopedLock()
		{

		}
		ScopedLock(Mutex& m)
		{

		}
		void lock() {
		}

		void unlock() {
		}
	} ;

	class Thread  {
	public:

		void lock() {
		}

		void unlock() {
		}
	} ;
	class Condition  {
	public:

		void lock() {
		}

		void unlock() {
		}
	} ;
} // namespace hamsterdb
#else

#define BOOST_ALL_NO_LIB // disable MSVC auto-linking
#include <boost/version.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/tss.hpp>
#include <boost/thread/condition.hpp>

// Always verify that a file of level N does not include headers > N!

#ifndef HAM_ROOT_H
#  error "root.h was not included"
#endif

namespace hamsterdb {

typedef boost::mutex::scoped_lock ScopedLock;
typedef boost::thread Thread;
typedef boost::condition Condition;

class Mutex : public boost::mutex {
  public:
#if BOOST_VERSION < 103500
    typedef boost::detail::thread::lock_ops<boost::mutex> Ops;

    void lock() {
      Ops::lock(*this);
    }

    void unlock() {
      Ops::unlock(*this);
    }
#endif
};

} // namespace hamsterdb
#endif

#endif /* HAM_MUTEX_H__ */
