#include <menu.h>

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
                    c.erase_all();
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
                c.del(cnt);
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
                std::string number(numar);
                if(number.size()<4)
                {
                    addstr("\nNumarul de telefon introdus este invalid!\n");
                    getch();
                    clear(15+cnt);
                    break;
                }
                if(c.verify(number))
                {
                    addstr("\nPersoana a fost deja adaugata. Numarul comenzilor sale a crescut.");
                    getch();
                    clear(15+cnt);
                    break;
                }

                addstr("\nIntrodu numele si prenumele contactului(pe un singur rand):\n");
                citire(nume,cnt);

                addch('\n');

                addstr("Introdu adresa contactului(strada, numar, oras etc.):\n");
                citire(adress,cnt);

                addch('\n');

                addstr("Introdu e-mail-ul contactului:\n");
                citire(email,cnt);

                addch('\n');

                std::string name(nume), adress1(adress), mail(email);
                c.add(number, name, adress1, mail);
                getch();
                clear(15+cnt);
                break;
            }

            case 3:
            {
                mvaddstr(12,2,"Ai ales sa cauti un numar\n");
                int cnt=0;
                c.find1(cnt);
                getch();
                clear(10+cnt);
                break;

            }

            case 4:
            {
                mvaddstr(12,2,"Ai inchis agenda!");
                running = false;
                c.saveData();
            }

            }
        }
    }

    move(15,15);
}

void startApp(){
    initscr();
    cbreak();
    curs_set(0);
    c.initialize();
    keypad(stdscr,TRUE);
    main_menu();
    endwin();
}

void clients::initialize()
{
    FILE *f;
    f = fopen("data.json","r");

    if(f == nullptr)
    {
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

bool clients::verify(std::string number)
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

void clients::add(std::string numb,std::string name1, std::string adress, std::string email)
{
    int index = getIndex(numb);
    v[index].push_back( { name1, numb, adress, email, 1 } );
}

void clients::find1(int &cnt)
{
    addstr("Introdu numarul de telefon al contactului cautat:\n");
    char input[80];
    citire(input,cnt);
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

void clients::del(int &cnt)
{
    addstr("Introdu numarul de telefon al persoanei pe care vrei sa il stergi:\n");
    char input[80];
    citire(input,cnt);
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

void clients::erase_all()
{
    for(int i=0; i<10000; ++i)
    {
        v[i].clear();
    }
    addstr("\nToti clientii au fost stersi cu succes!");
}

void clients::saveData()
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
