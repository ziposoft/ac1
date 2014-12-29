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


#define U32 unsigned int

static int
	my_string_compare(ham_db_t *db, const ham_u8_t *lhs, size_t lhs_length,
	const ham_u8_t *rhs, size_t rhs_length) 

{


	int s = strncmp((const char *)lhs, (const char *)rhs,
		lhs_length < rhs_length ? lhs_length : rhs_length);
	if (s < 0)
		return -1;
	if (s > 0)
		return +1;
	return 0;
}

static int	my_int_compare(ham_db_t *db, const ham_u8_t *lhs, size_t lhs_length,
						   const ham_u8_t *rhs, size_t rhs_length) {

	U32 left=*(U32*)lhs;
	U32 right=*(U32*)rhs;
	if (left < right)
		return -1;
	if (left > right)
		return +1;
	return 0;
}

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

	record.set_size((uint32_t)strlen(data)+1);
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
	//ham_parameter_t param=		  { HAM_PARAM_KEYSIZE, keysize };
  ham_parameter_t params[] = { /* parameters for ham_env_create_db */
    {HAM_PARAM_KEY_TYPE, HAM_TYPE_UINT32},
   // {HAM_PARAM_RECORD_SIZE, sizeof(uint32_t)},
    {0, }
  };
	printf("running create %x %x\n",size,keysize);
	env.create(db_name);
	db = env.create_db(1,0,&params[0]);
	//db = env.create_db(1,0,0);
	//db.set_compare_func(my_int_compare);
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
	hamsterdb::cursor cursor_list;
	hamsterdb::cursor cursor_sort;

	env.open(db_name);
	db_list = env.open_db(1);
	db_sort = env.create_db(2,HAM_ENABLE_DUPLICATES);

	cursor_list.create(&db_list);
	cursor_sort.create(&db_sort);
	while (1) 
	{
		try 
		{
			cursor_list.move_next(&key, &record);
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
	env.close(HAM_AUTO_CLEANUP);
	return 0;

}

int dump(int db_num) {

	hamsterdb::env env;      /* hamsterdb environment object */
	hamsterdb::db db;      /* hamsterdb database object */
	hamsterdb::key key;      /* a key */
	hamsterdb::record record;  /* a record */
	hamsterdb::cursor cursor;

	env.open(db_name);
	db = env.open_db(db_num);

	cursor.create(&db);

	while (1) 
	{
		try 
		{
			cursor.move_next(&key, &record);
			printf("[%d]=%s\n",*(int*)key.get_data(),record.get_data());
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
	int record_count=(uint32_t)db.get_key_count();

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
	printf("get=%d\n",i);

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
/* static modules */
ZP_MODULE_INCLUDE(  ZP_MOD(logger));

int main(int argc, char* argv[])
{


	root o;
	o.console.setroot(&o);
	try {

		o.console.runapp(argc,argv,true);
	}

	catch (hamsterdb::error &e) {
		std::cerr << "run_demo() failed with unexpected error "
			<< e.get_errno() << " ('"
			<< e.get_string() << "')" << std::endl;
		return (-1);
	}
	return 0;
}


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
			sscanf_s(argv[3],"%d",&count);
		}
		if(argc>4)
		{	
			sscanf_s(argv[4],"%d",&param2);
		}
		if(argc>2)
		{
			if(*argv[2]=='c')
				create(count,param2);
			if(*argv[2]=='d')
				dump(count);
			if(*argv[2]=='f')
				find(argv[3]);
			if(*argv[2]=='g')
				get(count);
			if(*argv[2]=='t')
				test(count,param2);
			if(*argv[2]=='a')
				add(count,argv[4]);
		}





	}
	catch (hamsterdb::error &e) {
		std::cerr << "run_demo() failed with unexpected error "
			<< e.get_errno() << " ('"
			<< e.get_string() << "')" << std::endl;
		return (-1);
	}
}
