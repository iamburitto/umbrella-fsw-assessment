# Umbra Technical Assessment Scaffold

This repo is set up so you can write the solution yourself without spending time on boilerplate.

## Included

- `src/propulsion_server.cpp`: default C++ placeholder server entrypoint
- `tests/test_propulsion_server.cpp`: default C++ unit test skeleton
- `src/propulsion_server.c`: optional C version
- `tests/test_propulsion_server.c`: optional C test version
- `Makefile`: build, run, and test targets

## Commands

```bash
make build
make run
make test
make build-c
make test-c
```

## Suggested Approach

- Keep the solution small enough to discuss comfortably in an interview.
- Solve against standard input and standard output unless you have a strong reason to use TCP.
- Fill in the four behavior tests first: fire, overwrite, cancel, repeated use.
- The default targets now prefer C++.
- If you decide to stay in C, use `make build-c`, `make run-c`, and `make test-c`.

## Notes

- The test scaffold uses only the standard library and `assert`.
- The current placeholders intentionally fail until you replace them.



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

// (0,  5,  "A"):  [0  1  2  3  4  5) 6  7  8  9  10  11  12  13  14  15
// (3,  10, "B"):   0  1  2 [3  4  5  6  7  8  9  10) 11  12  13  14  15
// (12, 15, "C"):   0  1  2  3  4  5  6  7  8  9  10  11 [12  13  14) 15
