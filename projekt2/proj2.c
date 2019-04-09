/*
 * Soubor: proj2.c
 * Projekt c. 2 - Prace s textem
 * Autor: Marek Ziska, xziska03@stud.fit.vutbr.cz
 * Skupina: 1BIB
 * Datum 24.11.2018
 *
 * Popis programu: Program pocita prirodzene logaritmy a exponencialne funkcie
 *				   pomocou taylorovho polynomu a zretazenych zlomkov
 *				  
 *				   
 * Obsah programu: Program obsahuje vsetky povinne funkcie
 *
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#define EXIT_FAILURE 1
#define EXIT_SUCCES 0
#define ERROR false
#define VALID true

/*----------------------------------PROTOTYPES---------------------------------------*/
double cfrac_log(double x, unsigned int n);			//vypocet logaritmu cez zretazene zlomky
double taylor_log(double x, unsigned int n);		//vypocet logaritmu cez taylorove polynomy
double taylor_pow(double x, double y, unsigned int n);		//vypocet mocniny pomocou taylorovho polynomu
double taylorcf_pow(double x, double y, unsigned int n);	//vypocet mocniny pomocou zretazenych zlomkov
void printResultsPow(double x, double y, double n);		//vytlaci vysledky mocniny
void printResultsLog(double x, double n);		//vytlaci vysledky logaritmu
void helpInstructions();		//tlaci napovedu
double testPow(double x, double y, bool *negativeNumFlag, bool *foundResult);		//test vstupnych hodnot mocniny
double testLog(double x, bool *foundResult);		//test vstupnych hodnot logaritmu
bool testLogInput(int argc, char *arg3, char *arg4, double *x, double *n);			//kontrola argumentov pre log
bool testPowInput(int argc, char *arg3, char *arg4, char *arg5, double *x, double *y, double *n);	//--||-- pow
bool isItFloat(double y);		//test cisla ci je float
/*----------------------------------PROTOTYPES---------------------------------------*/	

/*-------------------------------------MAIN------------------------------------------*/
int main(int argc, char *argv[])
{
	double x, n, y;		/*premenne vstupnych hodnoty*/
	bool correctInput;		/*false - ak zly vstup
							 *true - ak je vstup ok*/

/*kontrola ci bolo zadanych viac argumentov*/
	if(argc > 1){
		if((strcmp(argv[1], "--log")) == 0){		/*porovnanie ci na prvom argumente bola zadana funkcia log*/
			correctInput = testLogInput(argc, argv[2], argv[3], &x, &n);	/*funkcia vracia true ak je vstup ok*/
			if(correctInput){	
				printResultsLog(x, n);		/*funkcia vytlaci vysledky*/
			}
			else{
				return EXIT_FAILURE;
			}
		}
		else if((strncmp(argv[1], "--pow", 5)) == 0){		/*porovnanie ci na prvom argumente bola zadana funkcia pow*/
			correctInput = testPowInput(argc, argv[2], argv[3], argv[4], &x, &y, &n); /*funkcia vracia true ak je vstup ok*/
			if(correctInput){
				printResultsPow(x, y, n);		/*funkcia vytlaci vysledky*/
			}
			else{
				return EXIT_FAILURE;
			}
		}
		else if((strncmp(argv[1], "--help", 6)) == 0){
			helpInstructions();		/*ak na prvom argumente je --help program vypise napovedu a skonci*/
		}
		else{		/*ak na prvom argumente nebola zadana ani jedna zo zadanych funkcii vyhodi chybu*/
			fprintf(stderr, "Nastala chyba: Zadali ste nepodporovanu funkciu!\n"
							"Pre napovedu spustite program ako '$ ./proj2 --help'\n");
			return EXIT_FAILURE;
		}
	}
	else{		/*bol zadany len argument pre spustenie programu*/
		fprintf(stderr, "Nastala chyba: Nezadali ste ziadny argument!\n"
						"Pre napovedu spustite program ako '$ ./proj2 --help'\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCES;	
}
/*-------------------------------------MAIN------------------------------------------*/

/*-----------------------------------FUNCTIONS---------------------------------------*/

/*funkcia tlaci napovedu*/
void helpInstructions(){
	printf("\nNAPOVEDA"
		   "\nProgram: Projekt 2 - Iteracne vypocty\n"
           "Autor: Marek Ziska, xziska03@stud.fit.vutbr.cz\n"
           "\nPopis programu: VĂ˝poÄet prirodzenĂŠho logaritmu a exponenciĂĄlnej funkcie s " 
         			          "obecnĂ˝m zĂĄkladom len pomocou matematickĂ˝ch operĂĄciĂ­ +,-,*,/.\n\n"
           "Syntax spustenia:\n"
           "|$ ./proj2 --log X N|\n"
           " - poziadavka na vypocet logaritmu z cisla X v N iteracich\n"
           " - N musi byt nezaporne cele cislo"
           " - X moze byt lubovolne realne cislo\n\n"
           "|$ ./proj2 --pow X Y N|\n"
           " - poziadavka na vypocet exponencialnej funcie z cisla Y s obecnym zakladom X v N iteracĂ­ch\n"
           " - X a Y mozu byt lubovolne realne cisla\n"
           " - N musi byt nezaporne cele cislo\n\n");
}
/*funkcia vola funkcie na vypocet a zaroven tlaci vysledky*/
void printResultsLog(double x, double n){
	printf("       log(%.5g) = %.12g\n", x, log(x));
	printf(" cfrac_log(%.5g) = %.12g\n", x, cfrac_log(x , n));
	printf("taylor_log(%.5g) = %.12g\n", x, taylor_log(x , n));
}
/*funkcia vola funkcie na vypocet a zaroven tlaci vysledky*/
void printResultsPow(double x, double y, double n){	 
		printf("         pow(%.5g,%.5g) = %.12g\n", x, y, pow(x, y));
		printf("  taylor_pow(%.5g,%.5g) = %.12g\n", x, y, taylor_pow(x , y, n));
		printf("taylorcf_pow(%.5g,%.5g) = %.12g\n", x, y, taylorcf_pow(x , y, n));
}
/*funkcia vypocita logaritmus pomocou zretazenych zlomkov*/
double cfrac_log(double x, unsigned int n){
	double z = (x - 1) / (1 + x);		//vyjadrenie si z zo vzorca
	double denominator;		//deklaracia premennej na citatela
	double numerator;		//deklaracia premennej na menovatela
	double result;		//vysledok logaritmu
	bool foundResult = false;	 /*false ak sa nenasiel vysledok
								  *true ak sa nasiel vysledok(v testPow)*/

	result = testLog(x, &foundResult);		//funkcia testuje vstupy pre logaritmus

	if(foundResult){		/*ak vo funkcii testLog naslo vysledok tak ho vratim*/
		return result;
	}
	else{

		for(unsigned i = n; i > 1; i--){		//v cykle pocitame zretazene zlomky od spodu
			numerator = ((i - 1) * (i - 1)) * z * z;		//vypocet cisla v citateli
			denominator = (i * 2) - 1 - result;		//vypocet cisla v menovateli
			result = numerator / denominator;		//vypocet celej casti zretazeneho zlomku
		
		}
		result = (2 * z) / (1 - result);		//vypocet celeho zlomku 
		return result;		//funkcia vrati vysledok
	}	
}

/*funkcia na vypocet logaritmu pomocou taylorovych polynomov*/
double taylor_log(double x, unsigned int n){
	double y;		//premenna na vyjadrenie (1 - y)
	double numerator;		//pomocna premmena na medzivypocet
	double result;		//premenna do ktorej ukladame vysledok
	bool foundResult = false;	/*false ak sa nenasiel vysledok
								 *true ak sa nasiel vysledok(v testPow)*/

	result = testLog(x, &foundResult);		//funkcia testuje vstupy pre logaritmus

	if(foundResult){			//ak sa nasiel vysledok v testPow vratim vysledok
		return result;
	}
	else{
/*Ak je x mensie ako 1 pouzijeme prvy polynom -> log(1 - y) = -y - (y^2/2) - (y^3/3) - ...
 */
		if(x < 1){
			y = 1 - x;		/*vyjadrenie si y z (1 - x)*/
			numerator = y;	/*citatel v rozvoji*/
			result = result - numerator;	/*odcitanie prveho clena z result*/

			for(unsigned int i = 2; i <= n; i++)  {
				numerator *= y;		/*vypocet citatela v nasledujucom clene*/
				result -= (numerator / i);		/*odcitanie clena z result*/
			}
		}

/*Ak je x vacsie ako 1 pouzijeme polynom -> log(x) = ÎŁ(od 1 -> n) (((x - 1) / x)^i) / n
 */	
		else if(x > 1){
			y = (x-1) / x;		//citatel zo vzorca ked i = 1
			result = y;		//do vyslednej premmennej ulozime prvu cast z ÎŁ 

			for(unsigned int i = 2; i <= n; i++){			//cyklus konci pri dosiahnuti n-tej iteracie
				y *= ((x-1) / x) ;			//nĂĄsobenie predchĂĄdzajĂşce clena s terajsim
				result += (y / i);		/*vysledok nasobenia vydelime i-tym clenom a 
									 *pricitame ho do vyslednej premmenej*/
			}
		}
	}
	return result;		//funkcia vrati vysledok
}

/*funkcia na vypocitanie mocniny pomocou taylorovho polynomu*/
double taylor_pow(double x, double y, unsigned int n){
	bool negativeNumFlag = false;	/*false ak sa nenajde zaporne cislo
									 *true ak sa najde zaporne cislo(v testPow)*/
	bool foundResult = false;	/*false ak sa nenasiel vysledok
								 *true ak sa nasiel vysledok(v testPow)*/
	unsigned int i = 2;			//premenna na inkrementaciu zaciname 2 clenom preto i = 2
	double result;		/*do result ukladam vysledok*/
	double lna;			/*prvy clen vo vzorci je 1*/
	double fraction;		/*do premennej ukladam zlomky*/

	result = testPow(x, y, &negativeNumFlag, &foundResult);		/*test vstupnych hodnot*/

	if(foundResult){		//ak sa nasiel vysledok v testPow vratim vysledok
		return result;
	}
	else{

		lna = taylor_log(fabs(x), n);		//pouzitie taylorovho logaritmu na vypocet lna
		fraction = y * lna;		//citatel zo vzorca


		while (i <= n){			/*cyklus konci pri dosiahnuti max rozvoju*/
			result += fraction;		/*pripocitanie dalsieho clena z rozvoja*/
			fraction *= (y * lna) / i; 		/*vypocet i-teho clena rozvoja*/
			i++;		//inkrementacia
		}
		if(negativeNumFlag && ((int)y % 2 == 1)){
			return -result;			//funkcia vracia vysledok
		}
		else{
			return result;
		}
	}
}

/*funkcia na vypocitanie mocniny pomocou zretazenych zlomkov*/
double taylorcf_pow(double x, double y, unsigned int n){
	bool foundResult = false;	/*false ak sa nenasiel vysledok
								 *true ak sa nasiel vysledok(v testPow)*/
	bool negativeNumFlag = false;	/*false ak sa nenajde zaporne cislo
									 *true ak sa najde zaporne cislo(v testPow)*/
	unsigned int i = 2;			/*premenna na inkrementaciu v cykle*/
	double result;		/*prvy clen v rozvoji*/
	double lna;		/*premenna na ulozenie logaritmu*/
	double fraction;		/*premenna do ktorej ukladam jednotlive zlomky*/
	

	result = testPow(x, y, &negativeNumFlag, &foundResult);		/*test vstupnych hodnot*/

	if(foundResult){			//ak sa nasiel vysledok v testPow vratim vysledok
		return result;	
	}
	else {
		lna = cfrac_log(fabs(x), n);			//premenna vypoctu logaritmu
		fraction = y * lna;		//vypocet druheho clena v rozvoji

		while(i <= n){
			result += fraction;		//scitanie 2 clenov z rozvoja
			fraction *= (y * lna) / i;		//vypocet i-teho clena 
			i++;		//inkrementacia
		}
		if(negativeNumFlag && ((int)y % 2 == 1)){
			return -result;			/*funkcia vrati zaporny vysledok ak zaporny
									 *zaklad umocnujem na neparne cislo */
		}
		else{
			return result;		//funkcia vracia vysledok
		}
	}
}

/*test argumentov pre mocninu*/
bool testPowInput(int argc, char *arg3, char *arg4, char *arg5, double *x, double *y, double *n){
	char *ptr;		//pomocna premmena do strtod
	if((argc >= 2) && (argc < 5)){		/*ak bolo zadanych menej ako 5 a viac ako 3 argumenty program konci, vyhodi chybu*/
		fprintf(stderr, "Nastala chyba: Zadali ste malo argumentov\n"
						"Pre napovedu spustite program ako '$ ./proj2 --help'\n");
		return ERROR;
	}
	else if(argc == 5){		/*ak bolo zadanych 5 argumentov program pokracuje*/
		*x = strtod(arg3, &ptr);		/*prevod stringu do double*/
		if(*ptr == 0){		/*ak bolo zadane cislo, teda ptr na znak ktory nemoze tvorit double je nulovy
							 *program pokracuje dalej, inak program vyhodi chybu a konci
							 */
			*y = strtod(arg4, &ptr);		/*prevod stringu do double*/
			if(*ptr == 0){		/*rovnaky test ako na riadku 82*/
				*n = strtod(arg5, &ptr);		/*prevod stringu do double*/
				if(*ptr == 0){		/*rovnaky test ako na riadku 82*/
					if(*n < 0){
						fprintf(stderr, "Nastala chyba: Zadali ste zaporny pocet iteracii!\n"
										"Pre napovedu spustite program ako '$ ./proj2 --help'\n");
						return ERROR;
					}
					else{
						return VALID;		/*vstupy su ok*/
					}

				}
				else{
					fprintf(stderr, "Nastala chyba v piatom argumente programu 'N'\n"
									"Pre napovedu spustite program ako '$ ./proj2 --help'\n");
					return ERROR;
				}

			}
			else{
				fprintf(stderr, "Nastala chyba v stvrtom argumente programu 'Y'\n"
								"Pre napovedu spustite program ako '$ ./proj2 --help'\n");
				return ERROR;
			}
		}
		else{
			fprintf(stderr, "Nastala chyba v tretom argumente programu\n"
							"Pre napovedu spustite program ako '$ ./proj2 --help'\n");
			return ERROR;
		}
	}
	else{		/*ak bolo zadanych viac ako 5 argumentov program konci, vyhodi chybu*/
		fprintf(stderr, "Nastala chyba: Zadali ste privela argumentov!\n"
						"Pre napovedu spustite program ako '$ ./proj2 --help'\n");
		return ERROR;
	}
}

/*test argumentov pre logaritmus*/
bool testLogInput(int argc, char *arg3, char *arg4, double *x, double *n){
	char *ptr;		//pomocny pointer do strtod
	if(argc == 2){
		fprintf(stderr, "Malo argumentov: Nezadali ste ziady argument funkcie --log\n"		/*ak boli zadane len tri argumenty program konci, vyhodi chybu*/
						"Pre napovedu spustite program ako '$ ./proj2 --help'\n");	
		return ERROR;
	}
	else if(argc == 3){
		fprintf(stderr, "Malo argumentov: Nezadali ste pocet iteracii (n)!\n"		/*ak boli zadane len tri argumenty program konci, vyhodi chybu*/
						"Pre napovedu spustite program ako '$ ./proj2 --help'\n");	
		return ERROR;
	}
	else if(argc == 4){		/*ak boli zadane styri argumenty program pokracuje
 									 */
		*x = strtod(arg3, &ptr);		/*prevod stringu do double*/
		if(*ptr == 0){				/*ak bolo zadane cislo, teda ptr na znak ktory nemoze tvorit double je nulovy
									 *program pokracuje dalej, inak program vyhodi chybu a konci
									 */
			*n = strtod(arg4, &ptr);		/*prevod stringu do double*/
			if(*ptr == 0){		/*rovnaky test ako na riadku 52*/
				if(*n < 0){
					fprintf(stderr, "Nastala chyba: Zadali ste zaporny pocet iteracii!\n"
									"Pre napovedu spustite program ako '$ ./proj2 --help'\n");
					return ERROR;
				}
				else{
					return VALID;		/*vstupy su ok*/
				}
			}
			else{
				fprintf(stderr, "Nastala chyba: V stvrtom argumente nebolo zadane cislo!\n"
								"Pre napovedu spustite program ako '$ ./proj2 --help'\n");
				return ERROR;
			}
		}
		else{
			fprintf(stderr, "Nastala chyba: V tretom argumente nebolo zadane cislo!\n"
							"Pre napovedu spustite program ako '$ ./proj2 --help'\n");
			return ERROR;
		}			
	}
	else{		/*ak bolo zadanych vela argumentov program konci, vyhodi chybu*/
		fprintf(stderr, "Nastala chyba: V tretom argumente nebolo zadane cislo!\n"
						"Pre napovedu spustite program ako '$ ./proj2 --help'\n");
		return ERROR;
	}
}

/*funkcia testuje vstupne hodnoty logaritmu*/
double testLog(double x, bool *foundResult){
	if(x < 0){		/*a je x mensie ako 0, vracia NAN pretoze log nie je definoveny
					 *pre zaporne cisla*/
		*foundResult = true;
		return NAN;
	}
	else if(x == 0){				/*ak je sa x == 0, vracia -nekonecno*/
		*foundResult = true;
		return -INFINITY;
	}
	else if(isinf(x)){		/*test ci nebolo zadane inf*/
		*foundResult = true;
		return INFINITY;
	}
	else if(isnan(x)){
		*foundResult = true;
		return NAN;
	}
	return 0.0;		/*ak je vsetko OK vracia vratim do result nulu(prvy clen v rozvoji)*/
}

/*funkcia testuje vstupne hodnoty pre mocninu*/
double testPow(double x, double y, bool *negativeNumFlag, bool *foundResult){
	if(x == 0 && y == 0){
		*foundResult = true;
		return 1.0;
	}
	if(x == 0){			/*ak sa x == 0, funkcia vracia -nekonecno*/
		*foundResult = true;
		return 0.0;
	}
	else if(x < 0){			/*test ci x je zaporne*/
		*negativeNumFlag = true;
	}
	if(y == 1){			/*ak je y rovne 1 vratim x*/
		*foundResult = true;
		return x;
	}
	else if(y == 0){		/*ak je y rovne nule vysledok bude 1*/
		*foundResult = true;
		return 1.0;
	}
	if(isinf(x) && isnan(y)){		
		*foundResult = true;
		return NAN;
	}
	if(isnan(x) && isinf(y)){		
		*foundResult = true;
		return NAN;
	}
	else if(isinf(x) || isinf(y)){		/*test ci bolo zadane inf*/
		*foundResult = true;
		return INFINITY;
	}
	else if((x < 0) && isItFloat(y)){		/*test ci umocnujem zaporne cislo desatinnym*/
		*foundResult = true;
		return -NAN;
	}
	return 1.0;
	
}

/*funkcia kontroluje ci cislo je desatinne*/
bool isItFloat(double y){
	int compare;
	compare = y;
	if(y == compare){
		return false;
	}
	else{
		return true;
	}
	
}

/*-----------------------------------FUNCTIONS---------------------------------------*/
