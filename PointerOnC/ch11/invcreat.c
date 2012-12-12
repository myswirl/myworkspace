#include <stdlib.h>
#include <stdio.h>
#include "inventor.h"

//���ڴ���SUBASSEMBLY(װ���)�����¼�ĺ���
Invrec *create_subassy_record( int n_parts )
{
	Invrec	*new_rec;

	//��ͼΪInvrec�����ڴ�
	new_rec = malloc( sizeof( Invrec ) );
	if( new_rec != NULL ){
		
		new_rec->info.subassy = malloc( sizeof( Subassyinfo ) );
		if( new_rec->info.subassy != NULL ){
			//Ϊ�����ȡһ���㹻�������
			new_rec->info.subassy->part = malloc(
			    n_parts * sizeof( struct SUBASSYPART ) );
			if( new_rec->info.subassy->part != NULL ){
				//�����ֵ֪�ֶΣ�Ȼ�󷵻�
				new_rec->type = SUBASSY;
				new_rec->info.subassy->n_parts = n_parts;
				return new_rec;
			}

			//�ڴ��ѽ����꣬�ͷ�����ԭ�ȷ�����ڴ�
			free( new_rec->info.subassy );
		}
		free( new_rec );
	}
	return NULL;
}

//�ͷŴ����¼�ĺ���
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

