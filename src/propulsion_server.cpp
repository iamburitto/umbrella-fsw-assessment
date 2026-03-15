
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

using namespace std;  // I hate this but I hate seeing namespace:: everywhere more

typedef struct
{
    int seq;
    int fire_delay;
    bool command_pending;
} fire_command_t;

static fire_command_t g_fire_command;
static mutex g_fire_command_mutex;

static void fire_1000ms_thread()
{
    int tick_count = 0;
    int fire_count = 0;
    int fire_tick = -1;
    int last_seq = 0;

    while (true)
    {
        fire_command_t local_command;
        // get the latest fire command and status
        {
            lock_guard<mutex> lock(g_fire_command_mutex);
            local_command.seq = g_fire_command.seq;
            local_command.fire_delay = g_fire_command.fire_delay;
            local_command.command_pending = g_fire_command.command_pending;
        }

        // clang-format off
        printf("[tick=%d] pending=%d delay=%d target=%d fire_count=%d\n",
            tick_count,
            local_command.command_pending,
            local_command.fire_delay,
            fire_tick,
            fire_count);
        // clang-format on

        // check for no command pending
        if (false == local_command.command_pending)
        {
            // reset fire tick state when no command is pending
            fire_tick = -1;
            tick_count += 1;
            this_thread::sleep_for(chrono::seconds(1));
            continue;
        }

        // compute a new fire tick for a new command
        if ((fire_tick < 0) || (local_command.seq != last_seq)) // there is a bug here. Won't accept identical fire commands, gotta fix that
        {
            fire_tick = tick_count + local_command.fire_delay;
            last_seq = local_command.seq;
        }

        // check if time to fire
        if (tick_count >= fire_tick)
        {
            // clear command pending
            {
                lock_guard<mutex> lock(g_fire_command_mutex);
                g_fire_command.command_pending = false;
            }

            fprintf(stdout, "firing now! %d\n", fire_tick);

            // increment the fire count
            fire_count += 1;

            // reset fire tick for next command
            fire_tick = -1;
        }

        // increment the clock tick
        tick_count += 1;

        // sleep for a second (there are likely better ways to handle clock -
        // system tick hardware interrupt on a microcontroller)
        this_thread::sleep_for(chrono::seconds(1));
    }
}  // TODO: There is still a bug here with entering the same command value in a
   // row - it won't fire twice. For that we'd need a sequence number or
   // timestamp.

void read_cmd_thread()
{
    string line;

    while (getline(cin, line))
    {
        istringstream iss(line);
        int fire_delay = 0;
        char extra = '\0';

        // if we don't get an integer
        if (!(iss >> fire_delay))
        {
            continue;
        }
        
        // if there's a newline
        if (iss >> extra)
        {
            continue;
        }
        
        // -1 means cancel the pending fire
        if (fire_delay == -1)
        {
            {
                lock_guard<mutex> lock(g_fire_command_mutex);
                g_fire_command.seq += 1;
                g_fire_command.fire_delay = -1; // not sure if this should be here
                g_fire_command.command_pending = false;
            }
        }
        else
        {
            {
                lock_guard<mutex> lock(g_fire_command_mutex);
                g_fire_command.seq += 1;
                g_fire_command.fire_delay = fire_delay;
                g_fire_command.command_pending = true;
            }

            printf("%d\n", fire_delay);
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
