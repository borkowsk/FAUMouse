Czesc!
Przysylam przerobiony program analizatora myszy (v 3.2a)
1) Odnalazlem dwa bledy logiczne ktore usunalem:

-analiza z podzialem na grupy gubila punkty o maksymalnej odleglosci. Jesli mysz prawie stala to mogly byc prawie wszystkie punkty, stad mieliscie po dwa rekordy.
Teraz te punkty dolaczane sa do dalszej grupy.

-faktycznie prawie nigdy nie ma predkosci 0. Ze wzgledu na niedokladnosci FLOATING POINT
zazwyczaj jest to liczba rzedu 1e-6, 1e-7; W plikach wynikowych zera sa tylko dlatego ze jest
zalozona dokladnosc 3 miejsc po przecinku.
Licze teraz epsilon (=0.0001 domyslnie) i uznaje kursor za stojacy jesli:  -eps<V<eps

2) Dla calosci i dla grup czasowych operacje odbywaja sie niezaleznie wg tego samego algorytmu. Dla kontroli program wypluwa teraz na ekran numery rekordow dla poszczegolnych
grup.

3) Dodalem odczytywanie srodka ekranu wg pierwszego rekordu formatu "Warszawskiego"
jesli uzytkownik nie podal jawnie. Wypisuje to tez na ekranie zeby zwrocic uwage ze to
tylko domysl programu i moze nie byc do konca poprawny.

4) Wprowadzilem wczytywanie formatu amerykanskiego (wywoluje sie opcja  -fau ), ale...
Jak juz pisalem jest klopot z czasem. U mnie czas jest w sekundach. Byc moze czasem niecalkiem dokladnie bo uzywam zegara Windows i moga byc niewielkie przesuniecia.
Stad moge z odpowiedzialnoscia podawac V i A w pixelach/s i pixelach/s^2 odpowiednio.
Pozwala to porownywac dane rejestrowane z rozna rozdzielczoscia czasowa.

Wyglada ze program amerykanski po prostu numeruje rekordy.
Mozna przyjac ze zapis odbywa sie co 1s, ale sam nic nie licze tylko przepisuje 
to co zostalo zapisane w odpowiednich kolumnach wiec tylko kwestia czy 
podana jednostka jest prawidlowa czy nie i czy mozna porownywac dane
z mojego rejestratora z danymi z amarykanskiego.

Natomiast co do danych Krejtz'a to nie wiem czy sa one bardziej nieporownywalne niz
pozostale. Skoro i tak odleglosci maksymalne sa rozne to rozmiar ekranu gra role
drugorzedna. 

Klopot tylko z jednostkami predkosci (podobny jak dla danych amerykanskich).
Mozna by tez za jednostke predkosci przyjac 1% rozmiaru ekranu na sekunde
albo 1% maksymalnej odleglosci na sekunde i jeszcze bardziej to ujednolicic.

Milego testowania wersji alfa :) . Sprawdzilem to co mi wpadlo do glowy,
ale na pewno macie w tym wieksza wprawe.
        Pozdrowienia
        Wojtek
