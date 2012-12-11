#include <linux/init.h>
#include <linux/module.h> 

extern int add_integer(int a,int b);
extern int sub_integer(int a,int b);

static int hello_init(void)
{
	int res = add_integer(1,3);
    printk(KERN_WARNING "Hello, World! res=%d\n",res);
    return 0;
}

static void hello_exit(void)
{
	int res = sub_integer(2,1);
    printk(KERN_WARNING "Goodbye, cruel world!,res=%d\n",res);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("Dual BSD/GPL");
