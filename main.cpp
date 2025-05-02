#include <iostream>
#include <vector>
#include "windows.h"
#include <random>
#include <ctime>
#include <SFML/Graphics.hpp>
using namespace std;

class GameOfLife {
public:

    //Gra z pusta plansza na start
    void graj() {

        //Generowanie okienka z plansza
        sf::RenderWindow window(
        sf::VideoMode({750, 750}), "Game of Life (p - play/pause, l - losuj, r - reset, up - szybciej, down - wolniej)",
        sf::Style::Default, sf::State::Windowed,
        sf::ContextSettings{.antiAliasingLevel = 8});

        //Domyslne wartosci zmiennych (mozna rozszerzyc funkcje o wybor wymiarow planszy)
        kol = 50;
        wiersz = 50;
        predkosc = 500;
        czyPauza = true;

        plansza.assign(wiersz, vector<bool>(kol, false)); //Stworzenie pustej planszy

        drukuj(plansza, window);

        while (window.isOpen()) {
            if (!czyPauza) sf::sleep(sf::milliseconds(predkosc));

            while (auto const event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();
                else if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
                    if (key->code == sf::Keyboard::Key::P)
                        czyPauza = !czyPauza;
                    if (key->code == sf::Keyboard::Key::Escape)
                        window.close();
                    //Zwiekszanie/zmniejszanie odstepu miedzy turami w ms (min. 100ms)
                    if (key->code == sf::Keyboard::Key::Up)
                        if (predkosc>=150) predkosc -=50;
                    if (key->code == sf::Keyboard::Key::Down)
                        predkosc +=50;
                    //czynnosci wykonywane tylko w trakcie pauzy
                    else if (key->code == sf::Keyboard::Key::L && czyPauza)
                        Losuj(); //Losowe wypelnienie planszy
                    else if (key->code == sf::Keyboard::Key::R && czyPauza)
                        plansza.assign(wiersz, vector<bool>(kol, false)); //Wyczyszczenie planszy
                }
                //Zmiana stanu klocka po wcisnieciu przycisku myszy
                else if (auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mb->button == sf::Mouse::Button::Left && czyPauza) {
                        sf::Vector2i pos = sf::Mouse::getPosition(window); //Lokalizacja klikniecia
                        //Znalezienie odpowiadajacego kwadracika i zmiana jego stanu (przy zalozeniu szerokosci kwadracika = 15)
                        int i = pos.y / 15, j = pos.x / 15;
                        if (i >= 0 && i < kol && j >= 0 && j < wiersz)
                            plansza[i][j] = !plansza[i][j];
                    }
                }
            }
            NastPlansza(); //Wczytanie ulozenia w nastepnej turze

            if (!czyPauza) iteruj(); //Jesli gra nie jest zapauzowana to przejscie do nastepnej tury

            drukuj(plansza, window);
        }
    }
    //Gra z wlasna plansza 50x50
    void graj(vector<vector<bool>>& _plansza) {
        plansza = _plansza;

        //Sprawdzenie czy wczytana plansza ma wymiary 50x50
        if (!(plansza.size() == 50 && plansza[0].size() == 50)) {
            cout << "Twoja plansza musi miec wymiary 50x50!" << endl;
            return;
        }

        //Generowanie okienka z plansza
        sf::RenderWindow window(
        sf::VideoMode({750, 750}), "Game of Life (p - play/pause, l - losuj, r - reset, up - szybciej, down - wolniej)",
        sf::Style::Default, sf::State::Windowed,
        sf::ContextSettings{.antiAliasingLevel = 8});

        //Domyslne wartosci zmiennych
        kol = plansza.size();
        wiersz = plansza[0].size();
        predkosc = 500;
        czyPauza = true;

        drukuj(plansza, window);

        while (window.isOpen()) {
            if (!czyPauza) sf::sleep(sf::milliseconds(predkosc));

            while (auto const event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();
                else if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
                    if (key->code == sf::Keyboard::Key::P)
                        czyPauza = !czyPauza;
                    if (key->code == sf::Keyboard::Key::Escape)
                        window.close();
                    //Zwiekszanie/zmniejszanie odstepu miedzy turami w ms (min. 100ms)
                    if (key->code == sf::Keyboard::Key::Up)
                        if (predkosc>=150) predkosc -=50;
                    if (key->code == sf::Keyboard::Key::Down)
                        predkosc +=50;
                    //czynnosci wykonywane tylko w trakcie pauzy
                    else if (key->code == sf::Keyboard::Key::L && czyPauza)
                        Losuj(); //Losowe wypelnienie planszy
                    else if (key->code == sf::Keyboard::Key::R && czyPauza)
                        plansza.assign(wiersz, vector<bool>(kol, false)); //Wyczyszczenie planszy
                }
                //Zmiana stanu klocka po wcisnieciu przycisku myszy
                else if (auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mb->button == sf::Mouse::Button::Left && czyPauza) {
                        sf::Vector2i pos = sf::Mouse::getPosition(window); //Lokalizacja klikniecia
                        //Znalezienie odpowiadajacego kwadracika i zmiana jego stanu (przy zalozeniu szerokosci kwadracika = 15)
                        int i = pos.y / 15, j = pos.x / 15;
                        if (i >= 0 && i < kol && j >= 0 && j < wiersz)
                            plansza[i][j] = !plansza[i][j];
                    }
                }
            }
            NastPlansza(); //Wczytanie ulozenia w nastepnej turze

            if (!czyPauza) iteruj(); //Jesli gra nie jest zapauzowana to przejscie do nastepnej tury

            drukuj(plansza, window);
        }
    }

private:
    int kol;
    int wiersz;
    int predkosc;
    bool czyPauza;
    vector<vector<bool>> plansza;
    vector<vector<bool>> nastplansza;
    vector<vector<sf::RectangleShape>> widok;

    //przekonwertowanie planszy booleanskiej do SFML i jej wydrukowanie
    void drukuj(vector<vector<bool>>& _plansza, sf::RenderWindow& window) {
        widok.assign(wiersz, vector<sf::RectangleShape>(kol)); //stworzenie macierzy z kwadracikami
        for (int i = 0; i < wiersz; i++) {
            for (int j = 0; j < kol; j++) {
                //wlasciwosci kwadracika - czarna, cienka obwodka i wymiary 15x15
                widok[i][j] = sf::RectangleShape({15.f, 15.f});
                widok[i][j].setOutlineThickness(1);
                widok[i][j].setOutlineColor(sf::Color::Black);
                widok[i][j].setPosition({float(j*15), float(i*15)});
                //Wczytanie koloru w zaleznosci od stanu na planszy booleanskiej
                widok[i][j].setFillColor(_plansza[i][j] ? sf::Color::Black
                                                        : sf::Color::White);
                window.draw(widok[i][j]); //zapisanie ksztaltow w pamieci
            }
        }
        window.display(); //wydrukowanie wszystkich ksztaltow z pamieci
    }

    //Ustawienie losowej planszy
    void Losuj() {
        srand(time(nullptr)); //wygenerowanie losowego seedu
        for (int i = 0; i < wiersz; i++)
            for (int j = 0; j < kol; j++)
                plansza[i][j] = rand() % 2; //wylosowanie wartosci 0-1
    }

    //Sprawdzenie czy komórka przeżyje/umrze/narodzi się
    bool NastStan(vector<vector<bool>>& plansza, int _wiersz, int _kol) {
        int suma = 0; //Liczba sasiadow
        for (int poziom = _wiersz-1; poziom <=_wiersz+1; poziom++) {
            if(poziom<0 || poziom>=wiersz) continue; //sprawdzenie granic planszy

            for (int pion = _kol-1; pion<=_kol+1; pion++) {
                if(pion<0 || pion>=kol || (poziom==_wiersz && pion==_kol)) continue; //sprawdzenie granic planszy i czy sasiad nie jest sprawdzanym klockiem
                if(plansza[poziom][pion]) suma++;
            }
        }
        //Dla zywych komorek
        if (plansza[_wiersz][_kol]) {
            if (suma == 2 || suma == 3) return true; //Jeśli żywa komórka ma 2 lub 3 sąsiadów to przeżywa
            return false; //Jeśli ma <2 lub >3 sąsiadów to umiera
        }
        //Dla martwych komorek
        if (suma==3) return true; //Jeśli martwa komórka ma 3 sąsiądów to się narodzi
        return false;
    }

    //Załadowanie następnej tury
    void NastPlansza() {
        nastplansza.assign(wiersz, vector<bool>(kol, false));
        for (int i = 0; i < wiersz; i++)
            for (int j = 0; j < kol; j++)
                nastplansza[i][j] = NastStan(plansza, i, j);
    }

    //Przejście do następnej tury
    void iteruj() {
        for (int i = 0; i < wiersz; i++)
            for (int j = 0; j < kol; j++)
                plansza[i][j]=nastplansza[i][j];
    }
};

//wygenerowanie planszy 50 x50 z gliderem
vector<vector<bool>> glider() {
    vector<vector<bool>> plansza (50, vector<bool>(50, false));
    plansza[1][0] = true;
    plansza[0][2] = true;
    plansza[1][2] = true;
    plansza[2][1] = true;
    plansza[2][2] = true;
    return plansza;
}
//wygenerowanie planszy 50x50 z pulsarem
vector<vector<bool>> pulsar() {
    vector<vector<bool>> plansza (50, vector<bool>(50, false));
    vector<int> iter1 = {21, 22, 23, 27, 28, 29};
    vector<int> iter2 = {19, 24, 26, 31};

    for (int i = 0; i < iter1.size(); i++)
        for (int j = 0; j < iter2.size(); j++){
            plansza[iter1[i]][iter2[j]] = true;
            plansza[iter2[j]][iter1[i]] = true;
            }

    return plansza;
}

int main(){
    GameOfLife gra;

    //gra.graj(); //gra z pusta plansza

    //vector<vector<bool>> plansza = glider(); //wygenerowanie planszy z gliderem
    vector<vector<bool>> plansza = pulsar(); // wygenerowanie planszy z pulsarem
    gra.graj(plansza); //gra z wlasna plansza
    return 0;
}