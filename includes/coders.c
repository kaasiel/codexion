#include "codexion.h"
#include "helper.h"

void    compile(t_coder *coder)
{
    if (!acquire_dongles(coder))
        return;
    
    pthread_mutex_lock(&coder->mutex);
    coder->last_compile_start = get_time();
    pthread_mutex_unlock(&coder->mutex);
    printlog(coder, "\33[32mis compiling\33[0m");
    usleep(coder->time_to_compile * 1000);
    release_dongles(coder);
    pthread_mutex_lock(&coder->mutex);
    coder->max_compile--;
    pthread_mutex_unlock(&coder->mutex);
}

void    debug(t_coder *coder)
{
    printlog(coder, "\33[33mis debugging\33[0m");
    usleep(coder->time_to_debug * 1000);
}

void    refrcatinng(t_coder *coder)
{
    printlog(coder, "\33[34mis refactoring\33[0m");
    usleep(coder->time_to_refactor * 1000);
}