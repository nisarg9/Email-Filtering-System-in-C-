/*Nisarg Thakkar
Project 1, CS 251
nthakk28@uic.edu

Email filterning system 
This program loads different spamlists into a vector, displays that to console,
check a specific emaild id is spam or not based on loaded spam file, and lastly
filters emails and outputs non spam emails again based on loaded spam file by user */

#include <iostream>
#include <fstream>
#include <string>
#include "ourvector.h"

using namespace std;

/* parse input line, return what command and filename is entered
 command and filename is passed on to main function, returns void */

void parseInput(string& command, string& filename, string input)
{
    int found;
    found = input.find(" ");                                          //finding space in the input line
        
    command = input.substr(0, found);                              //getting command out of input line, before space
    if (command == "load" || command == "check" || command == "filter")
    {
        filename = input.substr(found + 1);                         //getting filename, after space
    }
}

/*saving email list file in vector
passing on vector to main , inputing filename and returning string based on fucntion did what it was supposed to do */

string saveInVector(string filename, ourvector<string>& emailVector)
{
    emailVector.clear();
    string eachLine;
    ifstream inpFS;
    inpFS.open(filename);                                              //opened file supplied by main

    if(!inpFS.is_open())                                                //if input stream can't open, return to main function
    {
        cout << "**Error, unable to open '" << filename << "'" << endl;
        return "no";
    }
    
    while(!inpFS.eof())
    {
        getline(inpFS, eachLine);                                          //get each line from input file
        if (!inpFS.fail()) 
        {
            emailVector.push_back(eachLine);                              //creating vector from email file pushing line by line
        }
    }
    inpFS.close();                                                        //closing the file since info is already in data structure
    cout << "# emails processed: " << emailVector.size() << endl;
    return "yes";                                                         //if reached here, meant everything ran successfully, return yes
}

/* loading spamlist provided by user to a vector, passing that spam vector to main
 * input spam file and spam vector, returning void */
 
void load(string& spamfile, ourvector<string>& spamVector)
{
    spamVector.clear();                                //clearing the vector before reloading
    string eachSpam;
       
    ifstream inFS;
    inFS.open(spamfile);
    
    if(!inFS.is_open())
    {
        cout << "**Error, unable to open '" << spamfile << "'" << endl;
        return;                                      //if input file can't open, return from function, wrong filename
    }
    else                                            //if file opens correctly
    {
        cout << "Loading '" << spamfile << "'" << endl;
    }
    
    while(!inFS.eof())
    {
        inFS >> eachSpam;                            //reading it line by line and putting into vector
        if (!inFS.fail()) 
        {
            spamVector.push_back(eachSpam);                    //creating spam vector line by line from input file
        }
    }
    inFS.close();
    cout << "# of spam entries: " << spamVector.size() << endl; //no of spam entries will be similar to vector size
}

/*dislaying the spams in the vector on the console
 * input spam vector, ouput void */

void disp(ourvector<string>& spamVector)
{
    for (auto i : spamVector)                                //using for-each loop print contents of spam
    {
        cout << i << endl;
    }
}

/*cheking if this ID is spamVector using binary search
 * input: search id, spam vector, output string based on what function did */

string binarySearch(string searchThis, ourvector<string>& spamVector) 
{
   int mid = 0;
   int low = 0;
   int high = spamVector.size() - 1;
      
   while (high >= low) 
   {
      mid =  low + ((high - low) / 2);
      string mid_spam = spamVector[mid];
      
      if (mid_spam < searchThis) 
      {
         low = mid + 1;
      }
      else if (mid_spam > searchThis) 
      {
         high = mid - 1;
      }
      else 
      {
         return "yes";                                         // if found the correct thing, then quit the function and return yes
      }
    }   
    return "no";                                               //if unable to find, return no
}

/*function to perform check command, cheking if given id is in spam folder
 * input spam vector, and id to check, output boolean */

bool check(string id, ourvector<string>& spamVector)
{
    int found;
    string username, domain, spamDomain, spamFound;
    string searchUsername, OrsearchStar;
    ourvector<string> searchVector;                        // will consist of two elements, id with username and star
    found = id.find("@");
    
    username = id.substr(0, found);                          //string before @ is username 
    domain = id.substr(found + 1);                             //string after @ is domain
    searchUsername = domain + ":" + username;                //formatting to match the format of spam file
    OrsearchStar = domain + ":*";                              //another possible format in spam file
    
    searchVector.push_back(searchUsername);                      //putting both possible format in vector
    searchVector.push_back(OrsearchStar);
    
    for (int i = 0; i < 2; i++)                                  //looping through that vector and performing binary search to find if id is spam or not
    {
        spamFound = binarySearch(searchVector[i], spamVector);
        if (spamFound == "yes")                                           //if binary search returns yes, that means it's spam
        {
            cout << id << " is spam" << endl;;
            return true;
        }
    }
    cout << id << " is not spam" << endl;                                //if binary search returns not found, then not a spam
    return false;
}
/*filtering emails based on spam list and printing it to opuput file specified by user
 * input email vector, spam vector, output stream, output void */

void filter(ourvector<string>& emailVector, ourvector<string>& spamVector, ofstream& outFS)
{
    
    int count = 0;
    for(string i : emailVector)                                  //for loop for each email id in the email list file
    {
        int space1, space2, pos;
        string email;
        space1 = i.find(" ");                                        //finding first space
        space2 = i.find(" ", pos = space1 + 1);                            //finding second space
        email = i.substr(space1 + 1, (space2 - (space1 + 1)));                  //email is between both spaces
                
        int found;
        string username, domain, spamDomain;
        string spamFound1 = "yes";                 //assuming everything is spam! if it's not (not found in spam file by binay search)
        string spamFound2 = "yes";                           //include those in ouput file
        string searchUsername, OrsearchStar;
        found = email.find("@");                            //finding @ in the email id to get domain and username seprated

        username = email.substr(0, found);                   //getting username out of email id
        domain = email.substr(found + 1);                  //getting domain out of email id
        searchUsername = domain + ":" + username;              //creating similar formats as spam file to perform search
        OrsearchStar = domain + ":*";         
               
        spamFound1 = binarySearch(searchUsername, spamVector);          //perform binary search with both formats
        spamFound2 = binarySearch(OrsearchStar, spamVector);
        if (spamFound1 == "no" && spamFound2 == "no")             //if both of them couldn't find the id in spam folder, it means it's not spam
        {  
            outFS << i << endl;                  //printing that output file
            count++;                         //keeping count of non spam emails
        }
       
    }
    cout << "# non-spam emails: " << count << endl;
}

int main()
{
    string input;
    string command;
    string filename;
    ourvector<string> spamVector, emailVector;
    
    cout << "** Welcome to spam filtering app **" << endl;
    cout << endl;
    cout << "Enter command or # to exit> ";
	getline(cin, input); //reading a full line
	
	while (input != "#")
	{
        parseInput(command, filename, input);        //passing input to parseInput function to get extract command file name out of it
        // do different stuff based on command type        
        if (command == "load")
        {            
            load(filename, spamVector);                 //load function
        }        
        else if (command == "display")
        {
            disp(spamVector);                 //disp function
        }        
        else if (command == "check")
        {
            string id = filename;            //renaming filename to id as it makes more sense when command is check
            check(id, spamVector);               //check function
        }        
        else if (command == "filter")
        {
            int space;
            space = filename.find(" ");            //finding first space
            if (space != -1)                   //if first space is found, meaning the inputted format is correct
            {
                string inputFile, outputFile, done;
                inputFile = filename.substr(0, space);            //first file is input file
                outputFile = filename.substr(space + 1);              // second file is output file
                
                done = saveInVector(inputFile, emailVector);             //open input file and save data into vector
                if (done == "yes")                                 //if above function correctly, deal with output file
                {
                    ofstream outfile(outputFile);               //creating putput file to write non spam emailVector
                    
                    if(!outfile.good())                          //if outful file is not good
                    {
                        cout << "**Error, unable to open '" << outputFile << "'" << endl;                   
                    }
                    else                                       //if output file is good, perform filtering
                    {
                        filter(emailVector, spamVector, outfile);                //filter function
                    }
                    outfile.close();                            //close the file
                }
            }
        }
        
        else                  //if entered the wrong command or format
        {
            cout << "**invalid command" << endl;
        }
        cout << endl;
        cout << "Enter command or # to exit> ";
		getline(cin, input);                   //saving entered command into input        
    }
    
    
}