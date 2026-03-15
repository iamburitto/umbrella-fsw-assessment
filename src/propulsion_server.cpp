
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
#include <mutex>
#include <thread>

// I know this is "bad" for some reason, but I hate namespace:: clutter
using namespace std;  

struct fire_command_t
{
    int seq; // increments on every new command so the fire thread can detect updates
    int fire_delay;
    bool command_pending;
};

static fire_command_t g_fire_command = {0, 0, false};;
static mutex g_fire_command_mutex;

static void propulsion_fire_thread()
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
            local_command = g_fire_command;
        }

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
        if (local_command.seq != last_seq)
        {
            fire_tick = tick_count + local_command.fire_delay;
            last_seq = local_command.seq;
        }
        
        // clang-format off
        fprintf(stdout, "tick=%3d pending=%d delay=%3d target=%3d fired=%2d\n",
            tick_count,
            local_command.command_pending,
            local_command.fire_delay,
            fire_tick,
            fire_count);
        // clang-format on

        // check if time to fire
        if (tick_count >= fire_tick)
        {
            // clear command pending
            {
                lock_guard<mutex> lock(g_fire_command_mutex);
                g_fire_command.command_pending = false;
            }

            fprintf(stdout, "[tick=%d] FIRE target=%d count=%d\n",
                tick_count,
                fire_tick,
                fire_count + 1);

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
}

void read_cmd_thread()
{
    string line;

    while (getline(cin, line))
    {
        istringstream iss(line);
        int fire_delay = 0;
        char extra = '\0';

        // skip line if it doesn't contain an integer at all
        if (!(iss >> fire_delay))
        {
            continue;
        }

        // skip lines that have extra non-whitespace characters at the end
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
                g_fire_command.command_pending = false;
            }
            
            fprintf(stdout, "[cmd] cancel\n");
        }
        else if (fire_delay < 0)
        {
            continue;
        }
        else
        {
            {
                lock_guard<mutex> lock(g_fire_command_mutex);
                g_fire_command.seq += 1;
                g_fire_command.fire_delay = fire_delay;
                g_fire_command.command_pending = true;
            }
            
            printf("[cmd] fire in %d sec\n", fire_delay);
        }
    }
}

int main(void)
{
    std::thread fire_thread(propulsion_fire_thread);
    std::thread read_thread(read_cmd_thread);

    fire_thread.join();
    read_thread.join();

    return 0;
}
