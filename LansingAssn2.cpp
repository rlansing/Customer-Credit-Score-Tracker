//***************************************************************************
//  FILENAME:	 LansingAssn2.CPP
//  DESCRIPTION: Program converts the credit scores of customers between the
//               ages of 25 and 60 inclusive to credit ratings, and makes the
//               data searchable by SSN.
//               input file data format:
//               333-44-5555 60 720
//               output file data format:
//               333-44-5555 1
//               
//  DATE:        November 01, 2014
//  DESIGNER:	 Robert Lansing
//  FUNCTIONS:	 main - calls other functions. closes files.
//               program description - displays program description
//               openInFile - opens input file
//               readData - reads data from file
//               creditRatingFinder - returns an enumerated credit rating value
//                                    based on credit score
//               parallelSelectionSortDecending - sorts parallel arrays in 
//                                                decending order
//               outData - ouputs data to output file
//               userPromptDisplay - displays available SSN to search, takes
//                                   user input and calls other functions
//                                   to check and search, displays credit score
//               ssnErrorCheck - check user input for formatting errors
//               binarySearch - performs a binary search on decending arrays
//               creditRatingString - displays a string based on enumerated
//                                    type
//***************************************************************************

#include <iostream> //for I/O
#include <iomanip>  //for output manipulators
#include <fstream>  //for stream variable
#include <string>   //for strings
#include <cctype>   //for character checking


using namespace std;

//global constants
enum creditRating {EXCELLENT, GOOD, AVERAGE, FAIR, POOR, BAD}; 
const int MAX_LIST = 7,   //max number of components in arrays
          MIN_AGE = 25,      //minimum age for storing in arrays
          MAX_AGE = 60,      //maximum age for storing in arrays
          EXCELLENT_L = 750, //excellent credit score lower boundary
          GOOD_L = 720,      //good credit score lower boundary
          GOOD_H = 749,      //good credit score upper boundary
          AVERAGE_L = 660,   //average credit score lower boundary
          AVERAGE_H = 719,   //average credit score upper boundary
          FAIR_L = 620,      //fair credit score lower boundary
          FAIR_H = 659,      //fair credit score upper boundary
          POOR_L = 600,      //poor credit score lower boundary
          POOR_H = 619;      //poor credit score upper boundary
const string CREDIT_FILE = "CREDIT.TXT",
             SORTED_OUTP = "SORTED.TXT";
const char SPACE = ' ';
      

// prototypes
void programDescription();
void openInFile(ifstream& variable, string fileName);
void readData(string ssnArray[], int creditRatingArray[], int& count,
              ifstream& inFile);
int creditRatingFinder(int creditScore);
void parallelSelectionSortDecending(string listA[], int listB[], int count);
void outData(ofstream& outPut, string arrayOne[], int arrayTwo[], int count);
void userPromptDisplay(string ssnArray[], int creditRatingArray[], int count);
bool ssnErrorCheck(string ssnPrompt);
void binarySearch(bool& found, int& rating, int count, string ssnPrompt, 
                 int creditRatingArray[], string ssnArray[]);
void creditRatingString(int rating);
//*********************************************************************
// FUNCTION: Main
// DESCRIPTION: calls other functions. closes files.
// INPUT:
//       Parameters: none
//       File:       calls funtion to open input file
// OUTPUT:
//        Return Val: none
//                   
//        Parameters: none
//    
// CALLS TO: programDescription, readData, parallelSelectionSortDecending
//           outData, userPromptDisplay
//**********************************************************************
int main()
{   
    //variable declarations
    string ssnArray[MAX_LIST];
    
    int creditRatingArray[MAX_LIST],
        count;
    
    ifstream inFile;
    ofstream outFile;
    
    //function calls
    programDescription(); //display program description

    openInFile(inFile, CREDIT_FILE); //open input file
    if (!inFile) //test if file opened successfully
    {
        cin.ignore();
        cin.get();
        return 1;
    }
    
    readData(ssnArray, creditRatingArray, count, inFile); //stores data in array
    
    inFile.close(); //close input file after data is read
    
    parallelSelectionSortDecending(ssnArray, creditRatingArray, count);
    
    outFile.open(SORTED_OUTP.c_str()); //opens output file
    
    outData(outFile, ssnArray, creditRatingArray, count); //outputs to file
    
    outFile.close();
    
    userPromptDisplay(ssnArray, creditRatingArray, count);
    
    
    cout << "Press [enter] to exit" << endl;
    cin.ignore();
    cin.get();
    return 0;
    
    
    
    
}
//*************************************************************************
//  FUNCTION:	 programDescription
//  DESCRIPTION: Displays purpose of program
//  INPUT:       Parameter:    none
//  OUTPUT: 	 program purpose to standard output device
//*************************************************************************
void programDescription()
{
    cout << "******************************************************************"
         << endl 
         << "This program will read the credit scores and SSN of customers" 
         << endl 
         << "between the ages of 25 and 60 (inclusive) from a input file,"
         << endl
         << "and store them in a data file. Credit ratings are searchable by"
         << endl
         << "the customers SSN. The program will output the customers credit"
         << endl
         << "rating." 
         << endl
         << "******************************************************************"
         << endl;
}
//*********************************************************************
// FUNCTION: openInFile
// DESCRIPTION: generic function opens input file
// INPUT:
//       Parameters: variable - input file stream variable to be associated
//                   filename - name of input file
//       File:       opens file containing SSN, age, and customer credit score
// OUTPUT:
//        Parameters: variable - associates with input file
//        displays: error message if file can't be opened
//**********************************************************************
void openInFile(ifstream& variable, string fileName)
{
    variable.open(fileName.c_str()); //associate variable with input file
                                        //and open file.
    
    if (!variable) //test if file opened successfully
    {
        cout << endl << "Error opening file." << endl << "Exiting program." 
             << endl;
    }
}
//*********************************************************************
// FUNCTION: readData
// DESCRIPTION: reads data from input file
// INPUT:
//       Parameters: ssnArray- contains SSN of customers
//                   creditRatingArray - contains enum type for credit rating
//                   count - number of items in array
//                   inFile - input file stream variable
//       File:       full SSN, customer age, credit score
// OUTPUT:
//        Parameters: stores data into parallel arrays and stores coung value
// CALLS TO: creditRatingFinder
//**********************************************************************
void readData(string ssnArray[], int creditRatingArray[], int& count,
              ifstream& inFile)
{
    string ssn;
    
    int age,
        creditScore;
        
    count = 0; //
        
    inFile >> ssn >> age >> creditScore; //priming read
    
    while(inFile)
    {   //while there is still data test if age is within set range
        if (age >= MIN_AGE and age <= MAX_AGE)
        {   //if age is in range check to see if space is still available
            if (count < MAX_LIST)
            {
                ssnArray[count] = ssn;
                creditRatingArray[count] = creditRatingFinder(creditScore);
                count++;
            }
            else
                cout << "Error too much data in the input file." << endl
                     << "There are more than " << MAX_LIST << " customers."
                     << endl;
        }
        inFile >> ssn >> age >> creditScore;
    }
}
//*********************************************************************
// FUNCTION: creditRatingFinder
// DESCRIPTION: uses extended if else to determine credit rating and returns
//              an enum value
// INPUT:
//        Parameters: creditScore 
// OUTPUT:
//        Return Val: enum type representing the credit rating
//**********************************************************************
int creditRatingFinder(int creditScore)
{
    if (creditScore >= EXCELLENT_L)
        return EXCELLENT;
    else if (creditScore >= GOOD_L and creditScore <= GOOD_H)
        return GOOD;
    else if (creditScore >= AVERAGE_L and creditScore <= AVERAGE_H)
        return AVERAGE;
    else if (creditScore >= FAIR_L and creditScore <= FAIR_H)
        return FAIR;
    else if (creditScore >= POOR_L and creditScore <= POOR_H)
        return POOR;
    else
        return BAD;
}
//*********************************************************************
// FUNCTION: parallelSelectionSortDecending
// DESCRIPTION: generic function sorts parallel arrays in decending order
// INPUT:
//       Parameters: listA - list used to determine the order of the arrays
//                   listB - parallel list sorted in decending order based on
//                           the values in listA
// OUTPUT:
//        File: Description of data written to file
//**********************************************************************
void parallelSelectionSortDecending(string listA[], int listB[], int count)
{
    int curTop,       //current top of unsorted list
        location,     //location to compare
        maxPosition,  //largest element location
        tempB;        //temporary storage for value in listB
        
    string tempA;     //temporary storage for value in listA
        
    //each run of for loop moves the top of the list forward one position    
    for (curTop = 0; curTop < count + 1; curTop++)
    {
        maxPosition = curTop; //assume first item in the list is the largest
        
        //search the unsorted portion of the list for the largest element
        for (location = curTop + 1; location < count + 1; location++)
             if (listA[location] > listA[maxPosition])
                 maxPosition = location;
                 
        //swap if the maximum position is not currently at the top of list 
        if (maxPosition != curTop)
        {
            //move largest item in the first list to the top
            tempA = listA[curTop];
            listA[curTop] = listA[maxPosition];
            listA[maxPosition] = tempA;
            
            //moves the item corresponding to the largest item in the parrallel
            //array
            tempB = listB[curTop];
            listB[curTop] = listB[maxPosition];
            listB[maxPosition] = tempB; 
        }
    }
}
//*********************************************************************
// FUNCTION: outData
// DESCRIPTION: outputs data to file
// INPUT:
//       Parameters: output - output file stream variable associated with file
//                   arrayOne - string array to be output to file
//                   arrayTwo - integer array to be ouput to file
//                   count - number of items in arrays
// OUTPUT:
//        File: each component of parallel arrays are output to the file
//              with one component of each per line
//              example:
//              (arrayOne)  (arrayTwo)
//              111-11-1111 0 
//              222-33-4444 3
//**********************************************************************
void outData(ofstream& outPut, string arrayOne[], int arrayTwo[], int count)
{   
    for (int index = 0; index < count; index++)
         outPut << arrayOne[index] << SPACE << arrayTwo[index] << endl;  
}
//*********************************************************************
// FUNCTION: userPromptDisplay
// DESCRIPTION: function displays all SSN's stored, prompts user to select
//              SSN to view credit rating for that customer, and displays
//              credit rating
// INPUT:
//       Parameters: ssnArray - array containing SSN strings
//                   creditRatingArrray - array containing credit rating values
//                   count - number of items in arrays
// OUTPUT:
//       Display: displays SSN stored in ssnArray, prompts user for SSN 
//                to search, displays credit rating for SSN or displays
//                that no SSN was found that matches user input
// CALLS TO: ssnErrorCheck, BinarySearch, creditRatingString
//**********************************************************************
void userPromptDisplay(string ssnArray[], int creditRatingArray[], int count)
{
    int rating; //to store credit rating value
    
    string ssnPrompt; //to store user input ssn value
           
    bool ssnFormat, //returned by error check, true if ssn entered correctly
         found = false;//used to check if ssn is found in binary search
    
    cout << "Social Security Numbers on file are:" << endl;
    for (int index = 0; index < count; index++)
    {
        cout << ssnArray[index] << SPACE;
        //creates new line after 5 ssn's are displayed
        if (index > 1 and index % 4 == 0) 
           cout << endl;
    } 
    
    cout << endl << endl;
    cout << "Enter SSN to find (or X to exit):"; //initial prompt
    cin >> ssnPrompt;
    
    while (ssnPrompt != "x" and ssnPrompt != "X")
    {
        ssnFormat = ssnErrorCheck(ssnPrompt);//function call to check user input
        if (ssnFormat)
        {   // if input is correct perform binary search
            binarySearch(found, rating, count, ssnPrompt, creditRatingArray, 
                         ssnArray);
            if (found)
            {    //if the ssn was found display the credit rating
                 cout << "The customer with SSN " << ssnPrompt << " has "
                      << "a(n) "; creditRatingString(rating); 
                 cout << " credit rating." << endl;
            }
            else
               cout << "No customer with SSN " << ssnPrompt << " was found.";    
        }
        cout << endl << endl;
        cout << "Enter SSN to find (or X to exit):"; //re-prompt to continue
        cin >> ssnPrompt;
    }    
}
//*********************************************************************
// FUNCTION: ssnErrorCheck
// DESCRIPTION: verifies correct SSN format
// INPUT:
//       Parameters: ssnPrompt - string entered by user holding SSN to check
// OUTPUT:
//        Return Val: bool - used to verify SSN format is correct
//        Display: outputs error message to standard output device if
//                    SSN is formatted incorrectly
//**********************************************************************
bool ssnErrorCheck(string ssnPrompt)
{
    int size;
    
    char item;
    
    size = ssnPrompt.length(); //measure the length of the string
    
    if (size < 11) //checks to see if string entered is too short
    {
        cout << "Input " << ssnPrompt << " is too short" << endl 
             << "SSN must be exactly 11 characters long, formatted as "
             << "###-##-####" << endl;
        return false;
    }   
    
    if (size > 11) //check to see if string entered is too long
    {
        cout << "Input " << ssnPrompt << " is too long" << endl 
             << "SSN must be exactly 11 characters long, formatted as "
             << "###-##-####" << endl;
        return false;
    }   
    
    for (int i = 0; i < size; i++) //loop to check all characters in string
    {
        item = ssnPrompt[i]; 
        
        if (i == 3 or i == 6)  //checks if count is the location of the dashes     
        {
            if (item != '-') //if the character at those locations are not 
            {                //dashes display error message
                cout << "Input dashes and digits " << ssnPrompt 
                     << " are formatted incorrectly." << endl
                     << "SSN must be exactly 11 characters long, formatted as "
                     << "###-##-####" << endl;
                
                return false;
            }
        }
       
       else if (!isdigit(item)) //checks if the character is a digit
       {
           cout << "Input dashes and digits " << ssnPrompt 
                << " are formatted incorrectly." << endl
                << "SSN must be exactly 11 characters long, formatted as "
                << "###-##-####" << endl;
                
           return false;
       }
    }      
    return true;
}
//*********************************************************************
// FUNCTION: binarySearch
// DESCRIPTION: searches a parallel array in decending order for an item,
//              if item is found the second parallel array location is stored
//              in rating variable.
// INPUT:
//       Parameters: found - used to test whether item is found
//                   rating - used to store corresponding array item
//                   count - number of items in array
//                   ssnPrompt - stored user input from prompt
//                   creditRatingArray - array containing enum credit ratings
//                   ssnArray - array containing customer SSN numbers
// OUTPUT:
//        Parameters: found - changed to true if item is found
//                    rating - stores corresponding array item if search 
//                             is successful
//**********************************************************************
void binarySearch(bool& found, int& rating, int count, string ssnPrompt, 
                 int creditRatingArray[], string ssnArray[])
{
    int index = 0,
        mid;
    
    found = false; //initally set to false
    
    while (index <= count and !found) //loop to search until item is found
    {                                 //or there is nothing left to search
        mid = (index + count) / 2; //finds middle of array
        
        if (ssnArray[mid] == ssnPrompt) 
        {
            found = true;
            rating = creditRatingArray[mid]; //corresponding location in second 
        }                                    //array is stored in variable
        else if (ssnArray[mid] > ssnPrompt)
           index = mid + 1;
        else
           count = mid - 1;
    }
}

//*********************************************************************
// FUNCTION: creditRatingString
// DESCRIPTION: switch  to determine credit rating using enum type
// INPUT:
//        Parameters: rating - value representing enum value of credit rating
// OUTPUT:
//        Display: displays credit rating to standard output device
//**********************************************************************
void creditRatingString(int rating)
{
    switch (rating)
    {
        case EXCELLENT:
             cout << "excellent";
             break;
        case GOOD:
             cout << "good";
             break;
        case AVERAGE:
             cout << "average";
             break;
        case FAIR:
             cout << "fair";
             break;
        case POOR:
             cout << "poor";
             break;
        case BAD:
             cout << "bad";
    }
}






