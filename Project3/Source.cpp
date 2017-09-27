#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <Windows.h>
#include <fstream>
#include <ShlObj.h>
//Required include statements

using namespace std;
//Avoids me typing "std::" before everything

string stringlower(string sUpper) {
	string sLower = "";
	for (int iStringPos = 0; iStringPos < sUpper.length(); ++iStringPos) {
		sLower += tolower(sUpper[iStringPos]);
	}

	return sLower;
}

void write() {
	string sFileName, sSentence;
	vector<string> vsSentence, vsDictionary;
	vector<int> viPositions;
	//Global variable declarations which are named after their function

	{
		char cFileNameBuffer[MAX_PATH];
		SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, cFileNameBuffer);
		string sTempString(cFileNameBuffer);
		sTempString.append("\\A453.txt");
		sFileName = sTempString;
	}
	//Block of code which uses shell functions to get the documents directory

	cin.clear();
	
	cout << "Please enter some text" << endl;
	getline(cin, sSentence);
	{
		stringstream issSentence(sSentence);
		string sTempString;
		while (issSentence >> sTempString) {
			sTempString = stringlower(sTempString);
			vsSentence.push_back(sTempString);
		}
	}
	//Input block which automatically splits the sentence to words

	for each (auto aSentenceWord in vsSentence) {
		while (ispunct(aSentenceWord[0]) != 0) {
			aSentenceWord.erase(aSentenceWord.begin());
		}

		while (ispunct(aSentenceWord[aSentenceWord.size() - 1])) {
			aSentenceWord.erase(aSentenceWord.end() - 1);
		}

		bool bFoundWord = false;
		for each (auto aDictionaryWord in vsDictionary) {
			if (aSentenceWord == aDictionaryWord) {
				bFoundWord = true;
				break;
			}
		}
		if (!bFoundWord && aSentenceWord != "") {
			vsDictionary.push_back(aSentenceWord);
		}
	}
	//Dictionary creation block which loops through the sentence looking for new words to add



	ofstream ofFile(sFileName, ofstream::out);
	for each (auto aDictionaryWord in vsDictionary) {
		ofFile << aDictionaryWord << " ";
	}
	ofFile << endl;

	string sWordAssembler = "";

	for (int iSentencePos = 0; iSentencePos < sSentence.size(); iSentencePos++) {
		//For every character in the sentence
		if (ispunct(sSentence[iSentencePos]) && ((iSentencePos + 1 == sSentence.size() || ispunct(sSentence[iSentencePos + 1])) || (iSentencePos == 0 || ispunct(sSentence[iSentencePos - 1]) )) || sSentence[iSentencePos] == ' ') {
			//If statement to make sure that the program detects the correct punctuation
			if (sWordAssembler != "") {
				bool bWordFound = false;
				for (int iDictionaryPos = 0; iDictionaryPos < vsDictionary.size(); iDictionaryPos++) {
					if (stringlower(sWordAssembler) == vsDictionary[iDictionaryPos]) {
						ofFile << iDictionaryPos;
						bWordFound = true;
					}
				}

				sWordAssembler = "";
			}
			//Checks if any words need assembling and assembles them if necessary
			ofFile << sSentence[iSentencePos];
		}
		else {
			sWordAssembler += sSentence[iSentencePos];
		}
	}
	//Reads the input character by character, compressing it into punctuation and dictionary references

	if (sWordAssembler != "") {
		bool bWordFound = false;
		for (int iDictionaryPos = 0; iDictionaryPos < vsDictionary.size(); iDictionaryPos++) {
			if (stringlower(sWordAssembler) == vsDictionary[iDictionaryPos]) {
				ofFile << iDictionaryPos;
				bWordFound = true;
			}
		}
	}
	//Last assembler check before index is not touched again

	ofFile << endl;
	for (int iSentencePos = 0; iSentencePos < sSentence.size(); iSentencePos++) {
		if (isupper(sSentence[iSentencePos])) {
			ofFile << iSentencePos << " ";
		}
	}

	ofFile.close();
	//Block of code to create a file stream to write the output to
}

void read() {
	string sFileName, sCurrentLine;
	vector<int> viPositions;
	vector<string> vsDictionary;
	//Global variable declarations which are named after their function

	{
		char cFileNameBuffer[MAX_PATH];
		SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, cFileNameBuffer);
		string sTempString(cFileNameBuffer);
		sTempString.append("\\A453.txt");
		sFileName = sTempString;
	}
	//Block of code which uses shell functions to get the documents directory

	ifstream ifFile(sFileName, ifstream::in);

	{
		string sFirstLine;
		getline(ifFile, sFirstLine);

		istringstream issFirstLine(sFirstLine);
		string sTmp;
		while (issFirstLine >> sTmp) {
			vsDictionary.push_back(sTmp);
		}

		if (vsDictionary.empty()) {
			vsDictionary.push_back("");
		}
	}
	//Grabs the first line (the dictionary)

	getline(ifFile, sCurrentLine);

	string sNumAssembler = "";
	string sLineAssembler = "";
	//Buffer variables for the data reader

	for (int iSentencePos = 0; iSentencePos < sCurrentLine.size(); iSentencePos++) {
		if (ispunct(sCurrentLine[iSentencePos]) || sCurrentLine[iSentencePos] == ' ') {
			if (sNumAssembler != "") {
				sLineAssembler += vsDictionary[stoi(sNumAssembler)];
				sNumAssembler = "";
			}
			sLineAssembler += sCurrentLine[iSentencePos];
		}
		else {
			sNumAssembler += sCurrentLine[iSentencePos];
		}
	}
	if (sNumAssembler != "") {
		sLineAssembler += vsDictionary[stoi(sNumAssembler)];
	}
	//Assembles the actual data


	{
		getline(ifFile, sCurrentLine);

		istringstream issCapsPositions(sCurrentLine);
		string sTmp;
		while (issCapsPositions >> sTmp) {
			viPositions.push_back(stoi(sTmp));
		}
	}
	//Capitaliser

	for (int iCapsPos = 0; iCapsPos < viPositions.size(); iCapsPos++) {
		sLineAssembler[viPositions[iCapsPos]] = toupper(sLineAssembler[viPositions[iCapsPos]]);
	}

	cout << sLineAssembler;
	//Prints the actual sentence

	ifFile.close();
}

int main() {
	string sOperation;
	cout << "Read from file (read) or write to file (write)?" << endl;
	while (sOperation != "read" && sOperation != "write") {
		getline(cin, sOperation);
		sOperation = stringlower(sOperation);
	}

	cin.clear();

	if (sOperation == "write") {
		write();
	}
	else {
		read();
	}

	cout << endl;
	system("pause");
	//Windows pause command to allow user to exit properly

	return 0;
}