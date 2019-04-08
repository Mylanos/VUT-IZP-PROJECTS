/*
 * Soubor: proj1.c
 * Projekt c. 1 - Prace s textem
 * Autor: Marek Ziska, xziska03@stud.fit.vutbr.cz
 * Skupina: 1BIB
 * Datum 4.11.2018 
 *
 * Popis programu: Program funguje ako textovy editor. Editor nebude interaktivny, 
 *				   tj. nebude komunikovat s uzivatelom, ale prikazy pre upravu textu 
 *				   bude nacitat zo vstupneho subora. Text sa bude upravovat podla 
 *				   prikazov ktore su tiez v subore. Program je riadkovy, tzn. pracuje
 *				   vzdy len s jednym riadkom.
 *				   
 * Obsah programu: Program obsahuje vsetky povinne prikazy a obsahuje aj jeden nepovinny prikaz - appendEOL
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define MAXchar 1005
#define MAXline 100
#define EXIT_FAILURE 1
#define EXIT_SUCCES 0

/*----------------------------------PROTOTYPES---------------------------------------*/
void closeCommandFile(FILE** subPrikazy);
void getLine(char *line);
void openCommandFile(char *argument1, FILE** subPrikazy);
void removeCommandChar(char *content, char *string);
void insertContent(char *string);
void beforeContent(char *string, char *line);
int isItNumber(char *string);
bool isEmpty(char *string);
void appendContent(char *content, char *line);
void getContent(char *array, FILE** subPrikazy);
void removeEOL(char *line);
void delete(int n, char *line);
void next(int n, char *line);
void appendEOL(char *line);
void substitution(char *string, char *line);
void commandExecution(char contentArray[MAXline][MAXchar]);
void help();
/*----------------------------------PROTOTYPES---------------------------------------*/

/*-------------------------------------MAIN------------------------------------------*/
int main(int argc, char *argv[])
{
	FILE *subPrikazy;				//pointer na FILE s prikazmy

	char choice; 					//charakter pre volanie napovedy
	int c;							//integer na nacitanie znakov z prik. subora		
	int numberOfCommand = 0;		//inicializovany int na inkrementaciu v cykle
	char contentArray[MAXline][MAXchar];	//2d pole do ktoreho sa uklada subor s prikazmy

/*test ci bolo zadanych malo argumentov
 */
	if (argc < 2) {
		fprintf(stderr, "Zadali ste malo argumentov: Pre spravne fungovanie programu si precitajte napovedu\n"
						"Chcete otvorit napovedu? (y/n)\n");
		scanf("%c", &choice);
		if(choice == 'y'){
			help();			//NAJVACSI BULLSHIT CO SOM MOHOL SPRAVIT
			return EXIT_FAILURE;
		}
		else { 
			return EXIT_FAILURE;
		}
	}
/*test ci bol zadany spravny pocet argumentov*/
	else if (argc == 2) {
/*funkcia na otvorenie suboru s prikazmy
 */
		openCommandFile(argv[1], &subPrikazy);	

/*cyklus nacitava charaktery zo suboru s prikazmy a nacitava ich do 
 *2d arrayu a prikazy si ulozim do prveho indexu 2d pola
 */
		while((c = getc(subPrikazy)) != EOF){
			contentArray[numberOfCommand][0] = c;
			getContent(contentArray[numberOfCommand], &subPrikazy);
			if(numberOfCommand < MAXline){
				numberOfCommand++;
			}
			else{
				fprintf(stderr, "Zadali ste prilis  vela prikazov, program podporuje len 100 prikazov\n");
				return EXIT_FAILURE;
			}
		}
	
	closeCommandFile(&subPrikazy);
/*vykonanie prikazov
 */
	commandExecution(contentArray);

	}
/*test ci bolo zadanych prilis vela argumentov*/
	else {
		fprintf(stderr,"Zadali ste prilis vela argumentov: Pre spravne fungovanie program si precitajte napovedu\n"
					   "Chcete otvorit napovedu? (y/n)\n");
		scanf("%c", &choice);
		if(choice == 'y'){
			help();
			return EXIT_FAILURE;
		}
		else { 
			return EXIT_FAILURE;
		}
	}

return EXIT_SUCCES;
}
/*-------------------------------------MAIN------------------------------------------*/


/*-----------------------------------FUNCTIONS---------------------------------------*/
/*funkcia zatvori subor s prikazmy a skontroluje ci sa zatvorili spravne
 */
void closeCommandFile(FILE** subPrikazy) {
    if (fclose(*subPrikazy) == EOF) {
        fprintf(stderr, "Subor s prikazmy sa nepodarilo zatvorit\n");
        exit(1);
    }
    else{
		return;
    }
}	

/*funkcia "getLine" nacita aktualny riadok z INPUTU
 */
void getLine(char *line) {
	int i = 0; 			//int na inkrementaciu
	int c;				//int na nacitanie charakterov
/*cyklus nacitava charaktery zo stdinputu a uklada ich do retazca "line"
 */
	while((c = fgetc(stdin)) != '\n') {
		if (c == EOF) {
			exit(1);
		}
		line[i] = c;
		i++;
	}
/*posledny charakter v retazci sa musi ukoncit s '\0' 
 */
	line[i] = '\0';
	return;
}

/*funkcia otvori subor s prikazmy
 */
void openCommandFile(char *argument1, FILE** subPrikazy) {
	char choice;	//znak na zobrazenie napovedy
	if((*subPrikazy = fopen(argument1, "r")) == NULL){
			fprintf(stderr, "Subor s prikazmy sa nepodarilo otvorit\n"
						    "Chcete otvorit napovedu? (y/n)\n");
		scanf("%c", &choice);
		if(choice == 'y'){
			help();
			exit(1);
		}
		else { 
			exit(1);
		}
	}
	return;
}

/*pomocna funkcia k isitnumber "removeCommandChar" sluzi na nacitanie retazca z 
 *jednoho do druheho tak ze v druhom nenacita prvy znak
 */
void removeCommandChar(char *content, char *string) {
	unsigned i;				//unsigned pretoze porovnavame s tym strlen
/*v cykluse sa zbavime prveho znaku v retazci - teda toho prikazu*/
	for(i = 1; i < (strlen(string)); i++){
		content[i - 1] = string[i];
	}
	return;
}

/*funkcia "insertContent" sluzi na vytlacenie CONTENTu z prikazoveho
 *suboru na jeden riadok
 */
void insertContent(char *string){
	char content[MAXchar];		//array pre nacitanie CONTENTU bez prikazu
	int count = 0;				//pocet charakterov
/*volame funkciu na zbavenie sa prveho znaku a pomocou funckie strlen zistime 
 *dlzku retazca a na koniec dame '\0' aby nam nevznikali chyby, nasledne
 */
	removeCommandChar(content, string);
	count = strlen(content);
	content[count] = '\0';
	puts(content);
	return;
}

/*funkcia "beforeContent" sluzi na vytlacenie CONTENTu z prikazoveho
 *subora pred aktualny riadok
 */
void beforeContent(char *string, char *line) { 
	char editedArray[MAXchar] = {'\0'};		//pomocna array na nacitanie upraveneho stringu
	int count;							//pocet charakterov v editedArray
	unsigned totalLineLength, i;		//dlzky stringov

/*zbavime sa prveho znaku z prikazoveho riadka a znova ho ukoncime s '\0',
 *nasledne sa zistime dlzku riadka s CONTENTOM a s riadkom z stdinu 
 *ak tato dlzka bude viac ako 1000 znakov vyhodi nam chybu, inak pred aktualny riadok
 *pripojime CONTENT, v cykle sa nam prehodia znaky z editedArray do aktualneho riadka
 */
	removeCommandChar(editedArray, string);
	count = strlen(editedArray);
	editedArray[count] = '\0';
	totalLineLength = ((strlen(editedArray)) + (strlen(line)));

	if(totalLineLength < MAXchar){
		strcat(editedArray, line);
		for(i = 0; i <= totalLineLength; i++){
			line[i] = editedArray[i];
		}
	}
	else{
		fprintf(stderr, "V prikaze nastala chyba: Vysledny riadok obsahoval prilis vela znakov\n");
		exit(1);
	}
	return;
}

/*funkcia "isItNumber" zistuje ci v stringu su len cisla ak najde iny znak
 *vracia chybu ak tam cislo je tak funkcia ho prehodi z stringu na integer
 *a vraziu tuto hodnotu
 */
int isItNumber(char *string) {
	char contentArray[MAXchar];	//array len na nacitanie CONTENTU	
	int value;					//hodnota cisla za prikazom
	unsigned i;					//porovnanie s dlzkou stringu
	char *ptr;					//pomocny pointer pre funkciu strtol

	for(i = 1; i < strlen(string); i++){
		if(!isdigit(string[i])){
			return -1;
		}
	}
	removeCommandChar(contentArray,string);
	value = strtol(contentArray, &ptr ,10);
	return value;
}

/*funkcia "isEmpty" sluzi na zistenie ci v stringu nieco je
 *je ak tam nic nieje funkcia vracia false naopak zase true
 */
bool isEmpty(char *string) {
	unsigned i;		//porovnanie s dlzkou stringu

	for (i = 1; i < strlen(string); i++) {
		if (!isspace(string[i])) {
			return false;
		}
	}
	return true;
}

/*funkcia appendContent sluzi na precitanie stringu bez EOL zo stdin 
 *a pripojenie na jeho koniec string z prikazoveho subora(CONTENT)
 */
void appendContent(char *content, char *line) { 
	char contentArray[MAXchar] = {'\0'};				//array lem na nacitanie CONTENTU
	removeCommandChar(contentArray, content);

/*znova zistuje ci nie je finalna dlzka vacsia ako 1000 ak nie je napoji CONTENT 
 *na koniec aktualneho riadka
 */
	if((strlen(line)+(strlen(contentArray))) < MAXchar){ 
		strcat(line, contentArray);
	}
	else{
		fprintf(stderr, "V prikaze nastala chyba: Vysledny riadok bol prilis dlhy!\n");
		exit(1);
	}
	return;
}

/*funkcia "getContent" sluzi na nacitanie 1 celeho stringu z prikazoveho
 *subora ale bez EOL, funkcia kontroluje ci riadok nie je prilis dlhy
 */
void getContent(char *array, FILE** subPrikazy) {
	int c;			//sluzi na nacitanie charakterov z prik. subora
	int i = 1;		//int na inkrementaciu

	while((c = getc(*subPrikazy)) != '\n' && c != EOF) {

		if(i < MAXchar) {
			array[i] = c;
			i++;
		}
		else {
			fprintf(stderr, "V prikazovom subore nastala chyba: Riadok bol prilis dlhy!\n");
			closeCommandFile(subPrikazy);
			exit(1);
		}
	}
	array[i] = '\0';
	return;
}

/*funkcia "removeEOL" sluzi na odstranenie EOL na aktualnom riadku
 */
void removeEOL(char *line) {
	int editedLineLength;		//dlzka spojeneho riadka
	char array[MAXchar];		//array na nacitanie nasledujucehos stringu
	getLine(array);
	strcat(line, array);
	editedLineLength = strlen(line);
	if(editedLineLength < MAXchar) {
		return;
	}
	else{
		fprintf(stderr, "Po odstraneni znaku EOL bol vysledny riadok prilis dlhy\n");
		exit(1);
	}
}

/*funkcia "delete" nacita riadok z stdin ale nevytlaci ho
 */
void delete(int n, char *line) {     
				  
	int i = 0;
	do {
		getLine(line);
		i++;
	} while(i < n);
	return;
}

/*funkcia "next" tlaci aktualny riadok alebo tlaci dany pocet riadkov ktore
 *dostal v argumente 
 */
void next(int n, char *line) { 			 
	int i = 0;							
	do {
		puts(line);
		getLine(line);
		i++;
	}while(i < n);
	return;
}

/*funkcia prida na aktualny riadok EOL
 */
void appendEOL(char *line){
	char *EOL = "\n";
	strcat(line, EOL);
	return;
}

/*funkcia "substitution" pracuje pre prikazy s a S. V pripade prikazu "s" hlada v aktualnom
 *riadku jeden retazec "CONTENT" ak ho najde nahradi ho s "REPLACEMENT". Prikaz "S" pracuje
 *analogicky ako u prikazu "s", len substituciu vykonava pre vsetky vyskyty "CONTENT"*/
void substitution(char *string, char *line){
	char patternArray[MAXchar] = {'\0'};
	char contentArray[MAXchar] = {'\0'};
	char replacementArray[MAXchar] = {'\0'};
	char bigEditedArray[MAXchar] = {'\0'};
	char smallEditedArray[MAXchar] = {'\0'};
	
	char *replacementPtr = NULL;
	char *substitutionPtr = NULL;
	char *repSubstitutionPtr = NULL;

	unsigned i, l = 0;
	int j = 0, k = 0, n = 0, o;
	int indexBegRep, replacementLength, lineLength, patternLength, smallArrLenght, bigArrLength;
/*if kontroluje ci bol zadany oddelovaci znak na mieste po prikaze (v mojom
 *pripade oddelovacie znaky su ' ' a ':') ak by tam tento znak nebol vyhodi chybu
 */
 	if(string[2] == string[1]){
			fprintf(stderr, "Zle zadany argument prikazu 's': PATTERN nebol zadany\n");
			exit(1);
		}
		else{
/*v cykle sa zbavime oddelovacieho znaku
 */
			for(i = 2; i < (strlen(string)); i++){
				contentArray[i - 2] = string[i];
			}
/*strchr nam vrati pointer na prvy vyskyt oddelovacieho znaku -> replacement pointer
 */
			replacementPtr = strchr(contentArray, string[1]);
			if(replacementPtr == NULL){
				fprintf(stderr, "Zle zadany argument prikazu 's': Nebol zadany REPLACEMENT\n");
				exit(0);
			}
			else {
/*v cykle ziskame iba pattern z contentarray
 */
				while((contentArray[j]) != string[1]){	
					patternArray[j] = contentArray[j];
					j++;
				}	
			}
		}	
/*kedze replacement pointer zacina od oddelovacieho znaku ktory v cielovej uprave
 *nechceme tak pouzivame znova funkciu na zbavenie sa prveho znaku v retazci
 */
	removeCommandChar(replacementArray, replacementPtr);
/*nasledne si vypocitam dlzku replacementu a aktualneho riadka
 */
	replacementLength = strlen(replacementArray);
	lineLength = strlen(line);
/*funcia strstr nam najde vyskyt retazca v druhom retazci, vracia pointer na 
 *tento vyskyt. Ak tento vyskyt nenajde program sa vracia z funkcie a pokracuje
 *dalej
 */
	substitutionPtr = strstr(line, patternArray);

	if (substitutionPtr == NULL){
		return;
	}
	indexBegRep = (strlen(line)) - (strlen(substitutionPtr));
	patternLength = strlen(patternArray);
/*v prvom cykle sa zapisuju znaky do pomocnej arrayi a to pokial sa nedostanem na index
 *patternu
 */
	while(k < indexBegRep) {
		smallEditedArray[k] = line[k];
		k++;
	}
/*inicializujeme si index replacementu a v tomto cykle zapisujeme do pomocnej arrayi
 *pokial sa nedostaneme na dlzku replacementu a indexu patternu
 */
	int replacementIndex = 0;
	while(k < (replacementLength + indexBegRep)) {
		smallEditedArray[k] = replacementArray[replacementIndex];
		k++;
		replacementIndex++;
	}
/*inicializujeme si index poslednej casti ktora bola za CONTENTOM a v cykle cyklime 
 *pokial nedosiahneme dlzku REPLACEMENTU a LINU
 */
	int endLineIndex = indexBegRep + patternLength;
	while(k < (replacementLength + lineLength)) {
		smallEditedArray[k] = line[endLineIndex];
		k++;
		endLineIndex++;
	}
/*Test ci na prvej pozicii je velke S, ak hej tak substitucia pokracuje pre vsetky
 *najdene retazce "CONTENT" v aktualnom riadku*/
	if(string[0] == 'S'){
/*test ci nie je CONTENT a REPLACEMENT ten isty retazec, ak su rovnake tak program
 *preskoci zbytocny cyklus*/
		if((strcmp(patternArray,replacementArray)) != 0){
			repSubstitutionPtr=strstr(smallEditedArray, patternArray);
/*cyklus prehladava riadok, ak najde CONTENT nahradi ho s REPLACEMENT.Ak nahradi 
 *vseky vyskyty CONTENT tak skonci. Pre substituciu pouzivam cykli ktore som 
 *uz popisal vyssie.*/
			do {
				smallArrLenght = strlen(smallEditedArray);
				indexBegRep = (smallArrLenght) - (strlen(repSubstitutionPtr));
				while(n < indexBegRep){
					bigEditedArray[n] = smallEditedArray[n];
					n++;
				}
				replacementIndex = 0;
				while(n < (replacementLength + indexBegRep)){
					bigEditedArray[n] = replacementArray[replacementIndex];
					n++;
					replacementIndex++;
					}
					endLineIndex = indexBegRep + patternLength;
				while(n < (replacementLength + smallArrLenght)){
					bigEditedArray[n] = smallEditedArray[endLineIndex];
					n++;
  					endLineIndex++;
				}
				bigArrLength = strlen(bigEditedArray);
				for(o = 0; o < bigArrLength; o++){
					smallEditedArray[o] = bigEditedArray[o];
					bigEditedArray[o] = '\0';
				}
				n = 0;
				repSubstitutionPtr = NULL;
				repSubstitutionPtr=strstr(smallEditedArray, patternArray);
				

			}while(repSubstitutionPtr != NULL);
		}
		else{
			return;
		}
	}

	if((strlen(smallEditedArray)) < MAXchar){
	/*v cykle prehodime z pomocnej arrayi do linu, line pre istotu pred
	 *prepisanim vynulujeme
	 */
		while(l <= (strlen(smallEditedArray))){
			line[l] = '\0';
			line[l] = smallEditedArray[l];
			l++;
		}
		return;
	}
	else{
		fprintf(stderr, "V prikaze nastala chyba: 'Riadok po substituci bol prilis dlhy'");
		exit(1);
	}
}

/*funkcia vytlaci napovedu*/
void help() {
	printf("\nNAPOVEDA"
		 "\nProgram: Projekt 1 - Prace s textem\n"
         "Autor: Marek Ziska, xziska03@stud.fit.vutbr.cz\n"
         "\nPopis programu: Program upravuje vstupny text podla prikazov zo subora s prikazmy."
         "Vstupny text program nacitava zo subora. Subor nesmie byt prazdny"
         "Program podporuje maximalne 1000 znakov na riadok\n\n"
         "Syntax spustenia:\n"
         "|$ ./proj1 JMENO_SOUBORU_S_PRIKAZY <VSTUPNI_SOUBOR|\n\n"
         "Prikazy:\n"
         "	-iCONTENT 		vlozi CONTENT pred aktualny riadok\n"
         "	-bCONTENT 		vlozi CONTENT na zaciatok aktualneho riadka\n"
         "	-aCONTENT 		vlozi CONTENT na koniec aktualneho riadka\n"
         "	-d 			vymaze aktualny riadok\n"
         "	-dN 			vymaze N riadkov (N > 0)\n"
         "	-r 			na aktualnom riadku zmaze znak konca riadku\n"
         "	-s PATTERN REPLACEMENT 	na aktualnom riadku nahradi PATTERN s REPLACEMENT\n"
         "	-s:PATTERN:REPLACEMENT 	na aktualnom riadku nahradi PATTERN s REPLACEMENT(lubovolny oddelovaci znak)\n"
         "	-S:PATTERN:REPLACEMENT 	na aktualnom riadku nahradi vsetky PATTERN s REPLACEMENT(lubovolny oddelovaci znak)\n"
         "	-n 			vytlaci aktualny riadok\n"
         "	-nN 			vytlaci N riadkov(N > 0)\n"
         "	-q 			ukonci editaciu\n"
         "	-gX			preskoc na X-ty prikaz a opakuj prikazy po tomto X-tom prikaze(1 <= X <= pocet prikazov)\n"
         "	-e 			prida na koniec aktualneho riadku znak konca riadku\n\n");
	return;
}

/*funkcia "commandExecution" vykonava prikazy ktore boli zadane v prikazovom subore
 */
void commandExecution(char contentArray[MAXline][MAXchar]) {
	int c, d, numberOfReps;
	char e;
	int i = 0, j;
	char actualLine[MAXchar] = {'\0'};
	bool gotoFlag = false;		//boolean na zistenie ci bolo zadane n
/*getLine nam nacita aktualny riadok
 */
	getLine(actualLine);
/*cyklus nam cykli pokial nam nedojdu nacitane prikazy z mainu
 */
	while(contentArray[i][0] != EOF && i <= MAXline){
/*premmenej c priradime znak na indexe [i][0] teda vzdy znak co je prvy v riadku
 */
		c = contentArray[i][0];
/*ak cyklus narazi na '\0' v subore, cyklus po znaku vytlaci zvysok riadkov 
 *zo stdin ktore neboli upravene
 */
		if (c == '\0') {
			puts(actualLine);		
			while((d = fgetc(stdin)) != EOF){
			putc(d, stdout);
			}
			return;
		}
/*nasledne pomocou switchu na premmenej c prerozdelujeme ktore prikazy sa idu vykonat
 */
			switch(c){
				case 'a':
					appendContent(contentArray[i], actualLine);
					i++;
					break;
				case 'b':
					beforeContent(contentArray[i], actualLine);
					i++;
					break; 
				case 'i':
					insertContent(contentArray[i]);
					i++;
					break;
				case 'd':
/*kontrola ci je nieco v argumente prikazu 'd': ak tam nic nieje vymaze sa aktualny riadok
 *ked tam nieco bude kontroluje sa ci to je cislo ak je tak finckia isItNumber vrati dane cislo
 *naopak nam vyhodi chybu
 */
					if (isEmpty(contentArray[i])) {
						delete(0, actualLine);
					} 
					else {
						numberOfReps = isItNumber(contentArray[i]);
						if(numberOfReps > 0){
							delete(numberOfReps, actualLine);
						}
						else {
							fprintf(stderr, "Zle zadany argument prikazu 'n': V argumente nebolo zadane cislo\n");
							help();
							exit(1);
						}
					}
					i++;
					break;
				case 'r':
					removeEOL(actualLine);
					i++;
					break;
				case 's':
					substitution(contentArray[i], actualLine);
					i++;
					break;
				case 'S':
					substitution(contentArray[i], actualLine);
					i++;
					break;
				case 'n':
/*tak ako pri prikaze delete tak aj tu kontrola ci je nieco v arguemente atd
 */
					if (isEmpty(contentArray[i])) {
						next(0, actualLine);
					} 
					else {
						numberOfReps = isItNumber(contentArray[i]);
						if(numberOfReps > 0){
							next(numberOfReps, actualLine);
						}
						else {
							fprintf(stderr, "Zle zadany argument prikazu 'n': V argumente nebolo zadane cislo.\n");
							exit(1);
						}
				 	}
					i++;
					break;
				case 'q':
					return;
					break;
				case 'g':
/*Prevencia zacyklenia: cyklus hlada ci bolo pred tym zadane n, ak hej tak pokracuje v goto
 */
					j = i;
					while(j >= 0){
						e = contentArray[j][0];
						if(e == 'n'){
							gotoFlag = true;
						}
						j--;
					}
/*kontrola ci nieco je v arguemente ak tam nic nieje vyhodi chybu, nasledne zistuje ci arguement
 *je cislo ak hej funkcia ""isItNumber vracia cislo ktore pre spravne fungovanie kontrolujeme 
 *ci je vacsie ako 0
 */
					if(gotoFlag){
						if (isEmpty(contentArray[i])) {
							fprintf(stderr, "Zle zadany argument prikazu 'g': Argument prikazu 'GOTO' nebol zadany\n");
							exit(1);
						} 
						else {
							numberOfReps = isItNumber(contentArray[i]);
							if(numberOfReps < 0) {
								fprintf(stderr, "Zle zadany argument prikazu 'g': Argument musi byt vacsi alebo rovny jednej'n'\n");
								exit(1);
							}
						}
						if((i + 1) > numberOfReps){
							i = numberOfReps - 1;
							break;
						}
						else if((i + 1) == numberOfReps){
							fprintf(stderr, "Nastala chyba: Prikaz 'GOTO' odkazuje sam na seba(nekonecny cyklus)\n");
							exit(1);
						}
						else{
							if(isEmpty(contentArray[numberOfReps])){
								fprintf(stderr, "Nastala chyba: Prikaz GOTO odkazoval na neexistujuci prikaz\n");
								exit(1);
							}
							else {
								i = numberOfReps - 1;
								break;
							}
						}
					}
					else{
						fprintf(stderr, "Nastala chyba: Nekonecny cyklus\n");
						exit(1);
					}
				case 'e':
					appendEOL(actualLine);
					i++;
					break;
				default:
					fprintf(stderr, "Zadali ste neplatny prikaz: "
									"Pre spravne fungovanie programu pouzite podporovane prikazy z Napovedy\n");
					
					help();
					return;
				}
		
	}

/*cyklus po znaku vytlaci zvysok riadkov z stdin ktore neboli upravene
 *ak by v prikazoch bol vytlaceny cely input program tento cyklus ignoruje
 */
	while((d = fgetc(stdin)) != EOF){
		putc(d, stdout);
	}

	return;
}
/*-----------------------------------FUNCTIONS---------------------------------------*/


