#include <iostream>
#include <vector>
#include "windows.h"
#include <random>
#include <ctime>
using namespace std;

class GameOfLife {
    public:

    //gra z losowo ustawiona planszą o podanych wymiarach
    void graj(int _kol, int _wiersz, int _iter) {
        kol = _kol;
        wiersz = _wiersz;
        iter = _iter;
        UstawPlansze(kol, wiersz);

        for (int i = 0; i < iter; i++) {
            drukuj();
            NastPlansza();
            iteruj();
            Sleep(1000);
        }
    };

    //gra z okreslona przez użytkownika planszą
    void graj_z_plansza(vector<vector<bool>>& _plansza, int _iter) {
        plansza = _plansza;
        kol = plansza.size();
        wiersz = plansza[0].size();
        iter = _iter;

        for (int i = 0; i < iter; i++) {
            drukuj();
            NastPlansza();
            iteruj();
            Sleep(500);
        }
    };

    private:
    int kol;
    int wiersz;
    int iter;
    vector<vector<bool>> plansza;
    vector<vector<bool>> nastplansza;

    //Ustaw początkową planszę o losowym układzie
    void UstawPlansze(int kol, int wiersz) {

        plansza.assign(wiersz, vector<bool>(kol, false));

        srand(time(0));
        for (int i = 0; i < wiersz; i++) {
            for (int j = 0; j < kol; j++) {
                plansza[i][j] = rand() % 2;
            }
        }
    };

    //Wydrukuj planszę
    void drukuj() {
        for (int i = 0; i < wiersz; i++) {
            cout << "|";
            for (int j = 0; j < kol; j++) {
                cout << (plansza[i][j] ? "#" : " ");
            }
            cout << "|" << endl;
        }
        for (int i = 0; i <= kol; i++) cout << "-";
        cout << endl;
    };

    //Sprawdź czy komórka przeżyje/umrze/narodzi się
    bool NastStan(vector<vector<bool>>& plansza, int _wiersz, int _kol) {
        int suma = 0;
        for (int poziom = _wiersz-1; poziom <=_wiersz+1; poziom++) {

            if(poziom<0 || poziom>=wiersz) continue;

            for (int pion = _kol-1; pion<=_kol+1; pion++) {
                if(pion<0 || pion>=kol || (poziom==_wiersz && pion==_kol)) continue;
                if(plansza[poziom][pion]) suma++;
            }
        }
        if (plansza[_wiersz][_kol]) {
            if (suma == 2 || suma == 3) return true;
            return false;
        }
        if (suma==3) return true;
        return false;
    };

    //załaduj następną turę
    void NastPlansza() {
        nastplansza.assign(wiersz, vector<bool>(kol, false));
        for (int i = 0; i < wiersz; i++) {
            for (int j = 0; j < kol; j++) {
                nastplansza[i][j] = NastStan(plansza, i, j);
            }
        }
    };

    //przejdź do następnej tury
    void iteruj() {
        for (int i = 0; i < wiersz; i++) {
            for (int j = 0; j < kol; j++) {
                plansza[i][j]=nastplansza[i][j];
                nastplansza[i][j]=false;
            }
        }
    };
};

int main() {

    GameOfLife gra;
    //gra.graj(40, 10, 10);


    vector<vector<bool>> plansza (30, vector<bool>(10, false));
    // Glider
    plansza[1][0] = true;
    plansza[0][2] = true;
    plansza[1][2] = true;
    plansza[2][1] = true;
    plansza[2][2] = true;

    gra.graj_z_plansza(plansza, 50);

    return 0;
}