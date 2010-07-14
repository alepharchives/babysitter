#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "process_manager.h"
#include "minunit.h"
#include "test_helper.h"

char *test_new_process() {
  TEST_HEADER("test_new_process");
  process_t *test_process = NULL;
  pm_new_process(&test_process);
  mu_assert(test_process->env_c == 0, "processes env count is not zero when initialized");
  mu_assert(test_process->env == NULL, "processes env is not NULL when initialized");
  mu_assert(test_process->command == NULL, "processes command is not NULL when initialized");
  
  pm_free_process(test_process); return 0;
}

char *test_pm_check_pid_status() {
  TEST_HEADER("test_pm_check_pid_status");
  mu_assert(pm_check_pid_status(-2) == -1, "can send a signal to a process lower than 1");
  mu_assert(pm_check_pid_status(1000) != 0, "can send a signal to a process lower than 1");
  return 0;
}

char *test_pm_valid_process() {
  TEST_HEADER("test_pm_valid_process");
  process_t *test_process = NULL;
  pm_new_process(&test_process);
  
  mu_assert(pm_process_valid(&test_process) == -1, "process shouldn't be valid, but is");
  mu_assert(!pm_malloc_and_set_attribute(&test_process->command, "ls -l"), "copy command failed");
  mu_assert(pm_process_valid(&test_process) == 0, "process should be valid");
  
  pm_free_process(test_process); return 0;
}

char *test_pm_malloc_and_set_attribute() {
  TEST_HEADER("test_pm_malloc_and_set_attribute");
  process_t *test_process = NULL;
  pm_new_process(&test_process);
  
  mu_assert(!pm_malloc_and_set_attribute(&test_process->command, "ls -l"), "copy command failed");
  mu_assert(!strcmp(test_process->command, "ls -l"), "copy command copied the value properly");
  
  pm_free_process(test_process); return 0;
}

char *test_pm_add_env() {
  TEST_HEADER("test_pm_add_env");
  process_t *test_process = NULL;
  pm_new_process(&test_process);
  
  mu_assert(test_process->env_c == 0, "processes env count is not zero when initialized");
  mu_assert(test_process->env == NULL, "processes env is not NULL when initialized");
  
  pm_add_env(&test_process, "BOB=sally");
  mu_assert(test_process->env_c == 1, "the environment variable did not copy properly");
  mu_assert(!strcmp(test_process->env[0], "BOB=sally"), "the environment variable did not copy properly");
  
  pm_add_env(&test_process, "NICK=name");  
  mu_assert(test_process->env_c == 2, "the environment variable did not copy properly");
  mu_assert(!strcmp(test_process->env[0], "BOB=sally"), test_process->env[0]);
  mu_assert(!strcmp(test_process->env[1], "NICK=name"), test_process->env[1]);
  
  pm_add_env(&test_process, "ARI=awesome");  
  mu_assert(test_process->env_c == 3, "the environment variable did not copy properly");
  mu_assert(!strcmp(test_process->env[0], "BOB=sally"), test_process->env[0]);
  mu_assert(!strcmp(test_process->env[1], "NICK=name"), test_process->env[1]);
  mu_assert(!strcmp(test_process->env[2], "ARI=awesome"), test_process->env[2]);
  // pm_add_env
  pm_free_process(test_process); return 0;
}

char *test_starting_a_process()
{
  TEST_HEADER("test_starting_a_process");
  process_t *test_process = NULL;
  process_return_t *ret;
  pm_new_process(&test_process);
  
  mu_assert(!pm_malloc_and_set_attribute(&test_process->command, "sleep 10.1"), "copy command failed");
  ret = pm_run_and_spawn_process(test_process);
  mu_assert(kill(ret->pid, 0) == 0, "process did not start");
  
  kill(ret->pid, SIGKILL); // Kill it entirely
  
  mu_assert(!pm_malloc_and_set_attribute(&test_process->command, ""), "copy command failed");
  ret = pm_run_and_spawn_process(test_process);
  
  pm_free_process(test_process); return 0;
}

char *test_running_a_process_as_a_script()
{
  TEST_HEADER("test_running_a_process_as_a_script");
  process_t *test_process = NULL;
  pm_new_process(&test_process);
  mu_assert(!pm_malloc_and_set_attribute(&test_process->command, "#!/bin/bash\ntouch /tmp/blah"), "copy command failed");
  
  pm_run_process(test_process);
  
  struct stat buffer;
  
  int status = stat("/tmp/blah", &buffer);
  mu_assert(status == 0, "command to run didn't work");
  unlink("/tmp/blah");
  
  pm_free_process(test_process); return 0;
}

char *test_running_a_process_with_pid_file_output()
{
  TEST_HEADER("test_running_a_process_with_pid_file_output");
  process_t *test_process = NULL;
  pm_new_process(&test_process);
  mu_assert(!pm_malloc_and_set_attribute(&test_process->command, "#!/bin/bash\nsleep 100 &\necho $! > $PID_FILE\n"), "copy command failed");
  process_return_t *ret;
  
  ret = pm_run_and_spawn_process(test_process);
  mu_assert(kill(ret->pid, 0) == 0, "process did not start");
  kill(ret->pid, SIGKILL); // Kill it entirely
    
  pm_free_process(test_process); return 0;
}


char *test_killing_a_process()
{
  TEST_HEADER("test_killing_a_process");
  process_t *test_process = NULL;
  process_t *test_process2 = NULL;
  process_return_t *ret = NULL;
  
  pm_new_process(&test_process);
  pm_new_process(&test_process2);
  
  mu_assert(!pm_malloc_and_set_attribute(&test_process->command, "/bin/sleep 102"), "copy command failed");
  ret = pm_run_and_spawn_process(test_process);
  test_process2->pid = ret->pid;
  pm_kill_process(test_process2);
  
  mu_assert(kill(ret->pid, 0) != 0, "process did not die");
  
  kill(ret->pid, SIGKILL); // Kill it entirely
  pm_free_process(test_process2);
  pm_free_process(test_process); return 0;
}
