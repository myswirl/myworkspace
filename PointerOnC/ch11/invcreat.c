#include <stdlib.h>
#include <stdio.h>
#include "inventor.h"

//用于创建SUBASSEMBLY(装配件)存货记录的函数
Invrec *create_subassy_record( int n_parts )
{
	Invrec	*new_rec;

	//试图为Invrec分配内存
	new_rec = malloc( sizeof( Invrec ) );
	if( new_rec != NULL ){
		
		new_rec->info.subassy = malloc( sizeof( Subassyinfo ) );
		if( new_rec->info.subassy != NULL ){
			//为零件获取一个足够大的数组
			new_rec->info.subassy->part = malloc(
			    n_parts * sizeof( struct SUBASSYPART ) );
			if( new_rec->info.subassy->part != NULL ){
				//填充已知值字段，然后返回
				new_rec->type = SUBASSY;
				new_rec->info.subassy->n_parts = n_parts;
				return new_rec;
			}

			//内存已近用完，释放我们原先分配的内存
			free( new_rec->info.subassy );
		}
		free( new_rec );
	}
	return NULL;
}

//释放存货记录的函数
void discard_inventory_record( Invrec *record )
{
	/*
	** Delete the variant parts of the record
	*/
	switch( record->type ){
	case SUBASSY:
		free( record->info.subassy->part );
		free( record->info.subassy );
		break;

	case PART:
		free( record->info.part );
		break;
	}

	/*
	** Delete the main part of the record
	*/
	free( record );
}

