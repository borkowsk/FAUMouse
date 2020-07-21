**FAUMouse3 jest pakietem do rejestracji ruchu myszy na ekranie za pośrednictwem WWW.**

Narzędzie składa się z części serwerowej i części przeglądarkowej:
- Po stronie serwera działają dwa programy napisane w C++ - *wwwserver* i *mousereg3* 
- Po stronie klienta działa program w jezyku JavaScript z nakładką w postaci biblioteki *P5.min.js* z Processingu

Do uruchomienia niezbędne są następujące pakiety:
- GIT (opcjonalnie)
- make (raczej jest preinstalowane na każdym linuxie)
- g++ (raczej jest preinstalowane na każdym linuxie)
- cmake
- biblioteka boost
- biblioteka magic (wymagana przez `fasada-core`, choć nie używana w tym projekcie)
- Processing.org z trybem P5 do edycji i testowania zmian w skrypcie zbierającym dane (opcjonalnie)

*Na UBUNTU instalacja koniecznych pakietów wygląda następująco:*

sudo apt-get install git

sudo apt-get install cmake

sudo apt-get install libboost-all-dev

sudo apt-get install libmagic-dev

*i ewentualnie make i g++:*

sudo apt-get update

sudo apt-get install build-essential

sudo apt-get install g++

*Katalog instalacyjny zawiera:*

- instalacja_pl.md - _ten plik_
- inslalling_en.md - _angielska wersja tego pliku_
- install.sh - _skrypt bash służący do kompilacji i przygotowania aplikacji do działania_
- start.sh - _skrypt bash uruchamiający skompilowaną aplikację i serwer www_ 
- stop.sh - _skrypt służący do zamknięcia serwisu, tworzony za każdym razem od nowa przez skrypt_ start.sh
- IPs.txt - _wynik sprawdzenie środowiska sieciowego_ (za pomocą_ *ifconfig* )
- screen.ini - _używana w skryptach konfiguracja kolorów na terminalu_
- service.log - _wyniki działania aplikacji mousereg3 z poprzedniego lub trwającego uruchomienia_
- src/regmouse3_main.cpp - _kod źródłowy aplikacji serwisu_
- src/regmouse3 - _aplikacja serwisu, jeśli kompilacja przebiegła poprawnie_
- private/ - _katalog na zebrane dane_
- public/ - _katalog którego zawartość jest widoczna poprzez serwer WWW_ . Kluczowy jest plik *index.html*, ktory należy edytować wg. potrzeb. Na wszelki wypadek istnieje jego kopia o nazwie *index_demo.html*
- public/MouseRegJs/ - _katalog z rejestratorem i biblioteką P5_. Może zawierać też inne pliki wygenerowane przez Processing
- public/MouseRegJs/MouseRegJs.js - _rejestrator działający w przeglądarce_.
- fasada_core/ - _importowany katalog z kodem źródłowym serwera WWW i bibliotekami komunikacji między serwerem, a korzystającymi z niego serwisami_

Sposób działania:

1) Uruchomić:

./install.sh

Jeśli dojdzie do końca to wyświetli adresy IP, które mogą zostać użyte

2) Uruchomić 

./start.sh

Uruchomi się serwis w wersji lokalnej. Powinno pojawić się okno przeglądarki ze stroną z katalogu ./public/ , która zawiera link do aplikacji rejestrującej. Jeśli rejestracja zadziała to w katalogu ./private/ powinien pojawić się jakiś plik `mouse*.raw` 

3) Uruchomić 

./stop.sh

Serwis lokalny zostanie zamknięty, a w oknie przeglądarki pojawi się odpowiednia informacja na ten temat

4) Uruchomić serwis z parametrami: IP serwisu i PORT. Trzeba wybrać odpowiednie IP!!! Numer portu w zasadzie dowolny, byle czterocyfrowy. Poniżej przykład wywołania z IP mojego laptopa W SIECI LOKALNEJ.

./start.sh  192.168.1.104 8888

Jeśli wybrane IP będzie dostępne globalnie lub mapowane to serwis powinien działać przynajmniej na przeglądarce Firefox.

**Aktualnie przetestowano poprawne działanie serwisu na platformie UBUNTU 16.04 i 18.04 i klienta w sieci lokalnej na przeglądarce Firefox w systemie UBUNTU oraz klienta na Windows. Wykryto niepoprawne działanie na przeglądarce Chrome, która wydaje się nie przesyłać danych, a jedynie ich nagłówek**

