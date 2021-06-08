#ifndef AUX_FUNCTIONS_H_INCLUDED
#define AUX_FUNCTIONS_H_INCLUDED

#include <string>
#include <cstring>
#include <curses.h>

void displayCh(int n, char x);

void clear(int linii);

void citire(char *str,int &cnt);

int getIndex(std::string s); ///get the last 4 digits


#endif // AUX_FUNCTIONS_H_INCLUDED
