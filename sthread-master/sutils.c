#include "sutils.h"
#include "sinclude.h"

sigset_t __old__,__new__;
void signal_block(void)
{
    sigemptyset(&__new__);
    sigaddset(&__new__,SIGPROF);
    sigprocmask(SIG_BLOCK,&__new__,&__old__);////blocked timer alarm///
    return;
}

void signal_release(void)
{
     sigprocmask(SIG_SETMASK,&__old__,NULL);///released timer alarm//
}