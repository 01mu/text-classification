# text-classification
Determines whether text message input from a test set (`spam`) is spam or ham based on the input of a training set (`hamspam`). Whether a text is spam or ham is known for `hamspam` (0 = ham, 1 = spam). Based on the 13.4% of text messages for `hamspam` that are spam, the algorithm determines that 85.5% of texts in `spam` are spam (14.4 percentage points off).
## Output:
```
Naive Bayes text classification with SMS spam/ham

Getting input from 'hamspam'
Getting individual words from 'hamspam'
Tokenizing and assigning text and classification inputs to training set vector
Determining probabilities for each word in training set
Determining whether each input in 'spam' is spam or ham

Test set (spam):
Spam %: 85.5876
Ham %: 14.4124
Total: 1353

Number of unique words in test set: 5629

Training set (hamspam):
Spam %: 13.4015
Ham %: 86.5985
Total: 5574

Number of unique words in training set: 12978
```
