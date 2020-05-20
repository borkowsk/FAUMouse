echo off

echo NAGRYWANIE URUCHOMIONE

call srecorder64.bat

echo NAGRYWANIE ZAKONCZONE
 
mousereg2004.exe mregconfig1.ini

echo PIERWSZA REJESTRACJA ZAKONCZONA

mousereg2004.exe mregconfig2.ini

echo DRUGA REJESTRACJA ZAKONCZONA

mkdir %1
copy output1.out %1\output_%1_1.out
copy output2.out %1\output_%1_2.out
copy nagranie.wav %1\nagranie_%1.wav

echo SKOPIOWANE DO KATALOGU %1

echo KONIEC BADANIA

pause

start %1