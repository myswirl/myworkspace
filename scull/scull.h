/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// scull.h
 * Author:			// lmy
 * Date:			// 20100626
 * Description:		// 简单字符设备驱动头文件
 * Version:			// 1.0
 * Function List:
 * History:
 *     <author>  <time>   <version >   <desc>
 *     David    10/06/12     1.0     build this moudle
 ***********************************************************/
#ifndef _SCULL_H_
#define _SCULL_H_

#define SCULL_DEBUG                                                         //定义此宏，则打印调试信息

#include <linux/ioctl.h>                                                    // needed for the _IOW etc stuff used later

#undef PDEBUG                                                               // 取消对PDEBUG的定义，以防重复定义
#ifdef SCULL_DEBUG
#ifdef __KERNEL__
#define PDEBUG( fmt, args ... ) printk( KERN_DEBUG "scull: " fmt, ## args ) //内核空间---打印函数
#else
#define PDEBUG( fmt, args ... ) fprintf( stderr, fmt, ## args )             //用户空间---打印函数
#endif
#else
#define PDEBUG( fmt, args ... )                                             //不打印调试信息
#endif

#undef PDEBUGG
#define PDEBUGG( fmt, args ... )                                            // nothing: it's a placeholder

#ifndef SCULL_MAJOR
#define SCULL_MAJOR 0                                                       // 主设备号，默认为0，动态分配
#endif

#ifndef SCULL_NR_DEVS
#define SCULL_NR_DEVS 4                                                     // 创建设备文件数目
#endif

#ifndef SCULL_P_NR_DEVS
#define SCULL_P_NR_DEVS 4                                                   /* scullpipe0 through scullpipe3 */
#endif

#ifndef SCULL_QUANTUM
#define SCULL_QUANTUM 4000
#endif

#ifndef SCULL_QSET
#define SCULL_QSET 1000
#endif

// The pipe device is a simple circular buffer. Here its default size
#ifndef SCULL_P_BUFFER
#define SCULL_P_BUFFER 4000
#endif

//Representation of scull quantum sets.
struct scull_qset
{
	void				**data;
	struct scull_qset	*next;
};
//表示每个设备
struct scull_dev
{
	struct scull_qset	*data;                          //指向第一个量子集的指针
	int					quantum;                        //当前量子的大小
	int					qset;                           //当前数组的大小
	unsigned long		size;                           //保存在其中的数据总量
	unsigned int		access_key;                     //由sculluid和scullpriv使用
	struct semaphore	sem;                            //互斥信号量
	struct cdev			cdev;                           //字符设备结构
};

//Split minors in two parts
#define TYPE( minor )	( ( ( minor ) >> 4 ) & 0xf )    /* high nibble */
#define NUM( minor )	( ( minor ) & 0xf )             /* low  nibble */

extern int	scull_major;                                // scull.c中定义
extern int	scull_nr_devs;
extern int	scull_quantum;
extern int	scull_qset;

int scull_trim( struct scull_dev *dev );


ssize_t scull_read( struct file *filp, char __user *buf, size_t count, loff_t *f_pos );


ssize_t scull_write( struct file *filp, const char __user *buf, size_t count, loff_t *f_pos );


//Ioctl definitions
#define SCULL_IOC_MAGIC 'k'                             // Use 'k' as magic number
#define SCULL_IOCRESET	_IO( SCULL_IOC_MAGIC, 0 )       //Please use a different 8-bit number in your code


/*
 * S means "Set" through a ptr,
 * T means "Tell" directly with the argument value
 * G means "Get": reply by setting through a pointer
 * Q means "Query": response is on the return value
 * X means "eXchange": switch G and S atomically
 * H means "sHift": switch T and Q atomically
 */
#define SCULL_IOCSQUANTUM	_IOW( SCULL_IOC_MAGIC, 1, int )
#define SCULL_IOCSQSET		_IOW( SCULL_IOC_MAGIC, 2, int )
#define SCULL_IOCTQUANTUM	_IO( SCULL_IOC_MAGIC, 3 )
#define SCULL_IOCTQSET		_IO( SCULL_IOC_MAGIC, 4 )
#define SCULL_IOCGQUANTUM	_IOR( SCULL_IOC_MAGIC, 5, int )
#define SCULL_IOCGQSET		_IOR( SCULL_IOC_MAGIC, 6, int )
#define SCULL_IOCQQUANTUM	_IO( SCULL_IOC_MAGIC, 7 )
#define SCULL_IOCQQSET		_IO( SCULL_IOC_MAGIC, 8 )
#define SCULL_IOCXQUANTUM	_IOWR( SCULL_IOC_MAGIC, 9, int )
#define SCULL_IOCXQSET		_IOWR( SCULL_IOC_MAGIC, 10, int )
#define SCULL_IOCHQUANTUM	_IO( SCULL_IOC_MAGIC, 11 )
#define SCULL_IOCHQSET		_IO( SCULL_IOC_MAGIC, 12 )


/*
 * The other entities only have "Tell" and "Query", because they're
 * not printed in the book, and there's no need to have all six.
 * (The previous stuff was only there to show different ways to do it.
 */
#define SCULL_P_IOCTSIZE	_IO( SCULL_IOC_MAGIC, 13 )
#define SCULL_P_IOCQSIZE	_IO( SCULL_IOC_MAGIC, 14 )

#define SCULL_IOC_MAXNR 14

#endif

/************************************** The End Of File **************************************/

