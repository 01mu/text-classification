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

    get_text_from_file();
    get_tokenized_words();
    tokenize_text();
    determine_word_probs();
    determine_input_class();

    cout << endl << endl << "Training set (hamspam): "
        << endl << "Spam %: " << (pos_prob *= 100)
        << endl << "Ham %: " << (neg_prob *= 100)
        << endl << "Total: " << total;

    cout << endl << endl
        << "Number of unique words in training set: " << words.size() << endl;

    return 0;
}
