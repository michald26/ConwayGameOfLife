# <p align="center">Gra w życie Conwaya</p>

## Opis
Automat komórkowy opracowany przez Johna Conwaya w 1970 roku. Gra toczy się na dwuwymiarowej planszy, na której każda z komórek może przyjąć jeden z dwóch stanów - może być żywa lub martwa. 
Warunki zmiany stanu komórki są uzależnione od liczby jej sąsiadów.

Jeśli w obecnej turze komórka jest żywa to:
- Posiadając mniej niż dwóch sąsiadów umiera z samotności,
- Posiadając dwóch lub trzech sąsiadów przeżywa,
- Posiadając więcej niż trzech sąsiadów umiera z przeludnienia.

Jeśli w obecnej turze komórka jest martwa to:
- Posiadając dokładnie trzech sąsiadów rodzi się,
- W przeciwnym przypadku pozostaje martwa.

## Implementacja
Gra została zbudowana w języku C++ z wykorzystaniem biblioteki SFML 3.0. Plansza zbudowana jest z 2500 komórek o wymiarach 15x15px, o wymiarach 50x50 komórek, czyli 750x750px. Wartym uwagi jest to, że plansza jest toroidalna, tj. jest zawinięta w obu wymiarach.
Gra jest zawarta w klasie GameOfLife - wywołując metodę graj(), użytkownikowi zostaje zwrócone okienko z pustą planszą. Użytkownik może nanieść swoje zmiany na planszę poprzez kliknięcie lewym przyciskiem myszy, bądź poprzez wylosowanie układu planszy. Możliwe jest także załadowanie gry z własną planszą (vector<vector<bool>>) 50x50 poprzez wywołanie metody graj(plansza). Dodatkowo, utworzono funkcje glider() i pulsar(), które zwracają planszę 50x50 z naniesionymi charakterystycznymi kształtami:

### Glider (statek)

![Game_of_life_animated_glider](https://github.com/user-attachments/assets/f484b341-a02b-4339-942f-678f792ca9ff)  

###### źródło: https://en.m.wikipedia.org/wiki/File:Game_of_life_animated_glider.gif

### Pulsar (oscylator)

![Game_of_life_pulsar](https://github.com/user-attachments/assets/1b7c0103-95c5-4bff-b647-b8306d3b58d8)

###### źródło: https://en.m.wikipedia.org/wiki/File:Game_of_life_pulsar.gif

## Sterowanie 
Gra ma charakter "zeroosobowy", jednak wprowadzono kilka klawiszy wpływających na rozgrywkę:
- Klawisz P pauzuje lub wznawia rozgrywkę (gra przy uruchomieniu domyślnie jest zapauzowana),
- Kliknięcie lewym przyciskiem myszy na kwadracik zmienia jego stan (możliwe wyłącznie w trybie pauzy),
- Klawisz L generuje losową planszę (możliwe wyłącznie w trybie pauzy),
- Klawisz R czyści obecną planszę (możliwe wyłącznie w trybie pauzy),
- Klawisz Esc wychodzi z gry,
- Strzałka w górę zwiększa tempo rozgrywki o 50ms (domyślnie gra ma tempo 500ms, maksymalne tempo to 100ms),
- Strzałka w dół zmniejsza tempo rozgrywki o 50ms.

## Symulacja Monte Carlo

Do klasy GameOfLife dodano metodę MonteCarlo, która przyjmuje argumenty n (liczba symulacji), tury (liczba tur w każdej z symulacji) i seed (ziarno funkcji pseudolosowej). Przy każdej nowej symulacji, generowana jest plansza, w której każdej z komórek jest przypisywany na podstawie ziarna jeden z dwóch równie prawdobodobnych stanów. 
Funkcja zwraca wartość double, która jest średnim odsetkiem żyjących komórek na planszy - wynik jest jedynie statystycznym szacunkiem, którego dokładność się zwiększa wraz z liczbą przeprowadzonych symulacji i może się różnić w zależności od wybranego ziarna. Poniżej przedstawiono wyniki przykładowych symulacji dla ziarna 1234gi:

### Średni udział żywych komórek na planszy po n symulacjach gier z określoną liczbą tur (seed = 1234):

|          | n = 1  | n = 10  | n = 50   | n = 100  | n = 250  | n = 500  | n = 1000 |
|----------|--------|---------|----------|----------|----------|----------|----------|
| 1 tura   | 26.76% | 27.94%  | 27.2024% | 27.2972% | 27.4266% | 27.3488% | 27.3631% |
| 10 tur   | 22.76% | 20.6%   | 19.7904% | 19.908%  | 20.0915% | 20.0172% | 19.9729% |
| 50 tur   | 12.64% | 12.428% | 12.2592% | 12.3176% | 12.18%   | 12.1143% | 12.039%  |
| 100 tur  | 9.8%   | 10.12%  | 9.3856%  | 9.3956%  | 9.44928% | 9.47464% | 9.46764% |
| 250 tur  | 8.4%   | 7.616%  | 7.2264%  | 6.9824%  | 6.8936%  | 6.77624% | 6.90988% |
| 500 tur  | 8.76%  | 5.672%  | 5.7824%  | 5.6236%  | 5.196%   | 5.2292%  | 5.3114%  |
| 1000 tur | 5.8%   | 3.512%  | 4.2352%  | 4.0696%  | 3.9568%  | 3.91344% | 3.95344% |

Wyniki symulacji wskazują, że wraz z rosnącą ilością tur, udział żywych komórek spada - spowodowane jest to stabilizacją układów. W dalszych etapach, niestabilne układy zanikają na rzecz stabilnych układy takich jak oscylatory czy struktury niezmienne (still lifes). 