
typedef int32 sint32;
typedef float float32;

struct IOVector3Struct
{
	float32		x;
	float32		y;
	float32		z;
};

struct IOQuaternionStruct
{
	float32		x;
	float32		y;
	float32		z;
	float32		w;
};

#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif
