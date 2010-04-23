%%% babysitter_config_srv.erl
%% @author Ari Lerner <arilerner@mac.com>
%% @copyright 04/22/10 Ari Lerner <arilerner@mac.com>
%% @doc The babysitter configuration
-module (babysitter_config).
-include ("babysitter.hrl").

-export ([read/1]).

-define (BABYSITTER_CONFIG_DB, 'babysitter_config_db').
-define (CONF_EXTENSION, ".conf").

%%-------------------------------------------------------------------
%% @spec (Dir::list()) -> {ok, FileNames}
%% @doc Take a directory of files and read and parse them into the 
%%      ets database.
%% @end
%%-------------------------------------------------------------------
read(Dir) ->
  case filelib:is_dir(Dir) of
    true -> read_dir(Dir);
    false ->
      case filelib:is_file(Dir) of
        true -> read_files([Dir], []);
        false -> throw({badarg, "Argument must be a file or a directory"})
      end
  end.

%%-------------------------------------------------------------------
%% @spec (Dir::list()) -> {ok, Files::list()}
%% @doc Read a directory and parse the the conf files
%%      
%% @end
%%-------------------------------------------------------------------
read_dir(Dir) ->
  Files = lists:filter(fun(F) -> filelib:is_file(F) end, filelib:wildcard(lists:flatten([Dir, "/*"]))),
  read_files(Files, []).

%%-------------------------------------------------------------------
%% @spec (Files::list(), Acc::list()) -> {ok, Files}
%% @doc Take a list of files and parse them into the config db
%%      
%% @end
%%-------------------------------------------------------------------
read_files([], Acc) -> {ok, lists:reverse(Acc)};
read_files([File|Rest], Acc) ->
  ok = parse_config_file(File),
  read_files(Rest, [filename:basename(File)|Acc]).

%%-------------------------------------------------------------------
%% @spec (Filepath::string()) ->    ok
%%                                | {error, Reason}
%% @doc Parse the file at Filepath
%%      
%% @end
%%-------------------------------------------------------------------
parse_config_file(Filepath) ->
  X = babysitter_config_parser:file(Filepath),
  erlang:display(X),
  ok.

%%-------------------------------------------------------------------
%% @spec (Proplist::list()) -> record()
%% @doc Take a proplist and fill a config_rec with the
%%      actions from the proplist
%% @end
%%-------------------------------------------------------------------
fill_record_from_proplist(Proplist) ->
  ok.

extract_into_action_rec(Param, Proplist) ->
  case proplists:get_value(Param, Proplist) of
    undefined -> #action_rec{};
    Value ->
      Before = proplists:get_value(pre, Value),
      Command = proplists:get_value(command, Value),
      After = proplists:get_value(post, Value),
      #action_rec{pre = Before, command = Command, post = After}
  end.