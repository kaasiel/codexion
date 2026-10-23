*This project has been created as part of the 42 curriculum by belaindr.*

<p align="center">
  <img src="Codexion.png" alt="Codexion output" width="40%">
</p>

# Codexion

## Description

Codexion is a project about threading, built around a variant of the dining philosophers problem. The philosophers are called **coders** and the forks are **USB dongles**. Coders sit in a circle with one dongle between each pair of neighbours, so every dongle is shared by exactly two coders.

Each coder repeats the same cycle: **compile**, **debug**, **refactor**. To compile, a coder needs **both** of its dongles. After a compile the dongles go through a **cooldown** before anyone can use them again. If a coder goes too long without starting a compile, it **burns out** and the simulation stops.

The simulation ends when every coder has compiled `number_of_compiles_required` times, or as soon as one coder burns out.

Written in C with POSIX threads (one thread per coder, plus one monitor thread).

## Instructions

```bash
make        # compiles codexion
make clean  # removes object files
make fclean # removes object files and the binary
make re     # full recompile
```

Compiled with `-Wall -Wextra -Werror -pthread`.

## Usage

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument | Meaning |
|---|---|
| `number_of_coders` | number of coders (and dongles), at least 1 |
| `time_to_burnout` | max time (ms) between the start of two compiles |
| `time_to_compile` | duration (ms) of a compile |
| `time_to_debug` | duration (ms) of debugging |
| `time_to_refactor` | duration (ms) of refactoring |
| `number_of_compiles_required` | compiles each coder must complete |
| `dongle_cooldown` | time (ms) a dongle stays unavailable after release |
| `scheduler` | `fifo` or `edf` |

**Examples:**!


```bash
./codexion 5 1500 200 200 200 2 200 fifo
./codexion 5 1500 200 200 200 2 200 edf
```

**Output format:** `<timestamp_in_ms> <coder_id> <action>`, where the timestamp is the time elapsed since the start of the simulation.

```
0 1 has taken a dongle
0 1 has taken a dongle
0 1 is compiling
200 1 is debugging
400 1 is refactoring
```

## Algorithms

### Ticket system

Every dongle has its own **min-heap** of the coders waiting for it (capacity 2, since only two coders share a dongle). The heap decides who has priority on that dongle.

Each time a coder asks for its dongles, `set_priority` gives it:

- a **ticket**: a global counter incremented under a mutex, so tickets are unique and strictly increasing (a smaller ticket means an earlier request);
- a **key**: the value the heap sorts on. It depends on the scheduler (see below).

The heap puts the smallest key first, and uses the ticket to break ties. The coder then pushes itself into the heap of its left and right dongle and waits until it is allowed to take both.

To decide whether it can take its dongles, a coder checks, for each of the two dongles:

1. the dongle is free and its cooldown is over;
2. the coder is **first in the heap** of that dongle, **or** the coder at the top cannot start anyway (its other dongle is already taken), in which case it is allowed to go ahead.

This second rule only needs to know whether the neighbours' other dongles are free, which is why a coder reads a *window* of 4 dongles: its own two, plus the other dongle of each neighbour (see the synchronisation section).

### FIFO vs EDF

Only the heap **key** changes between the two schedulers:

| Scheduler | key | Who goes first |
|---|---|---|
| `fifo` | the ticket | the coder who asked first |
| `edf` | `last_compile_start + time_to_burnout` | the coder closest to burning out |

- **FIFO** is fair in order of arrival, but it ignores how close a coder is to burning out.
- **EDF** (Earliest Deadline First) always serves the coder with the nearest burnout deadline, which keeps coders alive in tight configurations. At the very start all deadlines are equal, so the ticket breaks the tie and EDF behaves like FIFO for the first round.

Example: coders C4 and C1 compete for dongle D0, with `time_to_burnout = 800` and `time_to_compile = 300`. C4 asked first (ticket 6, deadline 1100), C1 asked second (ticket 7, deadline 800).

- With FIFO, C4 compiles first; C1 can only start after C4 finishes and the cooldown ends, which is after its deadline: C1 burns out.
- With EDF, C1 compiles first (800 < 1100); C4 starts later but is still within its deadline: nobody burns out.

## Blocking cases handled

- **Deadlock (circular wait):** a coder never holds one dongle while waiting for the other. It takes both dongles in one atomic step (under the window lock) or none. Mutexes are always locked in increasing dongle id order.
- **Single coder:** with one coder, both of its dongles are the same one, so it can never compile. It takes the dongle, waits, and burns out as expected.
- **Head-of-line blocking:** a coder at the top of a heap that cannot start (its other dongle is taken) does not block the coder behind it.
- **Dongle cooldown:** a released dongle gets an availability time (`now + cooldown`). Waiting coders wake up regularly and re-check it, since nobody signals the end of a cooldown.
- **Burnout detection:** a monitor thread checks every millisecond that no coder has gone longer than `time_to_burnout` since the start of its last compile. The burnout message is printed at most once, and nothing is printed after it.
- **Clean stop:** once the simulation ends, sleeping coders (`ft_usleep`), waiting coders (dongle wait) and the main loops all notice it and exit, so every thread can be joined and all memory is freed.
- **Starvation:** with `edf`, the coder closest to burnout is always served first; with `fifo`, no coder can be overtaken by a later request.

## Thread synchronisation mechanisms

| Mechanism | Used for |
|---|---|
| `pthread_mutex_t` per dongle | protects `is_taken`, `availabilitytime` and the dongle's heap |
| `pthread_cond_t` per dongle | a coder waits for a dongle to be released (`pthread_cond_timedwait`, 1 ms timeout), `release_dongles` calls `broadcast` |
| `print_mutex` | serialises output **and** protects `simulation_end`, so nothing is printed after the end |
| `ticket_mutex` | protects the global ticket counter |
| `coder->mutex` | protects `last_compile_start` and `max_compile`, which are read by the monitor |
| monitor thread | detects burnout and the end of the simulation |
| `pthread_join` | the main thread waits for all coder threads and the monitor |

**Window locking.** To read a consistent state, a coder locks the mutexes of 4 dongles: its left and right dongles, plus the dongle on the far side of each neighbour (`left - 1` and `right + 1`, modulo the number of coders). The ids are sorted and de-duplicated, then locked in increasing order, which guarantees a global lock order and prevents deadlocks. Inside the window, the check and the taking of the dongles are one atomic operation.

**Waiting.** A coder that cannot take its dongles releases the whole window except the dongle it sleeps on, waits on that dongle's condition variable (which releases its mutex), then re-locks the entire window in sorted order before checking again.

**Data races.** Data shared between threads is always accessed under one of the mutexes above. Values that never change after initialisation (`start_time`, `cooldown`, `strategy`, the dongle pointers) are read without locking. The program was tested with ThreadSanitizer (`-fsanitize=thread`) and Helgrind.

## Resources

- [Dining philosophers problem](https://www.geeksforgeeks.org/operating-systems/dining-philosopher-problem-using-semaphores/)
- [Threads theory](https://perso.ens-lyon.fr/francois.schwarzentruber/teaching/l3-prog/book/c_thread.html)
- `man pthread_create`, `man pthread_mutex_lock`, `man pthread_cond_timedwait`, `man gettimeofday`

**AI usage:** an AI assistant was used to help understand the scheduling and synchronisation concepts. The code itself was written and is understood by the author.