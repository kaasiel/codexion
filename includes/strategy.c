#include "codexion.h"

static int before(t_coder *a, t_coder *b)
{
    if(a->key != b->key)
        return (a->key < b->key);
    return (a->ticket < b->ticket);
}

static int  ready(t_dongles *d, t_coder *q)
{
    t_dongles   *other;

    other = q->left;
    if (q->left == d)
        other = q->right;
    return (!other->is_taken);
}

static int is_first(t_dongles *d, t_coder *c)
{
    int i;

    i = 0;
    while (i < d->queue_size)
    {
        if (d->queue[i] != c && before(d->queue[i], c) && ready(d, d->queue[i]))
            return (0);
        i++;
    }
    return (1);
}

static void enqueue( t_dongles *d, t_coder *c)
{
    d->queue[d->queue_size++] = c;
}

static void dequeue(t_dongles *d, t_coder *c)
{
    int i;
    int j;

    i = 0;
    j = 0;

    while (i < d->queue_size)
    {
        if(d->queue[i] !=c)
            d->queue[j++] = d->queue[i];
        i++;
    }
    d->queue_size = j;
}

static int can_take(t_coder *c)
{
    long    now;

    now = get_time();

    return (!c->left->is_taken && !c->right->is_taken
    && now >= c->left->availabilitytime && now >= c->right->availabilitytime
    && is_first(c->left, c) && is_first(c->right, c));
}

static int sim_end(t_data *d)
{
    int stop;

    pthread_mutex_lock(&d->print_mutex);
    stop = d->simulation_end;
    pthread_mutex_unlock(&d->print_mutex);

    return(stop);
}

static void wait_ms(t_data *d, long ms)
{
    struct timeval tv;
    struct timespec ts;

    gettimeofday(&tv, NULL);
    ts.tv_sec = tv.tv_sec + ms / 1000;
    ts.tv_nsec = (tv.tv_usec + (ms % 1000) * 1000) * 1000;

    if (ts.tv_nsec >= 1000000000L)
    {
        ts.tv_sec++;
        ts.tv_nsec -= 1000000000L;
    }
    
    pthread_cond_timedwait(&d->sched_cond, &d->sched_mutex, &ts);
}

int acquire_dongles(t_coder *c)
{
    t_data  *d;
    long    deadline;

    d = c->data;
    pthread_mutex_lock(&c->mutex);
    deadline = c->last_compile_start + c->time_to_burntout;
    pthread_mutex_unlock(&c->mutex);
    pthread_mutex_lock(&d->sched_mutex);
    c->ticket = d->ticket++;
    c->key = c->ticket;
    if (d->strategy == AL_EDF)
        c->key = deadline;
    enqueue(c->left, c);
    enqueue(c->right, c);
    while (!can_take(c) && !sim_end(d))
        wait_ms(d, 1);
    dequeue(c->left, c);
    dequeue(c->right, c);
    if (sim_end(d))
        return (pthread_mutex_unlock(&d->sched_mutex), 0);
    c->left->is_taken = 1;
    c->right->is_taken = 1;
    pthread_mutex_unlock(&d->sched_mutex);
    printlog(c, "\33[34mhas taken a dongle\33[0m");
    printlog(c, "\33[34mhas taken a dongle\33[0m");
    return (1);
}

void    release_dongles(t_coder *c)
{
    t_data  *d;
    long    until;

    d = c->data;
    until = get_time() + d->cooldown;
    pthread_mutex_lock(&d->sched_mutex);
    c->left->is_taken = 0;
    c->right->is_taken = 0;
    c->left->availabilitytime = until;
    c->right->availabilitytime = until;
    pthread_cond_broadcast(&d->sched_cond);
    pthread_mutex_unlock(&d->sched_mutex);
}