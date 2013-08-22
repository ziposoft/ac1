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
* A simple example, which creates a database, inserts some values,
* looks them up and erases them. Uses the C++ api.
*/

#include <iostream>
#include <string>
#include <ham/hamsterdb.hpp>

char*  db_name="test.db";
char get_rand_char()
{
	int r=rand()%26;
	r=r+'A';
	return (char)r;
}
int add(int ikey,char* data) {
	hamsterdb::env env;      /* hamsterdb environment object */
	hamsterdb::db db;      /* hamsterdb database object */
	hamsterdb::key key;      /* a key */
	hamsterdb::record record;  /* a record */

	/* Create a new environment file and a database in this environment */
	env.open(db_name);
	db = env.open_db(1);



	/*
	* Now we can insert, delete or lookup values in the database
	*
	* for our test program, we just insert a few values, then look them
	* up, then delete them and try to look them up again (which will fail).
	*/
	key.set_size(sizeof(ikey));
	key.set_data(&ikey);

	record.set_size((UINT32)strlen(data)+1);
	record.set_data((void*)data);

	db.insert(&key, &record);
	db.close();
	env.close();
	return 0;
}
int create(int size,int keysize) {
	int i;
	hamsterdb::env env;      /* hamsterdb environment object */
	hamsterdb::db db;      /* hamsterdb database object */
	hamsterdb::key key;      /* a key */
	hamsterdb::record record;  /* a record */
	if(keysize==0)
		keysize=21;
	 ham_parameter_t param=		  { HAM_PARAM_KEYSIZE, keysize };
	env.create(db_name);
	db = env.create_db(1,0,&param);
	char buf[12];
	for (i = 0; i < size; i++)
	{
		key.set_size(sizeof(i));
		key.set_data(&i);
		int len=rand()%10;
		int j;
		for(j=0;j<len;j++)
			buf[j]=get_rand_char();
		buf[j]=0;
		record.set_size(len+1);
		record.set_data((void*)buf);
		db.insert(&key, &record);
	}
	db.close();
	env.close();
	return 0;
}
int sort() {
	
	hamsterdb::env env;      /* hamsterdb environment object */
	hamsterdb::db db_list,db_sort;      /* hamsterdb database object */
	hamsterdb::key key;      /* a key */
	hamsterdb::record record;  /* a record */
	hamsterdb::cursor cursor;

	env.open(db_name);
	db_list = env.open_db(1);

	cursor.create(&db);
	while (1) 
	{
		try 
		{
			cursor.move_next(&key, &record);
			if(strcmp((char*)record.get_data(),str)==0)
			{
				printf("data=%s found at key %d\n",str,*(int*)key.get_data());

			}
		}
		catch (hamsterdb::error &e) 
		{
			/* reached end of the database? */
			if (e.get_errno() == HAM_KEY_NOT_FOUND)
			{
				printf("reached end \n");
				break;
			}
			else 
			{
				std::cerr << "cursor.move_next() failed: " << e.get_string()
					<< std::endl;
				return (-1);
			}
		}
	}

	/* Check if the value is ok */


	/*
	* close the database handle, then re-open it (just to demonstrate how
	* to open a database file)
	*/
	cursor.close();
	db.close();
	env.close();
	return 0;

}

int find(char* str) {
	
	hamsterdb::env env;      /* hamsterdb environment object */
	hamsterdb::db db;      /* hamsterdb database object */
	hamsterdb::key key;      /* a key */
	hamsterdb::record record;  /* a record */
	hamsterdb::cursor cursor;

	env.open(db_name);
	db = env.open_db(1);

	cursor.create(&db);
	/*
	* Now lookup all values
	*
	* for db::find(), we could use the flag HAM_RECORD_USER_ALLOC, if WE
	* allocate record.data (otherwise the memory is automatically allocated
	* by hamsterdb)
	*/

	while (1) 
	{
		try 
		{
			cursor.move_next(&key, &record);
			if(strcmp((char*)record.get_data(),str)==0)
			{
				printf("data=%s found at key %d\n",str,*(int*)key.get_data());

			}
		}
		catch (hamsterdb::error &e) 
		{
			/* reached end of the database? */
			if (e.get_errno() == HAM_KEY_NOT_FOUND)
			{
				printf("reached end \n");
				break;
			}
			else 
			{
				std::cerr << "cursor.move_next() failed: " << e.get_string()
					<< std::endl;
				return (-1);
			}
		}
	}

	/* Check if the value is ok */


	/*
	* close the database handle, then re-open it (just to demonstrate how
	* to open a database file)
	*/
	cursor.close();
	db.close();
	env.close();
	return 0;

}
int test(int iter,int show) {
	
	hamsterdb::env env;      /* hamsterdb environment object */
	hamsterdb::db db;      /* hamsterdb database object */
	hamsterdb::key key;      /* a key */
	hamsterdb::record record;  /* a record */


	env.open(db_name);
	db = env.open_db(1);
	int record_count=(UINT32)db.get_key_count();
	
	/*
	* Now lookup all values
	*
	* for db::find(), we could use the flag HAM_RECORD_USER_ALLOC, if WE
	* allocate record.data (otherwise the memory is automatically allocated
	* by hamsterdb)
	*/
	int i;
	key.set_size(sizeof(i));
	for(i=0;i<iter;i++)
	{
		int r=rand()%record_count;
		key.set_data(&r);
		record = db.find(&key);
		if(show)
		{
			printf("data=%s found at key %d\n",(char*)record.get_data(),*(int*)key.get_data());

		}
	}



	/*
	* close the database handle, then re-open it (just to demonstrate how
	* to open a database file)
	*/
	db.close();
	env.close();
	return 0;

}
int get(int i) {
	
	hamsterdb::env env;      /* hamsterdb environment object */
	hamsterdb::db db;      /* hamsterdb database object */
	hamsterdb::key key;      /* a key */
	hamsterdb::record record;  /* a record */


	env.open(db_name);
	db = env.open_db(1);
	/*
	* Now lookup all values
	*
	* for db::find(), we could use the flag HAM_RECORD_USER_ALLOC, if WE
	* allocate record.data (otherwise the memory is automatically allocated
	* by hamsterdb)
	*/
	key.set_size(sizeof(i));
	key.set_data(&i);

	record = db.find(&key);

	/* Check if the value is ok */
	printf("data=%s\n",(char*)record.get_data());


	/*
	* close the database handle, then re-open it (just to demonstrate how
	* to open a database file)
	*/
	db.close();
	env.close();
	return 0;

}

#if 0
/* now erase all values */
for (i = 0; i < LOOP; i++) {
	key.set_size(sizeof(i));
	key.set_data(&i);

	db.erase(&key);
}

/*
* Once more we try to find all values. Every db::find() call must
* now fail with HAM_KEY_NOT_FOUND
*/
for (i = 0; i < LOOP; i++) {
	key.set_size(sizeof(i));
	key.set_data(&i);

	try {
		record = db.find(&key);
	}
	catch (hamsterdb::error &e) {
		if (e.get_errno() != HAM_KEY_NOT_FOUND) {
			std::cerr << "db::find() returned error " << e.get_string()
				<< std::endl;
			return (-1);
		}
	}
}

/*
* Done! No need to close the database handles, they are closed in their
* destructor.
*/

std::cout << "success!" << std::endl;
return (0);
}
#endif

int
main(int argc, char **argv)
{
	int count=1;
	int param2=0;
	try {
		if(argc<2)
		{
			printf("%s [db name] [command] ...\n",argv[0]);
			return -1;
		}
		db_name=argv[1];
		if(argc>3)
		{	
			sscanf_s(argv[3],"%x",&count);
		}
		if(argc>4)
		{	
			sscanf_s(argv[4],"%d",&param2);
		}
		if(argc>2)
		{
			if(*argv[2]=='c')
				create(count,param2);
			if(*argv[2]=='f')
				find(argv[3]);
			if(*argv[2]=='g')
				get(count);
			if(*argv[2]=='t')
				test(count,param2);
			if(*argv[2]=='a')
				add(count,argv[3]);
		}





	}
	catch (hamsterdb::error &e) {
		std::cerr << "run_demo() failed with unexpected error "
			<< e.get_errno() << " ('"
			<< e.get_string() << "')" << std::endl;
		return (-1);
	}
}
