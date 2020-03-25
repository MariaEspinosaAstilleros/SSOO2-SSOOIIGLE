/******************************************************
 * Project:         Práctica 2 de Sistemas Operativos II
 * 
 * Program name:    searchResult.h
 
 * Author:          María Espinosa Astilleros
 * 
 * Date created:    9/3/2020
 * 
 * Purpose:         Contains the structure of the search elements and the definition of methods of the class SearchResult 
 * 
 ******************************************************/

#include <iostream>
#include <queue>

struct Content{
    int             nline; 
    std::string     line_prev; 
    std::string     line_post;
    std::string     word; 
};

class SearchResult{
    private:
        int                   _id_thread;
        int                   _start_line;
        int                   _end_line;
        std::queue<Content>   result_queue;
        
    public:
        SearchResult(int _id_thread, int _start_line, int _end_line);
        int     getThread(); 
        int     getStartLine(); 
        int     getEndLine(); 
        int     getNumContent(); 
        void    showResult(Content cont); //Show the result of search final
        void    addSearchQueue(Content cont); //Add elements search to the queue
        void    showResultQueue(); //Show the elements of queue
};