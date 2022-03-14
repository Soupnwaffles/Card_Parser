#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "card.h"

/*
 * I've left these definitions in from the
 * solution program. You don't have to
 * use them, but the `dupe_check()` function
 * unit test expects certain values to be
 * returned for certain situations!
 */
#define DUPE -1
#define NO_DUPE -2

/*
 * These are the special strings you need to
 * print in order for the text in the terminal
 * to be bold, italic or normal (end)
 */
#define BOLD "\e[1m"
#define ITALIC "\e[3m"
#define END "\e[0m"

/*
 * You will have to implement all of these functions
 * as they are specifically unit tested by Mimir
 */
int dupe_check(unsigned, char*);
char *fix_text(char*);
void free_card(CARD_T*);
CARD_T *parse_card(char*);
void print_card(CARD_T*);

int comparator(const void *a, const void *b){
	char *a_key = (char *) a; 
	CARD_T *b_key = *(CARD_T **)b;	
	return strcmp(a_key, b_key->name); 


}
/*
 * We'll make these global again, to make
 * things a bit easier
 */
CARD_T **cards = NULL;
size_t total_cards = 0;

int main(int argc, char **argv) {
	char *lineptr = NULL; 
	size_t n = 0; 
	int read_bytes = 0; 

	FILE *infile = fopen("hscards.csv", "r"); 
	if (infile == NULL) {return -2; }

	getline(&lineptr, &n, infile);
       // sizeo f CARD_T*???	
	CARD_T *result_card = malloc(sizeof(CARD_T)); 
	//       2. Read lines from the file...
	//          a. ft
	//          or each line, `parse_card()`
    //          b. add the card to the array
    	if ((read_bytes = getline(&lineptr, &n, infile))> 0){
		printf("READ_BYTES: %d\n", read_bytes); 
	
	}
	cards = malloc(sizeof(CARD_T*) *2 ); 
	cards[0] = malloc(sizeof(CARD_T)); 
	cards[1] = malloc(sizeof(CARD_T)); 
	cards[0]->id = 2222; 
	cards[0]->name = "File"; 
	cards[1]->id = 2212; 
	cards[1]->name = "Heyo"; 
	char *bae = "Heyo"; 
	unsigned ran = 2222; 
	total_cards += 2; 
	int ree = dupe_check(ran, bae); 
	printf("%d\n", ree); 
	//       3. Sort the array
	//       4. Print and free the cards
	//       5. Clean up!
	//free(cards[0]->name); 
	free(cards[0]); 
        free(cards[1]); 	
	free(cards); 
	free(result_card);
        free(lineptr); 	
	fclose(infile); 
	return 0;
}

/*
 * This function has to return 1 of 3 possible values:
 *     1. NO_DUPE (-2) if the `name` is not found
 *        in the `cards` array
 *     2. DUPE (-1) if the `name` _is_ found in
 *        the `cards` array and the `id` is greater
 *        than the found card's id (keep the lowest one)
 *     3. The last case is when the incoming card has
 *        a lower id than the one that's already in
 *        the array. When that happens, return the
 *        index of the card so it may be removed...
 */
int dupe_check(unsigned id, char *name) {
	CARD_T **resultpp = lfind(name, cards, &total_cards, sizeof(CARD_T *), comparator);   
	int result_val = 0; 
	if (resultpp != NULL){
		CARD_T *result = *resultpp; 
		if (result->id < id){
			result_val = DUPE; 
		}
		else{
			//NEED TO REPLACE OLD CARD W/ NEW 
			//WAY TO HOLD PLACE STILL NEEDED
			result_val = 0; 
		}
		printf("Result is: %s\n", result->name); 
	
	}
	else{
		printf("Result not found:\n"); 
		result_val = NO_DUPE; 
	}
        
		
	 	
	return result_val;
}

/*
 * This function has to do _five_ things:
 *     1. replace every "" with "
 *     2. replace every \n with `\n`
 *     3. replace every </b> and </i> with END
 *     4. replace every <b> with BOLD
 *     5. replace every <i> with ITALIC
 *
 * The first three are not too bad, but 4 and 5
 * are difficult because you are replacing 3 chars
 * with 4! You _must_ `realloc()` the field to
 * be able to insert an additional character else
 * there is the potential for a memory error!
 */
char *fix_text(char *text) {
	return NULL;
}

/*
 * This short function simply frees both fields
 * and then the card itself
 */
void free_card(CARD_T *card) {

}

/*
 * This is the tough one. There will be a lot of
 * logic in this function. Once you have the incoming
 * card's id and name, you should call `dupe_check()`
 * because if the card is a duplicate you have to
 * either abort parsing this one or remove the one
 * from the array so that this one can go at the end.
 *
 * To successfully parse the card text field, you
 * can either go through it (and remember not to stop
 * when you see two double-quotes "") or you can
 * parse backwards from the end of the line to locate
 * the _fifth_ comma.
 *
 * For the fields that are enum values, you have to
 * parse the field and then figure out which one of the
 * values it needs to be. Enums are just numbers!
 */
CARD_T *parse_card(char *line) {
	return NULL;
}

/*
 * Because getting the card class centered is such
 * a chore, you can have this function for free :)
 */
void print_card(CARD_T *card) {
	printf("%-29s %2d\n", card->name, card->cost);
	unsigned length = 15 - strlen(class_str[card->class]);
	unsigned remainder = length % 2;
	unsigned margins = length / 2;
	unsigned left = 0;
	unsigned right = 0;
	if (remainder) {
		left = margins + 2;
		right = margins - 1;
	} else {
		left = margins + 1;
		right = margins - 1;
	}
	printf("%-6s %*s%s%*s %9s\n", type_str[card->type], left, "", class_str[card->class], right, "", rarity_str[card->rarity]);
	printf("--------------------------------\n");
	printf("%s\n", card->text);
	printf("--------------------------------\n");
	printf("%-16d%16d\n\n", card->attack, card->health);
}

