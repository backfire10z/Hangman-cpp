//Expected output: Either you won or you lost, basically word for word. Hanging man is drawn each time with letters guessed, both correctly and in total
//Assumptions: One word out of a three word phrase is not guessable at once; only the entire phrase (there is validator). Also, that's just bad hangman strategy
//Input is explained by the program. Single player mode has 189 pre-input words to guess, randomly picked with a random seed. Multiplayer allows the user to enter the word or phrase to be guessed by someone else.


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <stdlib.h>
#include <iomanip>
#include <fstream>

using namespace std;

//Headers for my functions
void beginHangman();
void singlePlayer();
void getFileContent(string fileName, vector<string> &fileWords);
void multiPlayer();
void stringToArray(string inputPhrase, vector<char> &userWord);
void keyCreator(vector<char> &userWord, vector<char> &answerKey, vector<char> &beforeGuessAnswers);
void getAndErase(string &inputPhrase);
void hangingMan(int tries);
void resetScreen(int tries, vector<char> userWord, vector<char> answerKey);
void playGame(vector<char> userWord, vector<char> answerKey, vector<char> beforeGuessAnswers);

int main()
{
    
    //Clear screen for visuals
    cout << string(100, '\n');
    
    //Begin game
    beginHangman();
    
    return 0;
}

//Menu for single or multi player options
void beginHangman() {
    string userOption;
    string goBack;
    
 restart:
    cout << "Welcome to hangman, created by Josh Kleyman." << endl << endl;
    cout << setw(20) << "A - Single Player" << endl;
    cout << setw(18) << "B - Multiplayer" << endl;
    cout << setw(18) << "Enter an option: ";
    
    getAndErase(userOption);

    while (userOption.length() != 1 || (userOption != "a" && userOption != "b")) {    //1 character length and either
        cout << "Enter a valid option: ";                                           //a or b input validation
        getAndErase(userOption);
    }

//Selects gamemode
    if (userOption == "a") {
        cout << "You have selected single player. Press \"q\" to go back to the menu if this was incorrect, or press enter to continue: ";
        getline(cin, goBack);
        if (goBack == "q") {
            cout << string(100, '\n');
            goto restart;
        }
        singlePlayer();
    }
    
    if (userOption == "b") {
        cout << "You have selected multiplayer. Press q to go back to the menu if this was incorrect, or press enter to continue: ";
        getline(cin, goBack);
        if (goBack == "q") {
            cout << string(100, '\n');
            goto restart;
        }
        multiPlayer();
    }
}

//Single player gamemode setup
void singlePlayer() {
    string inputPhrase;         //String of user input (in this case, premade for single player)
    vector<char> userWord;      //Word user input as a char vector
    vector<char> answerKey;    //Letters user guesses
    vector<char> beforeGuessAnswers;   //Check if last guess is correct by comparing 
                                       //old answers to updated user answers
    vector<string> fileWords;       //Hold all singleplayer hangman words
    int randNum;                    //Picks a word from vector fileWords
    
    //Put contents of file into string vector
    getFileContent("words.txt", fileWords);

    //Random seed generator
    srand((unsigned) time (0));
    
    //Obtain random number from 0 to 188
    randNum = rand() % 189;
    
    //Find random word out of all words in file
    inputPhrase = fileWords.at(randNum);

        stringToArray(inputPhrase, userWord);       //transforms input into char array
        keyCreator(userWord, answerKey, beforeGuessAnswers);        //creates answer key for guessing

    
    playGame(userWord, answerKey, beforeGuessAnswers);  //Function call for actual gameplay
}

//Puts contents of the file into a vector
void getFileContent(string fileName, vector<string> &fileWords) {
    ifstream wordsFile;
    string word;
    
    wordsFile.open(fileName);       //Open file
    
    if (!wordsFile) {       //Check if file opened
        cout << "Unable to open file words.txt";
        return;   
    }
    
    while (getline(wordsFile, word)) {      //Place contents into vector
        fileWords.push_back(word);
    }
    
    wordsFile.close();      //Close file
} 

//Multiplayer gamemode setup
void multiPlayer() {
    vector<char> userWord;
    vector<char> answerKey;
    vector<char> beforeGuessAnswers;
    string inputPhrase;
    unsigned int i;
    
    cout << "Enter a word or phrase to be guessed: ";

	getAndErase(inputPhrase);       //Get and validate input

    stringToArray(inputPhrase, userWord);       //transforms input into char vector
    
    keyCreator(userWord, answerKey, beforeGuessAnswers);        //creates answer key for guessing

    playGame(userWord, answerKey, beforeGuessAnswers);  //Function call to begin gameplay
}

//Inputs a string into a character array 
void stringToArray(string inputPhrase, vector<char> &userWord) {
    for (int i = 0; i < inputPhrase.length(); i++) {
        userWord.push_back(inputPhrase.at(i));
    }
}

//Creates answerKey and beforeGuessAnswers
void keyCreator(vector<char> &userWord, vector<char> &answerKey, vector<char> &beforeGuessAnswers) {
    int wordLength = userWord.size();

    //Remove duplicate spaces in between words
    for (unsigned int i = 0; i < wordLength; i++) {
        if (userWord.at(i) == ' ') {
            if (userWord.at(i-1) == ' ') {
                userWord.erase(userWord.begin()+i);
                wordLength--;           //Prevents out-of-range error
                i--;                    //Maybe more than 2 spaces in a row, rechecks to be sure
            }
        }
    }

    for (unsigned int i = 0; i < userWord.size(); i++) {     //Create key for user's guesses to compare with actual word
        if (userWord.at(i) != ' ') {
            answerKey.push_back('_');   //All underscores (to be replaced with properly guessed letters)
        }
        else {
            answerKey.push_back(' ');   //Space between words if user inputted a phrase
        }
    }
    
    beforeGuessAnswers = answerKey;     //Check if last guess is correct by comparing
                                        //old answers to updated user answers
}

//Gets user input and deletes extra spaces. Also lowercases entire string
void getAndErase(string &inputPhrase) {
    getline(cin, inputPhrase);
    
    while (inputPhrase.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") != string::npos || inputPhrase.length() == 0 || inputPhrase.find_first_not_of(" \t\n\r\f\v") == string::npos) {   //Nothing but letters
        cout << "Only letters please. Kindly re-enter: ";
        getline(cin, inputPhrase);
    }

    inputPhrase.erase(0, inputPhrase.find_first_not_of(" \t\n\r\f\v"));    //Delete leading spaces 
    inputPhrase.erase(inputPhrase.find_last_not_of(" \t\n\r\f\v") + 1);     //Delete trailing spaces
    
    for_each(inputPhrase.begin(), inputPhrase.end(), [](char & c) {     //Lowercase entire user string
		c = ::tolower(c);
	});
}

//Draws hangman based on how many tries user has left
void hangingMan(int tries) {
    
    if (tries == 6) {
        cout << setw(7) << "____" << endl;
        cout << setw(8) << "|   |" << endl;
        cout << setw(7) << "|   " << endl;
        cout << setw(6) << "|  " << endl;
        cout << setw(6) << "|  " << endl;
        cout << setw(4) << "|" << endl;
        cout << setw(6) << "-----" << endl;
    }
    
    if (tries == 5) {
        cout << setw(7) << "____" << endl;
        cout << setw(8) << "|   |" << endl;
        cout << setw(8) << "|   O" << endl;
        cout << setw(6) << "|  " << endl;
        cout << setw(6) << "|  " << endl;
        cout << setw(4) << "|" << endl;
        cout << setw(6) << "-----" << endl;
    }
    
    if (tries == 4) {
        cout << setw(7) << "____" << endl;
        cout << setw(8) << "|   |" << endl;
        cout << setw(8) << "|   O" << endl;
        cout << setw(8) << "|   |" << endl;
        cout << setw(6) << "|  " << endl;
        cout << setw(4) << "|" << endl;
        cout << setw(6) << "-----" << endl;
    }
    
    if (tries == 3) {
        cout << setw(7) << "____" << endl;
        cout << setw(8) << "|   |" << endl;
        cout << setw(8) << "|   O" << endl;
        cout << setw(8) << "|  /|" << endl;
        cout << setw(6) << "|  " << endl;
        cout << setw(4) << "|" << endl;
        cout << setw(6) << "-----" << endl;
    }
    
    if (tries == 2) {
        cout << setw(7) << "____" << endl;
        cout << setw(8) << "|   |" << endl;
        cout << setw(8) << "|   O" << endl;
        cout << setw(9) << "|  /|\\" << endl;
        cout << setw(6) << "|  " << endl;
        cout << setw(4) << "|" << endl;
        cout << setw(6) << "-----" << endl;
    }
    
    if (tries == 1) {
        cout << setw(7) << "____" << endl;
        cout << setw(8) << "|   |" << endl;
        cout << setw(8) << "|   O" << endl;
        cout << setw(9) << "|  /|\\" << endl;
        cout << setw(8) << "|  / " << endl;
        cout << setw(4) << "|" << endl;
        cout << setw(6) << "-----" << endl;
    }
    
    if (tries == 0) {
        cout << setw(7) << "____" << endl;
        cout << setw(8) << "|   |" << endl;
        cout << setw(8) << "|   O" << endl;
        cout << setw(9) << "|  /|\\" << endl;
        cout << setw(9) << "|  / \\" << endl;
        cout << setw(4) << "|" << endl;
        cout << setw(6) << "-----" << endl;
    }
}

//Clear screen, draw hanging man, and output guesses thus far
void resetScreen(int tries, vector<char> userWord, vector<char> answerKey) {
    unsigned int i;
    
    cout << string(100, '\n');      //Clear screen for gameplay smoothness/visuals

    hangingMan(tries);              //Function call redraw hanging man

    for (i = 0; i < userWord.size(); i++) {     //Output what the user has correctly guessed so far
        cout << " " << answerKey.at(i);
    }
    
    cout << endl << endl;
}

//Actual gameplay
void playGame(vector<char> userWord, vector<char> answerKey, vector<char> beforeGuessAnswers) {
    vector<char> letterGuessed;         //Store all unique character guesses
    string userGuess;                   //User's guess: transferred to characterGuess if character or holds phrase guess
    string wordAnswer(userWord.begin(), userWord.end());    //Used to compare (string) guess to (vector) userWord
                                                            //by creating userWord as a string
    vector<string> userPhraseGuesses;       //Store all unique phrase guesses
    char characterGuess;            //Used to hold a character guess from userGuess, as opposed to a phrase
    int tries = 6;                      //Amount of incorrect guesses allowed
    int letterCount = 0;                //Number of unique guesses
    int phraseCount = 0;                //Number of unique phrase guesses
    unsigned int i;

    //Initial reset to hangman screen
    resetScreen(tries, userWord, answerKey);
    
//Loops until full word/phrase is guessed    
    while (answerKey != userWord) {
    newGuess:                               //Goto destination
        cout << "You have " << tries << " wrong tries left." << "   ";
        cout << "Here are characters you have guessed: ";
        for (i = 0; i < letterGuessed.size(); i++) {        //Output previously guessed letters
            cout << letterGuessed.at(i) << " ";
        }
        cout << "\nEnter a guess (character or entire answer): ";
        
        getAndErase(userGuess);     //Collect and validate userGuess input

        if (userGuess.length() == 1) {        //If guess is a character
            characterGuess = userGuess[0];
            letterGuessed.push_back(characterGuess);     //Store unique user guess
            letterCount++;
        
            for (i = 0; i < letterGuessed.size() - 1; i++) {        //Never compare newest guess to itself
                if (letterGuessed.at(letterCount - 1) == letterGuessed.at(i)) {  //Compare newest guess to
                                                                                 //all previous guesses
                    resetScreen(tries, userWord, answerKey);
                    cout << "You already guessed " << letterGuessed.at(i) << ".";
                    cout << endl << endl;
                    letterGuessed.pop_back();                           //Remove repeated guess
                    letterCount--;                                      //Number of guesses is now reduced by one^^
                    goto newGuess;                                      //Reset to beginning
                }
            }
            
            for (i = 0; i < userWord.size(); i++) {     //If guess matches letter of original word,
                if (characterGuess == userWord.at(i)) {      //place guessed letter into the correct answers array
                    answerKey.at(i) = characterGuess;
                }
            }
            if (beforeGuessAnswers == answerKey) {      //If the guess was wrong, answers array will
                tries--;                                //be the same as last time, so tries decrements
            }
        }
        
//If guess is the entire word/phrase
        else {
            if (userGuess.length() != userWord.size()) {
                resetScreen(tries, userWord, answerKey);
                cout << "Improper phrase size: phrase guesses may only be used for the entire answer.";
                cout << endl << endl;
                goto newGuess;
            }
            userPhraseGuesses.push_back(userGuess);
            phraseCount++;
            for (i = 0; i < userPhraseGuesses.size() - 1; i++) {        //Never compare newest guess to itself
                if (userPhraseGuesses.at(phraseCount - 1) == userPhraseGuesses.at(i)) {  //Compare newest guess to
                                                                                            //all previous guesses
                    resetScreen(tries, userWord, answerKey);
                    userPhraseGuesses.pop_back();                       //Remove repeated guess
                    phraseCount--;                                      //Guess now reduced by one^^
                    cout << "You already guessed " << userGuess << "." << endl << endl;
                    goto newGuess;
                }
            }
            if (userGuess == wordAnswer) {      //compare the guessed word to the user's word
                resetScreen(tries, userWord, userWord);
                cout << "You won! Congratulations!" << endl;
                return;
            }
            else {          //Phrase is wrong: tries - 1
                tries--;
            }
        }
        
        resetScreen(tries, userWord, answerKey);

        if (tries == 0) {       //Lose text if tries hit 0
            cout << "You lost. The word was: ";
            for (i = 0; i < userWord.size(); i++) {
                cout << userWord.at(i);
            }
            cout << ". Let's hang again!";
            return;
        }
        
        beforeGuessAnswers = answerKey;     //Update old answer array to match one with new guess
    }
    
    cout << "You won! Think you can do it again?";    //Winners text if word is guessed
    return;
}
