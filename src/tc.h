/*
* text-classification
* github.com/01mu
*/

#ifndef TC_H_
#define TC_H_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <time.h>

using namespace std;

struct attributes
{
    string word;
    double frequency;
};

struct review
{
    string text;
    vector<attributes> wordOccurances;
    int qualityClass;
};

struct wordProb
{
    string word;
    double probPos;
    double probNeg;
    int count;
};

bool compareByLengthPos(const wordProb &, const wordProb &);
bool compareByLengthNeg(const wordProb &, const wordProb &);
vector<string> tokenize(const char *, char);

// assign sms texts and spam/ham quality from file to vector
void getTextFromFile();
// tokenize the review vector and gather individual words from learning set
void getTokenizedTerms();
// assign text and quality to reviews vector
void tokenizeText();
// assign spam/ham probabilities to each word in words vector
void determineWordProbs();
// determine whether new set inputs are ham/spam based on old probabilities
void determineNewInputClass();

void outputTestSetFiles(vector<wordProb> &);
void outputTrainingSetFiles(vector<wordProb> &);

void topHamTestHS(int, vector<wordProb> &);
void topSpamTestHS(int, vector<wordProb> &);
void ProbDiffPos(vector<wordProb> &);
void ProbDiffNeg(vector<wordProb> &);
void TopPos(vector<string> &, vector<string> &, vector<wordProb> &);
void TopNeg(vector<string> &, vector<string> &, vector<wordProb> &);
void InPosNotNeg(vector<string> &, vector<string> &);
void InNegNotPos(vector<string> &, vector<string> &);
void InNegAndPos(vector<string> &, vector<string> &);

// probability of positive reviews in training set (spam)
extern double posProb;

// probability of negative reviews in training set (ham)
extern double negProb;

// total number of reviews in training set
extern double total;

// number of words that occur in positive reviews in training set (spam)
extern double posWordTotal;

// number of words that occur in negative reviews in training set (ham)
extern double negWordTotal;

// probability of positive reviews in test set (spam)
extern double testPos;

// probability of negative reviews in test set (ham)
extern double testNeg;

// total number of reviews in test set
extern double testTotal;

// vector containing every word
extern vector<string> words;

// vector of reviews
extern vector<review> reviews;

// words that occur in positive reviews (spam) and negative reviews (ham)
extern vector<wordProb> wordProbVec;

// review text
extern vector<string> rev_txt;

// quality for input lines (0 = ham 1 = spam)
extern vector<int> quality;

#endif
