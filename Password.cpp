#include "Password.h"
using CSC2110::ListArrayIterator;
#include "ListArrayIterator.h"
using CSC2110::ListArray;
#include "ListArray.h"
using CSC2110::String;
#include "Text.h"
//------------------------------
#include <iostream>
using namespace std;


Password::Password() 
{
   //create two empty lists
   all_words = new ListArray<String>;
   viable_words = new ListArray<String>;
   
   len = 0;
}

Password::~Password()
{
	ListArrayIterator<String>* iter = all_words->iterator();
	while (iter->hasNext())
	{
		delete iter->next();
	}
}

int Password::getNumMatches(String* curr_word, String* word_guess)
{
	int length = curr_word->length();
	int matches = 0;
	for(int i = 0; i < len; i++)
	{
		if(curr_word->charAt(i) == word_guess->charAt(i))
			matches++;
	}
	return matches;
}

void Password::addWord(String* word)
{

	if(all_words->isEmpty())
	{
		viable_words->add(word);
		all_words->add(word);
	    len = word->length();
	}
	else
	{
        if (word->length() != len) 
			return;
		viable_words->add(word);
		all_words->add(word);
/*

		String* items = all_words->get(1);
	
		items->displayString(); //Error Checking
	
		int item_len = items->length();

		cout << endl <<item_len << "     " << all_words->size(); //Error Checking
		cout << "     " << all_words->isEmpty() << endl; //Error Checking

		if (len == item_len) //Makes sure word added is same length as words in the word list
		{
			viable_words->add(word);
			all_words->add(word);
		}
*/
	}
}

void Password::guess(int try_password, int num_matches)
{
	String* guess = all_words->get(try_password); 
	int num_match = 0;
	int i = 1;
	int n = 0;
	
	ListArrayIterator<String>* iter = viable_words->iterator();
	String* next = iter->next();
	while (iter->hasNext())
	{
		num_match = getNumMatches(guess, next);
		if (num_match != num_matches)
		{
			viable_words->remove(i-n);
			n++;
			if (viable_words->get(5) == all_words->get(20))
				cout << "Fail";
		}
		i++;
		next = iter->next();
	}
	if (getNumMatches(viable_words->get(i-n), guess) != num_matches)
			viable_words->remove(i-n);
}

int Password::getNumberOfPasswordsLeft() //returns the number of possible passwords remaining
{
	return viable_words->size();
}

void Password::displayViableWords() //display the current list of possible passwords
{
	ListArrayIterator<String>* iter = viable_words->iterator();
	cout<<endl;
	while(iter->hasNext())
	{
		String* word = iter->next();
		word->displayString();
		cout << endl;
	}
	cout<<endl;
}

String* Password::getOriginalWord(int index) //get a word from the original list of all passwords, 1-based
{
	return all_words->get(index);
}






int Password::bestGuess()
{
   int best_guess_index = -1;
   int best_num_eliminated = -1;
   int num_viable_passwords = getNumberOfPasswordsLeft();

   //loop over ALL words, even if they have been eliminated as the password
   int count = 1;
   ListArrayIterator<String>* all_iter = all_words->iterator();
   while(all_iter->hasNext())
   {
      String* original_word = all_iter->next();

      //loop over only those words that could still be the password
      //count up the number of matches between a possible password and a word in the original list
      int* count_num_matches = new int[len + 1];

      for (int i = 0; i < len; i++) 
      {
         count_num_matches[i] = 0;
      }

      ListArrayIterator<String>* viable_iter = viable_words->iterator();
      while(viable_iter->hasNext())
      {
         String* viable_word = viable_iter->next();
         int num_matches = getNumMatches(viable_word, original_word);
         count_num_matches[num_matches]++;
      }
      delete viable_iter;

      //find the largest number in the count_num_matches array
      //the largest number indicates the guess that will generate the most eliminations
      int most_num_matches = 0;
      for (int j = 0; j < len; j++) 
      {
         int curr_num_matches = count_num_matches[j];
         if (curr_num_matches > most_num_matches)
         {
            most_num_matches = curr_num_matches;
         }
      }

      //compute the fewest that can possibly be eliminated by guessing the current word (original list)
      int num_eliminated = num_viable_passwords - most_num_matches;

      //select the word to guess that maximizes the minimum number of eliminations (minimax)
      if (num_eliminated > best_num_eliminated)
      {
         best_num_eliminated = num_eliminated;
         best_guess_index = count;
      }
      
      count++;
      delete[] count_num_matches;
   }

   delete all_iter;
   return best_guess_index;  //return a 1-based index into the all_words list of words (careful)
}