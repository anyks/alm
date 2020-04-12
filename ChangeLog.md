# Change Log

## 2.0.5 Bug fixes

## 2.0.4 Bug fixes and new methods
### Methods
- **find** - ngrams search method in text
- **sentences** - sentences generation method
### Params
- flag to accurate check ngram
- file address *.txt for export oov words
- sentences count for generation

## 2.0.3 Bug fixes

## 2.0.2 Bug fixes

## 2.0.1 Optimization and bug fixes

## 2.0.0 New methods and bug fixes
### Methods
- **ppl** - perplexity calculation method
- **checktext** - text validation method
- **fixcase** - words case correction method
- **counts** - method counts of ngrams in the text
### Params
- ngram size for the counts method
- text to be processed as a string
- file address *.txt for texts export
- file address *.txt or dir path for texts import
- flag arpa file loading without pre-processing the words

## 1.0.4 Performance optimization and bug fixes
- flag to save intermediate data during training

## 1.0.3 Multiprocessor training and bug fixes
- flag to performing data segmentation during training
- flag to clearing temporary data during training
- segment size for training data segmentation
- number of threads for data collection
- file address *.txt or dir path for words import

## 1.0.2 New methods and bug fixes
- vprune - vocabulary pruning method
- flag allowing the use of words consisting of mixed dictionaries
- file address for restore mixed words for import

## 1.0.1 Performance improvements, bug fixes
- flag to reset the frequency of an unknown word
- flag export in binary dictionary of all data
- flag to disable all tokens
- flag identifying all tokens as 〈unk〉
- list of tokens to disable
- list of tokens for identification as 〈unk〉
- file address domain zones for import
- file address abbreviations for import
