// test_console.cpp : Defines the entry point for the console application.
//
#include "zipolib/include/z_console.h"
#include "parse_xml.h"
#include "zipolib/include/z_filesystem.h"


/*
class z_xml_activity : public z_xml_elm
{
public:


};
class z_xml_activity_type : public z_xml_elm_type_T<z_xml_activity>
{
public:
	z_xml_activity_type() : 
	z_obj_vector<z_xml_activity> _activities;

	virtual void callback_new_elm(z_xml_activity* elm) 
	{
		_activities<<elm;
	};


};
*/

class z_map_elm : public z_stl_map<z_string,z_xml_elm*> {};
class root
{
public:
	root()
	{
		_p_logger=&z_logger_get();
		_blank_xml_filename="blanktemplate.tcx";
		_param_xml_filename="test.xml";
		_csv_file_name="listing.csv";

		_move_operation=false;
	}
	z_map_elm _activities_map;

	bool _move_operation;
	
	z_string _csv_file_name;
	z_string _blank_xml_filename;
	z_console console;
	z_logger* _p_logger;
	z_string _param_xml_filename;
	z_status act_dir();
	z_status act_move();
	z_status act_parse();
	z_status act_dump();
	z_xml_parser _parser;
	z_xml_parser _parser_new;

	z_file _new_list_file;
	z_xml_elm* _new_activities;
	z_status parse_tcx(ctext name,z_file& fileout);
};
z_status root::act_move()
{
	z_status status;

	
	_activities_map.clear();

	_move_operation=true;
	_new_list_file.open("new_list.csv","w");
	act_dir();

	int year;
	int month;

	for(year=2005;year<2016;year++)
	{
		for(month=1;month<13;month++)
		{
			int num_matched=0;
			z_string yearmonth;
			z_string filename_tcx;
			yearmonth.Format("%04d-%02d",year,month);
			printf("processing %s\n",yearmonth.c_str());
			filename_tcx<<yearmonth <<".tcx";


			_parser_new.reset();
			status= _parser_new.parse_file(_blank_xml_filename);
			if(status)
				return status;
			z_xml_elm* elm=_parser_new._root_node;
			if(!elm)
				return Z_ERROR_MSG(zs_child_not_found,"no root node?");
			_new_activities=elm->get_first_elm("Activities");
			if(!_new_activities)
				return Z_ERROR_MSG(zs_child_not_found,"Could not find _new_activities node");



			z_xml_elm* node_activity;
			z_string key;
			z_map_elm::iter i_elm=_activities_map.begin();
			while(_activities_map.get_next(i_elm,key,node_activity))
			{
				if(key.compare(0,7,yearmonth)==0)
				{
					num_matched++;
					node_activity->move_to_new_parent(_new_activities);
					_new_activities->add_child_item(new z_xml_data("\n"));
				}
			}

			if(num_matched)
			{


				z_file newfile;
				newfile.open(filename_tcx,"w");
				_parser_new.output(newfile);
			}
		}
	}
	return zs_ok;
}

z_status root::parse_tcx(ctext filename,z_file& total_list)
{
	z_status status;

//	z_xml_activity_type activites;

	_parser.reset();

	status= _parser.parse_file(filename);
	if(status)
		return status;

	z_xml_elm* elm=_parser._root_node;
	if(!elm)
		return Z_ERROR_MSG(zs_child_not_found,"no root node?");
	z_xml_elm* node_activities=elm->get_first_elm("Activities");
	if(!node_activities)
		return Z_ERROR_MSG(zs_child_not_found,"Could not find Activities node");


	size_t cursor_activity=0;
	while(1)
	{
		z_file_string_buffer csv_line;
		z_string id;
		z_string date;
		z_string data;
		z_string year;
		z_string tod;
		z_xml_elm* node_activity=node_activities->get_next_elm(cursor_activity,"Activity");
		if(!node_activity)
			break;
		elm=node_activity->get_first_elm("Id");
		if(!elm)
			return Z_ERROR_MSG(zs_child_not_found,"Could not find Id node");
		elm->get_data(id);
		
		tod.assign(id,11,8);
		date=id;
		date.resize(10);
		year=date;
		year.resize(4);

		csv_line <<filename<<","<<id<<","<< node_activity->get_attrib("Sport") <<',';
		csv_line << date ;
		csv_line << ','<< tod ;
		size_t cursor_lap=0;
		int distance_meters=0;
		int time_seconds=0;
		double miles=0,pace=0,minutes=0;
		int laps=0;
		bool has_track=false;
		while(1)
		{
			z_string data;
			z_xml_elm* node_lap=node_activity->get_next_elm(cursor_lap,"Lap");
			if(!node_lap)
				break;
			laps++;
			z_xml_elm* dist=node_lap->get_first_elm("DistanceMeters");
			if(!dist)
			{
				Z_ERROR_MSG(zs_child_not_found,"No distance in lap?");
				break;
			}
			dist->get_data(data);
			distance_meters+=data.GetDecVal();

			z_xml_elm* node_time=node_lap->get_first_elm("TotalTimeSeconds");
			if(!node_time)
			{
				Z_ERROR_MSG(zs_child_not_found,"No time in lap?");
				break;
			}
			data="";
			node_time->get_data(data);
			time_seconds+=data.GetDecVal();
			has_track=(node_lap->get_first_elm("Track")!=0);


		}
		miles=distance_meters;
		miles=miles/1609;
		minutes=time_seconds;
		minutes=minutes/60;
		if(miles)
			pace=minutes/miles;
		csv_line.putf(",%.2lf",miles);
		csv_line.putf(",%.2lf",minutes);
		csv_line.putf(",%.2lf",pace);
		csv_line<<  (has_track? ",TRACK" : ",empty")<<"\n";
		total_list << csv_line.get_buffer();
		if(!_move_operation)
			continue;

		if(has_track)
		{
			if((miles>0.20))
			{
				if(_activities_map.exists(id)==false)
				{
					node_activity->move_to_new_parent(0);
					_activities_map[id]=node_activity;
					_new_list_file << csv_line.get_buffer();

				}
			}
		}


		

	}
    return status;

}

z_status root::act_parse()
{
	z_file csvfile(_csv_file_name,"w");

	return parse_tcx(_param_xml_filename,csvfile);
}
z_status root::act_dump()
{
	_parser.output(zout);
	return zs_ok;
}
z_status root::act_dir()
{
	z_directory dir;
	z_status status;
	z_strlist list;
	dir.open("input");
	z_file csvfile(_csv_file_name,"w");
	//zout <<  dir.get_path();
	dir.get_files_by_extension("tcx",list);
	size_t i;
	for (i=0;i<list.size();i++)
	{
		z_string path;
		path= dir.get_path();
		path<< "/" << list[i];
		zout << "Parsing "<< path << "\n";
		if(status= parse_tcx(path,csvfile))
			break;
	}
	csvfile.close();
	return status;
}


ZFACT(root)
{
	ZOBJ(console);
	ZOBJ_X(_parser,"p",ZFF_PROP,"parser");
	ZPOBJ(_p_logger,"log",ZFF_PROP,"Logger");
	ZPROP_X(_csv_file_name,"csvfile",ZFF_PROP ,"  ") ;
	ZACT_XP(act_move,"move",ZFF_ACT_DEF,"move",0);
	ZACT_XP(act_dump,"dump",ZFF_ACT_DEF,"dump",0);
	ZACT_XP(act_dir,"dir",ZFF_ACT_DEF,"dump",0);
	ZACT_XP(act_parse,"parse",ZFF_ACT_DEF,"parse",1,
		ZPARAM_X(_param_xml_filename,"filename",ZFF_PARAM,"Name of file"));

};
int main(int argc, char* argv[])
{

	root o;
	o.console.setroot(&o);
	o.console.runapp(argc,argv,true);

	return 0;
}





ZP_MODULE_INCLUDE(  ZP_MOD(logger));
