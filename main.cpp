#include <fstream>
#include <curses.h>   ///Trebuie instalata libraria ncurses
#include <string>
#include <cstring>
#include <json.hpp>
#include <list>

using json = nlohmann::json;

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

bool esc = 0;

void citire(char *str,int &cnt)
{
    do
    {
        getstr(str);
        ++cnt;   ///cnt - number of lines until correct input
        /*if(getch()==27){
            esc = true;
            return;
        }*/
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

struct pbook
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

    void initiliaze()
    {
        FILE *f;
        f = fopen("data.json","r");

        if(f == nullptr){
            return;
        }

        std::string content;
        json j;
        fseek(f, 0, SEEK_END);
        content.resize(ftell(f));
        rewind(f);
        fread(&content[0], 1, content.size(), f);
        fclose(f);
        if(content != "")
        {
            try
            {
                j = json::parse(content);
            }
            catch(json::exception &e)
            {
            }
            j = json::parse(content);
            int cnt = 0;
            while(!j[cnt].is_null())
            {
                std::string phone_number = j[cnt]["phone_number"];
                int index = getIndex(phone_number);
                v[index].push_back( { j[cnt]["name"], j[cnt]["phone_number"], j[cnt]["adress"], j[cnt]["email"], j[cnt]["orders"] } );
                ++cnt;
            }
        }


    }

    bool verify(std::string number)
    {
        int index = getIndex(number);
        for(std::list<Person>::iterator it = v[index].begin(); it != v[index].end(); ++it)
        {
            if(it->phone_number == number)
            {
                ++it->orders;
                return true;
            }
        }
        return false;
    }

    void add(std::string numb,std::string name1, std::string adress, std::string email)
    {
        int index = getIndex(numb);
        v[index].push_back( { name1, numb, adress, email, 1 } );
    }

    void find1(int &cnt)
    {
        addstr("Introdu numarul de telefon al contactului cautat:\n");
        char input[80];
        //char *input;
        citire(input,cnt);
        if(esc){
            return;
        }
        std::string searched(input);
        int index = getIndex(searched);
        for(std::list<Person>::iterator it = v[index].begin(); it != v[index].end(); ++it)
        {
            if(it->phone_number == searched)
            {
                displayCh(50,'-');
                addch('\n');
                printw("Numele persoanei este: ");
                const char* str = &it->name[0];
                printw(str);
                printw("\nNumarul de telefon al persoanei este: ");
                str = &it->phone_number[0];
                printw(str);
                printw("\nAdresa persoanei este: ");
                str = &it->adress[0];
                printw(str);
                printw("\nE-mail-lul persoanei este: ");
                str = &it->email[0];
                printw(str);
                printw("\nNumarul comenzilor totale ale persoanei este: ");
                str  = &(std::to_string(it->orders))[0];
                printw(str);
                addch('\n');
                displayCh(50,'-');
                addch('\n');
                return;
            }
        }
        printw("Persoana cautata nu a fost adaugata!");

    }
    void del(int &cnt)
    {
        addstr("Introdu numarul de telefon al persoanei pe care vrei sa il stergi:\n");
        char input[80];
        citire(input,cnt);
        if(esc){
            return;
        }
        std::string toDel(input);
        int index = getIndex(toDel);
        for(std::list<Person>::iterator it = v[index].begin(); it != v[index].end(); ++it)
        {
            if(it->phone_number == toDel)
            {
                addstr("  Esti sigur ca vrei sa stergi contactul acesta?(Y/N)\n");
                char choice = getch();
                if(tolower(choice) == 'y')
                {
                    v[index].erase(it);
                    addstr("\n Contactul a fost sters cu succes!");
                }
                else
                {
                    addstr("\n  Te-ai razgandit.");
                }
                return;
            }
        }
        addstr("  Contactul pe care vrei sa il stergi nu exista.");

    }
    void display_agend(int &cnt)
    {
        bool is_empty = 1;
        for(int i=0; i<10000; ++i)
        {
            if(!v[i].empty())
            {
                is_empty = 0;
                break;
            }
        }
        if(is_empty)
        {
            addstr("Lista de contacte e goala");
            return;
        }
        displayCh(50,'-');
        addch('\n');
        for(int i=0; i<10000; ++i)
        {
            for(std::list<Person>::iterator it = v[i].begin(); it!=v[i].end(); ++it)
            {
                printw("Numele persoanei: ");
                const char* str = &it->name[0];
                printw(str);
                printw("\nNumarul de telefon al persoanei: ");
                str = &it->phone_number[0];
                printw(str);
                printw("\nAdresa persoanei: ");
                str = &it->adress[0];
                printw(str);
                printw("\nE-mail-lul persoanei: ");
                str = &it->email[0];
                printw(str);
                printw("\nNumarul comenzilor totale ale persoanei: ");
                str  = &(std::to_string(it->orders))[0];
                printw(str);
                addch('\n');
                displayCh(50,'-');
                addch('\n');
                cnt+=20;
            }
        }
    }

    void erase_all()
    {
        for(int i=0; i<10000; ++i)
        {
            v[i].clear();
        }
        addstr("\nToti clientii au fost stersi cu succes!");
    }

    void saveData()
    {
        FILE *f;
        f = fopen("data.json", "w");
        json j = json::array();
        for(int i=0; i<10000; ++i)
        {
            for(std::list<Person>::iterator it = v[i].begin(); it!=v[i].end(); ++it)
            {
                j.insert(j.end(), { { {"name", it->name},{"phone_number", it->phone_number},{"adress", it->adress},{"email", it->email}, {"orders", it->orders} } });
            }
        }
        std::string data;
        data = j.dump();
        fwrite(&data[0], 1, data.size(), f);
    }

} v;

void main_menu()
{
    int menu_item=0, x=3;
    bool running = true;

    mvaddstr(1,2,"Meniul Agendei      (Navigate using arrow keys. Press Enter to proceed. Press ESC to cancel operation.)");

    refresh();

    mvaddstr(3,0,"->");

    refresh();

    mvaddstr(3,2,"2) Sterge toate persoanele din agenda");

    refresh();

    mvaddstr(4,2,"3) Sterge o singura persoana din agenda");

    refresh();

    mvaddstr(5,2,"4) Adauga o persoana in agenda");

    refresh();

    mvaddstr(6,2,"5) Cauta o persoana in agenda");

    refresh();

    mvaddstr(7,2,"6) Quit Program & Save Data");

    refresh();

    while(running)
    {
        noecho();
        curs_set(0);
        int input=getch();

        if(input==KEY_DOWN  && x != 7) //down button pressed
        {
            mvprintw(x,0,"  ");
            refresh();
            x++;
            mvprintw(x,0,"->");
            refresh();
            menu_item++;
            continue;

        }
        if(input==KEY_UP  && x != 3) //up button pressed
        {

            mvprintw(x,0,"  ");
            refresh();
            x--;
            mvprintw(x,0,"->");
            refresh();
            menu_item--;
            continue;
        }
        if(input==10)  /// Enter key pressed. 10 este valoare in tabelul ascii a tastei enter( \n )
        {
            echo();
            curs_set(1);
            switch(menu_item)
            {
            case 0:
            {
                mvaddstr(12,2,"Ai ales sa stergi toate contactele\n");

                addstr("  Esti sigur ca vrei sa stergi toate contactele?(Y/N)\n");
                char choice=getch();
                if(tolower(choice) == 'y')
                {
                    v.erase_all();
                }
                else
                    addstr("\n  Te-ai razgandit.");

                getch();
                clear(15);
                break;
            }

            case 1:
            {
                mvaddstr(12,2,"Ai ales sa stergi un contact\n");
                int cnt=0;
                v.del(cnt);
                if(esc){
                    clear(15+cnt);
                    esc = false;
                    break;
                }
                getch();
                clear(10+cnt);
                break;
            }

            case 2:
            {
                mvaddstr(12,2,"Ai ales sa adaugi un numar in agenda\n");
                char nume[80],numar[80], adress[150], email[80];
                int cnt=0;
                addstr("\nIntrodu numarul de telefon al contactului:\n");
                citire(numar,cnt);
                if(esc){
                    clear(15+cnt);
                    esc = false;
                    break;
                }
                std::string number(numar);
                if(number.size()<4)
                {
                    addstr("\nNumarul de telefon introdus este invalid!\n");
                    getch();
                    clear(15+cnt);
                    break;
                }
                if(v.verify(number))
                {
                    addstr("\nPersoana a fost deja adaugata. Numarul comenzilor sale a crescut.");
                    getch();
                    clear(15+cnt);
                    break;
                }

                addstr("\nIntrodu numele si prenumele contactului(pe un singur rand):\n");
                citire(nume,cnt);
                if(esc){
                    clear(15+cnt);
                    esc = false;
                    break;
                }
                addch('\n');

                addstr("Introdu adresa contactului(strada, numar, oras etc.):\n");
                citire(adress,cnt);
                if(esc){
                    clear(15+cnt);
                    esc = false;
                    break;
                }
                addch('\n');

                addstr("Introdu e-mail-ul contactului:\n");
                citire(email,cnt);
                if(esc){
                    clear(15+cnt);
                    esc = false;
                    break;
                }
                addch('\n');

                std::string name(nume), adress1(adress), mail(email);
                v.add(number, name, adress1, mail);
                getch();
                clear(15+cnt);
                break;
            }

            case 3:
            {
                mvaddstr(12,2,"Ai ales sa cauti un numar\n");
                int cnt=0;
                v.find1(cnt);
                if(esc){
                    clear(15+cnt);
                    esc = false;
                    break;
                }
                getch();
                clear(10+cnt);
                break;

            }

            case 4:
            {
                mvaddstr(12,2,"Ai inchis agenda!");
                running = false;
                v.saveData();
            }

            }
        }
    }

    move(15,15);
}
int main()
{
    initscr();
    cbreak();
    curs_set(0);
    v.initiliaze();
    keypad(stdscr,TRUE);
    main_menu();
    endwin();
    return 0;
}
