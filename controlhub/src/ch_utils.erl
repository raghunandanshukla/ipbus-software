%% Author: rob
%% Created: Dec 15, 2010
%% Description: TODO: Add description to utils
-module(ch_utils).

-include("ch_global.hrl").


%% Exported Functions
-export([print_binary_as_hex/1]).


%%% ------------------------------------------------------------------------------------
%%% API Functions (public interface)
%%% ------------------------------------------------------------------------------------

%% -------------------------------------------------------------------------------------
%% @doc Given a binary containing an integer number of 32-bit words, it will print to
%%      console the content as a column of 32-bit-wide hex-numbers.
%% 
%% @spec print_binary_as_hex(Binary::binary()) -> ok.
%% @end
%% -------------------------------------------------------------------------------------
print_binary_as_hex(Binary) when is_binary(Binary) ->
    case size(Binary) rem 4 of
        0 -> do_print_binary_as_hex(Binary);
        _ -> {error, needs_integer_number_32bit_words}
    end.


%%% --------------------------------------------------------------------
%%% Internal functions (private)
%%% --------------------------------------------------------------------

%% Extracts a 32-bit value from the binary and breaks it down into eight nibbles
%% that get converted into the characters 0-9,a-f as appropriate and printed to
%% the console. Rinse and repeat until binary is sucked dry.
%% @spec do_print_binary_as_hex(binary()) -> ok.
do_print_binary_as_hex(<<Nibble1:4/integer, Nibble2:4/integer, Nibble3:4/integer, Nibble4:4/integer,
                         Nibble5:4/integer, Nibble6:4/integer, Nibble7:4/integer, Nibble8:4/integer,
                         Remainder/binary>>) ->
  HexNumbersLine = [Nibble1, Nibble2, Nibble3, Nibble4, Nibble5, Nibble6, Nibble7, Nibble8],
  HexLettersLine = lists:map(fun(X) -> hex_number_to_hex_letter(X) end, HexNumbersLine),
  io:format("    0x~c~c~c~c~c~c~c~c~n", HexLettersLine),
  do_print_binary_as_hex(Remainder);

do_print_binary_as_hex( << >> ) -> ok.


%% Converts a number between 0-15 to the appropriate ASCII hex chararacter (0123456789abcdef)
%% @spec hex_number_to_hex_letter(HexNumber::integer()) -> integer()
hex_number_to_hex_letter(HexNumber) ->
    case HexNumber of
        0  -> $0;
        1  -> $1;  
        2  -> $2;
        3  -> $3;
        4  -> $4;
        5  -> $5;
        6  -> $6;
        7  -> $7;
        8  -> $8;
        9  -> $9;
        10 -> $a;
        11 -> $b;
        12 -> $c;
        13 -> $d;
        14 -> $e;
        15 -> $f
    end.
