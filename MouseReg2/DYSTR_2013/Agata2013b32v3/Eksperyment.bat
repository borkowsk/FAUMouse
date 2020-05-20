@echo off

echo Eksperyment %1 %2 %3 %4

echo NAGRYWANIE URUCHOMIONE

REM Niestety srecorder zwraca sterowanie zaraz po starcie 
REM start /WAIT srecorder32.exe Niestety /WAIT i tak nie dzia³a!
srecorder32.exe

rem To wyœwietli okienko "na przeczekanie"
bin\RtfView.exe Nagranie.ini

rem Je¿eli nie u¿ywamy instrukcji nagrywania to:
rem trzeba poczekaæ ¿eby siê nagra³o - odpowiednio d³ugi czas (?)
rem timeout /T 30 /NOBREAK  
 
bin\RegMouse2013.exe mregconfig1.ini

echo PIERWSZA REJESTRACJA ZAKONCZONA

bin\RegMouse2013.exe mregconfig2.ini

echo DRUGA REJESTRACJA ZAKONCZONA

echo PRZENOSZENIE DO PODKATALOGU

@echo on
mkdir %1
move Metryczka%1.* %1\
move output1.out %1\output_%1_1.out
move output2.out %1\output_%1_2.out
move nagranie.wav %1\nagranie_%1.wav

@echo off

echo WYNIKI PRZENIESIONO DO KATALOGU %1

bin\RtfView.exe Dzieki.ini

echo KONIEC

rem alternatywny sposób czekania
rem CHOICE /C:AB /D:A /T:30 > NUL - takoe sobie