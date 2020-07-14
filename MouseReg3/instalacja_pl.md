**FAUMouse3 jest pakietem do rejestracji ruchu myszy na ekranie za pośrednictwem WWW.**

Narzędzie składa się z części serwerowej i części przeglądarkowej:
- Po stronie serwera działają dwa programy napisane w C++ - wwwserver i mousereg3 
- Po stronie klienta działa program w jezyku JavaScript z nakładką w postaci biblioteki P5.min.js z Processingu

Do uruchomienia niezbędne są następujące pakiety:
- GIT (opcjonalnie)
- make (raczej jest na każdym linuxie)
- g++ (raczej jest na każdym linuxie)
- cmake
- biblioteka boost
- Processing do edycji i testowania zmian w skrypcie zbierającym dane (opcjonalnie)

*Na UBUNTU ich instalacja wygląda następująco:*

sudo apt-get install git
sudo apt-get install cmake
sudo apt-get install libboost-all-dev

*i ewentualnie make i g++*

sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install g++

Katalog zawiera:

- instalacja_pl.md - ten plik
- inslalation_en.md - angielska wersja tego pliku
- install.sh - skrypt bash służący do kompilacji i przygotowania aplikacji do działania
- start.sh - skrypt bash uruchamiający skompilowaną aplikację i serwer www 
- stop.sh - skrypt służący do zamknięcia serwisu
- IPs.txt - wynik sprawdzenie środowiska sieciowego (za pomocą *ifconfig*)
- screen.ini - używana w skryptach konfiguracja kolorów na terminalu
- service.log - wyniki działania aplikacji mousereg3 z poprzedniego lub trwającego uruchomienia
- src/regmouse3_main.cpp - kod źródłowy aplikacji serwisu
- src/regmouse3 - aplikacja serwisu, jeśli kompilacja przebiegła poprawnie
- private/ - katalog na zebrane dane
- public/ - katalog którego zawartość jest widoczna poprzez serwer WWW. Kluczowy jest plik index.html, ktory należy edytować wg. potrzeb. Na wszelki wypadek istnieje jego kopia o nazwie index_demo.html
- public/MouseRegJs/ - katalog z rejestratorem i biblioteką P5 . Może zawierać też inne pliki wygenerowane przez Processing
- public/MouseRegJs/MouseRegJs.js - rejestrator działający w przeglądarce.

Sposób działania:

1) Uruchomić:

./install.sh

Jeśli dojdzie do końca to wyświetli adresy IP które mogą zostać użyte

2) Uruchomić 

./start.sh

Uruhomi się serwis w wersji lokalnej. Powinno pojawić się okno przeglądarki ze stroną z katalogu ./public/ , która zawiera link do aplikacji rejestrującej. Jeśli rejestracja zadziała to w katalogu ./private/ powinien pojawić się plik mouse*.raw 

3) Uruchomić 

./stop.sh

Serwis lokalny zostanie zamknięty, a w oknie przeglądarki pojawi się informacja o jego niedostępności

4) Uruchomić serwis z parametrami: IP serwisu i PORT. Trzeba wybrać odpowiednie IP!!! Numer portu w zasadzie dowolny, byle czterocyfrowy. Poniżej przykład wywołania z IP mojego laptopa W SIECI LOKALNEJ.

./start.sh  192.168.1.104 8888

Jesli dane IP bedzie dostępne globalnie serwis powinien działać przynajmniej na przeglądarce Firefox.

**Aktualnie przetestowano poprawne działanie serwisu na platformie UBUNTU 18.04 i klienta w sieci lokalnej na przeglądarce Firefox w systemie UBUNTU 18.04 i klienta na Windows. Wykryto niepoprawne działanie na przeglądarce Chrome, która wydaje się nie przesyłać danych, a jedynie ich nagłówek**

