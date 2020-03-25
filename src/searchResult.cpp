/******************************************************
 * Project:         Práctica 2 de Sistemas Operativos II
 * 
 * Program name:    searchResult.cpp
 
 * Author:          María Espinosa Astilleros
 * 
 * Date created:    9/3/2020
 * 
 * Purpose:         Implementation of the class SearchResult
 * 
 ******************************************************/

#include <iostream>
#include <queue>
#include "../include/color.h"
#include "../include/searchResult.h"


/*Constructor*/
SearchResult::SearchResult(int _id_thread, int _start_line, int _end_line){
    this -> _id_thread = _id_thread; 
    this -> _start_line = _start_line;
    this -> _end_line = _end_line; 
}

/*Getters declaration*/
int SearchResult::getThread(){ return _id_thread; }
int SearchResult::getEndLine(){ return _end_line; }
int SearchResult::getStartLine(){ return _start_line; }
int SearchResult::getNumContent(){ return result_queue.size(); }

/*Add elements search to the queue*/
void SearchResult::addSearchQueue(Content cont){ result_queue.push(cont); }

/*Show the elements of queue*/
void SearchResult::showResultQueue(){
    while(!result_queue.empty()){
        showResult(result_queue.front());
        result_queue.pop(); 
    }
}

/*Show the result of search final*/
void SearchResult::showResult(Content cont){
    std::cout << "[" << "Thread " << _id_thread << " start: " << _start_line;
    std::cout << " - finish: " << _end_line << "]";
    std::cout << " :: " << "line " << cont.nline << " :: ";
    std::cout << "... " << cont.line_prev << " ";
    std::cout << YELLOW << cont.word << RESET << " ";
    std::cout << cont.line_post << " ..." << std::endl;
}