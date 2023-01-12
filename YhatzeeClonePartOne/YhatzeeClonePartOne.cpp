/* Ariadne D-H Yhatzee Clone Part One
   Create fundamental Runcycle for the project that has basic functions of single player Yhatzee game
   In a command line terminal
*/


#include <iostream>
/* Dice object is contained within the turn object
*  keeps track of the dice values, calls roll.
*  Array int [5] diceValues = dice values rolls once on initiation
*  Array boolean [5] diceKeep = true false if a die will be keept next roll default all false
*  int rollCount = number of rolls used this turn
* 
*  getters for all var
*  close method terminates at the end of turn
*/
class dice
{

};

/* Stores the data for current turn, contains the dice object
*  refrences the score object to play or zero a score box
*  turn ends and is terminated when a play is made or a zero is placed
*  
*  object Dice = the dice object for the current turn
*  ---object Score = the passed reffrence to the score object for the game state
*  
*  getters for the data points
*  terminate method
*/
class turn
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

};

/* Contains the turn object and score object, refrences the print screen function. 
*  created by the run game fucntion. interfaces between score and turn initiates and terminates 
*  turns.
* 
*   object current_turn = the turn object, contained here for data control
*   object game_score = the score object, contained here for data control 
*   
*   getters and setters, used to refrence the game display
*   check game end = if end game returns a true goes throug the game end process
*/
class gameState
{

};

/* Dice rolling method, generates an array of psudo-random numbers
*  in all false spots on the passed bool array. used by dice object instances 
* 
*  return Array int[5} dice values = the new generated values 
*  
*  Array bool[5] kept_dice = true values in this array refrence which dice arent rolled
* 
*/
private int[5] roll_dice(bool[5] diceKept)
{

}

/* Runs the game, ends a session, ends the application, can generate a new
*  game instance. passes diffrent ints to main for close based on diffrent end cases
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
private int run_yhatzee()
{

}
/* cleans and writes to screen to function as a game interface
*  todo specs for this fun, later priority
*/
private -- screenPrint(-diffrent peices of data to pass for screen printing-)
/* More dummed down print metod for in-development testing
*  todo, specs of method
*/
private int testPrint(-associated data, and diffrent print cases for use in testing - )
{

}

/* Main method, initiates game run
*  prints difrent return values based on
*  how game ended
*/
int main()
{
    std::cout << "Hello World!\n";
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
