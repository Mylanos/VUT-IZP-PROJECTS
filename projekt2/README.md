Projekt 2 - Iterační výpočty

Implementujte výpočet přirozeného logaritmu a exponenciální funkce s obecným základem pouze pomocí matematických operací +,-,*,/.

SYNTAX SPUSTENIA

Program se spouští v následující podobě:
    ./proj2 --log X N
    
    alebo
    
    ./proj2 --pow X Y N
    
Argumenty programu:
    --log X N požadavek pro výpočet přirozeného logaritmu z čísla X v N iteracích (Taylorova polynomu a zřetězeného zlomku).

    --pow X Y N požadavek pro výpočet exponenciální funkce z čísla Y s obecným základem X v N iteracích (Taylorova polynomu a zřetězeného zlomku)
    
    
1. podúkol - Implementace Taylorova polynomu
    
    https://gyazo.com/aa58e694fb4f7150af3bdea6fa5d4874
    
    https://gyazo.com/b6ab7ab33bd53b38e64583dae2c39dd0
    
Výstup programu:

V případě výpočtu logaritmu (argument --log) program tiskne následující řádky:
                      log(X) = LOG_X
               cfrac_log(X) = CFRAC_LOG_X
              taylor_log(X) = TAYLOR_LOG_X
      
V případě výpočtu exponenciální funkce (argument --pow) program tiskne následující řádky:
                 pow(X,Y) = POW
          taylor_pow(X,Y) = TAYLOR_POW
        taylorcf_pow(X,Y) = TAYLORCF_POW
        
        
Příklady vstupů a výstupů

      $ ./proj2 --log 0.25 1
               log(0.25) = -1.38629436112
         cfrac_log(0.25) = -1.2
        taylor_log(0.25) = -0.75
        
      $ ./proj2 --pow 1.23 4.2 5
                pow(1.23,4.2) = 2.38562110403
         taylor_pow(1.23,4.2) = 2.38071292586
       taylorcf_pow(1.23,4.2) = 2.38079698166 
      
      $ ./proj2 --pow 0.25 2.5 1
                 pow(0.25,2.5) = 0.03125
          taylor_pow(0.25,2.5) = 1
        taylorcf_pow(0.25,2.5) = 1
        
      $ ./proj2 --log 1.131401114526 4
                 log(1.1314) = 0.123456789012
           cfrac_log(1.1314) = 0.123456789012
          taylor_log(1.1314) = 0.123452108537  
        
