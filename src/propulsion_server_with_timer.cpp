// normal c things
#include <cstdio>

// c++ things
#include <iostream>
#include <sstream>
#include <string>

/* for std::thread */
#include <chrono>  // for millisecond sleep
#include <condition_variable>
#include <mutex>
#include <thread>

// I know this is "bad" for some reason, but I hate namespace:: clutter
using namespace std;

struct fire_command_t
{
    int seq;  // increments on every new command so the fire thread can detect
              // updates
    bool command_pending;
    int fire_delay;
    chrono::steady_clock::time_point fire_deadline;
    bool shutdown_requested;
};

static mutex g_fire_command_mutex;
static condition_variable g_fire_command_cv;
static fire_command_t g_fire_command = {
    0, false, 0, chrono::steady_clock::time_point(), false};

// fire thread sleeps indefinitely until a command arrives
// read thread wakes it with notifier by calling notify_one()
static void propulsion_fire_thread()
{
    while (true)
    {
        fire_command_t local_command;
        unique_lock<mutex> lock(g_fire_command_mutex);

        while (false == g_fire_command.command_pending)
        {
            if (true == g_fire_command.shutdown_requested)
            {
                return;
            }

            fprintf(stdout, "[fire] idle\n");
            g_fire_command_cv.wait(lock);
        }

        local_command = g_fire_command;

        auto now = chrono::steady_clock::now();
        auto remaining =
            chrono::duration_cast<chrono::seconds>(
                local_command.fire_deadline - now)
                .count();

        fprintf(stdout,
                "[fire] seq=%d pending=%d delay=%d remaining=%ld\n",
                local_command.seq,
                local_command.command_pending,
                local_command.fire_delay,
                remaining);

        cv_status wait_status =
            g_fire_command_cv.wait_until(lock, local_command.fire_deadline);

        if (wait_status == cv_status::no_timeout)
        {
            if (true == g_fire_command.shutdown_requested)
            {
                return;
            }

            fprintf(stdout, "[fire] updated before deadline\n");
            continue;
        }

        if ((true == g_fire_command.command_pending) &&
            (g_fire_command.seq == local_command.seq))
        {
            g_fire_command.command_pending = false;
            lock.unlock();

            fprintf(stdout,
                    "[fire] seq=%d firing now!\n",
                    local_command.seq);
        }
    }
}

static void read_cmd_thread()
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

            // notify fire thread when shared state changes
            g_fire_command_cv.notify_one();
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
                g_fire_command.fire_deadline =
                    chrono::steady_clock::now() +
                    chrono::seconds(fire_delay);  // compute absolute fire deadline
            }

            // notify fire thread when shared state changes
            g_fire_command_cv.notify_one();
            fprintf(stdout, "[cmd] fire in %d sec\n", fire_delay);
        }
    }

    {
        lock_guard<mutex> lock(g_fire_command_mutex);
        g_fire_command.shutdown_requested = true;
    }

    g_fire_command_cv.notify_one();
}

int main(void)
{
    thread fire_thread(propulsion_fire_thread);
    thread read_thread(read_cmd_thread);

    fire_thread.join();
    read_thread.join();

    return 0;
}
