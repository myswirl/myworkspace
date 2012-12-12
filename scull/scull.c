/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// �ļ���
 * Author:			// ����
 * Date:			// ����
 * Description:		// ģ������
 * Version:			// �汾��Ϣ
 * Function-1:		// ���ܺ���1����
 * Function-2:		// ���ܺ���2����
 * History:			// ��ʷ�޸ļ�¼
 *     <author>  <time>   <version>   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>           /* printk() */
#include <linux/slab.h>             /* kmalloc() */
#include <linux/fs.h>               /* everything... */
#include <linux/errno.h>            /* error codes */
#include <linux/types.h>            /* size_t */
#include <linux/fcntl.h>            /* O_ACCMODE */
#include <linux/cdev.h>
#include <asm/system.h>             /* cli(), *_flags */
#include <asm/uaccess.h>            /* copy_*_user */
#include "scull.h"                  /* local definitions */

int scull_major	   = SCULL_MAJOR;   //���豸��
int scull_minor	   = 0;             //���豸��
int scull_nr_devs  = SCULL_NR_DEVS; //�����豸��Ŀ
int scull_quantum  = SCULL_QUANTUM;
int scull_qset	   = SCULL_QSET;

module_param( scull_major, int, S_IRUGO );
module_param( scull_minor, int, S_IRUGO );
module_param( scull_nr_devs, int, S_IRUGO );
module_param( scull_quantum, int, S_IRUGO );
module_param( scull_qset, int, S_IRUGO );

struct scull_dev *scull_devices;                /* allocated in scull_init_module */


/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input-1:        //
* Input-2:        //
* Output-1:       //
* Return:         //
* Others:         //
***********************************************************/
int scull_trim( struct scull_dev *dev )
{
	struct scull_qset	*next, *dptr;
	int					qset = dev->qset;       /* "dev" is not-null */
	int					i;

	for( dptr = dev->data; dptr; dptr = next )  /* all the list items */
	{
		if( dptr->data )
		{
			for( i = 0; i < qset; i++ )
			{
				kfree( dptr->data[i] );
			}
			kfree( dptr->data );
			dptr->data = NULL;
		}
		next = dptr->next;
		kfree( dptr );
	}
	dev->size	   = 0;
	dev->quantum   = scull_quantum;
	dev->qset	   = scull_qset;
	dev->data	   = NULL;
	return 0;
}

/***********************************************************
* Function:       // scull_open
* Description:    // �ṩ�����������Գ�ʼ��������
* Input:          //
* Output:         //
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
int scull_open( struct inode *inode, struct file *filp )
{
	struct scull_dev *dev;
	//��container_of, ͨ��inode��i_cdev�õ�scull_dev�ṹ
	dev				   = container_of( inode->i_cdev, struct scull_dev, cdev );
	filp->private_data = dev; //�����Ժ�Ը�ָ��ķ���

	//���豸��д��ʽ��ʱ�����ĳ��Ƚ�����Ϊ0
	if( ( filp->f_flags & O_ACCMODE ) == O_WRONLY )
	{
		if( down_interruptible( &dev->sem ) )
		{
			return -ERESTARTSYS;
		}
		scull_trim( dev );  /* ignore errors */
		up( &dev->sem );
	}
	return 0;               /* success */
}

/***********************************************************
* Function:       // scull_release
* Description:    // �ر��豸
* Input:          //
* Output:         //
* Return:         //
* Others:         //
***********************************************************/
int scull_release( struct inode *inode, struct file *filp )
{
	return 0;
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          //
* Output:         //
* Return:         //
* Others:         //
***********************************************************/
struct scull_qset *scull_follow( struct scull_dev *dev, int n )
{
	struct scull_qset *qs = dev->data;

	/* Allocate first qset explicitly if need be */
	if( !qs )
	{
		qs = dev->data = kmalloc( sizeof( struct scull_qset ), GFP_KERNEL );
		if( qs == NULL )
		{
			return NULL; /* Never mind */
		}
		memset( qs, 0, sizeof( struct scull_qset ) );
	}

	/* Then follow the list */
	while( n-- )
	{
		if( !qs->next )
		{
			qs->next = kmalloc( sizeof( struct scull_qset ), GFP_KERNEL );
			if( qs->next == NULL )
			{
				return NULL; /* Never mind */
			}
			memset( qs->next, 0, sizeof( struct scull_qset ) );
		}
		qs = qs->next;
		continue;
	}
	return qs;
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          //
* Output:         //
* Return:         //
* Others:         //
***********************************************************/
ssize_t scull_read( struct file *filp, char __user *buf, size_t count, loff_t *f_pos )
{
	struct scull_dev	*dev = filp->private_data;
	struct scull_qset	*dptr;                          /* the first listitem */
	int					quantum	   = dev->quantum, qset = dev->qset;
	int					itemsize   = quantum * qset;    /* how many bytes in the listitem */
	int					item, s_pos, q_pos, rest;
	ssize_t				retval = 0;

	if( down_interruptible( &dev->sem ) )
	{
		return -ERESTARTSYS;
	}
	if( *f_pos >= dev->size )
	{
		goto out;
	}
	if( *f_pos + count > dev->size )
	{
		count = dev->size - *f_pos;
	}

	/* find listitem, qset index, and offset in the quantum */
	item   = (long)*f_pos / itemsize;
	rest   = (long)*f_pos % itemsize;
	s_pos  = rest / quantum; q_pos = rest % quantum;

	/* follow the list up to the right position (defined elsewhere) */
	dptr = scull_follow( dev, item );

	if( dptr == NULL || !dptr->data || !dptr->data[s_pos] )
	{
		goto out; /* don't fill holes */
	}
	/* read only up to the end of this quantum */
	if( count > quantum - q_pos )
	{
		count = quantum - q_pos;
	}

	if( copy_to_user( buf, dptr->data[s_pos] + q_pos, count ) )
	{
		retval = -EFAULT;
		goto out;
	}
	*f_pos	  += count;
	retval	   = count;

out:
	up( &dev->sem );
	return retval;
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          //
* Output:         //
* Return:         //
* Others:         //
***********************************************************/
ssize_t scull_write( struct file *filp, const char __user *buf, size_t count,
                     loff_t *f_pos )
{
	struct scull_dev	*dev = filp->private_data;
	struct scull_qset	*dptr;
	int					quantum	   = dev->quantum, qset = dev->qset;
	int					itemsize   = quantum * qset;
	int					item, s_pos, q_pos, rest;
	ssize_t				retval = -ENOMEM; /* value used in "goto out" statements */

	if( down_interruptible( &dev->sem ) )
	{
		return -ERESTARTSYS;
	}

	/* find listitem, qset index and offset in the quantum */
	item   = (long)*f_pos / itemsize;
	rest   = (long)*f_pos % itemsize;
	s_pos  = rest / quantum; q_pos = rest % quantum;

	/* follow the list up to the right position */
	dptr = scull_follow( dev, item );
	if( dptr == NULL )
	{
		goto out;
	}
	if( !dptr->data )
	{
		dptr->data = kmalloc( qset * sizeof( char * ), GFP_KERNEL );
		if( !dptr->data )
		{
			goto out;
		}
		memset( dptr->data, 0, qset * sizeof( char * ) );
	}
	if( !dptr->data[s_pos] )
	{
		dptr->data[s_pos] = kmalloc( quantum, GFP_KERNEL );
		if( !dptr->data[s_pos] )
		{
			goto out;
		}
	}
	/* write only up to the end of this quantum */
	if( count > quantum - q_pos )
	{
		count = quantum - q_pos;
	}
	//���豸�������ݵ��û��ռ�
	if( copy_from_user( dptr->data[s_pos] + q_pos, buf, count ) )
	{
		retval = -EFAULT;
		goto out;
	}
	*f_pos	  += count;
	retval	   = count;

	/* update the size */
	if( dev->size < *f_pos )
	{
		dev->size = *f_pos;
	}

out:
	up( &dev->sem );
	return retval;
}

// �ļ��������豸����������������������ӵ��豸���
struct file_operations scull_fops =
{
	.owner	   = THIS_MODULE,
	.read	   = scull_read,    //�������豸��ȡ����
	.write	   = scull_write,   //���豸��������
	.open	   = scull_open,
	.release   = scull_release,
};


/***********************************************************
* Function:       // scull_setup_cdev
* Description:    // scull�豸ע��
* Input:		  //
* Output:		  //
* Return:         // ��
* Others:         // cdev�ṹ��Ƕ�뵽��struct scull_dev�У��������cdev_init��ִ�иýṹ�ĳ�ʼ��
***********************************************************/
static void scull_setup_cdev( struct scull_dev *dev, int index )
{
	int err, devno = MKDEV( scull_major, scull_minor + index );
	//�ں��ڲ�ʹ��cdev�ṹ����ʾ�ַ��豸,cdev�ṹǶ�뵽�Լ��豸�ض��Ľṹ�У�cdev_init����ʼ���ѷ��䵽�Ľṹ
	cdev_init( &dev->cdev, &scull_fops );
	dev->cdev.owner = THIS_MODULE;
	//dev->cdev.ops = &scull_fops;	        //������ʡ�ԣ���cdev_init���Ѿ�����
	err = cdev_add( &dev->cdev, devno, 1 ); //�����ں˸ýṹ����Ϣ
	if( err )
	{
		PDEBUG( KERN_NOTICE "Error %d adding scull%d", err, index );
	}
}

/***********************************************************
* Function:       // scull_cleanup_module
* Description:    //
* Return:         // ��
* Others:         // ����˵��
***********************************************************/
static void __exit scull_cleanup_module( void )
{
	int		i;
	dev_t	devno = MKDEV( scull_major, scull_minor );

	/* Get rid of our char dev entries */
	if( scull_devices )
	{
		for( i = 0; i < scull_nr_devs; i++ )
		{
			scull_trim( scull_devices + i );
			cdev_del( &scull_devices[i].cdev ); //��ϵͳ���Ƴ�һ���ַ��豸
		}
		kfree( scull_devices );
	}

	//cleanup_module is never called if registering failed
	unregister_chrdev_region( devno, scull_nr_devs );
	PDEBUG( "exit---unregister_chrdev_region" );
}

/***********************************************************
* Function:       // scull_init_module
* Description:    // ��ʼ�����������ܰ���:ע���豸��ţ��ַ��豸ע��
* Return:         //
* Others:         //
***********************************************************/
static int __init scull_init_module( void )
{
	int		result, i;
	dev_t	dev = 0;                                                    //���������豸��ţ��������豸�źʹ��豸��

	if( 0 != scull_major )
	{
		dev	   = MKDEV( scull_major, scull_minor );
		result = register_chrdev_region( dev, scull_nr_devs, "scull" ); //ָ���豸��ţ�����ע��
		PDEBUG( "register device major %d\n", scull_major );
	} else
	{                                                                   //��̬��ȡ���豸��
		result		   = alloc_chrdev_region( &dev, scull_minor, scull_nr_devs, "scull" );
		scull_major	   = MAJOR( dev );
		PDEBUG( "alloc register device major %d\n", scull_major );
	}
	if( result < 0 )
	{
		PDEBUG( KERN_WARNING "can't get major %d\n", scull_major );
		return result;
	}

	//allocate the devices -- we can't have them static, as the number can be specified at load time
	scull_devices = kmalloc( scull_nr_devs * sizeof( struct scull_dev ), GFP_KERNEL );
	if( !scull_devices )
	{
		result = -ENOMEM;
		goto fail; // Make this more graceful
	}
	memset( scull_devices, 0, scull_nr_devs * sizeof( struct scull_dev ) );

	//��ʼ��ÿ���豸
	for( i = 0; i < scull_nr_devs; i++ )
	{
		scull_devices[i].quantum   = scull_quantum;
		scull_devices[i].qset	   = scull_qset;
		init_MUTEX( &scull_devices[i].sem );
		scull_setup_cdev( &scull_devices[i], i );
	}

	return 0; //�ɹ�

fail:
	scull_cleanup_module( );
	return result;
}

module_init( scull_init_module );
module_exit( scull_cleanup_module );

MODULE_AUTHOR( "mingyang" );
MODULE_LICENSE( "Dual BSD/GPL" );

/************************************** The End Of File **************************************/

