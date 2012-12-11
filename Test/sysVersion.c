#include <stdio.h>
#include <sys/utsname.h>
int main()
{
        struct utsname name;
        uname(&name);
        printf("%s,%s,%s,%s,%s,%s\n", name.sysname, name.nodename,name.release,name.version, name.machine, name.__domainname);
        return 0;
}
