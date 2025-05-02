# <p align="center">Gra w życie Conwaya</p>

## Opis
Aparat komórkowy opracowany przez Johna Conwaya w 1970 roku. Gra toczy się na dwuwymiarowej planszy, na której każda z komórek może przyjąć jeden z dwóch stanów - może być żywa lub martwa. 
Warunki zmiany stanu komórki są uzależnione od liczby jej sąsiadów.

Jeśli w obecnej turze komórka jest żywa to:
- Posiadając mniej niż dwóch sąsiadów umiera z samotności,
- Posiadając dwóch lub trzech sąsiadów przeżywa,
- Posiadając więcej niż trzech sąsiadów umiera z przeludnienia.

Jeśli w obecnej turze komórka jest martwa to:
- Posiadając dokładnie trzech sąsiadów rodzi się,
- W przeciwnym przypadku pozostaje martwa.

## Implementacja
Gra została zbudowana w języku C++ z wykorzystaniem biblioteki SFML 3.0. Plansza zbudowana jest z 250 kwadracików o wymiarach 15x15px, o wymiarach 50x50 komórek, czyli 750x750px. Gra jest zawarta w klasie GameOfLife - wywołując metodę graj(), użytkownikowi zostaje zwrócone okienko z pustą planszą. Użytkownik może nanieść swoje zmiany na planszę poprzez kliknięcie lewym przyciskiem myszy, bądź poprzez wylosowanie układu planszy. Możliwe jest także załadowanie gry z własną planszą (vector<vector<bool>>) 50x50 poprzez wywołanie metody graj(plansza). Dodatkowo, utworzono funkcje glider() i pulsar(), które zwracają planszę 50x50 z naniesionymi charakterystycznymi kształtami:

### Glider

![Game_of_life_animated_glider](https://github.com/user-attachments/assets/f484b341-a02b-4339-942f-678f792ca9ff)  

###### źródło: https://en.m.wikipedia.org/wiki/File:Game_of_life_animated_glider.gif

### Pulsar

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
