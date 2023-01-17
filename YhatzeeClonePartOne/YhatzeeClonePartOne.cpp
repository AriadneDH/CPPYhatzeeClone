/* Ariadne D-H Yhatzee Clone Part One
   Create fundamental Runcycle for the project that has basic functions of single player Yhatzee game
   In a command line terminal
*/


#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>

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
                if (this->dice_keep[i])
                {
                    //more efficient placement instead of another loop
                    this->dice_keep[i] = false;
                    continue;
                }
                this->dice_values[i] = (rand() % 6) + 1;
            }
            
        }

    public:
        //only constructor, does one roll
        dice()
        {
            this->roll_count = 0;
            rollDice();
        }
        //pass index of the die to keep or not, 0-4 only, false signals invalid input
        bool keepDie(int die_index)
        {
            if (0 <= die_index && die_index <= 4)
            {
                if (this->dice_keep[die_index])
                {
                    this->dice_keep[die_index] = false;
                }
                else
                {
                    this->dice_keep[die_index] = true;
                }
                return true;
            }
            return false;
        }
        //tries roll, returns false if no rolls left
        bool tryRoll()
        {
            if (this->roll_count < 2)
            {
                rollDice();
                this->roll_count++;
                return true;
            }
            return false;
        }
        //Resets the die for the next turn
        void nextTurn()
        {
            this->roll_count = 0;
            for (int i = 0; i < 5; i++)
            {
                this->dice_keep = false;
            }
            rollDice();

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
*   String player_name.
*   object dice = contained within the turn loop system.
*   object game_score = the score object, contained here for data control
*
*   getters and setters, used to refrence the game display
*   check game end = if end game returns a true goes throug the game end process
*/
class gameState
{
    private:
        string player_name;


    public:

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
        
        //bool yhatzee_bonus[13]; just make an exception to yhatzee scoring and run total of yhatzee bonus

        //cuts repeated code down
        bool scoreTaken(int score_index)
        {
            if (this->score_played[score_index])
            {
                return true;
            }
            return false;
        }
        //function for bottom score types
        //2 3 4 and 5 of a kind, if at any point
        bool countLikeVal(vector<int> dice_values, int numto_count)
        {
            int likenum_count = 0;
            for (int i = 0; i < 5; i++)
            {
                if (i != 0)
                {
                    if (dice_values[i] == dice_values[(i - 1)])
                    {
                        continue;
                    }
                    for (int& dice_val : dice_values)
                    {
                        if (dice_val == dice_values[i])
                        {
                            likenum_count++;
                        }

                    }

                }
                else
                {
                    for (int& dice_val : dice_values)
                    {
                        if (dice_val == dice_values[i])
                        {
                            likenum_count++;
                        }
                        //when yahtzee if any not equal simply exit
                        else if (numto_count == 5)
                        {
                            return false;
                        }

                    }
                }
                //for all if it has less than num to count then go again
                //for full house use, if it has more than two for the two check continue
                if (likenum_count < numto_count || (numto_count == 2 && likenum_count != 2))
                {
                    likenum_count = 0;
                    continue;
                }
                
                //has same or more than req num of num
                return true;
            }
            //no instances of specified count
            return false;
        }
        //function for the two straights
        //find lowest value, count consecutive +1 increases
        //attempt 2x for target lenght of 
        bool checkStraight(vector<int> dice_values, int target_length)
        {
            int straight_length = 1;
            sort(dice_values.begin(), dice_values.end());
            for (int i = 0; i < 4; i++)
            {
                if (dice_values[i] + 1 == dice_values[i + ])
                {
                    straight_length++;
                }
                else
                {
                    straight_length = 1;
                }
                if (straight_length == target_length)
                {
                    return true;
                }
            }
            return false;
        }

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
           
        }
        //getters
        vector<bool> getScorePlayed()
        {
            return { this->score_played[0],this->score_played[1],this->score_played[2] ,this->score_played[3], 
                this->score_played[4], this->score_played[5], this->score_played[6], this->score_played[7],
            this->score_played[8], this->score_played[9] , this->score_played[10] , this->score_played[11],
            this->score_played[12]};
        }
        vector<int> getScoreValues()
        {
            return { this->score_values[0],this->score_values[1],this->score_values[2] ,this->score_values[3],
                this->score_values[4], this->score_values[5], this->score_values[6], this->score_values[7],
            this->score_values[8], this->score_values[9] , this->score_values[10] , this->score_values[11],
            this->score_values[12] };
        }
        int getTotalScore()
        {
            int running_total = 0;
            for (int score : this->score_values)
            {
                running_total += score;
            }
            if (this->upper_bonus)
            {
                running_total += 35;
            }
            this->total_score = running_total;
            return this->total_score;
        }
        bool getUpperBonus()
        {
            return this->upper_bonus;
        }
        
        //mutators
        //try to score basic yhatzee rules
        bool tryScore(int score_index, vector<int> dice_values)
        {
            if ((0 > score_index || score_index > 12) || scoreTaken(score_index))
            {
                return false;
            }
            
            //first 6 are identical
            //not realy complex enough to deserve a function though
            if (score_index < 6)
            {
                int likenum_count = 0;
                
                for (int& die_val : dice_values)
                {
                    if (die_val == (score_index + 1))
                    {
                        likenum_count++;
                    }
                }
                if (likenum_count == 0)
                {
                    return false;
                }
                this->score_values[score_index] = (likenum_count * (score_index + 1));
                this->score_played[score_index] = true;
                //check if upper bonus reached
                int upper_total = 0;
                for (int i = 0; i < 6; i++)
                {
                    upper_total += this->score_values[i];
                }
                if (upper_total >= 63 && this->upper_bonus == false)
                {
                    this->upper_bonus = true;
                }
                return true;
            }
            switch (score_index)
            {
                //while 3, 4, yhatzee, and full house could be encapsulated in one if,
                //I would still have to do a conditional to check which one, only slightly decresing repeted code.
                //3of a kind
                case 6:
                {
                    if (countLikeVal(dice_values, 3))
                    {
                        int added_dice = 0;
                        for (int& dice_val : dice_values)
                        {
                            added_dice += dice_val;
                        }
                        this->score_values[score_index] = added_dice;
                        this->score_played[score_index] = true;
                        return true;
                    }
                    return false;

                }
                //4 of a kind
                case 7:
                {
                    if (countLikeVal(dice_values, 4))
                    {
                        int added_dice = 0;
                        for (int& dice_val : dice_values)
                        {
                            added_dice += dice_val;
                        }
                        this->score_values[score_index] = added_dice;
                        this->score_played[score_index] = true;
                        return true;
                    }
                    return false;

                }
                //full house
                // exact same just recurses twice
                return false;
                case 8:
                {
                    if (countLikeVal(dice_values, 2) && countLikeVal(dice_values, 3))
                    {
                        this->score_values[score_index] = 25;
                        this->score_played[score_index] = true;
                        return true;
                    }
                    
                    return false;
                }
                case 9:
                {
                    if (checkStraight(dice_values, 4))
                    {
                        this->score_values[score_index] = 30;
                        this->score_played[score_index] = true;
                        return true;
                    }
                    return false;

                }
                case 10: 
                {
                    if (checkStraight(dice_values, 5))
                    {
                        this->score_values[score_index] = 40;
                        this->score_played[score_index] = true;
                        return true;
                    }
                    return false;

                }
                //Yhatzee
                case 11:
                {
                    if (countLikeVal(dice_values, 5))
                    {
                        
                        this->score_values[score_index] = 50;
                        this->score_played[score_index] = true;
                        return true;
                    }
                    return false;

                }
                //chance
                case 12:
                {
                    int added_dice = 0;
                    for (int& dice_val : dice_values)
                    {
                        added_dice += dice_val;
                    }
                    this->score_values[score_index] = added_dice;
                    this->score_played[score_index] = true;
                    return true;
                }
                default:
                    break;
                    
            }
            return false;
        }

        //try to cross off a score
        //used nested if because behavior not unique per case
        bool tryCross(int score_index)
        {
            
            if (0 > score_index || score_index > 12)
            {
                return false;   
            }
            if (scoreTaken(score_index))
            {
                return false;
            }
            this->score_played[score_index] = true;
            this->score_values[score_index] = 0;
            return true;
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
    vector<int> die_val = my_dice.getDiceVal();
    dice my_dice = dice();
    score my_score = score();
    my_dice.tryRoll();
    
    vector<int> test_dice = { 2, 2, 2, 2, 1 };
    //test cross
   bool t1 = my_score.tryCross(12);
    //try false cross
   bool t2 = my_score.tryCross(12);
    //try false play
   vector<int> diceval = my_dice.getDiceVal();
   bool t3 = my_score.tryScore(0, diceval);
    //try play
   bool t4 = my_score.tryScore(1, test_dice);
   */
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
