/*
 * Soubor: proj3.c
 * Projekt c. 2 - Jednoduchá shluková analýza
 * Autor: Marek Ziska, xziska03@stud.fit.vutbr.cz
 * Skupina: 1BIB
 * Datum 9.12.2018
 *
 * Popis programu: Zhluková analýza (tiež clusterová analýza, anglicky cluster analysis)
 *                 je viacrozmerná štatistická metóda, ktorá se používa ku klasifikaci 
 *                 objektov. Slúžia k triedeniu jednotiek do skupín (zhlukov) tak, že jednotky 
 *                 náležiace do rovnakej skupiny sú si podobnejšie ako objekty z ostatných skupín.
 *                
 *                 
 * Obsah programu: Program obsahuje všetky povinné funkcie
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shl
 uku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // TODO

    if((c->obj = (struct obj_t *) malloc(sizeof(struct obj_t) * cap)) == NULL){
    	fprintf(stderr, "Nastala chyba v objekte: Objekt sa nepodarilo alokovat!\n");
        c->capacity = 0;
        return;		
    }
	c->size = 0;
    c->capacity = cap;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO
	if(c->capacity > 0){
        free(c->obj);
        
    }
	c->size = 0;
	c->capacity = 0;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO
    if(c->size < c->capacity){
    	*c->obj = obj;
    	c->size++;
    }

}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO

  	int new_capacity;
  	if(c1->size == c1->capacity){
    	new_capacity = c1->capacity + c2->capacity;
    	*c1 = *resize_cluster(c1, new_capacity);
    	if(c2->size > 1){
    		for(int j = 0; j < c2->size; j++){
    			c1->obj[c1->size]= c2->obj[j];   
    			c1->size++;
    		}
    	}
    	else{
    		c1->obj[c1->size]= *c2->obj;   
    		c1->size++;
    	}
    
    }
    sort_cluster(c1); 
}

/**********************************************************************/
/* Prace s polem shluku */
void print_clusters(struct cluster_t *carr, int narr);
/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO  
    if(narr > 0 && idx < narr && idx > -1){
        for(int i = idx; i < narr; i++){
            if((i + 1) == narr){
            
            }
            else{
                carr[i] = carr[i + 1]; 
            }
        }
        return narr - 1;
    }
    else {
        return -1;
    }
 }

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO

    //TATO FUNKCIA JE ISTO FUNKCNA

    float x, y, result;
    if(o1->x <= o2->x){
    	if(o1->y <= o1->y){
    		x = o2->x - o1->x;
    		y = o2->y - o1->y;
    		result = (x * x) + (y * y);
    	}
    	else if(o1->y > o2->y){
    		x = o1->x - o2->x;
    		y = o1->y - o2->y;
    		result = ((x * x) + (y * y));
    	}
	}
	if(o1->x > o2->x){
		if(o1->y <= o2->y){
    		x = o1->x - o2->x;
    		y = o2->y - o1->y;
    		result = (x * x) + (y * y);
    	}
    	else if(o1->y > o2->y){
    		x = o2->x - o1->x;
    		y = o1->y - o2->y;
    		result = ((x * x) + (y * y));
    	}
	}
    return sqrtf(result);
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO

    //TATO FUNKCIA JE ISTO FUNKCNA

    float shortest = INT_MAX;
    float distance = 0;
    for(int i = 0; i < c1->size; i++){
    	for(int j = 0; j < c2->size; j++){
			distance = obj_distance(&c1->obj[i], &c2->obj[j]);
			if(distance < 0){
				return -1.0;
			}
			if(distance < shortest){
				shortest = distance;
			}
    	}
    }
    return shortest;

}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);
    // TODO

    //TATO FUNKCIA JE ISTO FUNKCNA

    float smallest = INT_MAX, closest = 0;
    for(int i = 0; i < narr - 1; i++){
    	for(int j = i + 1; j < narr; j++){
			closest = cluster_distance(&carr[i], &carr[j]);
			if(closest < 0){         
				*c1 = -1;
				*c2 = -1;
			}
			if(closest < smallest){				
				smallest = closest;
				*c1 = i;
				*c2 = j;
    		}
		}
    }
 
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

void freeMallocs(struct cluster_t *c, int narr){
    for(int k = 0; k < narr; k++){
      clear_cluster(&c[k]);
    }
    free(c);
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);
    // TODO
    FILE *objectsFile;
    int number, i = 0;

    //otvorenie subora
    if((objectsFile = fopen(filename, "r")) == NULL){
			fprintf(stderr, "Nastala chyba v subore: Subor s objektmi sa nepodarilo otvorit!\n");
			fclose(objectsFile);
			return -2;
	}

	if((fscanf(objectsFile, "count=%d", &number)) == 1){ //v subore je vzdy ha prvej riadke ("count=CISLO") - nacitava toto cislo
                                                         //ak nacita uspesne cislo fscanf vracia pocet tychto uspesnych nahrati(1)
		if((*arr = (struct cluster_t *)malloc (sizeof(struct cluster_t) * number)) == NULL){ 
            fprintf(stderr, "Nastala chyba v alokacii: Nepodarilo sa alokovat cluster!\n");//alokovanie priestoru pre vsetky clustere
            fclose(objectsFile);//zaroven test ci sa podarilo alokovat
			return -2;
		}		

		struct obj_t obj_curr; //deklaracia a inicializacia pomocnej struktury objektu

		while(i < number){  //cyklus nacitava objekty zo subora a vklada ich do clusterov
			if((fscanf(objectsFile, "%d %f %f", &obj_curr.id, &obj_curr.x, &obj_curr.y)) == 3){  
                if(obj_curr.x < 0 || obj_curr.x > 1000){
                    fprintf(stderr, "Nastala chyba v subore s objektmi: Suradnica 'X' objektu %d je v nepodporovanom intervale!\n", obj_curr.id);
                    fclose(objectsFile);
                    freeMallocs(*arr, number);
                    return -1;
                }
                if(obj_curr.y < 0 || obj_curr.y > 1000){
                    fprintf(stderr, "Nastala chyba v subore s objektmi: Suradnica 'Y' objektu %d je v nepodporovanom intervale!\n", obj_curr.id);
                    fclose(objectsFile);
                    freeMallocs(*arr, number);
                    return -1;
                }
			     init_cluster(&(*arr)[i], 1);  //inicializacia clusteru
			     append_cluster(&(*arr)[i], obj_curr);    //pripojenie objektu na cluster
			     i++;
            }
            else{
                fprintf(stderr, "Nastala chyba v subore s objektmi: Objekt bol v zlom formate!\n");
                fclose(objectsFile);
                freeMallocs(*arr, number);
                return -1;
            }
		}
	fclose(objectsFile);   //zatvorenie subora
	return i;   //funkcia vracia pocet nacitanych objektov
	}
	else {
		fprintf(stderr, "Nastala chyba v subore s objektmi: Nepodarilo sa nacitat pocet objektov!\n");
		fclose(objectsFile);
		return -1;
	}
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;
    // TODO
    char *ptr;  //pomocny pointer(v strtoul kontrola ci je cislo)
    unsigned long ammountOfClusters;    //pocet pozadovany clusterov z argumentu programu
    int loadedClusters;     //pocet nacitanych clusterov
    int index1 = -1;   //index 1. najblizsieho cisla
    int index2 = -1;   //index 2. najblizsieho cisla

    if(argc > 1 && argc < 4){  //test poctu argumentov
    	if(argc == 3){ //test - bolo zadanych aj pocet pozadovanych argumentov
    		ammountOfClusters = strtoul(argv[2], &ptr, 10); //nacitanie poctu pozadovanych argumentov
    		if(*ptr == 0){    //ak bolo zadane cislo ptr je nulovy(neukazuje na pismeno)

    			loadedClusters = load_clusters(argv[1], &clusters); //vo funkcii sa nacitaju objekty zo subora do clusterov

                if(loadedClusters == -1){ //chyba v nacitani objektov
                    return 1;
                }
                else if(loadedClusters == -2){ //nastala chyba v alokovani pamate
                    return 1;
                }

                    //a vracaju pocet nacitanych objektov/clusterov

    			if((int)ammountOfClusters > loadedClusters){ //ak je poziadavka vacsia ako pocet nacitanych -> error(?????)
    				fprintf(stderr, "Chyba v argumente programu: Pozadovany pocet clusterov je vacsi ako pocet objektov v subore!\n");
                    freeMallocs(clusters, loadedClusters);  //uvolnenie alokovanej pamati
    				return 1;
    			}
    			else if((int)ammountOfClusters == loadedClusters){    //ak sa poziadavka rovna poctu nacitanych objektov ->vytlacia sa
    				print_clusters(clusters, loadedClusters);
    				freeMallocs(clusters, loadedClusters); //uvolnovanie pamati
    				return 1;
    			}

    			if((int)ammountOfClusters < 0){	  //ak je pocet pozadovanych clusterov zaporny ->error
    				fprintf(stderr, "Chyba v argumente programu: Pocet objektov nemoze byt zaporny\n");
    				freeMallocs(clusters, loadedClusters);  //uvolnenie pamati
    				return 1;
    			}

    			while((int)ammountOfClusters != loadedClusters){  //cyklus pokym sa nerovna poziadavka s nacitanymi clutermi
    				find_neighbours(clusters, loadedClusters, &index1, &index2);   //hlada najblizsich susedov(zapisuje ich na index)
    				if(index1 == -1 || index2 == -1){   //ak by sa nenasli najblizsi susedia -> error (????)
    					fprintf(stderr, "Nastala chyba: nebol najdeny najblizsi cluster\n");
    					freeMallocs(clusters, loadedClusters); //uvolnenie nacitanej pamati
    					return 1;
    				}
    				merge_clusters(&clusters[index1], &clusters[index2]);  //spaja najblizsie clusters
    				loadedClusters = remove_cluster(clusters, loadedClusters, index2);//funkcia zmaze pripojeny cluster(vracia novy pocet clusterov)
                                                                             //respektive pocet sa zmensi o jedna
                                                                                       //dekrementacia cyklu while (riadok 466)
    			}

    			print_clusters(clusters, loadedClusters); //tlaci vsetky clustere
    			freeMallocs(clusters, loadedClusters); //uvolnenie pamati
    		}
    		else{
    			fprintf(stderr, "Nastala chyba v argumente programu\n");
    			return 1;
    		}
    	}
    	else{ //nebol zadany 3. argument
    		loadedClusters = load_clusters(argv[1], &clusters);  //nacita objekty zo subora a vklada ich do clusterov
            if(loadedClusters == -1){ //chyba v nacitani objektov
                    return 1;
            }
            else if(loadedClusters == -2){ //nastala chyba v alokovani pamate
                    return 1;
            }
    		print_clusters(clusters, loadedClusters);    //tlaci vsetky clustere
    		freeMallocs(clusters, loadedClusters);
    	}
    }
    else{
    	fprintf(stderr, "Pri spusteny ste zadali nespravny pocet argumentov!\n");
    	return 1;
    }

    return 0;
}
