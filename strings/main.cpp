// String searching lab exercise.
// Edwyn Mckie

#include <chrono>
#include <cassert>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <fstream> // needed for file
#include "utils.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;
using std::ofstream;

//define the alias for the clock type we're going to use
typedef std::chrono::steady_clock the_clock;

//my file to save values to
ofstream my_file("Timings.csv");
#define d 256 //number of characters in the english alphabet

/** Return first position of pat in text, or -1 if not found. */
Position find_bruteforce(const string& pat, const string& text) {
	Position pat_len = pat.size();
	Position text_len = text.size();

	for (Position i = 0; i < text_len - pat_len; ++i) {
		Position j;
		//show position we're currently at
		//feed it the text we want,and the current position
		show_context(text, i); 
		for (j = 0; j < pat_len; j++) {
			if (text[i + j] != pat[j]) {
				break; // Doesn't match here.
			}
		}
		if (j == pat_len) {
			return i; // Matched here.
		}
	}
	return -1; // Not found.
}

/** skip ahead by the length of the string if we do not match. */
Position find_skipping(const string& pat, const string& text) {
	Position pat_len = pat.size();
	Position text_len = text.size();

	bool in_pattern[256];
	for (int i = 0; i < 256; ++i)
	{
		in_pattern[i] = false;
	}
	
	for (char c : pat)
	{
		in_pattern[int(c)] = true;
	}

	for (Position i = 0; i < text_len - pat_len; ++i) {
		if (!in_pattern[int(text[i + pat_len - 1])])
		{
			i += pat_len - 1; //skip forwards
			continue;
		}
		
		Position j;
		//show position we're currently at
		//feed it the text we want,and the current position
		show_context(text, i);
		for (j = 0; j < pat_len; j++) {
			if (text[i + j] != pat[j]) {
				break; // Doesn't match here.
			}
		}
		if (j == pat_len) {
			return i; // Matched here.
		}
	}
	return -1; // Not found.
}

/** skip ahead by the length of the string if we do not match. */
Position find_bm(const string& pat, const string& text) {
	Position pat_len = pat.size();
	Position text_len = text.size();

	int skip[256];
	for (int i = 0; i < 256; ++i)
	{
		skip[i] = pat_len; // Not in the pattern.
	}
	for (int i = 0; i < pat_len; ++i)
	{
		skip[int(pat[i])] = (pat_len - 1) - i;
	}


	for (Position i = 0; i < text_len - pat_len; ++i) {
		int s = skip[int(text[i + pat_len - 1])]; 
		if (s != 0) 
		{
			i += s - 1; // Skip forwards.
			continue;
		}
		Position j;
		//show position we're currently at
		//feed it the text we want,and the current position
		show_context(text, i);
		for (j = 0; j < pat_len; j++) {
			if (text[i + j] != pat[j]) {
				break; // Doesn't match here.
			}
		}
		if (j == pat_len) {
			return i; // Matched here.
		}
	}
	return -1; // Not found.
}

/** skip ahead by the length of the string if we do not match. */
void find_bm_multiple(const string& pat, const string& text) 
{
	Position pat_len = pat.size();
	Position text_len = text.size();
	vector<Position> Results;
	int NumberofMatches = 0;
	int skip[256];
	for (int i = 0; i < 256; ++i)
	{
		skip[i] = pat_len; // Not in the pattern.
	}
	for (int i = 0; i < pat_len; ++i)
	{
		skip[int(pat[i])] = (pat_len - 1) - i;
	}


	for (Position i = 0; i < text_len - pat_len; ++i)
	{
		if ((text.size() - i) < pat_len) 
		{
			int s = skip[int(text[i + pat_len - 1])];
			if (s != 0)
			{
				i += s - 1; // Skip forwards.
				continue;
			}
		}
		Position j;
		//show position we're currently at
		//feed it the text we want,and the current position
		for (j = 0; j < pat_len; j++) 
		{
			if (text[i + j] != pat[j]) {
				break; // Doesn't match here.
			}
		}
		if (j == pat_len) {
			 // Matched here add to the vector
			Results.push_back(i);
			//print results to the screen
			//cout << "Match found: " << Results[Results.size() - 1] << endl;
			NumberofMatches++;
		}
	}
	cout << pat << " was found: " << NumberofMatches << " time(s)" << endl;
}

void Rabin_Karp(const string& pat, const string& text) 
{
	int PatternLength = pat.size();
	int TextLength = text.size();
	int PrimeNumber = 2;
	int j = 0; //counter
	int PatternHash = 0;
	int TextHash = 0;
	int Hash = 1;
	int NumberofMatches = 0;

	for (int i = 0; i < PatternLength - 1; i++)	
	{
		Hash = (Hash * d) % PrimeNumber;
	}

	for (int i = 0; i < PatternLength; i++)
	{
		PatternHash = (d * PatternHash + pat[i]) % PrimeNumber;
		TextHash = (d * TextHash + pat[i]) % PrimeNumber;
	}

	for (int i = 0; i <= TextLength - PatternLength; i++)
	{
		if (PatternHash == TextHash)
		{
			for (j = 0; j < PatternLength; j++)
			{
				if (text[i + j] != pat[j])
				{
					break;
				}
			}
			if (j == PatternLength)
			{
				//cout << "Pattern found at index: " << i << endl;
				NumberofMatches++;
			}
		}
		if (i < TextLength - PatternLength)
		{
			TextHash = (d * (TextHash - text[i] * Hash) + text[i + PatternLength]) % PrimeNumber;
			if (TextHash < 0)
			{
				TextHash = TextHash + PrimeNumber;
			}
		}
	}
	cout << pat << " was found: " << NumberofMatches << " time(s)" << endl;
}

int main(int argc, char *argv[]) {
	string text;//declare text as a string
	float time_taken[2];
	string FileName = "";

	string pat[10]; //pat = pattern
	pat[0] ="244";
	pat[1] = "Tohka";
	pat[2] = "Origami";
	pat[3] = "Yoshino";
	pat[4] = "Shido";
	pat[5] = "Spirit";
	pat[6] = "DEM";
	pat[7] = "Angel";
	pat[8] = "Battle";
	pat[9] = "blade";
	
	//set up headers
	my_file << "Character limit " << "," << "Boyer Moore Time taken"  << "," << "Rabin Karp Time taken" << endl;
	for (int i = 0; i < 17; i++)
	{	//load_jute_book(text); //call the load function and pass it the file .txt
		FileName ="DateALiveVolume" + std::to_string(i + 1) + ".txt";
		load_file(FileName, text);
		cout << "String size: " << text.size() << endl;
		for (int j = 0; j <10; j++)
		{
			//Position pos = find_bruteforce(pat, text);
			//Position pos = find_skipping(pat, text);
			//Position pos = find_bm(pat, text);
			//cout << "Searching for:" << pat << endl;
			cout << "Boyer Moore" << endl;

			//time how long it takes to Search via Boyer Moore
			the_clock::time_point start = the_clock::now();
			find_bm_multiple(pat[j], text);
			the_clock::time_point end = the_clock::now();
			time_taken[0] = duration_cast<milliseconds>(end - start).count();

			//print the time taken
			cout << "time taken to Search " << time_taken[0] << "ms"<< endl;
		//	system("pause");

			cout << "Rabin Karp" << endl;
			// time how long it takes to Search via Rabin karp
			start = the_clock::now();
			Rabin_Karp(pat[j], text);
			end = the_clock::now();
			time_taken[1] = duration_cast<milliseconds>(end - start).count();
			//print the time taken
			cout << "time taken to Search " << time_taken[1] << "ms" << endl<<endl;
			my_file << text.size() << "," << time_taken[0] << "," << time_taken[1] << endl;

			cout << endl << endl;
			//show_context(text, pos);			
		}
		system("pause");

	}	
	return 0;
}