#include "pch.h"
#include "game.h"
#include <assert.h>

tictactoe_status const empty_board = {0};

tictactoe_status const strategy_x[] = 
{
#include "strategy_x.h"
};

tictactoe_status const strategy_o[] = 
{
#include "strategy_o.h"
};

tictactoe_game::tictactoe_game(void):
   started(false)
{
   status.assign(0);
}

bool tictactoe_game::is_winning(tictactoe_status const & status, tictactoe_player const player) const
{
   auto mark = static_cast<char>(player);
   return 
      (status[0] == mark && status[1] == mark && status[2] == mark) ||
      (status[3] == mark && status[4] == mark && status[5] == mark) ||
      (status[6] == mark && status[7] == mark && status[8] == mark) ||
      (status[0] == mark && status[4] == mark && status[8] == mark) ||
      (status[2] == mark && status[4] == mark && status[6] == mark) ||
      (status[0] == mark && status[3] == mark && status[6] == mark) ||
      (status[1] == mark && status[4] == mark && status[7] == mark) ||
      (status[2] == mark && status[5] == mark && status[8] == mark);
}

std::pair<tictactoe_cell, tictactoe_cell> const tictactoe_game::get_winning_line() const
{
   auto mark = static_cast<char>(tictactoe_player::none);
   if(is_victory(tictactoe_player::computer))
      mark = static_cast<char>(tictactoe_player::computer);
   else if(is_victory(tictactoe_player::user))
      mark = static_cast<char>(tictactoe_player::user);

   if(mark != 0)
   {
      if(status[0] == mark && status[1] == mark && status[2] == mark) 
         return std::make_pair(tictactoe_cell(0,0), tictactoe_cell(0,2));
      if(status[3] == mark && status[4] == mark && status[5] == mark)
         return std::make_pair(tictactoe_cell(1,0), tictactoe_cell(1,2));
      if(status[6] == mark && status[7] == mark && status[8] == mark)
         return std::make_pair(tictactoe_cell(2,0), tictactoe_cell(2,2));
      if(status[0] == mark && status[4] == mark && status[8] == mark)
         return std::make_pair(tictactoe_cell(0,0), tictactoe_cell(2,2));
      if(status[2] == mark && status[4] == mark && status[6] == mark)
         return std::make_pair(tictactoe_cell(0,2), tictactoe_cell(2,0));
      if(status[0] == mark && status[3] == mark && status[6] == mark)
         return std::make_pair(tictactoe_cell(0,0), tictactoe_cell(2,0));
      if(status[1] == mark && status[4] == mark && status[7] == mark)
         return std::make_pair(tictactoe_cell(0,1), tictactoe_cell(2,1));
      if(status[2] == mark && status[5] == mark && status[8] == mark)
         return std::make_pair(tictactoe_cell(0,2), tictactoe_cell(2,2));
   }

   return std::make_pair(tictactoe_cell(), tictactoe_cell());
}

void tictactoe_game::start(tictactoe_player const player)
{
   strategy.clear();
   if(player == tictactoe_player::computer)
      std::copy(std::begin(strategy_x), std::end(strategy_x), std::inserter(strategy, std::begin(strategy)));
   else if(player == tictactoe_player::user)
      std::copy(std::begin(strategy_o), std::end(strategy_o), std::inserter(strategy, std::begin(strategy)));
   status.assign(0);
   started = true;
}

bool tictactoe_game::move(tictactoe_cell const cell, tictactoe_player const player)
{
   if(status[cell.row*3 + cell.col] == 0)
   {
      status[cell.row*3 + cell.col] = static_cast<char>(player);

      if(is_victory(player))
      {
         started = false;
      }

      return true;
   }

   return false;
}

std::set<tictactoe_status> tictactoe_game::lookup_strategy() const
{
   std::set<tictactoe_status> nextsubstrategy;

   for(auto const & s : strategy)
   {
      bool match = true;
      for(int i = 0; i < 9 && match; ++i)
      {
         if(s[i] < status[i])
            match = false;
      }

      if(match)
      {
         nextsubstrategy.insert(s);
      }
   }

   return nextsubstrategy;
}

tictactoe_status tictactoe_game::lookup_move() const
{
   tictactoe_status newbest = {0};
   for(auto const & s : strategy)
   {
      int diff = 0;
      for(int i = 0; i < 9; ++i)
      {
         if(s[i] > status[i])
            diff++;
      }

      if(diff == 1)
      {
         newbest = s;
         if(is_winning(newbest, tictactoe_player::computer))
         {
            break;
         }
      }
   }

   assert(newbest != empty_board);

   return newbest;
}

tictactoe_cell tictactoe_game::move(tictactoe_player const player)
{
   tictactoe_cell cell;

   strategy = lookup_strategy();

   if(!strategy.empty())
   {
      auto newstatus = lookup_move();

      for(int i = 0; i < 9; ++i)
      {
         if(status[i] == 0 && newstatus[i]==static_cast<char>(player))
         {
            cell.row = i/3;
            cell.col = i%3;
            break;
         }
      }

      status = newstatus;

      if(is_victory(player))
      {
         started = false;
      }
   }

   return cell;
}