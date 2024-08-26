//By Lanausse
//Last Modified Aug 23, 2024 @ 10:32 AM
#include <unordered_map>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

//Vars
int guesses = 0;
int maxGuesses = 6;

string username;
int password;

int gamesWon;
int gamesLost;
int gamesPlayed;
int currentWinStreak;
int maxWinStreak;
int winsInNGuesses[6];

bool debugEnabled = false;

//ANSI Stuff
static const string ansiStart = "\033[";
static const string ansiBell = "\\07";
	//Formatting
static const string ansiReset	= "0m";
static const string ansiBold = "1;";
static const string ansiFaint = "2;";
static const string ansiItalic = "3;";
static const string ansiUnderline = "4;";
static const string ansiBlink = "5;";
static const string ansiFastBlink = "6;";
static const string ansiSwapColors  = "7;";

	//TextColors
static const string ansiRed		= "31m";
static const string ansiGreen	= "32m";
static const string ansiYellow	= "33m";
static const string ansiBlue	= "34m";
static const string ansiWhite	= "37m";
static const string ansiLightRed	= "91m";
static const string ansiLightGreen	= "92m";
static const string ansiLightYellow	= "93m";
static const string ansiLightBlue	= "94m";
static const string ansiReallyWhite	= "97m";

//Funcs

void print(string str, bool isDBG = false) {
	if (!isDBG) {
		cout << str;
	} else if (debugEnabled) {
		cout << ansiStart + ansiBold + ansiLightBlue + "[DEBUG]:" + ansiStart + ansiReset << " " << str;
	}
}

int getFileLineLength(string name) {
	int lineCount = 0;
	ifstream file(name);

    if (file.is_open()) {
		string tmpLine;
		while (getline(file, tmpLine)) lineCount++;
	} else {
		print(ansiStart + ansiRed + "Error! Invalid file name.\n" + ansiStart + ansiReset, true);
		return -1;	
	}

	file.close();
	return lineCount;
}

bool guess(string word, string guess) {
	string output[word.length()];
	if (debugEnabled) for (string& tmp: output) tmp = "+";
	
	if (guess.length() == word.length()) {
		guesses++;

		if (guess == word) {
			print(ansiStart + ansiSwapColors + ansiLightGreen + word + ansiStart + ansiReset + "\n");
			return true;
		}

		for ( int idx = 0; idx < guess.length(); idx++) {

			if (word[idx] == guess[idx]) { //Letter is in the correct spot

				output[idx] = ansiStart + ansiSwapColors + ansiLightGreen + guess[idx] + ansiStart + ansiReset;
				word[idx] = '+';
				guess[idx] = '='; 

			}
		}

		for ( int idx = 0; idx < guess.length(); idx++) {

			if (word.find(guess[idx]) != string::npos) { //Letter is in the correct spot

				output[idx] = ansiStart + ansiSwapColors + ansiLightYellow + guess[idx] + ansiStart + ansiReset;
				word[word.find(guess[idx])] = '~';
				guess[idx] = '*'; 

			} else if (guess[idx] != '=') output[idx] = guess[idx];
		}
		print("Word:  " + word + "\n", true);
		print("Guess: " + guess + "\n", true);
		
	}
	
	for (string c: output) print(c);
	print("\n");
	return false;
}

bool writeToSave(const string& name) {
	ofstream file(name+".sav");
	if (file.is_open()) {
		file.write((char *) &password,sizeof(password));
		file.write((char *) &gamesWon,sizeof(gamesWon));
		file.write((char *) &gamesLost,sizeof(gamesLost));
		file.write((char *) &gamesPlayed,sizeof(gamesPlayed));
		file.write((char *) &currentWinStreak,sizeof(currentWinStreak));
		file.write((char *) &maxWinStreak,sizeof(maxWinStreak));
		file.write((char *) &winsInNGuesses,sizeof(winsInNGuesses));
		return true;
	} else return false;
}

void hashPassword() {
	//Password
	string strPass;
	print("Password: ");
	cin >> strPass;
	cin.ignore();
	hash<string> hasher;
	password = hasher(strPass);
	strPass = "";
}

bool yesNoPrompt(){
	string choice;
	do {
		getline(cin, choice);
	} while (choice[0] != 'y' && choice[0] != 'Y' && choice[0] != 'n' && choice[0] != 'N');

	if (choice[0] == 'y' || choice[0] == 'Y') return true;
	return false;
}

bool createAccount(const string& name) {
	print("\n");
	print("Hmm... I can't seems to find your name on file.\n");
	print("Would you like to make a new account? (y/n) ");

	if (yesNoPrompt()) {
		print("Set your password!\n");
		hashPassword();
		return writeToSave(name);

	} else {
		return false;
	}

}

bool readFromSave(const string& name) {
	ifstream file(name+".sav");
	if (file.is_open()) {

		hashPassword();
		//Check Password
		int savedPassword;
		file.read((char *) &savedPassword, sizeof(password));
		
		print("Hash: " + to_string(password) + ", Saved Hash:" + to_string(savedPassword) + "\n", true);
		
		if (savedPassword == password) {

			file.read((char *) &gamesWon, sizeof(gamesWon));
			file.read((char *) &gamesLost, sizeof(gamesLost));
			file.read((char *) &gamesPlayed, sizeof(gamesPlayed));
			file.read((char *) &currentWinStreak,sizeof(currentWinStreak));
			file.read((char *) &maxWinStreak,sizeof(maxWinStreak));
			file.read((char *) &winsInNGuesses,sizeof(winsInNGuesses));
			return true;
		} else return false;
	} else { //New Account Creation
		return createAccount(name);
	}
}

bool login(){
	print("Please Login.\n");
	print("Username: ");
	cin >> username;
	cin.ignore();
	return readFromSave(username);
}

void showUserStats() {
	print(ansiStart+ansiBold+ansiWhite+"--- Your Statistics ---"+ansiStart+ansiReset+"\n");
	print(ansiStart+ansiBold+ansiReallyWhite+"Games Played: "+ansiStart+ansiReset+to_string(gamesPlayed) + "\n");
	print(ansiStart+ansiGreen+"Wins: "+ansiStart+ansiReset+to_string(gamesWon) + "\n");
	print(ansiStart+ansiLightRed+"Losess: "+ansiStart+ansiReset+to_string(gamesLost) + "\n");
	print(ansiStart+ansiLightYellow+"Current Win Streak: "+ansiStart+ansiReset+to_string(currentWinStreak) + "\n");
	print(ansiStart+ansiBold+ansiLightYellow+"Best Win Streak: "+ansiStart+ansiReset+to_string(maxWinStreak) + "\n");

	int idx = 1;
	for (const int& numbOfWins: winsInNGuesses) {
		print(to_string(idx)+ " Guess");
		if (idx > 1) { print("es "); } else { print("   "); };

		print("|");

		int percent = (numbOfWins / (double) gamesWon)*10;
		for (int i = 0; i < 10; ++i) {
			if (idx % 2 == 0 && i < percent) {
				print("▒");
			} else if (i < percent) {
				print("█");
			} else print(" ");
		}
		print("| " + to_string(numbOfWins) + "\n");
		idx++;
	}
	
	print("\n");
}

int main() {
	print("Debug is enabled!\n", true);

	if (login()) {

		print("\nWelcome");
		if (gamesPlayed > 0) print(" back");
		print(" " + username + "!\n");

		//Stats
		showUserStats();

		//Load Word List
		ifstream file("wordlist5.txt");
		int listLength = getFileLineLength("wordlist5.txt");
		string wordList[listLength];
		if (file.is_open()) {
			string line;

			for (int idx = 0; idx < listLength; idx++) {
				getline(file, line);
				wordList[idx] = line;
			}
		}
		file.close();

		//Main Game Loop
		do{
			gamesPlayed++;

			srand(time(NULL));
			string cWord = wordList[rand() % listLength];
			//if (debugEnabled) cWord = "wheat";
			print("The word is \"" + cWord + "\"!\n", true);

			guesses = 0;
			string userGuess;
			bool guessResult;
			do {
				print("_____\n");
				print("\033[A");
				getline(cin, userGuess);
				print("\033[A");
				guessResult = guess(cWord, userGuess);

			} while (!guessResult && guesses != maxGuesses);

			if (guessResult) {
				gamesWon++;
				currentWinStreak++;
				winsInNGuesses[guesses-1]++;
				print("You win! ");

				if (maxWinStreak < currentWinStreak) maxWinStreak = currentWinStreak;
			} else {
				gamesLost++;
				print("You lost... ");

				if (maxWinStreak < currentWinStreak) maxWinStreak = currentWinStreak;
				currentWinStreak = 0;
			}

			print("The word was " + cWord + ".\n");
			print("Guesses: " + to_string(guesses) + "\n");
			writeToSave(username);

			print("\nPlay again? (y/n) ");
		} while (yesNoPrompt());
	}
	print("\nExiting...");
	return 0;
}
//🭀🭁🭂🭃🭄🭅🭆🭇🭈🭉🭊🭋🭌🭍🭎🭏
//🯁🯂🯃 GO
//🮘🮘
//█
//▒▒
//🯰🯱🯲🯳🯴🯵🯶🯷🯸🯹


//    🭋🭀
//🭈🭆🭁🭌🭑🬽
//🭣🭧🭒🭝🭜🭘
//   🭦🭛
// echo -e '  🭋🭀\n🭈🭆🭁🭌🭑🬽\n🭣🭧🭒🭝🭜🭘\n  🭦🭛 Messed up looking star'