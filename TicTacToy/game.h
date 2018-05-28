#pragma once

enum class tictactoe_player : char
{
   none = 0,
   computer = 1,
   user = 2,
};

typedef std::array<char, 9> tictactoe_status;

struct tictactoe_cell
{
   int row;
   int col;

   tictactoe_cell(int r = INT_MAX, int c = INT_MAX):row(r), col(c)
   {}

   bool is_valid() const {return row != INT_MAX && col != INT_MAX;}
};

class tictactoe_game
{
   bool started;
   tictactoe_status status;
   std::set<tictactoe_status> strategy;

   std::set<tictactoe_status> lookup_strategy() const;
   tictactoe_status lookup_move() const;

   bool is_winning(tictactoe_status const & status, tictactoe_player const player) const;
   bool is_full(tictactoe_status const & status) const {return 0 == std::count_if(std::begin(status), std::end(status), [](int const mark){return mark == 0;});}

public:
   tictactoe_game(void);

   void start(tictactoe_player const player);
   bool move(tictactoe_cell const cell, tictactoe_player const player);
   tictactoe_cell move(tictactoe_player const player);

   bool is_started() const {return started;}
   bool is_victory(tictactoe_player const player) const {return is_winning(status, player);}
   bool is_finished() const 
   {
      return is_full(status) || 
             is_victory(tictactoe_player::user) ||
             is_victory(tictactoe_player::computer);
   }

   tictactoe_status const get_status() const {return status;}
   std::pair<tictactoe_cell, tictactoe_cell> const get_winning_line() const;
};

