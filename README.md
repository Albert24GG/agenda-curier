# Agenda-Curier
Agenda-Curier este o aplicatie simpla care poate fi folosita pentru stocarea datelor clientilor unei firme de curierat.
# Instalare
- Mai intai trebuie instalate librariile [ncurses](https://ftp.gnu.org/pub/gnu/ncurses/) (Mac/Linux) sau [pdcurses](https://pdcurses.org/) (Windows) si libraria [nlohmann](https://github.com/nlohmann/json).
- Apoi trebuie instalate fisierele din folderul [src](https://github.com/Albert24GG/agenda-curier/tree/main/src).
# Utilizare
```cpp
#include <menu.h>

//Se apeleaza functia void startApp() care va porni o fereastra cu programul
//La pornire datele din fisierul data.json sunt incarcate in program

int main(){
    startApp();
}
```
Datele introduse in program vor fi salvate in fisierul ***data.json*** din acelasi folder cu proiectul. Daca acest fisier nu exista, el va fi creat odata cu salvarea datelor.

# Interfata

![](https://github.com/Albert24GG/agenda-curier/blob/3e7d99da401dc501ea1b049acaab6e8e02a1799d/menu.png)
