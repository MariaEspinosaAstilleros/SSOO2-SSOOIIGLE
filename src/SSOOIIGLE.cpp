/******************************************************
 * Project:         Práctica 2 de Sistemas Operativos II
 * 
 * Program name:    SSOOIIGLE.cpp
 
 * Author:          María Espinosa Astilleros
 * 
 * Date created:    10/3/2020
 * 
 * Purpose:         Receive the name of the file, the word to search and the number of threads that it will do the search.
 *                  Distribute the lines of the file to the different threads and it searches the word.
 *                  Finally, show the word to search and the times it appear in file.
 * 
 ******************************************************/

#include <iostream>
#include <thread>
#include <string>
#include <algorithm> //std::for_each
#include <vector>
#include <queue>
#include <fstream> 
#include <mutex>
#include <locale>   //std::tolower
#include <functional>
#include <sstream>  //std::isstringstream 
#include <cctype>   //ispunct
#include <clocale>  //std::setlocale


#include "../include/color.h" 
#include "../include/searchResult.h"

#define  NUM_CARACTER_ERASE 1 //Number of characters erase when find symbols

/*Functions declaration*/
std::vector<std::string>    divisionLine(std::string line);
std::string                 changeToLowercaseAndEraseSimbols(std::string line); 
int                         countFileLines(std::string filename);  
void                        messageWelcome(); 
void                        messageFinish();
void                        divisionLinesAndCreateThreads(int num_threads, int num_lines, std::string file_name, std::string word_search); 
void                        createThreads(); 
void                        obtainWordsLine(int id_thread, std::string file_name, std::string word_search);
void                        searchWord(std::vector<std::string> words_line, std::string word_search, int index, int actual_line);
void                        waitThreadsEnd();
void                        numWords(std::string word_search);         

/*Globals variables*/
std::vector<std::thread>    g_vthreads;   //Vector of threads
std::vector<SearchResult>   g_vresult;    //Vector of search result 
std::mutex                  g_sem;
int                         g_total = 0;  //Number of times appear the word searched


/******************************************************
 * Function name:    main
 * Date created:     10/3/2020
 * Input arguments: 
 * Purpose:          Principal method of class
 * 
 ******************************************************/
int main(int argc, char* argv[]){
    std::setlocale(LC_ALL, "UTF-8");
    messageWelcome(); 

    if(argc != 4){  /*Arguments check*/
        fprintf(stderr, "Error. Use: ./SSOOIIGLE <name_file> <word> <numbers_threads>"); 
        return EXIT_FAILURE; 
    }

    std::string     file_name   = argv[1]; 
    std::string     word_search = changeToLowercaseAndEraseSimbols(argv[2]);  
    int             num_threads = atoi(argv[3]);  
    int             num_lines   = countFileLines(file_name); 

    divisionLinesAndCreateThreads(num_threads, num_lines, file_name, word_search); 
    waitThreadsEnd(); 
    numWords(word_search); 
    messageFinish();

    return EXIT_SUCCESS; 

}

/******************************************************
 * Function name:    messageWelcome
 * Date created:     10/3/2020
 * Input arguments: 
 * Purpose:          Show a welcome message of our searcher SSOOIIGLE
 * 
 ******************************************************/
void messageWelcome(){
    std::cout << "WELCOME TO SEARCHER" << BOLDBLUE << " SS" << BOLDRED << "O" << BOLDYELLOW << "O";
    std::cout << BOLDBLUE << "II" << BOLDGREEN << "GL" << BOLDRED << "E\n" << RESET << std::endl;  
}

/******************************************************
 * Function name:    messageFinish
 h
 * Date created:     16/3/2020
 * Input arguments: 
 * Purpose:          Show a goodbye message of our searcher SSOOIIGLE
 * 
 ******************************************************/
void messageFinish(){
    std::cout << "FINISH TO SEARCHER" << BOLDBLUE << " SS" << BOLDRED << "O" << BOLDYELLOW << "O";
    std::cout << BOLDBLUE << "II" << BOLDGREEN << "GL" << BOLDRED << "E" << RESET << std::endl; 
}

/******************************************************
 * Function name:    changeToLowercaseAndEraseSimbols
 * Date created:     11/3/2020
 * Input arguments:  The word to search or a line of the file
 * Purpose:          Replace the string to lowercase and erase the symbols
 * 
 ******************************************************/
std::string changeToLowercaseAndEraseSimbols(std::string line){

    std::for_each(line.begin(), line.end(), [](char & c){
        c = ::tolower(c);
    });

    for (unsigned i = 0; i < line.size(); i++) { 
        if (ispunct(line[i])) { 
            line.erase(i, NUM_CARACTER_ERASE); 
        } 
    } 

    return line;
}

/******************************************************
 * Function name:    countFileLines
 * Date created:     11/3/2020
 * Input arguments:  The file where we do the search  
 * Purpose:          Obtain number of lines that has the file  
 * 
 ******************************************************/
int countFileLines(std::string filename){
    int             nlines = 0; 
    std::ifstream   fp(filename, std::ifstream::binary); 

	while(fp.good()){  
        std::string line; 
		if(std::getline(fp, line, '\n')){
			nlines++;
		}
	}

	fp.close();

	return nlines;
}

/******************************************************
 * Function name:    divisionLinesAndCreateThreads
 * Date created:     12/3/2020
 * Input arguments:  The number of threads, the number of lines of the file, 
 *                   the file and the word to search
 * Purpose:          Divide the assignments of each thread, 
 *                   create the data structure that save the result 
 *                   and finally, create the threads that start to search the word
 * 
 ******************************************************/
void divisionLinesAndCreateThreads(int num_threads, int num_lines, std::string file_name, std::string word_search){
    int start           = 0; 
    int end             = 0;
    int id_thread       = 0;  
    int lines_thread    = num_lines / num_threads;  

    for(unsigned i = 0; i < num_threads; i++){
        start       = (lines_thread * i) + 1;
        end         = lines_thread * (i + 1); 
        id_thread   =  i + 1; 

        if(i == (num_threads -1)){ end = num_lines; } 

        SearchResult sr(id_thread, start, end); 
        std::unique_lock<std::mutex> ul(g_sem); 
        g_vresult.push_back(sr);  
        ul.unlock(); 

        /*Creating the threads*/
        std::cout << GREEN << "* Creating the thread " << id_thread  << " *" << RESET << std::endl; 
        g_vthreads.push_back(std::thread(obtainWordsLine, id_thread, file_name, word_search));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

/******************************************************
 * Function name:    obtainWordsLine
 * Date created:     16/3/2020
 * Input arguments:  The thread id, the file and the word to search 
 * Purpose:          Search in each file fragment that correspond to each thread and
 *                   each line will divide in a vector where we save the words that it has
 * 
 ******************************************************/
void obtainWordsLine(int id_thread, std::string file_name, std::string word_search){
    int                         start_; 
    int                         end_; 
    int                         index       = id_thread - 1;    /*Index of the vector of results*/
    int                         actual_line = 1;                /*Number of line where we work*/
    std::string                 line;                           /*Line where we work*/
    std::vector<std::string>    words_line;                     /*Vector that save the number of words that has the line where we work*/
    std::ifstream               fp(file_name, std::ifstream::in); 

    std::unique_lock<std::mutex> ul(g_sem);  
    start_ = g_vresult.at(index).getStartLine(); 
    end_   = g_vresult.at(index).getEndLine(); 
    ul.unlock();     

    std::cout << GREEN << "The thread " << id_thread << " start to search the word..." << RESET << std::endl;

    while (actual_line <= end_){
        std::getline(fp, line, '\n'); 
        if(actual_line >= start_){ 
            line        = changeToLowercaseAndEraseSimbols(line);    
            words_line  = divisionLine(line); 
            searchWord(words_line, word_search, index, actual_line);
        } 

        actual_line++; 
    }
}

/******************************************************
 * Function name:    searchWord
 * Date created:     23/3/2020
 * Input arguments:  The vector of words, the word to search, 
 *                   the index of vector and the actual line 
 * Purpose:          Search the word inside the vector "words_line"
 * 
 ******************************************************/
void searchWord(std::vector<std::string> words_line, std::string word_search, int index, int actual_line){
    Content cont;

    for(unsigned i = 0; i < words_line.size(); i++){
        if(words_line.at(i).compare(word_search) == 0){
            
            cont.nline = actual_line;
            cont.word = words_line.at(i); 

            /*Check the start line*/
            if(i == 0){
                cont.line_prev = "*Start of line*"; 
            }else{
                cont.line_prev = words_line.at(i-1); 
            }

            /*Check the end line*/
            if(i == words_line.size()-1){
                cont.line_post = "*End of line*";
            }else{
                cont.line_post = words_line.at(i+1); 
            }

            /*Add the result inside the vector of results*/   
            std::lock_guard<std::mutex> lk(g_sem); 
            g_vresult.at(index).addSearchQueue(cont);
        }
    } 
}

/******************************************************
 * Function name:    divisionLine
 * Date created:     16/3/2020
 * Input arguments:  Line of file
 * Purpose:          Separate the words of the line and it saved in a vector    
 * 
 ******************************************************/
std::vector<std::string> divisionLine(std::string line){
    std::vector<std::string>    vector_words;
    std::string                 token; 
    std::istringstream          iss (line, std::istringstream::in); 

    while(std::getline(iss, token, ' ')){
        vector_words.push_back(token); 
    }

    return vector_words;
}


/******************************************************
 * Function name:    waitThreadsEnd
 * Date created:     20/3/2020
 * Input arguments:  
 * Purpose:          Wait the threads end, 
 *                   count the times appear the word in file 
 *                   and show the search result. 
 * 
 ******************************************************/
void waitThreadsEnd(){
    std::cout << GREEN << "Waiting the threads end...\n" << RESET << std::endl; 
    std::for_each(g_vthreads.begin(), g_vthreads.end(), std::mem_fn(&std::thread::join)); 
    
    for(SearchResult sr : g_vresult){ 
        g_total += sr.getNumContent(); 
    }

    for(unsigned i = 0; i < g_vthreads.size(); i++){
        std::lock_guard<std::mutex> lk(g_sem);
        g_vresult.at(i).showResultQueue();
        
    }
}

/******************************************************
 * Function name:    numWords
 * Date created:     20/3/2020
 * Input arguments:  The word to search 
 * Purpose:          Show the word to search and the times it appear in file  
 * 
 ******************************************************/
void numWords(std::string word_search){
    std::lock_guard<std::mutex> lk(g_sem); 
    std::cout << "\nThe word " << CYAN <<  word_search << RESET;
    std::cout << " appear in file " << CYAN << g_total  << " times\n" << RESET << std::endl; 
}
