#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(){
	ifstream file("wordlist.txt");
	ofstream newFile("wordlist5.txt");
	if (file.is_open() && newFile.is_open()) {
		string line;
		while (getline(file,line))
			if (line.length() == 5) {
				cout << line << endl;
				newFile.write((char *) &line,sizeof(line));
			}
	}
}
