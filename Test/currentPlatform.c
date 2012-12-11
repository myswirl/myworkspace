#include <stdio.h>
int main(void)
{
	printf("------ Current Compile Environment ------ \n");
#if defined(_WIN32) && !defined(__GNUC__) && defined(_M_IX86)
	#define _ENV_PLATFORM_32BIT
	printf("32 bit Windows, MSVC.\n");
#elif defined(__APPLE__) && defined(__i386__)
	#define _ENV_PLATFORM_32BIT
	printf("32 bit MacOS.\n");
#elif defined(__sun) && defined(__i386)
	#define _ENV_PLATFORM_32BIT
	printf("32 bit Solaris.\n");
#elif defined(__linux__) && defined(__i386__)
	#define _ENV_PLATFORM_32BIT
	printf("32 bit Linux.\n");
#elif defined(__APPLE__) && defined(__x86_64__)
	#define _ENV_PLATFORM_64BIT
	printf("64 bit MacOS.\n");	
#elif defined(__linux__) && defined(__x86_64__)
	#define _ENV_PLATFORM_64BIT
	printf("64 bit Linux.\n");
#elif defined(__linux__) && defined(__tilegx__)
	#define _ENV_PLATFORM_64BIT
	printf("64 bit Linux Tilera, tilegx.\n");
#elif defined(__sun) && defined(__x86_64)
	#define _ENV_PLATFORM_64BIT
	printf("64 bit Solaris.\n");
#elif defined(_MSC_VER) && defined(_M_X64)
	#define _ENV_PLATFORM_64BIT
	printf("64 bit Windows.\n");
#else
	printf("Unknown platform ,not support!\n");
#endif

	printf("------ Data Type Define ------\n");
#ifdef _ENV_PLATFORM_32BIT
	typedef unsigned char     DJ_U8;
	typedef char              DJ_S8;
	typedef unsigned short    DJ_U16;
	typedef short             DJ_S16;
	typedef unsigned long     DJ_U32;
	typedef long              DJ_S32;
	typedef unsigned long long DJ_U64;
	typedef long long         DJ_S64;
	typedef float    					DJ_F32;
	typedef double   					DJ_F64;
	typedef int              	DJ_SI32;
	typedef unsigned int     	DJ_UI32;
#elif _ENV_PLATFORM_64BIT
	typedef unsigned char     DJ_U8;
	typedef char              DJ_S8;
	typedef unsigned short    DJ_U16;
	typedef short             DJ_S16;
	typedef unsigned long     DJ_U32;
	typedef long              DJ_S32;
	typedef unsigned long long DJ_U64;
	typedef long long         DJ_S64;
	typedef float    					DJ_F32;
	typedef double   					DJ_F64;
	typedef int              	DJ_SI32;
	typedef unsigned int    	DJ_UI32;
#endif

	if(sizeof(DJ_U8) != 1)
	{
		printf("Error, DJ_U8 define wrong!\n");
	}	
	if(sizeof(DJ_S8) != 1)
	{
		printf("Error, DJ_S8 define wrong!\n");
	}
	if(sizeof(DJ_U16) != 2)
	{
		printf("Error, DJ_U16 define wrong!\n");
	}
	if(sizeof(DJ_S16) != 2)
	{
		printf("Error, DJ_S16 define wrong!\n");
	}
	if(sizeof(DJ_U32) != 4)
	{
		printf("Error, DJ_U32 define wrong!\n");
	}
	if(sizeof(DJ_S32) != 4)
	{
		printf("Error, DJ_S32 define wrong!\n");
	}
	if(sizeof(DJ_U64) != 8)
	{
		printf("Error, DJ_U64 define wrong!\n");
	}
	if(sizeof(DJ_S64) != 8)
	{
		printf("Error, DJ_S64 define wrong!\n");
	}
	if(sizeof(DJ_F32) != 4)
	{
		printf("Error, DJ_F32 define wrong!\n");
	}
	if(sizeof(DJ_F64) != 8)
	{
		printf("Error, DJ_F64 define wrong!\n");
	}
	if(sizeof(DJ_SI32) != 4)
	{
		printf("Error, DJ_SI32 define wrong!\n");
	}
	if(sizeof(DJ_UI32) != 4)
	{
		printf("Error, DJ_UI32 define wrong!\n");
	}
		
	return 0;	
}
