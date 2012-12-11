#include <linux/init.h>
#include <linux/module.h> 

static char* name = "Myswirl Hi";
static int  age = 30;

module_param(age, int, S_IRUGO);
module_param(name, charp, S_IRUGO);

static int hello_init(void)
{
    printk(KERN_EMERG "name=%s\n",name);
    printk(KERN_EMERG "age=%d\n",age);
    return 0;
}

static void hello_exit(void)
{
    printk(KERN_WARNING "Goodbye, param_moduel!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("Dual BSD/GPL");
