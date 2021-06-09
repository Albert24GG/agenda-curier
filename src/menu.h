#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <fstream>
#include <curses.h>
#include <string>
#include <json.hpp>
#include <list>
#include <aux_functions.h>

using json = nlohmann::json;

struct clients
{
    struct Person
    {
        std::string name;
        std::string phone_number;
        std::string adress;
        std::string email;
        int orders;
    };

    std::list<Person> v[10001]; ///v[i] -> i represents the last 4 digits of the number

    void initialize();

    bool verify(std::string number);

    void add(std::string numb, std::string name1, std::string adress, std::string email);

    void find1(int &cnt);

    void del(int &cnt);

    void erase_all();

    void saveData();

}c;

void main_menu();

void startApp();

#endif // MENU_H_INCLUDED
