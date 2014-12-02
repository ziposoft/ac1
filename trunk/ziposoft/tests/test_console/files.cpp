// test_console.cpp : Defines the entry point for the console application.
//

#include "test_console.h"
#include "zipolib/include/z_factory_static.h"
#if 0
class z_intf_random_access
{
public:
	z_intf_random_access()
	{
		total_size=0;
		offset=0;
		width=1;
		length=0x10;

	}
 	size_t total_size;

	int offset;
	int width;
	int length;
	virtual int read(int offset,int width,U64& data)
	{
		return zs_not_implemented;
	}
	virtual int write(int offset,int width,U64 data)
	{
		return zs_not_implemented;
	}
	virtual int dump()
	{

		int i;
		z_status status;
		for (i = offset; i < (offset+length); i+=width)
		{
			U64 data;
			if( (i%0x10)==0)
				printf("\n%02x : ",i);

			status=read(i,width,data);
			if(status)
				return status;
			switch(width)
			{
			case 1:		
				//TODO support BIG endian
				printf("%02x ",(U8) data);break; 
			case 2:			
				printf("%04x ",(U16) data);break;
			case 4:			
				printf("%04x ",(U32) data);break;
			case 8:			
				printf("%08llx ",(U64) data);break;
			default:
				printf("Invalid width: %d\n",width);
				break;
			}
		}
		printf("\n");
		return 0;
	}
	virtual int write_pattern_incrementing()
	{
		return 0;
	}
	virtual int write_pattern_set()
	{

		return 0;
	}
};

class z_intf_mapped_access: public  z_intf_random_access
{
public:

	z_intf_mapped_access()
	{
		_p_data=0;

	}
	char* _p_data;
	virtual int read(int offset,int width,U64& data)
	{
		U8* p;
		p=(U8*)_p_data+offset;


		switch(width)
		{
		case 1:		
			data=*(U8*)p;
			break;
		case 2:			
			data=*(U16*)p;
			break;
		case 4:			
			data=*(U32*)p;
			break;
		case 8:			
			data=*(U64*)p;
			break;
		default:
			break;
		}
		return 0;
	}
	virtual int write(int offset,int width,U64 data)
	{
		return zs_not_implemented;
	}
};
class z_binary_file	: public  z_intf_mapped_access
{
public:
	z_file _file;

	z_string filename;
	virtual int load()
	{
		z_status status=_file.open(filename,"rb");
		if(status==zs_ok)
		{
			_file.read_all(_p_data,total_size);
			_file.close();
		}
		return status;
	}
	virtual int save()
	{
		z_status status=_file.open(filename,"wb");
		if(status==zs_ok)
		{
			_file.write(_p_data,total_size);
			_file.close();
		}
		return status;
	}


};

class dog
{
public:
	dog()
	{
		_name="george";
	}
	dog(ctext name)
	{
		_name=name;
	}
	z_string _name;
	ctext get_map_key() { return _name.c_str(); }

};

class root
{
public:
	root()
	{
		x.push_back("x");
		x<<"y";
		s="ant\"hony";
		i=27;

		dog_vect << new dog("fido")<< new dog("jorge");
		dog_map << new dog("ralph")<< new dog("dooright");

	}
	z_console console;
	z_logger* _p_logger;
	z_binary_file binfile;
	z_obj_vector_map <dog> dog_vect;
	z_obj_map  <dog> dog_map;
	z_strlist x;
	int i;
	z_string s;
	int add() {  x<<s;return 0;}
	int show() { zout << '\n'<<s<<'\n';return 0;}


};



ZFACT(z_intf_random_access)
{
	ZACT(dump);
	ZACT(write_pattern_incrementing);
	ZACT(write_pattern_set);

	
	ZPROP(offset);
	ZPROP(width);
	ZPROP(length);
};

ZFACT_V(z_binary_file,z_intf_random_access)
{
	ZPROP(filename);
	ZACT_XP(save,"save",ZFF_ACT_DEF,"Save to file",1,ZPARAM(filename));
	ZACT_XP(load,"load",ZFF_ACT_DEF,"Load from file",1,ZPARAM(filename));
	

};	

ZFACT(dog)
{
	ZPROP(_name);
};
	

ZFACT(root)
{
	ZOBJ(console);
	ZOBJ(binfile);
	ZPOBJ(_p_logger);
	ZPROP(dog_vect);
	ZPROP(dog_map);
	ZPROP(x);
	ZPROP(i);
	ZACT(add);
	ZACT_XP(show,"show",ZFF_ACT_DEF,"desc",1,ZPARAM(s));

};	  
#ifdef BUILD_VX
int ztest()
{
	zout.set_handle((size_t)stdout);
	gz_in.set_handle((size_t)stdin);
	zout << "starting ztest\n";
	
	root o;
	o.console.setroot(&o);
	o.console.run();
	return 0;

}
#else


#endif
#endif


