// clang-format off

// # Umbra Technical Assessment Scaffold

// This repo is set up so you can write the solution yourself without spending time on boilerplate.

// ## Included

// - `src/propulsion_server.cpp`: default C++ placeholder server entrypoint
// - `tests/test_propulsion_server.cpp`: default C++ unit test skeleton
// - `Makefile`: build, run, and test targets

// ## How to build it

// make (will do everything but run)
// make build
// make run
// make test

// ## Approach

// There is a mutex protected piece of shared memory for a single fire command:

// struct fire_command_t
// {
//     int seq; // increments on every new command so the fire thread can detect updates
//     bool command_pending;
//     int fire_delay;
// };

// There are two threads:

// 1. read thread (gets commands from stdin, updates shared mutex-protected fire_command_t struct)
// 2. fire thread (ticks the "clock", fires the thruster)

// For this demo the clock tick just comes from a sleep() command.
// In real flight software there is a clock source in hardware (sometimes steered by GPS) that drives a system tick interrupt.
// Time is hard. Ask me about it.

// Known limitations

// Current implementation is simple:

// - no graceful shutdown
// - no precise clock
// - stdout from two threads can interleave slightly
// - command input and timing are coarse 1-second resolution

// But structurally it demonstrates:

// - thread separation
// - mutex protection of shared memory between threads
//     - (good for central command and telemetry table that needs to get processed all at same time)
// - command sequencing

// In real life:

// - The driver thread would probably be its own real-time "task" controlled by an executor task round robin style
// - I'd probably try to avoid any concurrency at all in the driver if I could help it since its hard to debug
// - I'd have a queue (circular buffer) for commands
// - Only a set number of commands per cycle.
// - GNC commands prioritized if gnc commanding is enabled.
//     - I'd turn the latest gnc command into a fire command (s).
//     - Possibly triggered on next clock cycle depending on system and GNC preferences.
// - Otherwise would accept commands from circular buffer queue as they came and do something like this code. 

// ## TEST

// - Many layers, like an onion. None have to be perfect.

// - The tests here use only the standard library and `assert`.
//     - There's a billion different test frameworks we could use.
// - I didn't do test driven development here. It's not good for prototyping.

// ### Personal philosophy about tests

// - I don't think unit tests should be used for code coverage. People go overboard with it.
//     - Unit tests are for individual functions, not for everything.
//     - 0% code coverage:
//         - makes code too scary to change (here be dragons)
//         - causes learned helplessness (lack of motivation)
//         - makes code review hell
//     - 100% code coverage:
//         - makes code too brittle and time consuming to change
//         - more time spent debugging test code than making progress
//         - test reports and lines of code become more important than good, maintainable, working code
//     - Unit tests should be added as needed as guardrails for developers to quietly sanity check - not for systems integration folks.
//     - As things get refactored into functions, or you find a bug, add another unit test.
//     - Don't make it weird - just set up a framework to make it easy to do the right thing when needed.
//     - Organize it a little, not too much.
//     - The order of "getting it working (first bytes)", code review, and unit testing really matters a lot for software team culture.
//     - Software folks need devkits and hardware setups to fiddle with, and the tools/support to set them up as needed especially starting out.
//         - Give everyone the option if they feel strongly about it.
//         - Again, controlling this stuff too much is bad.
// - I do not think people should be hired as test-only. It's a trap.
// - If you are going to have a QA team, do not sit them next to the flight software developers. Makes it impossible to speak and learn freely.
// - Flight software needs to be a chill, fun, psychologically safe vibe of likeminded people (when you know you know.)
//     - Best to have separate area semi-protected from other teams once big enough (so we can blow off steam)
//     - Best to control over our own (sometimes messy) hardware (those who want to be in charge of it will build it)

// clang-format off

// Asynchronous Propulsion Server Problem Statement

// You are a member of the Flight Software Team at Umbra Space and are responsible for writing code that manages the satellite's propulsion system. Firing the propulsion system involves waiting for a certain period of time before ignition. The following is an example usage of this system:

// Expected behavior: 

// - Flight computer receives a command with a relative time to fire propulsion.
// - Behavior: Start a countdown; when time elapses, print “firing now!”.

// - If another command arrives before the current one fires:
// - New command overwrites the previous pending command.

// Example:

// - At absolute time t = 0, send a command to the computer to fire the propulsion in 15 seconds
// - At absolute time t = 2, send a command to the computer to fire the propulsion in 30 seconds
// - At absolute time t = 32, the computer begins firing the propulsion

// Here's what that would look like:

// ./your_program
// 15
// 30
// firingnow!

// Visual timeline of example usage:

// Time (seconds):  0        2                       15              30              32
//              |--------|-----------------------|---------------|---------------|
// Command:          -> 15   -> 30                         
// Action:                                           (no fire)      (no fire)         FIRE

// Explanation:

// - At t=0: Command received to fire in 15s (would fire at t=15)
// - At t=2: Command received to fire in 30s (would fire at t=32)
// - At t=15: First command's fire time, but superseded by later command
// - At t=32: Second command's fire time, propulsion fires

// clang-format on










If we are being honest, I'm more of a low level person. I enjoy understanding things from the bottom up, and don't like taking shortcuts.

If I was going to switch to Rust, I'd ideally like to do initial board bringup/firmware development in Rust on a devkit so I'd actually know what was happening. It's only supported on a few specific pieces of hardware right now. There are a few hardware architectures I'd like to get a crack at (whether FPGA, embedded linux, baremetal, FreeRTOS scheduler, or Something Else TM).

In real life, one person usually does that work for a long time after hardware folks set it up, and then sets up a hardware abstraction layer and tutorial to bring on maybe one other person, and it takes a significant amount of time before they are ready to actually support software folks iterating on it, and even then it needs to be slow. For an actual realtime system (or a microcontroller) I'm not really sure it ever makes sense to have software-only folks.

It's scary to trust a person to do that - unless they have done it once (or twice) before, are good at working with avionics folks, and know what the consequences are for everyone if that information stays gatekept later. Especially for a microcontroller. They need to be shielded in the beginning to train themselves with tutorials on a devkit, or secretly start early and bring themselves up on the correct devkit.

I know this job is probably meant to support things that already exist. Just saying.


you first built simple deterministic logic

then refactored into deadline-based waiting

without overengineering