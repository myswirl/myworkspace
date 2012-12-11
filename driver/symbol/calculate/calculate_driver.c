#include <linux/init.h>
#include <linux/module.h> 

int add_integer(int a,int b)
{
	return a+b;
}
int sub_integer(int a,int b)
{
	return a-b;
}
static int __init sym_init(void)
{
    return 0;
}

static void __exit sym_exit(void)
{
}

module_init(sym_init);
module_exit(sym_exit);

MODULE_LICENSE("Dual BSD/GPL");

EXPORT_SYMBOL(add_integer);
EXPORT_SYMBOL(sub_integer);
