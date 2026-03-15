
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

// clang-format on

// normal c things
#include <cstdbool>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// c++ things
#include <iostream>

/* for std::thread */
#include <cerrno>
#include <chrono>  // for millisecond sleep
#include <cstdio>
#include <mutex>
#include <thread>

using namespace std;  // I hate this but I hate seeing namespace:: everywhere
                      // more

// global shared fire delay + mutex
static volatile int g_fire_delay = -1;
static mutex g_fire_delay_mutex;

// global shared command pending flag
static volatile bool g_command_pending = false;
static mutex g_command_pending_mutex;

// thread that fires every second
static void fire_1000ms_thread()
{
    static int tick_count = 0;
    static int fire_count = 0;

    while (1)
    {
        // get the latest command pending status
        g_fire_delay_mutex.lock();
        static bool command_pending = g_command_pending;
        g_fire_delay_mutex.unlock();

        // check for no command pending
        if (false == command_pending) continue;

        // get the latest fire delay command
        g_fire_delay_mutex.lock();
        static int fire_delay = g_fire_delay;
        g_fire_delay_mutex.unlock();

        // check for invalid fire delay
        if (-1 == fire_delay)
            continue;  // TODO: maybe set command pending to false here

        // check for no fire commands actually received yet
        if (fire_count <= 0) continue;

        // check if time to fire
        int fire_tick = tick_count + fire_delay;
        if (command_pending && tick_count >= fire_tick)
        {
            fprintf(stdout, "firing now!\n");

            // clear command pending
            g_fire_delay_mutex.lock();
            g_command_pending = false;
            g_fire_delay_mutex.unlock();
        }

        // increment the fire count
        fire_count += 1;

        // increment the clock tick
        tick_count += 1;

        // sleep for a second (there are likely better ways to handle clock -
        // system tick hardware interrupt on a microcontroller)
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

void read_cmd_thread()
{
    while (true)
    {
        int fire_delay = 0;
        int rc = scanf("%d", &fire_delay);

        // got an integer
        if (rc == 1)
        {
            if (fire_delay == -1)
            {
                // cancel outstanding fire commands
                g_command_pending_mutex.lock();
                g_command_pending = false;
                g_fire_delay_mutex.unlock();
            }
            else
            {
                // set fire delay
                g_fire_delay_mutex.lock();
                g_fire_delay = fire_delay;
                g_fire_delay_mutex.unlock();

                // set command pending
                g_command_pending_mutex.lock();
                g_command_pending = true;
                g_fire_delay_mutex.unlock();

                printf("%d\n", fire_delay);
            }
        }
        else if (rc == EOF)
        {
            break;  // stdin was closed
        }
        else
        {
            std::scanf(
                "%*s");  // consume the non-integer character and throw it away
        }
    }
}

int main(void)
{
    std::thread fire_thread(fire_1000ms_thread);
    std::thread read_thread(read_cmd_thread);

    fire_thread.join();
    read_thread.join();

    fprintf(stderr, "TODO: implement propulsion server\n");

    return 0;
}
