## The files

- `src/propulsion_server.cpp`: default C++ placeholder server entrypoint
- `tests/test_propulsion_server.cpp`: simple C++ unit test skeleton (yes there are frameworks for this, see lengthy note on my unit test philosophy below)
- `Makefile`: build, run, and test targets

- Also notice `src/propulsion_server_with_timer.cpp` - that's my AI - boosted first crack at using C++ notifiers and fancier mutexes to do a countdown timer sort of thing instead of tick based polling. I think it turned out to be way more confusing and difficult to debug. I try to avoid concurrency when I can and stay deterministic for critical code. But its worth taking a look at - could be combined with the oldie but goodie way of doing things.

It looks like C++ is trying to do whatever Rust does with ownership. Not that I understand that. I don't.

## How to build it and run it

Tested in ubuntu:

`make run`

If you want the fancy asynchronous version, just replace the `src/propulsion_server.cpp` code and copy-paste `src/propulsion_server_with_timer.cpp` into it.

`make run` will still work.

Sorry, I got a little lazy there.

## Approach

There is a mutex protected piece of shared memory for a single fire command:

```c
struct fire_command_t
{
    int seq; // increments on every new command so the fire thread can detect updates
    bool command_pending;
    int fire_delay;
};
```

There are two threads:

1. read thread (gets commands from stdin, updates shared mutex-protected fire_command_t struct)
2. fire thread (ticks the "clock", fires the thruster)

For this demo the clock tick just comes from a sleep() command.
In real flight software there is a clock source in hardware (sometimes steered by GPS) that drives a system tick interrupt.
Time is hard. Ask me about it some(time) ;D

## If I could do it again

I'd write it state-machine style.
And I'd use regular mutexes with lock/unlock, not the fancy guards.

## Limitations

Current implementation is simple:

- no graceful shutdown
- no precise clock
- stdout from two threads can interleave slightly
- command input and timing are coarse 1-second resolution

But structurally it demonstrates:

- thread separation
- mutex protection of shared memory between threads
    - (good for central command and telemetry table that needs to get processed all at same time)
- command sequencing

In real life:

- The driver thread would probably be its own real-time "task" controlled by an executor task round robin style
- I'd probably try to avoid any concurrency at all in drivers since its incredibly hard to debug.
- I prefer determinism to multithreading for flight code that deals with hardware.
- I'd have a queue (simple statically allocated no-frills circular buffer) for commands
- Only a set number of commands per cycle.
- GNC commands prioritized if gnc commanding is enabled.
    - I'd turn the latest gnc command into a fire command (s).
    - Possibly triggered on next clock cycle depending on system and GNC preferences.
- Otherwise would accept commands from circular buffer queue as they came and do something like this code. 

## TEST

- The test functions I defined use only the standard library and `assert`.
    - There's a billion different test frameworks we could use.
- I didn't actually fill them in.
- Sometimes I do test driven development. But not for fast prototyping. Read on.

### Personal philosophy about tests

- Many layers, like an onion. None have to be perfect.

- I don't think unit tests should be used for code coverage the way most folks (and many standards) do. People go overboard with it.
    - Unit tests are for individual functions, not for everything.
    - `0%` code coverage:
        - makes code too scary to change (here be dragons)
        - causes learned helplessness (lack of motivation)
        - makes code review hell
    - `100%` code coverage:
        - makes code too brittle and time consuming to change
        - more time spent debugging test code than making progress
        - test reports and lines of code become more important than good, maintainable, working code
        - makes code review hell
    - Unit tests should be added as needed as guardrails for developers to quietly sanity check - not for systems integration folks.
    - As things get refactored into functions, or you find a bug, add another unit test.
    - Organize it a little, not too much. Don't make it weird.
    - Set up a unit test framework with coverage to make it easy to add tests as needed (or suggest that others do)
    - make test coverage easy to see and tests easy to write, and folks will choose to do it.
    - The order of "getting it working (first bytes)", code review, and unit testing really matters a lot
        - Ask me how I learned that the hard way. I've got many stories and lessons learned.
        - I learned the same lessons with test scripts - ask me about that too.
        - Ask me what I'd choose to do now.
    
- Software folks need devkits to fiddle with and freedom to learn if they show interest. That's how you get HITLs without anyone asking.

## Work Preferences

I enjoy understanding things from the bottom up, and don't like taking shortcuts.

If I was going to switch to Rust, I'd ideally like to do initial bringup/firmware development in Rust on a devkit so I'd actually know what was happening. It's only supported on a few specific pieces of hardware right now. There are a few hardware architectures I'd like to get a crack at (whether FPGA, embedded linux, baremetal, FreeRTOS scheduler, or Something Else TM).

I like FPGA and microcontroller work too.

I started out building everything from scratch with devkits (FPGA upwards.)

I majored in computer engineering.