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
/**
 * [funkcia zatvori subor s prikazmy a skontroluje ci sa zatvorili spravne]
 * @param subPrikazy [subor s prikazmy]
 */
void closeCommandFile(FILE** subPrikazy);

/**
 * [nacita aktualny riadok z INPUTU]
 * @param line [string na ulozenie riadka]
 */
void getLine(char *line);

/**
 * [funkcia otvori subor s prikazmy]
 * @param argument1  [description]
 * @param subPrikazy [description]
 */
void openCommandFile(char *argument1, FILE** subPrikazy);

/**
 * [nacita retazec z jednoho do druheho tak ze v druhom nenacita prvy znak]
 * @param content [upraveny string(destination)]
 * @param string  [upravovany string(source)]
 */
void removeCommandChar(char *content, char *string);

/**
 * [vytlaci CONTENT z prikazoveho suboru na jeden riadok]
 * @param string [cielovy retazec]
 */
void insertContent(char *string);

/**
 * [vytlaci CONTENT z prikazoveho subora pred aktualny riadok]
 * @param string [string pred upravou]
 * @param line   [string po uprave]
 */
void beforeContent(char *string, char *line);

/**
 * [zistuje ci v stringu su len cisla]
 * @param  string [description]
 * @return        [(-1) - ak najde inky znak ako cislo, (hodnota) - prevedene cislo z stringu]
 */
int isItNumber(char *string);

/**
 * [kontrola ci sa nieco nachadza v stringu]
 * @param  string [kontrolovany retazec]
 * @return        [false - ak v stringu nic nieje, true - ak v stringu nieco je]
 */
bool isEmpty(char *string);

/**
 * [precitanie stringu bez EOL zo stdin, pripaja na koniec string z prikazoveho subora(CONTENT)]
 * @param content [riadok s prikazom a s CONTENTOM]
 * @param line    [upraveny riadok s CONTENTOM]
 */
void appendContent(char *content, char *line);

/**
 * [nacitava riadok z prikazoveho subora bez EOL]
 * @param array      [vytiahnuty string (CONTENT)]
 * @param subPrikazy [subor s prikazmy]
 */
void getContent(char *array, FILE** subPrikazy);

/**
 * [odstrani EOL na aktualnom riadku]
 * @param line [upravovany aktualny riadok]
 */
void removeEOL(char *line);

/**
 * [nacita riadok z stdin ale nevytlaci ho(vymaze sa)]
 * @param n    [pocet riadkov na vymazanie]
 * @param line [aktualny riadok]
 */
void delete(int n, char *line);

/**
 * [tlaci 'n' aktualnych riadok]
 * @param n    [pocet riadkov]
 * @param line [aktualny riadok]
 */
void next(int n, char *line);

/**
 * [prida na aktualny riadok EOL]
 * @param line [aktualny riadok]
 */
void appendEOL(char *line);

/**
 * [funkcia "substitution" pracuje pre prikazy s a S. V pripade prikazu "s" hlada v aktualnom
   riadku jeden retazec "CONTENT" ak ho najde nahradi ho s "REPLACEMENT" Prikaz "S" pracuje
   analogicky ako u prikazu "s", len substituciu vykonava pre vsetky vyskyty "CONTENT"]
 * @param string [description]
 * @param line   [description]
 */
void substitution(char *string, char *line);

/**
 * [vykonava prikazy ktore boli zadane v prikazovom subore]
 * @param contentArray [2d pole textu z upravovaneho subora]
 */
void commandExecution(char contentArray[MAXline][MAXchar]);

/**
 * [funkcia vytlaci napovedu]
 */
void help();
/*----------------------------------PROTOTYPES---------------------------------------*/

/*-------------------------------------MAIN------------------------------------------*/
/**
 * [kontorla argumentov, praca so suborom, volanie prikazov]
 * @param  argc [pocet argumentov]
 * @param  argv [pointer na argumenty]
 * @return      [EXIT_FAILURE(1) - chyba, EXIT_SUCCES(0) - uspech]
 */
int main(int argc, char *argv[])
{
	FILE *subPrikazy;
	char choice; 							//napovedy
	int c;									//nacitanie znakov z prik. subora		
	int numberOfCommand = 0;				//inkrementacia v cykle
	char contentArray[MAXline][MAXchar];	//2d pole do ktoreho sa uklada subor s prikazmy

	if (argc < 2)	/*malo argumentov*/
	{		
		fprintf(stderr, "Zadali ste malo argumentov: Pre spravne fungovanie programu si precitajte napovedu\n"
						"Chcete otvorit napovedu? (y/n)\n");
		scanf(" %c", &choice);
		if(choice == 'y')
		{
			help();
			return EXIT_FAILURE;
		}
		else 
			return EXIT_FAILURE;
	}
	else if (argc == 2)	/*zadany spravny pocet argumentov*/
	{
		openCommandFile(argv[1], &subPrikazy);	/*funkcia na otvorenie suboru s prikazmy*/
		while((c = getc(subPrikazy)) != EOF)	/*cyklus nacitava charaktery zo suboru s prikazmy a nacitava ich do */
		{										/*2d arrayu prikazy uklada do prveho indexu 2d pola*/
			contentArray[numberOfCommand][0] = c;
			getContent(contentArray[numberOfCommand], &subPrikazy);
			if(numberOfCommand < MAXline)
			{
				numberOfCommand++;
			}
			else
			{
				fprintf(stderr, "Zadali ste prilis  vela prikazov, program podporuje len 100 prikazov\n");
				return EXIT_FAILURE;
			}
		}
	
	closeCommandFile(&subPrikazy);
	commandExecution(contentArray);		/*vykonanie prikazov*/

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

void closeCommandFile(FILE** subPrikazy)
{
    if (fclose(*subPrikazy) == EOF)
    {
        fprintf(stderr, "Subor s prikazmy sa nepodarilo zatvorit\n");
        exit(1);
    }
	return;
}	

void getLine(char *line) {
	int i = 0; 			
	int c;				//nacitanie charov

	while((c = fgetc(stdin)) != '\n')	/*cyklus nacitava charaktery zo stdin a uklada do retazca "line"*/
	{
		if (c == EOF)
		{
			exit(1);
		}
		line[i] = c;
		i++;
	}
	line[i] = '\0';		/*posledny char v retazci mus byt '\0' */
	return;
}

void openCommandFile(char *nazovSubora, FILE** subPrikazy)
{
	char choice;	//znak na zobrazenie napovedy
	if((*subPrikazy = fopen(nazovSubora, "r")) == NULL)
	{
		fprintf(stderr, "Subor s prikazmy sa nepodarilo otvorit\n"
						"Chcete otvorit napovedu? (y/n)\n");
		scanf("%c", &choice);
		if(choice == 'y')
		{
			help();
			exit(1);
		}
		else
		{ 
			exit(1);
		}
	}
	return;
}

void removeCommandChar(char *content, char *string)
{
	unsigned i;				//unsigned pretoze porovnavame s tym strlen

	for(i = 1; i < (strlen(string)); i++)
	{
		content[i - 1] = string[i];
	}
	return;
}

void insertContent(char *string)
{
	char content[MAXchar];		//array pre nacitanie CONTENTU bez prikazu
	int length = 0;				//pocet charakterov

	removeCommandChar(content, string);	/*odstrani prvy znak*/
	length = strlen(content);
	content[length] = '\0';
	puts(content);
	return;
}

void beforeContent(char *string, char *line)
{ 
	char editedArray[MAXchar] = {'\0'};		//pomocna array na nacitanie upraveneho stringu
	int count;								//pocet charakterov v editedArray
	unsigned totalLineLength, i;			//dlzky stringov

	removeCommandChar(editedArray, string);	/*zbavime sa prveho znaku z prikazoveho riadka a znova ho ukoncime s '\0'*/
	count = strlen(editedArray);
	editedArray[count] = '\0';
	totalLineLength = ((strlen(editedArray)) + (strlen(line)));	/*vysledna dlzka riadka po uprave*/

	if(totalLineLength < MAXchar)
	{
		strcat(editedArray, line);		/*pripojime CONTENT*/
		for(i = 0; i <= totalLineLength; i++)
		{
			line[i] = editedArray[i];	/*prehodime znaky z editedArray do aktualneho riadka*/
		}
	}
	else
	{
		fprintf(stderr, "V prikaze nastala chyba: Vysledny riadok obsahoval prilis vela znakov\n");
		exit(1);
	}
	return;
}

int isItNumber(char *string)
{
	char contentArray[MAXchar];	//array len na nacitanie CONTENTU	
	int value;					//hodnota cisla za prikazom
	unsigned i;					//porovnanie s dlzkou stringu
	char *ptr;					//pomocny pointer pre funkciu strtol

	for(i = 1; i < strlen(string); i++)
	{
		if(!isdigit(string[i]))
		{
			return -1;
		}
	}
	removeCommandChar(contentArray,string);
	value = strtol(contentArray, &ptr ,10);
	return value;
}

bool isEmpty(char *string)
{
	unsigned i;		//porovnanie s dlzkou stringu

	for (i = 1; i < strlen(string); i++) {
		if (!isspace(string[i])) {
			return false;
		}
	}
	return true;
}

void appendContent(char *content, char *line)
{ 
	char contentArray[MAXchar] = {'\0'};				//array lem na nacitanie CONTENTU
	removeCommandChar(contentArray, content);

	if((strlen(line)+(strlen(contentArray))) < MAXchar)	/*ak je dlzka upraveneho riadka mensia ako 1000 napoji CONTENT na koniec aktualneho riadka */
	{ 	
		strcat(line, contentArray);
	}
	else
	{
		fprintf(stderr, "V prikaze nastala chyba: Vysledny riadok bol prilis dlhy!\n");
		exit(1);
	}
	return;
}

void getContent(char *array, FILE** subPrikazy)
{
	int c;			//sluzi na nacitanie charakterov z prik. subora
	int i = 1;		//int na inkrementaciu

	while((c = getc(*subPrikazy)) != '\n' && c != EOF)
	{

		if(i < MAXchar)
		{
			array[i] = c;
			i++;
		}
		else
		{
			fprintf(stderr, "V prikazovom subore nastala chyba: Riadok bol prilis dlhy!\n");
			closeCommandFile(subPrikazy);
			exit(1);
		}
	}
	array[i] = '\0';
	return;
}

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

void delete(int n, char *line) {     
				  
	int i = 0;
	do{
		getLine(line);
		i++;
	} while(i < n);
	return;
}

void next(int n, char *line) { 			 
	int i = 0;							
	do {
		puts(line);
		getLine(line);
		i++;
	}while(i < n);
	return;
}

void appendEOL(char *line){
	char *EOL = "\n";
	strcat(line, EOL);
	return;
}

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

	/*kontrola ci bol zadany oddelovaci znak na mieste po prikaze (v mojomoddelovacie znaky su ' ' a ':')
 	*ak by tam tento znak nebol vyhodi chybu*/
 	if(string[2] == string[1])
 	{
			fprintf(stderr, "Zle zadany argument prikazu 's': PATTERN nebol zadany\n");
			exit(1);
		}
		else{
			/*v cykle sa zbavime oddelovacieho znaku*/
			for(i = 2; i < (strlen(string)); i++)
			{
				contentArray[i - 2] = string[i];
			}
			/*strchr vrati pointer na prvy vyskyt oddelovacieho znaku -> replacement pointer*/
			replacementPtr = strchr(contentArray, string[1]);
			if(replacementPtr == NULL)
			{
				fprintf(stderr, "Zle zadany argument prikazu 's': Nebol zadany REPLACEMENT\n");
				exit(0);
			}
			else
			{
				/*v cykle ziskame iba pattern z contentarray*/
				while((contentArray[j]) != string[1])
				{	
					patternArray[j] = contentArray[j];
					j++;
				}	
			}
		}	
	/*kedze replacement pointer zacina od oddelovacieho znaku ktory v cielovej uprave
 	 *nechceme tak pouzivame znova funkciu na zbavenie sa prveho znaku v retazci*/
	removeCommandChar(replacementArray, replacementPtr);
	/*nasledne si vypocitam dlzku replacementu a aktualneho riadka*/
	replacementLength = strlen(replacementArray);
	lineLength = strlen(line);
	/*funcia strstr nam najde vyskyt retazca v druhom retazci, vracia pointer na 
 	*tento vyskyt. Ak tento vyskyt nenajde program sa vracia z funkcie a pokracuje dalej*/
	substitutionPtr = strstr(line, patternArray);

	if (substitutionPtr == NULL)
	{
		return;
	}
	indexBegRep = (strlen(line)) - (strlen(substitutionPtr));
	patternLength = strlen(patternArray);

	/*v prvom cykle sa zapisuju znaky do pomocneho retazca a to pokial sa nedostanem na index
 	*patternu*/
	while(k < indexBegRep) 
	{
		smallEditedArray[k] = line[k];
		k++;
	}

	/*v cykle zapisujeme do pomocneho retazca pokial sa nedostaneme na dlzku replacementu a indexu patternu*/
	int replacementIndex = 0;
	while(k < (replacementLength + indexBegRep))
	{
		smallEditedArray[k] = replacementArray[replacementIndex];
		k++;
		replacementIndex++;
	}
	/*inicializujeme si index poslednej casti ktora bola za CONTENTOM a v cykle cyklime 
 	*pokial nedosiahneme dlzku REPLACEMENTU a LINU */
	int endLineIndex = indexBegRep + patternLength;
	while(k < (replacementLength + lineLength))
	{
		smallEditedArray[k] = line[endLineIndex];
		k++;
		endLineIndex++;
	}

	/*Test ci na prvej pozicii je velke S(prikaz S), ak hej tak substitucia pokracuje pre vsetky
 	*najdene retazce "CONTENT" v aktualnom riadku*/
	if(string[0] == 'S')
	{
		/*test ci nie je CONTENT a REPLACEMENT ten isty retazec, ak su rovnake tak program
 		*preskoci zbytocny cyklus*/
		if((strcmp(patternArray,replacementArray)) != 0)
		{
			repSubstitutionPtr=strstr(smallEditedArray, patternArray);
			/*cyklus prehladava riadok, ak najde CONTENT nahradi ho s REPLACEMENT.Ak nahradi 
 			*vseky vyskyty CONTENT tak skonci. Pre substituciu pouzivam cykli ktore som 
 			*uz popisal vyssie.*/
			do {
				smallArrLenght = strlen(smallEditedArray);
				indexBegRep = (smallArrLenght) - (strlen(repSubstitutionPtr));
				while(n < indexBegRep)
				{
					bigEditedArray[n] = smallEditedArray[n];
					n++;
				}
				replacementIndex = 0;
				while(n < (replacementLength + indexBegRep))
				{
					bigEditedArray[n] = replacementArray[replacementIndex];
					n++;
					replacementIndex++;
					}
					endLineIndex = indexBegRep + patternLength;
				while(n < (replacementLength + smallArrLenght))
				{
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
		else
		{
			return;
		}
	}

	if((strlen(smallEditedArray)) < MAXchar)
	{
		/*v cykle prehodime z pomocnej arrayi do linu*/
		while(l <= (strlen(smallEditedArray)))
		{
			line[l] = '\0';
			line[l] = smallEditedArray[l];
			l++;
		}
		return;
	}
	else
	{
		fprintf(stderr, "V prikaze nastala chyba: 'Riadok po substituci bol prilis dlhy'");
		exit(1);
	}
}

void help() 
{
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

void commandExecution(char contentArray[MAXline][MAXchar])
{
	int c, d, numberOfReps;
	char e;
	int i = 0, j;
	char actualLine[MAXchar] = {'\0'};
	bool gotoFlag = false;		//kontrola ci bolo zadane n

	getLine(actualLine);	/*getLine nam nacita aktualny riadok */
	while(contentArray[i][0] != EOF && i <= MAXline)	/*cyklus pokial nam nedojdu nacitane prikazy z mainu */
	{
		c = contentArray[i][0];		/*premmenej c priradime znak na indexe [i][0] teda vzdy znak co je prvy v riadku */
		if (c == '\0')				/*ak cyklus narazi na '\0' v subore, po znaku tlaci zvysok riadkov zo stdin ktore neboli upravene*/
		{
			puts(actualLine);		
			while((d = fgetc(stdin)) != EOF){
			putc(d, stdout);
			}
			return;
		}
			switch(c)	/*prerozdeluje ktore prikazy budu vykonavane*/
			{	
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
					if (isEmpty(contentArray[i]))	/*ak v argumente 'd' nic nieje vymaze sa aktualny riadok ked tam nieco je kontroluje sa ci to je cislo*/
					{
						delete(0, actualLine);
					} 
					else
					{
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
					if (isEmpty(contentArray[i]))	/*ako pri prikaze delete tak aj tu kontrola ci je nieco v argumente atd*/
					{
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
					j = i;
					while(j >= 0)	/*Prevencia zacyklenia: cyklus hlada ci bolo pred tym zadane n, ak hej tak pokracuje v goto*/
					{
						e = contentArray[j][0];
						if(e == 'n'){
							gotoFlag = true;
						}
						j--;
					}

					/*kontrola ci nieco je v arguemente ak tam nic nieje vyhodi chybu, nasledne zistuje ci arguement
					 *je cislo ak hej funkcia ""isItNumber vracia cislo ktore pre spravne fungovanie kontrolujeme 
 					 *ci je vacsie ako 0*/
					if(gotoFlag)
					{
						if (isEmpty(contentArray[i])) 
						{
							fprintf(stderr, "Zle zadany argument prikazu 'g': Argument prikazu 'GOTO' nebol zadany\n");
							exit(1);
						} 
						else 
						{
							numberOfReps = isItNumber(contentArray[i]);
							if(numberOfReps < 0) {
								fprintf(stderr, "Zle zadany argument prikazu 'g': Argument musi byt vacsi alebo rovny jednej'n'\n");
								exit(1);
							}
						}
						if((i + 1) > numberOfReps)
						{
							i = numberOfReps - 1;
							break;
						}
						else if((i + 1) == numberOfReps)
						{
							fprintf(stderr, "Nastala chyba: Prikaz 'GOTO' odkazuje sam na seba(nekonecny cyklus)\n");
							exit(1);
						}
						else
						{
							if(isEmpty(contentArray[numberOfReps]))
							{
								fprintf(stderr, "Nastala chyba: Prikaz GOTO odkazoval na neexistujuci prikaz\n");
								exit(1);
							}
							else
							{
								i = numberOfReps - 1;
								break;
							}
						}
					}
					else
					{
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
	while((d = fgetc(stdin)) != EOF)
	{
		putc(d, stdout);
	}
	return;
}
/*-----------------------------------FUNCTIONS---------------------------------------*/

