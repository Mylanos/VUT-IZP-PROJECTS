/**
 * \file proj3.h
 */

/**
 * \mainpage Zhluková analýza
 *
 * \section intro_sec Úvod
 * Zhluková analýza (tiež clusterová analýza, anglicky cluster analysis)
 * je viacrozmerná štatistická metóda, ktorá se používa ku klasifikaci 
 * objektov. Slúžia k triedeniu jednotiek do skupín (zhlukov) tak, že jednotky 
 * náležiace do rovnakej skupiny sú si podobnejšie ako objekty z ostatných skupín.
 * Každý objekt je identifikovaný celým číslom. Objekty sú uložené v textovom súbore.
 * \see http://is.muni.cz/th/172767/fi_b/5739129/web/web/clsrov.html
 *
 * \section compile_sec Preklad programu
 * gcc -std=c99 -Wall -Wextra -Werror -DNDEBUG proj3.c -o proj3 -lm
 * \subsection debug_arg -DNDEBUG
 * Definícia makra NDEBUG (argument -DNDEBUG) je z dôvodu anulovania efektu ladiacich informácií.
 * \subsection math_arg -lm
 * Prepojenie s matematickou knižnicou (argument -lm) je z dôvodu výpočtu vzdialenosti objektov.
 *
 * \section exec_syntax Spôsob spustenia programu
 * ./proj3 SOUBOR [N]
 *
 * SOUBOR je názov súbora so vstupnými dátami.
 *
 * N je voliteľný argument definujúcí cieľový počet zhlukov. N > 0.  Pri absencí argumentu je hodnota N = 1.
 *
 * \section objectfile_format Formátovanie vstupného súbora
 * Vstupné dáta sú uložené v textovom súbore. Prvý riadok súbora je vyhradený pre počet objektov v súbore a má formát:
 * count=N
 * kde číslo je počet objektov v súbore. Nasleduje na každom riadku definícia jednoho objektu. Počet riadkov súbora
 * zodpovedá minimálne počtu objektov + 1 (prvý riadok). Dalšie riadky súbora ignorujte. Riadok definujúci objekt je vo formáte:
 * OBJID X Y
 * kde OBJID je v rámci súboru jednoznačný celočíselný identifikátor, X a Y sú súradnice objektu a sú to celé čísla. 
 * Platí 0 <= X <= 1000, 0 <= Y <= 1000.
 *
 * \section file_info Informácie o súbore
 * \link
 * proj3.h
 * \endlink
 *
 * \author Marek Žiška <xziska03@stud.fit.vutbr.cz>
 * \date 14. December 2018
 * \version 1.0
 */

/**
 * \struct obj_t
 *
 * \brief Štruktúra objektu v priestore.
 *
 * Objekt je definovaný x-súradnicou a y-súradnicou objektu v priestore. Každý objekt
 * má identifikačné číslo.
 *
 * \param id 	identifikačné číslo objektu
 * \param x X súradnica objektu
 * \param y Y súradnica objektu
 */

struct obj_t {
    int id;
    float x;
    float y;
};

/**
 * \struct cluster_t
 *
 * \brief Štruktúra zhluku objektov.
 *
 * O každom zhluku evidujeme aktuálnu velkosť zhluku, to je aktuálny počet objektov v zhluku.
 * Ďalej evidujeme kapacitu objektu to je počet objektov pre ktoré je v zhluku alokovaný priestor v pamäti.
 * Tento priestor alokujeme pre pointer na štruktúru objektu, ktorý je tretím atribútom štruktúry.
 *
 * \param size 		Aktuálna veľkosť objektu (počet objektov v zhluku).
 * \param capacity 	Kapacita objektu (počet objektov, pre ktoré je rezervované miesto v pamäti).
 * \param obj		Ukazatel na pole objektov v zhluku.
 */
struct cluster_t {
    int size;			
    int capacity;		
    struct obj_t *obj;
};

/**
 * \addtogroup memory_work Práca s pamäťou
 * Alokácia a uvolnenie pamäte.
 * @{	
 */

/**
 * \brief Inicializuje zhluk 'c'. Alokuje priestor pre 'cap' (kapacitu) objektu.
 *
 * \param c 	Ukazatel na štruktúru zhluku.
 * \param cap 	Celé číslo, ktoré vyjadruje kapacitu zhluku, pre ktorú alokujeme pamäť.
 * 
 * \pre 	Kapacita objektu musí byť väčšia alebo rovná nule.
 *
 * \post 	Zhluk bude mať alokovanú  pamäť pre 'cap' kapacitu zhluku ak nenastala chyba pri alokácií.
 *			Ak sa nepodarí alokovať pamäť pre zhluk, kapacita bude 0.
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * \brief Inicializuje zhluk na prázdny zhluk. Odstráni všetky objekty zhluku.
 *
 * \param c 	Ukazatel na štruktúru zhluku, ktorá bude uvoľnovaná.
 *
 * \post 	Uvolní alokovanú pamäť pre všetky objekty zhluku.
 */
void clear_cluster(struct cluster_t *c);

/**
 * \brief Uvoľní pamäť pre zhluk 'c' a pre všetky (kapacita zhluku) objekty v zhluku 'c'.
 *
 * \param c		Ukazatel na štruktúru zhluku, ktorý uvoľnuje.
 * \param narr	Kapacita uvoľňovaného zhluku.
 *
 * \post Zhluk bude dealokovaný z pamäte.
 */
void freeMallocs(struct cluster_t *c, int narr);
/**
 * @}
 */

/**
 * \addtogroup array_work Práca s poľom zhlukov
 * @{	
 */
/// Čast objektov zhluku. Doporučená hodnota pre alokáciu.
extern const int CLUSTER_CHUNK;

/**
 * \brief  Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 *
 * \param c			Ukazatel na štruktúru zhluku, ktorý sa rozšíruje.
 * \param new_cap 	Nová kapacita zhluku, pre ktorú sa realokuje pamäť.
 * 
 * \pre 	Kapacita zhluku 'c' a nová kapacita bude väčšia alebo rovná nule.
 *
 * \post 	Kapacita zhluku 'c' bude zmenená na novú kapacitu. Pre túto kapacitu bude aj
 *			alokovaný priestor v pamäti (ak nenastala chyba).
 *
 * \return 	Zhluk s novou kapacitou.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * \brief 	Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 *			nevejde.
 *
 * \param c		Ukazatel na štruktúru zhluku, do ktorého pridávama objekt.
 * \param obj	Ukazatel na štruktúru objektu.
 * 
 * \post 	Na poslednú pozíciu zhluku 'c' pridá objekt 'obj' a teda zvýši aj
 *			size (veľkosť) zhluku.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * \brief 	Spája 2 clustery, teda pridá objekty zo zhluku 'c2' do 'c1'.
 *
 * 			Do zhluku 'c1' pridá objekty 'c2'. Ak to je potreba zhluk 'c1' bude rozšírený .
 *			Objekty zhluku 'c1' budú zoradené vzostupne podľa identifikačného čísla.
 *
 * \param c1 		Ukazatel na štruktúru prvého zhluku.
 * \param c2 		Ukazatel na štruktúru druhého zhluku.
 * 
 * \post 	Na poslednú pozíciu zhluku 'c' pridá objekt 'obj' a teda zvýši 
 *			size (veľkosť) zhluku.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * \brief 	Odstráni jeden zhluk zo všetkých zhlukov na indeze 'idx'.
 *
 *			Odstráni zhluk z poľa zhlukov 'carr' o veľkosti 'narr'.
 *			Zhluk ktorý odstraňujeme sa nachádza na indexe 'idx'.
 *
 * \param carr		Ukazateľ na štruktúru zhluku.
 * \param narr 		Počet zhlukov v poli.
 * \param idx		Index na ktorom sa nachádza zhluk na odstránenie.
 * 
 * \post 	Z poľa zhlukov 'carr' bol odstránení zhluk na indexe 'idx' a pole
 *			bude o jedna menšie.
 *
 * \return Vracia nový počet zhlukov v poli.
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * \brief 	Počíta Euklidovskú vzdialenosť medzi dvoma objektmi.
 *
 * \param o1	Ukazateľ na štruktúru prvého objektu.
 * \param o2 	Ukazateľ na štruktúru druhého objektu.
 *
 * \return Vzdialenosť dvoch objektov.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * \brief Počíta vzdialenosť dvoch zhlukov.
 *
 * \pre Počet objektov v zhluku 'c1' bude väčší než 0.
 *
 * \pre Počet objektov v zhluku 'c2' bude väčší než 0.
 *
 * \param c1 	Ukazatel na štruktúru prvého zhluku.
 * \param c2 	Ukazatel na štruktúru druhého zhluku.
 *
 * \return Vzdialenosť zhluku 'c1' a 'c2'.
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * \brief Nájde dva najbližšie zhluky.
 *
 * \post Indexy dvou najbližších zhlukov budú uložené v 'c1' a 'c2'.
 *
 * Identifikuje najbližšie zhluky a indexy nájdených zhlukov v poli 'carr' ukladá do 'c1' a 'c2'.
 *
 * \param carr		Ukazateľ na štruktúru zhluku, v ktorom hľadáme susedov.
 * \param narr 		Počet všetkých zhlukov.
 * \param c1		Index jedného z nájdených zhlukov
 * \param c2 		Index druhého z nájdených zhlukov
 *
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * \brief Zoradenie objektu v zhluku vzostupne podľa ich ID.
 *
 * \post Objekty v zhluku 'c' budú zoradené vzostupne podľa ID.
 *
 * \param c Zhluk určený pre zoradenie objektov.
 */
void sort_cluster(struct cluster_t *c);

/**
 * \brief Tlačí zhluk 'c', teda všetky objekty v zhluku.
 *
 * \post Objekty zhluku 'c' budú vypísané na štandardný výstup.
 *
 * \param c Ukazateľ na zhluk, ktorý sa bude tlačiť.
 */
void print_cluster(struct cluster_t *c);

/**
 * \brief Načítanie objektov zo súbora.
 * 
 * 	Zo súboru 'filename' načítava objekty. Pre každý objekt vytvorí zhluk a uloží objekt do zhluku 
	a alokuje pre nich priestor.
 *
 * \pre V zložke programu musí existovať súbor 'filename', ktorý bude v správnom formáte.
 *
 * \post Každý objekt bude uložený v zhluku.
 *
 * \param filename	 Názov súbora pre načítanie objektov
 * \param arr 		 Ukazateľ na štruktúru zhlukov načítaných zo súbora.
 *
 * \return  Počet načítaných zhlukov. V prípade chyby s alokovaním pamäte vracia -2 a keď 
 * 			nastane chyba s načítaním súbora -1.
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * \brief Tlačí pole zhlukov na štandardný výstup.
 *
 * \post Všetky objekty zhluku v poli zhlukov 'carr' budú vypísané na štandardný výstup.
 *
 * \param carr Pole zhlukov určené na tlačenie.
 * \param narr Počet zhlukov v poli.
 */
void print_clusters(struct cluster_t *carr, int narr);
/**
 * @}
 */
