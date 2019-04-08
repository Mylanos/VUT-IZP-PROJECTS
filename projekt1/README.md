Projekt 1 - Práce s textem

INFO

Cílem projektu je na praktickém problému naučit studenty vytvářet jednoduché programy, dekomponovat problém na podproblémy a                  správně zapisovat zdrojové soubory. Praktickým problémem je implementace textového editoru. Editor nebude interaktivní, tj. nebude komunikovat s uživatelem, ale příkazy pro úpravu textu bude načítat ze vstupního souboru. Editor bude řádkový (úpravy bude provádět po jednotlivých řádcích). Editor bude proudový - editace textu bude provedena čtením upravovaného vstupu po řádcích a zápisu výsledného obsahu souboru po řádcích na standardní výstup.

  Program implementujte ve zdrojovém souboru proj1.c. Vstupní binární textová data budou čtena ze standardního vstupu (stdin), výstup bude tisknut na standardní výstup (stdout). Chování programu lze upřesnit jeho argumenty (viz spuštění programu).
  
  PREKLAD
  
  $ gcc -std=c99 -Wall -Wextra -Werror proj1.c -o proj1
  
  SYNTAX SPUSTENIA
  
  ./proj1 JMENO_SOUBORU_S_PRIKAZY
  
  PRIKAZY 
  
iCONTENT - (insert) vloží před aktuální řádek řádek s obsahem CONTENT.
bCONTENT - (before) vloží na začátek aktuálního řádku obsah CONTENT.
aCONTENT - (after/append) vloží na konec aktuálního řádku obsah CONTENT.
d - (delete) smaže aktuální řádek.
dN - smaže N řádků (N > 0).
r - (remove EOL) na aktuálním řádku odstraní znak konce řádku.
s PATTERN REPLACEMENT - (substitute) na aktuálním řádku provede substituci (nahrazení) výskytu řetězce PATTERN za řetězec REPLACEMENT. PATTERN musí být neprázdný. Pokud aktální řádek neobsahuje podřetězec PATTERN, příkaz nic nedělá.
s:PATTERN:REPLACEMENT - substituce řetězce, přičemž PATTERN a REPLACEMENT mohou být od sebe oddělené libovolným tisknutelným znakem, zde vyjádřeno znakem :. PATTERN v takovém případě nesmí obsahovat daný oddělovač.
S:PATTERN:REPLACEMENT - stejné jako s:PATTERN:REPLACEMENT, ale nahrazuje všechny výskyty PATTERN.
n - (next) posun aktuálního řádku o jeden níž, tj. tiskne aktuální řádek.
nN - posun aktuálního řádku o N níž. (N > 0).
q - (quit) ukončí editaci, další řádky nezpracovává a ani netiskne. Pokud byl aktuální řádek nějakým způsobem modifikován, řádek bude vytištěn.
gX - (goto) přeskoč s aktuálním příkazem pro editaci na X-tý příkaz. X vyjadřuje číslo řádku (příkazu) v příkazovém souboru (1 <= X <= počtu řádků příkazového souboru). Pozor, tento příkaz může způsobit, že se i správně implementovaný editor zacyklí.
e - (append EOL) přidej na konec aktuálního řádku znak konce řádku.

PRIKLAD VYSTUPOV

1)
$ cat mkcsv.txt
a,
r
n
n
g1
 
$ cat vstup
Jmeno pacienta
rok narozeni
Antonin Jedlicka
1963
Alzbeta Jedla
1965
 
$ ./proj1 mkcsv.txt <vstup
Jmeno pacienta,rok narozeni
Antonin Jedlicka,1963
Alzbeta Jedla,1965

2)

$ cat tabular.txt
b| 
a |
n
g1
 
$ cat vstup
Toto je
nejaky soubor
 
$ ./proj1 tabular.txt <vstup
| Toto je |
| nejaky soubor |

