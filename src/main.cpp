/*
* text-classification
* github.com/01mu
*/

#include <iostream>
#include "lib/tc.cpp"

using namespace std;

int main()
{
    cout << "Naive Bayes text classification with SMS spam/ham" << endl;

    getTextFromFile();
    getTokenizedTerms();
    tokenizeText();
    determineWordProbs();
    determineNewInputClass();

    posProb *= 100;
    negProb *= 100;

    cout << endl << endl << "Training set (hamspam): "
        << endl << "Spam %: " << posProb
        << endl << "Ham %: " << negProb
        << endl << "Total: " << total;

    cout << endl << endl
        << "Number of unique words in training set: " << words.size() << endl;

    return 0;
}
