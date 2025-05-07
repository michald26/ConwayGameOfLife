#include <iostream>
#include <vector>
#include "windows.h"
#include <random>
#include <ctime>
#include <SFML/Graphics.hpp>
using namespace std;

class GameOfLife {
public:

    //Konstruktor

    GameOfLife(): kol(50), wiersz(50), predkosc(500), czyPauza(true),
                  plansza(50, vector<bool>(50, false)),
                  nastplansza(50, vector<bool>(50, false)),
                  widok(50, vector<sf::RectangleShape>(50)),
                  gen(random_device{}()){}

    //Gra z pustą planszą na start
    void graj() {

        //Generowanie okienka z planszą
        sf::RenderWindow window(
        sf::VideoMode({750, 750}), "Game of Life (p - play/pause, l - losuj, r - reset, up - szybciej, down - wolniej)",
        sf::Style::Default, sf::State::Windowed,
        sf::ContextSettings{.antiAliasingLevel = 8});

        //Domyślne wartości zmiennych (można rozszerzyć funkcję o wybór wymiarów planszy)
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
                    //Zwiększanie/zmniejszanie odstępu między turami w ms (min. 100ms)
                    if (key->code == sf::Keyboard::Key::Up)
                        if (predkosc>=150) predkosc -=50;
                    if (key->code == sf::Keyboard::Key::Down)
                        predkosc +=50;
                    //Czynności wykonywane tylko w trakcie pauzy
                    else if (key->code == sf::Keyboard::Key::L && czyPauza)
                        Losuj(); //Losowe wypelnienie planszy
                    else if (key->code == sf::Keyboard::Key::R && czyPauza)
                        plansza.assign(wiersz, vector<bool>(kol, false)); //Wyczyszczenie planszy
                }
                //Zmiana stanu klocka po wciśnięciu przycisku myszy
                else if (auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mb->button == sf::Mouse::Button::Left && czyPauza) {
                        sf::Vector2i pos = sf::Mouse::getPosition(window); //Lokalizacja kliknięcia
                        //Znalezienie odpowiadającego kwadracika i zmiana jego stanu (przy założeniu szerokści kwadracika = 15px)
                        int i = pos.y / 15, j = pos.x / 15;
                        if (i >= 0 && i < wiersz && j >= 0 && j < kol)
                            plansza[i][j] = !plansza[i][j];
                    }
                }
            }
            NastPlansza(); //Wczytanie ułożenia w następnej turze

            if (!czyPauza) iteruj(); //Jeśli gra nie jest zapauzowana to przejście do nastepnej tury

            drukuj(plansza, window);
        }
    }
    //Gra z własną planszą 50x50
    void graj(vector<vector<bool>>& _plansza) {
        plansza = _plansza;

        //Sprawdzenie czy wczytana plansza ma wymiary 50x50
        if (!(plansza.size() == 50 && plansza[0].size() == 50)) {
            cout << "Twoja plansza musi miec wymiary 50x50!" << endl;
            return;
        }

        //Generowanie okienka z planszą
        sf::RenderWindow window(
        sf::VideoMode({750, 750}), "Game of Life (p - play/pause, l - losuj, r - reset, up - szybciej, down - wolniej)",
        sf::Style::Default, sf::State::Windowed,
        sf::ContextSettings{.antiAliasingLevel = 8});

        //Domyślne wartości zmiennych
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
                    //Zwiększanie/zmniejszanie odstępu między turami w ms (min. 100ms)
                    if (key->code == sf::Keyboard::Key::Up)
                        if (predkosc>=150) predkosc -=50;
                    if (key->code == sf::Keyboard::Key::Down)
                        predkosc +=50;
                    //Czynnosci wykonywane tylko w trakcie pauzy
                    else if (key->code == sf::Keyboard::Key::L && czyPauza)
                        Losuj(); //Losowe wypełnienie planszy
                    else if (key->code == sf::Keyboard::Key::R && czyPauza)
                        plansza.assign(wiersz, vector<bool>(kol, false)); //Wyczyszczenie planszy
                }
                //Zmiana stanu klocka po wciśnięciu przycisku myszy
                else if (auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mb->button == sf::Mouse::Button::Left && czyPauza) {
                        sf::Vector2i pos = sf::Mouse::getPosition(window); //Lokalizacja kliknięcia
                        //Znalezienie odpowiadającego kwadracika i zmiana jego stanu (przy założeniu szerokści kwadracika = 15px)
                        int i = pos.y / 15, j = pos.x / 15;
                        if (i >= 0 && i < wiersz && j >= 0 && j < kol)
                            plansza[i][j] = !plansza[i][j];
                    }
                }
            }
            NastPlansza(); //Wczytanie ułożenia w nasępnej turze

            if (!czyPauza) iteruj(); //Jeśli gra nie jest zapauzowana to przejście do następnej tury

            drukuj(plansza, window);
        }
    }

    //Monte Carlo - funkcja zwraca udział żywych komórek na planszy
    //po n symulacjach losowych gier z określoną liczbą tur
    double MonteCarlo(int tury, int n, int seed){
        mt19937 generator(seed);
        double wynik = 0.0;
        wiersz = 50;
        kol = 50;
        for (int i = 0; i < n; i++) {
            plansza.assign(wiersz, vector<bool>(kol, false)); //pusta plansza
            Losuj(generator); //wylosowanie poczatkowego stanu planszy
            int suma = 0;
            //Iterowanie do zadeklarowanej tury
            for (int j=0; j<tury; j++) {
                NastPlansza();
                iteruj();
            }
            //Policzenie żywych komórek na finalnej planszy
            for (int k = 0; k < wiersz; k++)
                for (int l = 0; l < kol; l++)
                    suma+=plansza[k][l]; //Zliczenie żywych komórek (True równoważne z 1)
            wynik+=suma; //Dodanie do ostatecznego wyniku
        }
        return wynik/(n*kol*wiersz);
    }

private:
    int kol;
    int wiersz;
    int predkosc;
    bool czyPauza;
    mt19937 gen;
    uniform_int_distribution<int> dist{0, 1};
    vector<vector<bool>> plansza;
    vector<vector<bool>> nastplansza;
    vector<vector<sf::RectangleShape>> widok;

    //Przekonwertowanie planszy booleańskiej do SFML i jej wydrukowanie
    void drukuj(vector<vector<bool>>& _plansza, sf::RenderWindow& window) {
        widok.assign(wiersz, vector<sf::RectangleShape>(kol)); //Stworzenie macierzy z kwadracikami
        for (int i = 0; i < wiersz; i++) {
            for (int j = 0; j < kol; j++) {
                //Własciwości kwadracika - czarna, cienka obwódka i wymiary 15x15
                widok[i][j] = sf::RectangleShape({15.f, 15.f});
                widok[i][j].setOutlineThickness(1);
                widok[i][j].setOutlineColor(sf::Color::Black);
                widok[i][j].setPosition({float(j*15), float(i*15)});
                //Wczytanie koloru w zależności od stanu na planszy booleańskiej
                widok[i][j].setFillColor(_plansza[i][j] ? sf::Color::Black
                                                        : sf::Color::White);
                window.draw(widok[i][j]); //Zapisanie kształtów w pamięci
            }
        }
        window.display(); //Wydrukowanie wszystkich kształtów z pamięci
    }

    //Ustawienie losowej planszy
    void Losuj() {
        for (int i = 0; i < wiersz; i++)
            for (int j = 0; j < kol; j++)
                plansza[i][j] = dist(gen);
    }

    //Ustawienie losowej planszy z ziarnem (do Monte Carlo)
    void Losuj(mt19937& generator) {
        for (int i = 0; i < wiersz; i++)
            for (int j = 0; j < kol; j++)
                plansza[i][j] = dist(generator);
        //plansza[i][j] = rand() % 2; //wylosowanie wartosci 0-1
    }

    //Sprawdzenie czy komórka przeżyje/umrze/narodzi się
    bool NastStan(vector<vector<bool>>& plansza, int _wiersz, int _kol) {
        int suma = 0; //Liczba żywych sąsiadów
        for (int poziom = _wiersz - 1; poziom <= _wiersz + 1; poziom++) {
            for (int pion = _kol - 1; pion <= _kol + 1; pion++) {
                //Pominięcie sprawdzanej komórki
                if (poziom == _wiersz && pion == _kol) continue;
                //Nałożenie modulo na indeksy utworzy toroidalną planszę
                //Dodanie do indeksu wymiaru rozwiąże problem z ujemnymi indeksami (lewa i górna krawędź)
                int i = (poziom + wiersz) % wiersz;
                int j = (pion  + kol) % kol;
                if (plansza[i][j]) suma++;
            }
        }

        //Dla żywych komórek
        if (plansza[_wiersz][_kol]) {
            if (suma == 2 || suma == 3) return true; //Jeśli żywa komórka ma 2 lub 3 sąsiadów to przeżywa
            return false; //Jeśli ma <2 lub >3 sąsiadów to umiera
        }
        //Dla martwych komórek
        if (suma==3) return true; //Jeśli martwa komórka ma 3 sąsiadów to się narodzi
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

//Wygenerowanie planszy 50 x50 z gliderem
vector<vector<bool>> glider() {
    vector<vector<bool>> plansza (50, vector<bool>(50, false));
    plansza[1][0] = true;
    plansza[0][2] = true;
    plansza[1][2] = true;
    plansza[2][1] = true;
    plansza[2][2] = true;
    return plansza;
}

//Wygenerowanie planszy 50x50 z pulsarem
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

    //gra.graj(); //Gra z pustą planszą

    vector<vector<bool>> plansza = glider(); //Wygenerowanie planszy z gliderem
    //vector<vector<bool>> plansza = pulsar(); //Wygenerowanie planszy z pulsarem
    gra.graj(plansza); //Gra z własną planszą

    /*
    //Monte Carlo
    vector<int> tury = {1, 10, 50, 100, 250, 500, 1000};
    vector<int> n = {1, 10, 50, 100, 250, 500, 1000};

    for (int i = 0; i < tury.size(); i++)
        for (int j = 0; j < n.size(); j++)
            cout << "tury = " << tury[i] << ", n = " << n[j] << ", udzial zywych: " <<
                100*gra.MonteCarlo(tury[i], n[j], 1234) << "%" << endl;
    */

    return 0;
}