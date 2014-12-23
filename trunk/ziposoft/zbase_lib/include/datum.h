#ifndef DATUM_H
#define DATUM_H
#include "zbase_lib/include/zb.h"


class zb_datum: public z_refcount
{
public:
	template <class _TYPE> z_status get<_TYPE>(_TYPE& data);
	template <class _TYPE> z_status set<_TYPE>(_TYPE& data);

};

class zb_datum_int32 : public zb_datum
{
	I32 _data; 
public:
	zb_datum_int32(I32 data) { _data=data; }
};
class zb_datum_int64 : public zb_datum
{
	I64 _data; 
public:
	zb_datum_int64(I64 data) { _data=data; }
};
class zb_datum_string : public zb_datum
{
	z_string _data; 
public:
	zb_datum_string(ctext data);
};



#endif
