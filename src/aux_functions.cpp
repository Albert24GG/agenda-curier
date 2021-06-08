#include <aux_functions.h>

void displayCh(int n,char x)
{
    for(int i=0; i<n; ++i)
        addch(x);
}
void clear(int linii)
{
    move(12,2);
    for(int i=0; i<linii; ++i)
    {
        displayCh(100,' ');
        addch('\n');
    }
}

void citire(char *str,int &cnt)
{
    do
    {
        getstr(str);
        ++cnt;   ///cnt - number of lines until correct input
    }
    while(!strlen(str));
}

int getIndex(std::string s) ///get the last 4 digits
{
    int len = s.size();
    int rez = 0;
    for(int i = len-4; i < len; ++i)
    {
        rez = rez*10 + (s[i]-'0');
    }
    return rez;
}
