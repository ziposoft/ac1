// test_console.cpp : Defines the entry point for the console application.
//

#include "zipolib/include/zipolib_cpp.h"
#include "zipolib/include/z_console.h"
#include "zipolib/include/z_factory_static.h"

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
		if(!_p_data)
			return zs_not_open;
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
	virtual int newfile()
	{
		_p_data=new char[total_size];
		z_status status=save();


		return status;
	}

};

ZFACT(z_intf_random_access)
{
	ZACT_XP(dump,"dump",0,"Dump data",3,
		ZPARAM(offset),
		ZPARAM(length),

		ZPARAM(width)
		);

	ZACT(write_pattern_incrementing);
	ZACT(write_pattern_set);

	
	ZPROP(offset);
	ZPROP(width);
	ZPROP(length);
};
ZFACT_V(z_intf_mapped_access,z_intf_random_access)
{



}
ZFACT_V(z_binary_file,z_intf_mapped_access)
{
	ZPROP(filename);
	ZACT_XP(save,"save",0,"Save to file",1,ZPARAM(filename));
	ZACT_XP(newfile,"new",0,"Create new file",2,
		ZPARAM(filename),ZPARAM(length)
		);
	ZACT_XP(load,"load",0,"Load from file",1,ZPARAM(filename));
	

};

class VmeMaster	: public  z_intf_mapped_access
{
public:
	VmeMaster()
	{
	}

};
class Vme
{
public:
	Vme()
	{
		int i;
		for(i=0;i<8;i++)
			master << new VmeMaster();
	}
	z_obj_vector <VmeMaster> master;

};
ZFACT_V(VmeMaster,z_intf_mapped_access)
{
};

ZFACT(Vme)
{
	ZPROP(master);
};

class root
{
public:
	root()
	{
		_p_logger=&gz_logger;
	}
	z_console console;
	z_logger* _p_logger;
	z_binary_file binfile;
	Vme vme;


};
ZFACT(root)
{
	ZOBJ(console);
	ZOBJ(binfile);
	ZPOBJ(_p_logger);
	ZOBJ(vme);

};
#ifdef BUILD_VX
int ztest()
{
	gz_out.set_handle((size_t)stdout);
	gz_in.set_handle((size_t)stdin);
	gz_out << "starting ztest\n";
	
	root o;
	o.console.setroot(&o);
	o.console.run();
	return 0;

}
#else
int main(int argc, char* argv[])
{

	root o;
	o.console.setroot(&o);
	o.console.init(argv[0]);
	o.console.loadcfg();
	o.console.runapp(argc,argv);
	o.console.savecfg();

	return 0;
}

#endif




//ZP_MODULE_INCLUDE( ZP_MOD(parse), ZP_MOD(logger));
ZP_MODULE_INCLUDE(  ZP_MOD(logger));

#pragma comment(linker, "/alternatename:zp_module_master_list_size=zp_module_master_list_size_exe")

