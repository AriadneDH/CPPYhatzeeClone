/* Ariadne D-H Yhatzee Clone Part One
   Create fundamental Runcycle for the project that has basic functions of single player Yhatzee game
   In a command line terminal
*/


#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>

#include <time.h>
#include <array>
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
         * 
         *  update to a diffrent method of random whenever implementing the final version
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
                this->dice_keep[i] = false;
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
                if (dice_values[i] + 1 == dice_values[i + 1])
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
/* Contains the dice object and score object, 
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
        dice dice_set;
        score game_score;
       
        //attempt a score
        bool scorePlay(int box_choice)
        {
            vector<int> dice_vals = this->dice_set.getDiceVal();
            return this->game_score.tryScore(box_choice, dice_vals);

        }
        //attempt a cross
        bool scoreCross(int box_choice)
        {
            return this->game_score.tryCross(box_choice);
        }

    public:
        gameState()
        {
            this->player_name = "unnamed_user";
            this->dice_set = dice();
            this->game_score = score();

        }
        gameState(string player_name)
        {
            this->player_name = player_name;
            this->dice_set = dice();
            this->game_score = score();

        }
        /* Game interaction methods
        *  used for attempts to do specific
        *  interactions with or between objects.
        *  most return bool to show proper interaction
        * 
        *  takes the inputs, calls appropriate methods
        *  overloads for the current implementation
        */

        //dice interactions
        bool diceInteract()
        {
            bool roll_check = this->dice_set.tryRoll();
            return roll_check;
        }
        bool diceInteract(int die_index)
        {
            bool valid_index = this->dice_set.keepDie(die_index);
            return valid_index;
        }

        //score interactions
        // method true = play false = cross
        // return false = invalid choice
        bool scoreInteract(bool method_choice, int box_choice)
        {
            bool valid_interaction = false;
            if (method_choice)
            {
                valid_interaction = scorePlay(box_choice);
            }
            else
            {
                valid_interaction = scoreCross(box_choice);
            }
            
            if (valid_interaction)
            {
                this->dice_set.nextTurn();
            }
            return valid_interaction;
        }
        //checks all the boxes are played, returns early if one is false
        bool attemptGameEnd()
        {
            bool valid_end = false;
            vector<bool> scores_played = this->game_score.getScorePlayed();
            for (int i = 0; i < 13; i++)
            {
                if (scores_played[i] == false)
                {
                    return valid_end;
                }
            }
            valid_end = true;
            return valid_end;
        }
        


        /* Getter methods for the diffrent data peices.
        *  for the objects they grab diffrent parts for display
        *  this abstraction is done so that a function can have
        *  access to getting the data peices without having acess
        *  to the interaction function calls
        */

        //return the dice values
        std::array<int, 5> getDiceVals()
        {
            std::array<int, 5> dice_vals;
            //direct copy later
            std::vector<int> dice_copy = this->dice_set.getDiceVal();
            for (int i = 0; i < 5; i++)
            {
                dice_vals[i] = dice_copy[i];
            }
            return dice_vals;
        }

        //return the saved dice
        std::array<bool, 5> getSavedDice()
        {
            std::array<bool, 5> dice_kept;
            //direct copy later
            std::vector<bool> dice_copy = this->dice_set.getDiceKeep();
            for (int i = 0; i < 5; i++)
            {
                dice_kept[i] = dice_copy[i];
            }
            return dice_kept;
        }


            //return the score values
        std::array<int, 13> getScoreVals()
        {
            std::array<int, 13> score_vals;
            //direct copy later
            std::vector<int> score_copy = this->game_score.getScoreValues();
            for (int i = 0; i < 13; i++)
            {
                score_vals[i] = score_copy[i];
            }
            return score_vals;
        }
            //return the taken scores
        std::array<bool, 13> getTakenScores()
        {
            std::array<bool, 13> taken_scores;
            //direct copy later
            std::vector<bool> score_copy = this->game_score.getScorePlayed();
            for (int i = 0; i < 13; i++)
            {
                taken_scores[i] = score_copy[i];
            }
            return taken_scores;
        }
            //return the roll count
        int getRollCount()
        {
            return this->dice_set.getRollCount();
        }

            //return the total scores
        int getTotalScore()
        {
            return this->game_score.getTotalScore();
        }
            //return the top bonus
        bool getUpperBonus()
        {
            return this->game_score.getUpperBonus();
        }
        std::string& getPlayerName()
        {
            return this->player_name;
        }


};

/* write up the screen print for use in data display
* passed a gamestate object 
* 
*/
void screenPrint(gameState player_object)
{
    std::array<int, 13> score_vals = player_object.getScoreVals();
    std::array<bool, 13> score_played = player_object.getTakenScores();

    std::array<int,5> dice_vals = player_object.getDiceVals();
    std::array<bool,5> saved_dice = player_object.getSavedDice();

    int roll_count = player_object.getRollCount() + 1;
    int total_score = player_object.getTotalScore();
    bool upper_bonus = player_object.getUpperBonus();
    string player_name = player_object.getPlayerName();

    cout << player_name << endl;
    //scores
    for(int i = 0; i < 13; i++)
    {
        cout << score_vals[i] << ", ";
    }
    if (upper_bonus)
    {
        cout << "T" << endl;
    }
    else
    {
        cout << "F" << endl;
    }
    for (int i = 0; i < 13; i++)
    {
        if (score_played[i])
        {
            cout << "T, ";
        }
        else
        {
            cout << "F, ";
        }
    }
    cout << endl;
    //dice
    for (int i = 0; i < 5; i++)
    {
        cout << dice_vals[i] << ", ";
    }
    cout << roll_count << endl;
    for (int i = 0; i < 5; i++)
    {
        if (saved_dice[i])
        {
            cout << "T, ";
        }
        else
        {
            cout << "F, ";
        }
    }
    cout << endl;
}
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

    // going to run a full game test case
    gameState new_player = gameState("Ari");
    /*unintuitive parameters
    * for the calls, as the point is
    * just testing
    *
    * dice interactions
    * 1 roll
    * 2 keep index 0
    * 3 keep index 1
    * 4 keep index 2
    * 5 keep index 3
    * 6 keep index 4
    * score interactions
    * 7 play index 0, 8 play index 1, 9 play index 2
    * 10 play index 3, 11 play index 4, 12 play index 5
    * 13 play index 6, 14 play index 7, 15 play index 8
    * 16 play index 9, 17 play index 10, 18 play index 11
    * 19 play index 12
    *
    * 20 cross index 0, 21 cross index 1, 22 cross index 2
    * 23 cross index 3, 24 cross index 4, 25 cross index 5
    * 26 cross index 6, 27 cross index 7, 28 cross index 8
    * 29 cross index 9, 30 cross index 10, 31 cross index 11
    * 32 cross index 12
    *
    * gamestate exclusive
    * 
    * 33 try end early.
    * 
    * //test early
    * 34 try keep dice bellow index
    * 35 try play above index
    * 36 try cross above index
    *
    * this wont be pretty
    */
    while (new_player.attemptGameEnd() == false)
    {
        screenPrint(new_player);
        int choice = 0;
        cin >> choice;
        cout << endl;
        //This goes agianst everything I do as a SE but it is just for logic/case testing
        switch (choice)
        {
            //Just a repeated format that could be so much more efficient and secure in every way
            case 1:
            {
                if(new_player.diceInteract())
                { 
                    cout << "Valid Selection" << endl;
                }
                else
                {
                    cout << "Invalid Selection Roll Dice" << endl;
                }
                break;
                
            }
            case 2:
            {
                if(new_player.diceInteract(0))
                { 
                    cout << "Valid Selection save dice 1" << endl;
                }
                else
                {
                    cout << "Invalid Selection save dice 1" << endl;
                }
                break;
                
            }
            case 3:
            {
                if (new_player.diceInteract(1))
                {
                    cout << "Valid Selection save dice 2" << endl;
                }
                else
                {
                    cout << "Invalid Selection save dice 2" << endl;
                }
                break;

            }
            case 4:
            {

                if (new_player.diceInteract(2))
                {
                    cout << "Valid Selection save dice 3" << endl;
                }
                else
                {
                    cout << "Invalid Selection save dice 3" << endl;
                }
                break;
            }
            case 5:
            {

                if (new_player.diceInteract(3))
                {
                    cout << "Valid Selection save dice 4" << endl;
                }
                else
                {
                    cout << "Invalid Selection save dice 4" << endl;
                }
                break;
            }
            case 6:
            {
                if (new_player.diceInteract(4))
                {
                    cout << "Valid Selection save dice 5" << endl;
                }
                else
                {
                    cout << "Invalid Selection save dice 5" << endl;
                }
                break;
            }

            case 7:
            {
                if (new_player.scoreInteract(true, 0))
                {
                    cout << "Valid Selection play score 1s" << endl;
                }
                else
                {
                    cout << "Invalid Selection play score 1s" << endl;
                }
                break;

            }
            case 8:
            {

                if (new_player.scoreInteract(true, 1))
                {
                    cout << "Valid Selection play score 2s" << endl;
                }
                else
                {
                    cout << "Invalid Selection play score 2s" << endl;
                }
                break;
            }
            case 9:
            {

                if (new_player.scoreInteract(true, 2))
                {
                    cout << "Valid Selection play score 3s" << endl;
                }
                else
                {
                    cout << "Invalid Selection play score 3s" << endl;
                }
                break;
            }
            case 10:
            {

                if (new_player.scoreInteract(true, 3))
                {
                    cout << "Valid Selection play score 4s" << endl;
                }
                else
                {
                    cout << "Invalid Selection play score 4s" << endl;
                }
                break;
            }
            case 11:
            {

                if (new_player.scoreInteract(true, 4))
                {
                    cout << "Valid Selection play score 5s" << endl;
                }
                else
                {
                    cout << "Invalid Selection play score 5s" << endl;
                }
                break;
            }
            case 12:
            {

                if (new_player.scoreInteract(true, 5))
                {
                    cout << "Valid Selection play score 6s" << endl;
                }
                else
                {
                    cout << "Invalid Selection play score 6s" << endl;
                }
                break;
            }
            case 13:
            {

                if (new_player.scoreInteract(true, 6))
                {
                    cout << "Valid Selection play score 3 of a kind" << endl;
                }
                else
                {
                    cout << "Invalid Selection play score 3 of a kind" << endl;
                }
                break;

            }
            case 14:
            {
                if (new_player.scoreInteract(true, 7))
                {
                    cout << "Valid Selection play score 4 of a kind" << endl;
                }
                else
                {
                    cout << "Invalid Selection play score 4 of a kind" << endl;
                }
                break;
            }
            case 15:
            {
                if (new_player.scoreInteract(true, 8))
                {
                    cout << "Valid Selection play score full house" << endl;
                }
                else
                {
                    cout << "Invalid Selection play score full house" << endl;
                }
                break;
            }
            case 16:
            {
                if (new_player.scoreInteract(true, 9))
                {
                    cout << "Valid Selection play score small straight" << endl;
                }
                else
                {
                    cout << "Invalid Selection play score small straight" << endl;
                }
                break;
            }
            case 17:
            {
                if (new_player.scoreInteract(true, 10))
                {
                    cout << "Valid Selection play score large straight" << endl;
                }
                else
                {
                    cout << "Invalid Selection play score large straight" << endl;
                }
                break;
            }
            case 18:
            {
                if (new_player.scoreInteract(true, 11))
                {
                    cout << "Valid Selection play score Yhatzee" << endl;
                }
                else
                {
                    cout << "Invalid Selection play score Yhatzee" << endl;
                }
                break;
            }
            case 19:
            {
                if (new_player.scoreInteract(true, 12))
                {
                    cout << "Valid Selection play score Chance" << endl;
                }
                else
                {
                    cout << "Invalid Selection play score Chance" << endl;
                }
                break;
            }
            case 20:
            {
                if (new_player.scoreInteract(false, 0))
                {
                    cout << "Valid Selection cross score 1s" << endl;
                }
                else
                {
                    cout << "Invalid Selection cross score 1s" << endl;
                }
                break;

            }
            case 21:
            {

                if (new_player.scoreInteract(false, 1))
                {
                    cout << "Valid Selection cross score 2s" << endl;
                }
                else
                {
                    cout << "Invalid Selection cross score 2s" << endl;
                }
                break;
            }
            case 22:
            {
                if (new_player.scoreInteract(false, 2))
                {
                    cout << "Valid Selection cross score 3s" << endl;
                }
                else
                {
                    cout << "Invalid Selection cross score 3s" << endl;
                }
                break;
            }
            case 23:
            {

                if (new_player.scoreInteract(false, 3))
                {
                    cout << "Valid Selection cross score 4s" << endl;
                }
                else
                {
                    cout << "Invalid Selection cross score 4s" << endl;
                }
                break;
            }
            case 24:
            {

                if (new_player.scoreInteract(false, 4))
                {
                    cout << "Valid Selection cross score 5s" << endl;
                }
                else
                {
                    cout << "Invalid Selection cross score 5s" << endl;
                }
                break;
            }
            case 25:
            {

                if (new_player.scoreInteract(false, 5))
                {
                    cout << "Valid Selection cross score 6s" << endl;
                }
                else
                {
                    cout << "Invalid Selection cross score 6s" << endl;
                }
                break;
            }
            case 26:
            {

                if (new_player.scoreInteract(false, 6))
                {
                    cout << "Valid Selection cross score 3 of a kind" << endl;
                }
                else
                {
                    cout << "Invalid Selection cross score 3 of a kind" << endl;
                }
                break;

            }
            case 27:
            {
                if (new_player.scoreInteract(false, 7))
                {
                    cout << "Valid Selection cross score 4 of a kind" << endl;
                }
                else
                {
                    cout << "Invalid Selection cross score 4 of a kind" << endl;
                }
                break;
            }
            case 28:
            {
                if (new_player.scoreInteract(false, 8))
                {
                    cout << "Valid Selection cross score full house" << endl;
                }
                else
                {
                    cout << "Invalid Selection cross score full house" << endl;
                }
                break;
            }
            case 29:
            {
                if (new_player.scoreInteract(false, 9))
                {
                    cout << "Valid Selection cross score small straight" << endl;
                }
                else
                {
                    cout << "Invalid Selection cross score small straight" << endl;
                }
                break;
            }
            case 30:
            {
                if (new_player.scoreInteract(false, 10))
                {
                    cout << "Valid Selection cross score large straight" << endl;
                }
                else
                {
                    cout << "Invalid Selection cross score large straight" << endl;
                }
                break;
            }
            case 31:
            {
                if (new_player.scoreInteract(false, 11))
                {
                    cout << "Valid Selection cross score Yhatzee" << endl;
                }
                else
                {
                    cout << "Invalid Selection cross score Yhatzee" << endl;
                }
                break;
            }
            case 32:
            {
                if (new_player.scoreInteract(false, 12))
                {
                    cout << "Valid Selection cross score Chance" << endl;
                }
                else
                {
                    cout << "Invalid Selection cross score Chance" << endl;
                }
                break;
            }
            case 33:
            {
                if (new_player.attemptGameEnd())
                {
                    cout << "Valid Selection end game" << endl;
                }
                else
                {
                    cout << "Invalid Selection end game" << endl;
                }
                break;

            }
            case 34:
            {
                if (new_player.diceInteract(7))
                {
                    cout << "Valid Selection" << endl;
                }
                else
                {
                    cout << "Invalid Selection" << endl;
                }
                break;

            }
            case 35:
            {
                if (new_player.scoreInteract(true, 14))
                {
                    cout << "Valid Selection play invalid" << endl;
                }
                else
                {
                    cout << "Invalid Selection play invalid" << endl;
                }
                break;

            }
            case 36:
            {
                if (new_player.scoreInteract(false, 15))
                {
                    cout << "Valid Selection cross invalid" << endl;
                }
                else
                {
                    cout << "Invalid Selection play invalid" << endl;
                }
                break;

            }
            
        default:
            break;
        }


    }
    screenPrint(new_player);
    return 1;
}
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
