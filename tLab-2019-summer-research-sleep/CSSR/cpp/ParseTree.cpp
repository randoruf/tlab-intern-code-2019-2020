/////////////////////////////////////////////////////////////////////////////
//Title:	ParseTree.cpp
//Author:	Kristina Klinkner
//Date:		July 23, 2003
//Description:  Class for ParseTree used to store all strings and their
//              frequency of occurence up to 
//              a set maximum length which appear in a given data file.
//              For use with CSSR.
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2002 Kristina Klinkner
//    This file is part of CSSR
//
//    CSSR is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    CSSR is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with CSSR; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//////////////////////////////////////////////////////////////////////////////

#include "ParseTree.h"


///////////////////////////////////////////////////////////////////////////
// Function: ParseTree::MakeAlphaHash
// Purpose: creates a hash table which returns the index of an alphabet
//          symbol
// In Params: none
// Out Params: none
// In/Out Params: none
// Pre- Cond: alphabet file has been read in
// Post-Cond: hash table contains all alpha info and has returned table
//            to calling function
//////////////////////////////////////////////////////////////////////////
HashTable2 *ParseTree::MakeAlphaHash() {
  char *symbol = new char[2];

  symbol[1] = '\0';

  for (int k = 0; k < m_alphaSize; k++) {
    symbol[0] = m_alpha[k];
    m_alphaHash->Insert(symbol, k);
  }
  delete[] symbol;
  return m_alphaHash;
}


///////////////////////////////////////////////////////////////////////////
// Function: ParseTree::Insert
// Purpose: inserts a new string into the Parse Tree
// In Params: the string to insert and the root of the current subtree
// Out Params: none
// In/Out Params: none
// Pre- Cond: none
// Post-Cond: tree exists with at least one string
//////////////////////////////////////////////////////////////////////////
void ParseTree::Insert(char string[], TreeNode *&root) {
  //base case, we are done with string
  if (strlen(string) < 1) {
    return;
  }

  //a node does not exist for this substring yet
  if (root == NULL) {
    //add a node
    root = new TreeNode;
    root->m_symbol = string[0];
    root->m_count++;
    //make recursive call with remaining string
    Insert(&(string[1]), root->m_child);
  }
    //this substring already exists
  else if (root->m_symbol == string[0]) {
    //increase the count of the substring
    root->m_count++;
    //and make recursive call
    Insert(&(string[1]), root->m_child);
  }
    //the substring may be in a sibling node
  else if (root->m_symbol != string[0]) {
    TreeNode **temp;
    temp = &root;
    while (*temp != NULL && (*temp)->m_symbol != string[0]) {
      temp = &((*temp)->m_sibling);
    }
    //or if not, it should be added there
    if (*temp == NULL) {
      *temp = new TreeNode;
      (*temp)->m_symbol = string[0];
    }
    (*temp)->m_count++;
    //recursive call made with sibling node
    Insert(&(string[1]), (*temp)->m_child);
  }
}


///////////////////////////////////////////////////////////////////////////
// Function: ParseTree::RemoveTree
// Purpose: deletes the Parse Tree
// In Params: the root of the current subtree
// Out Params: none
// In/Out Params: none
// Pre- Cond: tree exists
// Post-Cond: tree has been deleted
//////////////////////////////////////////////////////////////////////////
void ParseTree::RemoveTree(TreeNode *&root) {
  TreeNode *temp;

  if (root != NULL) {
    while (root != NULL) {
      temp = root;
      RemoveTree(root->m_child);
      root = root->m_sibling;
      delete temp;
    }
  }
}


///////////////////////////////////////////////////////////////////////////
// Function: ParseTree::FindStrings
// Purpose: wrapper function for protected version of FindStrings
// In Params: the length of strings looked for
// Out Params: the growable array of strings
// In/Out Params: none
// Pre- Cond: tree exists
// Post-Cond: strings of length 'length' have been put in an array
//            along with the counts of their children strings
//////////////////////////////////////////////////////////////////////////
void ParseTree::FindStrings(int length, G_Array *array) {
  char parentString = '\0';
  FindStrings(m_root, length, &parentString, array);
  return;
}


///////////////////////////////////////////////////////////////////////////
// Function: ParseTree::DecStringCount
// Purpose: wrapper function for protected version of DecStringCount
// In Params: the string to decrement
// Out Params: none
// In/Out Params: none
// Pre- Cond: tree exists
// Post-Cond: occurences of string and substrings have been decremented
//////////////////////////////////////////////////////////////////////////
void ParseTree::DecStringCount(char *stringToDec) {
  DecStringCount(stringToDec, m_root);
}


///////////////////////////////////////////////////////////////////////////
// Function: ParseTree::FindStrings
// Purpose: collects all strings of length 'length' in an array
// In Params: root of the tree, length of strings sought, a string to hold
// Out Params: the growable array of strings
// In/Out Params:  the parent symbols as tree is traversed
// Pre- Cond: tree exists
// Post-Cond: strings of length 'length' have been put in an array
//            along with the counts of their children strings
//////////////////////////////////////////////////////////////////////////
void ParseTree::FindStrings(TreeNode *root, int length, char *parentString,
                            G_Array *array) {
  //if strings do not exist at this branch return
  if (root == NULL) {
    return;

    //when at the desired level of the tree cycle thru list of siblings and add
    //each symbol to parent string
  }
  else if (length < 2) {
    //determine length of parentString
    int stringLength = 0;
    if (*parentString != '\0') {
      stringLength = strlen(parentString);
    }

    char *tempString = new char[stringLength + 2];
    TreeNode *tempChild;
    char *symbol = new char[2];
    int *counts = new int[m_alphaSize];
    while (root != NULL) {
      //copy over any parent string information
      symbol[0] = root->m_symbol;
      symbol[1] = '\0';
      if (stringLength != 0) {
        strcpy(tempString, parentString);
        strcat(tempString, symbol);
      }
      else {
        strcpy(tempString, symbol);
      }

      //initialize array
      for (int i = 0; i < m_alphaSize; i++) {
        counts[i] = 0;
      }

      //add counts of each child node to array
      //in order of alphabet symbols
      tempChild = root->m_child;
      while (tempChild != NULL) {
        for (int k = 0; k < m_alphaSize; k++) {
          if (m_alpha[k] == tempChild->m_symbol) {
            counts[k] = tempChild->m_count;
          }
        }
        tempChild = tempChild->m_sibling;
      }
      //input each in growable array class
      array->Insert(tempString, counts, m_alphaSize);
      root = root->m_sibling;
    }
    //free memory
    delete[] tempString;
    delete[] symbol;
    delete[] counts;
    return;
  }

    //check correct level in all possible branches
  else {
    //determine length of parentString
    int stringLength2 = 0;
    if (*parentString != '\0') {
      stringLength2 = strlen(parentString);
    }

    char *newParent = new char[stringLength2 + 2];
    char *symbol2 = new char[2];
    while (root != NULL) {
      //attach present symbol to parent string
      symbol2[0] = root->m_symbol;
      symbol2[1] = '\0';

      if (stringLength2 != 0) {
        strcpy(newParent, parentString);
        strcat(newParent, symbol2);
      }
      else {
        strcpy(newParent, symbol2);
      }

      //call each sibling recursively
      FindStrings(root->m_child, length - 1, newParent, array);
      root = root->m_sibling;
    }
    //free memory
    delete[] newParent;
    delete[] symbol2;
    return;
  }
}


///////////////////////////////////////////////////////////////////////////
// Function: ParseTree::CreateTree
// Purpose: reads data and stores all strings of up to maximum length 
//          in tree
// In Params: none
// Out Params: none
// In/Out Params:  none
// Pre- Cond: tree is empty
// Post-Cond: tree is full
//////////////////////////////////////////////////////////////////////////
void ParseTree::FillTree() {
  //look at futures of 1 for maximum
  //length strings
  int max_length = m_maxLength + 1;
  char *string = new char[max_length + 1];
  int i, j, k;

  for (i = 0; i <= (m_dataSize - max_length); i++) {
    for (k = 0; k < max_length; k++) {
      string[k] = m_data[k + i];
    }

    string[k] = '\0';
    Insert(string);
  }

  for (k = 0; k < max_length; k++, i++) {
    for (j = 0; j < max_length - k; j++) {
      string[j] = m_data[j + i];
    }

    string[j] = '\0';
    Insert(string);
  }
}


//////////////////////////////////////////////////////////////////////////
// Function: ParseTree::GetDataInput 
// Purpose: reads in the data string
// In Params: the data input filename 
// Out Params: none
// In/Out Params: none
// Pre- Cond: Filenames must have been read in at command line,
//	          size of alphabet file must be smaller than data file
// Post-Cond: Data info in ParseTree is initialized
//////////////////////////////////////////////////////////////////////////
void ParseTree::GetDataInput(char data_vector[]) {
  m_data = data_vector; 
  m_dataSize = strlen(m_data); 
  m_adjustedDataSize = (m_dataSize - (m_maxLength - 1)); 

  return; 
}


//////////////////////////////////////////////////////////////////////////
// Function: ParseTree::GetAlphaInput 
// Purpose: reads in the alphabet file
// In Params: the alphabet input filename 
// Out Params: none
// In/Out Params: none
// Pre- Cond: Filenames must have been read in at command line,
//	          size of alphabet file must be smaller than data file
// Post-Cond: Alphabet info in ParseTree is initialized
//////////////////////////////////////////////////////////////////////////
void ParseTree::GetAlphaInput(char alphabet_vector[]) {
  m_alpha = alphabet_vector; 
  m_alphaSize = strlen(m_alpha);
    
  return; 
}


//////////////////////////////////////////////////////////////////////////
// Function: ParseTree::CheckAlphaInput 
// Purpose: checks the alphabet string for spaces or nonsense
// In Params: ParseTree class variables --- m_alpha, m_alphaSize
// Out Params: none
// In/Out Params: none
// Pre- Cond: Alpha file has been read in 
// Post-Cond: Alphabet information in ParseTree class is valid
//////////////////////////////////////////////////////////////////////////
void ParseTree::CheckAlphaInput() {
  //check alphabet
  for (int k = 0; k < m_alphaSize; k++) {
    if (!(isalnum(m_alpha[k]))) {
      mexErrMsgTxt("Alphabet contains characters which are not alphanumeric.\n");
    }
  }

  return; 
}


//////////////////////////////////////////////////////////////////////////
// Function: ParseTree::CheckDataInput 
// Purpose: checks the data string for spaces or nonsense
// In Params: ParseTree class variables --- m_data, m_dataSize, m_alphaSize
// Out Params: none
// In/Out Params: none
// Pre- Cond: Alpha and Data files have been read in
// Post-Cond: Alphabet and Data info in ParseTree class is valid
//////////////////////////////////////////////////////////////////////////
void ParseTree::CheckDataInput() {

  bool accept_flag = false;
  //check data stream for characters which are not in the designated alphabet
  for (int i = 0; i < m_dataSize; i++) {
    // loop through the alphabet, and check if the current character is in the alphabet set. 
    for (int j = 0; j < m_alphaSize; j++) {
      if (m_data[i] == m_alpha[j]) {
        accept_flag = true;
      }
    }

    // every character is in the alphaset. 
    if (accept_flag) {
      accept_flag = false;      // reset the accept_flag to false.  
    //if symbol is not in alphabet, check for spaces
    }else{
      cerr << "Data contains characters which are not in the alphabet: " << m_data[i] << "\n";
      mexErrMsgTxt("Data contains characters which are not in the alphabet");
    }
  }
  
  return; 
}


//////////////////////////////////////////////////////////////////////////
// Function: ParseTree::ReadInput 
// Purpose: reads in the alphabet of symbols and data string
// In Params: the input filenames
// Out Params: an integer that denotes failure or success
// In/Out Params: none
// Pre- Cond: Filenames must have been read in at command line,
//	          size of alphabet file must be smaller than data file
// Post-Cond: Alphabet,Data arrays and related info in ParseTree class set
//////////////////////////////////////////////////////////////////////////
void ParseTree::ReadInput(char alphabet_vector[], char data_vector[]) {
  //Read in files
  GetAlphaInput(alphabet_vector);
  GetDataInput(data_vector);
  //Remove any extra spaces, endlines, etc.
  //And make sure input is valid
  CheckAlphaInput();
  CheckDataInput();
  return;
}


///////////////////////////////////////////////////////////////////////////
// Function: ParseTree::DecStringCount
// Purpose: decrements the recorded occurence of a string in the Parse Tree
// In Params: the string to decrement and the root of the current subtree
// Out Params: none
// In/Out Params: none
// Pre- Cond: none
// Post-Cond: counts of the string in the tree are one lower at each symbol
//////////////////////////////////////////////////////////////////////////
void ParseTree::DecStringCount(char string[], TreeNode *&root) {
  //base case, we are done with string
  if (strlen(string) < 1) {
    return;
  }

  //a node does not exist for this substring 
  if (root == NULL) {
    //no match
    mexErrMsgTxt("Attempting to decrement occurence of string which doesn't \nshow up in data,  ParseTree::DecStringCount ");
  }
    //this substring already exists
  else if (root->m_symbol == string[0]) {
    //increase the count of the substring
    root->m_count--;
    //and make recursive call
    DecStringCount(&(string[1]), root->m_child);
  }

    //the substring may be in a sibling node
  else if (root->m_symbol != string[0]) {
    TreeNode **temp;
    temp = &root;
    while (*temp != NULL && (*temp)->m_symbol != string[0]) {
      temp = &((*temp)->m_sibling);
    }
    //or if not, it should be added there
    if (*temp == NULL) {
      //no match
      mexErrMsgTxt("Attempting to decrement occurence of string which doesn't \nshow up in data,  ParseTree::DecStringCount "); 
    }
    (*temp)->m_count--;
    //recursive call made with sibling node
    DecStringCount(&(string[1]), (*temp)->m_child);
  }
}


///////////////////////////////////////////////////////////////////////////
// Function: ParseTree::MakeSynchAdjustments
// Purpose: decrements the recorded occurence of synchronization data
//           in the Parse Tree
// In Params: the synchronization string and the index at which synchronization
//            ended
// Out Params: none
// In/Out Params: none
// Pre- Cond: tree exits, counts are positive
// Post-Cond: tree counts have been adjusted for all max lenght substrings
//            of the synch data
//////////////////////////////////////////////////////////////////////////
void ParseTree::MakeSynchAdjustements(char *synchString, int index) {
  int counter = 0;
  int stringLength = strlen(synchString);
  char *tempString = new char[stringLength + m_maxLength + END_STRING];
  char *nextLString = new char[m_maxLength + END_STRING];
  strcpy(tempString, synchString);
  int i, j, k;
  //int tempCounter = 0;


  for (i = 0; i < m_maxLength - 1; i++) {
    nextLString[i] = m_data[index - 1 + i];
  }
  nextLString[i] = '\0';
  strcat(tempString, nextLString);

  //look at all max length strings and decrement occurences
  if (strlen(tempString) >= ((unsigned long) m_maxLength)) {
    while (((unsigned long) counter) < (strlen(tempString) - m_maxLength)) {
      for (k = 0; k < m_maxLength; k++) {
        nextLString[k] = tempString[k + counter];
      }
      counter++;
      nextLString[k] = '\0';
      DecStringCount(nextLString);
    }
  }

  for (k = 0; k < m_maxLength; k++) {
    for (j = 0; j < m_maxLength - k; j++) {
      nextLString[j] = tempString[k + j];
    }
    nextLString[j] = '\0';
    DecStringCount(nextLString);
  }
  m_adjustedDataSize -= (strlen(synchString) - (m_maxLength - 1));
}

///////////////////////////////////////////////////////////////////////////
// Function: ParseTree::FindRoots
// Purpose: collects all strings of length 1 in an array, and their
//          unconditional frequencies in another array
// In Params: root of the tree
// Out Params: the size of the arrays, the arrays of symbols and frequencies
// In/Out Params:  none
// Pre- Cond: tree exists
// Post-Cond: strings of length 1 have been put in an array
//            and their frequencies have been put in another
//////////////////////////////////////////////////////////////////////////
int ParseTree::FindRoots(TreeNode *root, char charArray[], int intArray[]) {
  int size = 0;

  //check to make sure data exists
  if (m_dataSize == 0) {
    mexErrMsgTxt("no data in tree\n");
  }
  while (root != NULL) {
    charArray[size] = root->m_symbol;
    intArray[size] = root->m_count;
    size++;
    root = root->m_sibling;
  }
  return size;
}
