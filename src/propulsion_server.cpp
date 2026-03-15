
// clang-format on

// normal c things
#include <cstdbool>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// c++ things
#include <iostream>
#include <sstream>
#include <string>

/* for std::thread */
#include <cerrno>
#include <chrono>  // for millisecond sleep
#include <cstdio>
#include <mutex>
#include <thread>

using namespace std;  // I hate this but I hate seeing namespace:: everywhere
                      // more

// global shared fire delay + mutex
static int g_fire_delay = 0;
static mutex g_fire_delay_mutex;

// global shared command pending flag
static bool g_command_pending = false;
static mutex g_command_pending_mutex;

static void fire_1000ms_thread()
{
    int tick_count = 0;
    int fire_count = 0;
    int fire_tick = -1;
    int last_fire_delay = -1;

    while (true)
    {
        printf("tick count: %d\n", tick_count);

        // get the latest command pending status
        bool command_pending = false;
        {
            lock_guard<mutex> lock(g_command_pending_mutex);
            command_pending = g_command_pending;
        }

        // check for no command pending
        if (false == command_pending)
        {
            // reset fire tick state when no command is pending
            fire_tick = -1;
            last_fire_delay = -1;

            tick_count += 1;
            this_thread::sleep_for(chrono::seconds(1));
            continue;
        }

        // get the latest fire delay command
        int fire_delay = 0;
        {
            lock_guard<mutex> lock(g_fire_delay_mutex);
            fire_delay = g_fire_delay;
        }

        // compute a new fire tick for a new command
        if ((fire_tick < 0) || (fire_delay != last_fire_delay))
        {
            fire_tick = tick_count + fire_delay;
            last_fire_delay = fire_delay;
        }

        // check if time to fire
        if (tick_count >= fire_tick)
        {
            // clear command pending
            {
                lock_guard<mutex> lock(g_command_pending_mutex);
                g_command_pending = false;
            }

            fprintf(stdout, "firing now! %d\n", fire_tick);

            // increment the fire count
            fire_count += 1;

            // reset fire tick for next command
            fire_tick = -1;
            last_fire_delay = -1;
        }

        // increment the clock tick
        tick_count += 1;

        // sleep for a second (there are likely better ways to handle clock -
        // system tick hardware interrupt on a microcontroller)
        this_thread::sleep_for(chrono::seconds(1));
    }
} // TODO: There is still a bug here with entering the same command value in a row - it won't fire twice. For that we'd need a sequence number or timestamp.

void read_cmd_thread()
{
    string line;

    while (getline(cin, line))
    {
        istringstream iss(line);
        int fire_delay = 0;
        char extra = '\0';

        if (!(iss >> fire_delay))
        {
            continue;
        }

        if (iss >> extra)
        {
            continue;
        }

        if (fire_delay == -1)
        {
            {
                lock_guard<mutex> lock(g_command_pending_mutex);
                g_command_pending = false;
            }
        }
        else
        {
            {
                lock_guard<mutex> lock(g_fire_delay_mutex);
                g_fire_delay = fire_delay;
            }

            {
                lock_guard<mutex> lock(g_command_pending_mutex);
                g_command_pending = true;
            }

            // printf("%d\n", fire_delay);
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
