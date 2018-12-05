# SimplifiedSpellChecker
This is a simplified implementation of a spell checker. The main objective is not the spell checker but rather the duration it
takes std::set or std::unordered_set to do such operation. 

# How to Run
This program accepts two command-line argument. The first argument is a name of plain-text file with a text to check. The
second argument is a plain-text file with the list of correct words; this is the dictionary of your spell checker.

# What it Does
The program will create a text file where each line contains the wrong word from the checked text and the suggestions how to
correct this word. It is possible to have no suggestions. The word is considered wrong if it is not in the dictionary.

The program starts with the creation a dictionary. In this program, dictionary is just a collection of all known correct
words. Those words should be read from the dictionary file (the second command-line argument) and stored in either std::set
or std::unordered_set.

> Note: The program is defaulted to std::set, to change it just swap std::set with std::unordered_set

The time that is being measured is the running time of the spell-checking itself for the given text and given dictionary files. 

The types of mistakes that this spell-checker will recognize are:
- a single letter is missing somewhere in the word. So, if the word is “moter”, one of the suggested corrections should be “mother”.
- a single extra letter appears in the word. If the word is “cadse”, one of the suggested corrections should be “case”.
- a single letter is replaced by the other letter. For the same “moter” word, the list of corrections should include “meter” and “motor”.
- two neighboring letters swapped their positions. If the word is “nto”, one of the suggested corrections should be “not”.
