rem Plik wsadowy produkujacy hurtowo wszystkie pliki *.dat  z *.out
rem Jako parametry mozna dawac opcje obrobki, ale nie nazwe pliku
rem   
rem     all_out   -ex3  -sk4
rem
for %%i in (*.out) do analyser.exe %%i %1 %2 %3 %4 %5 %6 %7 %8 %9