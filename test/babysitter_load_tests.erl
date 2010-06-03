-module (babysitter_load_tests).
-include_lib("eunit/include/eunit.hrl").

setup() ->
  application:start(sasl),
  ok = application:start(os_mon),
  ok = application:start(babysitter),
  ok.
  
teardown(_) ->
  ok = application:stop(babysitter),
  ok = application:stop(os_mon),
  application:stop(sasl),
  ok.

test_starting_node_test_() ->
  {spawn,
    {foreach,
      fun setup/0,
      fun teardown/1,
      [
        ?_test(test_memory())
      ]
    }
  }.

test_memory() ->
  _OriginalFreeMemory = proplists:get_value(free_memory, memsup:get_system_memory_data()),
  {ok, ErlProcess1, _Pid1} = babysitter:bs_spawn_run("sleep 100.1", []),
  _NewFreeMemory1 = proplists:get_value(free_memory, memsup:get_system_memory_data()),
  % Non-deterministic... unrealistic test
  % ?assert(OriginalFreeMemory > NewFreeMemory1),
  {ok, ErlProcess2, _Pid2} = babysitter:bs_spawn_run("sleep 101.1", []),
  _NewFreeMemory2 = proplists:get_value(free_memory, memsup:get_system_memory_data()),
  % ?assert(OriginalFreeMemory > NewFreeMemory2),
  {ok, ErlProcess3, _Pid3} = babysitter:bs_spawn_run("sleep 102.1", []),
  _NewFreeMemory3 = proplists:get_value(free_memory, memsup:get_system_memory_data()),
  % ?assert(OriginalFreeMemory > NewFreeMemory3),
  ErlProcess1 ! {stop}, ErlProcess2 ! {stop}, ErlProcess3 ! {stop},
  ?assert(true).