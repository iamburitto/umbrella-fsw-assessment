#include <assert.h>
#include <stdio.h>

static void test_fires_after_delay()
{
  assert(!"TODO: implement fire-after-delay test");
}

static void test_new_command_overwrites_pending_command()
{
  assert(!"TODO: implement overwrite test");
}

static void test_minus_one_cancels_outstanding_commands()
{
  assert(!"TODO: implement cancel test");
}

static void test_can_fire_multiple_times_in_one_execution()
{
  assert(!"TODO: implement repeated-fire test");
}

int main()
{
  test_fires_after_delay();
  test_new_command_overwrites_pending_command();
  test_minus_one_cancels_outstanding_commands();
  test_can_fire_multiple_times_in_one_execution();

  puts("all tests passed");
  return 0;
}
