/* Ariadne D-H Yhatzee Clone Part One
   Create fundamental Runcycle for the project that has basic functions of single player Yhatzee game
   In a command line terminal
*/


#include <iostream>
#include <iomanip>
#include <fstream>

#include <time.h>
#include <vector>
#include <cstdlib>
using namespace std;
/* Dice object is contained within the turn object
*  keeps track of the dice values, calls roll.
*  array int [5] diceValues = dice values rolls once on initiation
*  array boolean [5] diceKeep = true false if a die will be keept next roll default all false
*  int rollCount = number of rolls used this turn
* 
*  getters for all var
*  close method terminates at the end of turn
*/
class dice
{
    private:
        int dice_values[5];
        //more efficient than declairing length then looping for define
        bool dice_keep[5] = { false, false, false, false, false };
        int roll_count;

        /* Dice rolling method, generates an array of psudo-random numbers
         *  in all false spots on the passed bool array. used by dice object instances
         *  --return Array int pointer dice values = the new generated values
         *  --Array bool[5] kept_dice = true values in this array refrence which dice arent rolled
         *  to be more clean, multiple calls, simply returns random num.
         *  return int the new random val,
         *  moved to the dice object
         */
        void rollDice()
        {
            srand(time(NULL));
            for (int i = 0; i < 5; i++)
            {
                if (dice_keep[i])
                {
                    //more efficient placement instead of another loop
                    dice_keep[i] = false;
                    continue;
                }
                dice_values[i] = (rand() % 6) + 1;
            }
            
        }

    public:
        //only constructor, does one roll
        dice()
        {
            roll_count = 0;
            rollDice();
        }
        //pass index of the die to keep or not, 0-4 only, false signals invalid input
        bool keepDie(int die_index)
        {
            if (0 <= die_index && die_index <= 4)
            {
                if (dice_keep[die_index])
                {
                    dice_keep[die_index] = false;
                }
                else
                {
                    dice_keep[die_index] = true;
                }
                return true;
            }
            return false;
        }
        //tries roll, returns false if no rolls left
        bool tryRoll()
        {
            if (roll_count < 2)
            {
                rollDice();
                roll_count++;
                return true;
            }
            return false;
        }
        //getters
        vector<bool> getDiceKeep()
        {
            return { this->dice_keep[0], this->dice_keep[1], this->dice_keep[2], this->dice_keep[3], this->dice_keep[4] };
        }
        vector<int> getDiceVal()
        {
            return { this->dice_values[0], this->dice_values[1], this->dice_values[2], this->dice_values[3], this->dice_values[4] };
        }
        int getRollCount()
        {
            return this->roll_count;
        }
        
};

/* Contains the turn object and score object, refrences the print screen function.
*  created by the run game fucntion. interfaces between score and turn initiates and terminates
*  turns.
*
*   object dice = contained within the turn loop system.
*   object game_score = the score object, contained here for data control
*
*   getters and setters, used to refrence the game display
*   check game end = if end game returns a true goes throug the game end process
*/
class gameState
{

};


/* Contains all of the score category values, a boolean if they are played
*  and the score value once played, is called to update and refrence score data.
*  
*  Array int [13] scoreValues = array of all the score values of the game state
*  Array boolean[13] scorePlayed = tracks which plays have been made
*  int totalScore = the combined gamescore
*  
*  play (category) = attempts to play category, checks if dice are aplicable calls update score
*  zero (category) = attempts to zero a category, checks if allready filled calls update score
*  end_game = if all 13 boxes checked, calculates bonuses and calls update score
*  getters for all peices
*/
class score
{
    private:
        int score_values[13];
        bool score_played[13];
        int total_score;
        bool upper_bonus;
        int yhatzee_bonus;
        //bool yhatzee_bonus[13]; just make an exception to yhatzee scoring and run total of yhatzee bonus

    public:
        score()
        {
            for (int i = 0; i < 13; i++)
            {
                this->score_values[i] = 0;
                this->score_played[i] = false;
            }
            this->total_score = 0;
            this->upper_bonus = false;
            this->yhatzee_bonus = 0;
        }
        //getters
        vector<bool> getScorePlayed()
        {
            return { this->score_played[0],this->score_played[1],this->score_played[2] ,this->score_played[3], 
                this->score_played[4], this->score_played[5], this->score_played[6], this->score_played[7],
            this->score_played[8], this->score_played[9] , this->score_played[10] , this->score_played[11],
            this->score_played[12]};
        }
        vector<int> getDiceVal()
        {
            return { this->score_values[0],this->score_values[1],this->score_values[2] ,this->score_values[3],
                this->score_values[4], this->score_values[5], this->score_values[6], this->score_values[7],
            this->score_values[8], this->score_values[9] , this->score_values[10] , this->score_values[11],
            this->score_values[12] };
        }
        int getTotalScore()
        {
            return this->total_score;
        }
        bool getUpperBonus()
        {
            return this->upper_bonus;
        }
        //number of yhatzee bonus;
        int getYhatzeeBonus()
        {
            return this->yhatzee_bonus;
        }

        //try to score basic yhatzee rules
        bool tryScore(int score_index)

        //try to cross off a score
        //used nested if because behavior not unique per case
        bool tryCross(int score_index)
        {
            
            if (0 <= score_index && score_index <= 13)
            {
               
                if (this->score_played[score_index])
                {
                    return false;
                }
                else
                {
                    this->score_played[score_index] = false;
                    this->score_values[score_index] = 0;
                    return true;
                }
            }
            return false;
        }
};



/* Runs the game, ends a session, ends the application, can generate a new
*  game instance. passes different ints to main for close based on different end cases
*  also interprates and controlls user input to call appropriate methods, meat and potatos of calls.
* 
*  0 regular end
*  1 crash
* 
*  Object gamestate = current gamestate
*  
*  new_game = creates a new game instance
*  end_game = ends the current game instance
*  quit_app = closes the application
*/
int runYhatzee()
{
   /* dice my_dice = dice();
    bool var1 = my_dice.tryRoll();
    bool var2 = my_dice.keepDie(1);
    bool var3 = my_dice.keepDie(2);
    bool var4 = my_dice.tryRoll();
    bool var5 = my_dice.keepDie(6);
    vector<bool> die_keep = my_dice.getDiceKeep();
    vector<int> die_val = my_dice.getDiceVal();*/
    return 1;
}
/* cleans and writes to screen to function as a game interface
*  todo specs for this fun, later priority
*
private -- screenPrint(-different peices of data to pass for screen printing-)
*/
/* More dummed down print metod for in-development testing
*  todo, specs of method
*
int testPrint(-associated data, and different print cases for use in testing - )
{

}
*/
/* Main method, initiates game run
*  prints difrent return values based on
*  how game ended
*/
int main()
{
    int returnVal = runYhatzee();
    return returnVal;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
