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
   	CARD_T *result_card = NULL; 
	//       2. Read lines from the file...
	//          a. ft
	//          or each line, `parse_card()`
    //          b. add the card to the array
    	read_bytes = getline(&lineptr, &n, infile); 
    	do{
    	if (read_bytes >0){
		result_card = parse_card(lineptr); 
		if (result_card != NULL){
			cards = realloc(cards, sizeof(CARD_T*) *(total_cards + 1)); 
			cards[total_cards] = malloc(sizeof(CARD_T)); 
			memmove(cards[total_cards], result_card, sizeof(CARD_T)); 
			total_cards +=1; 	
		}
   	}
	}while ((read_bytes=getline(&lineptr,&n,infile))>0); 
	 

 
	//       3. Sort the array
	//       4. Print and free the cards
	//       5. Clean up!
	//free(cards[0]->name); 
	printf("total cards is %ld\n", total_cards); 
	for (int j = 0; j<total_cards; j++){
		//printf("freed is %d\n", j); 
		free_card(cards[j]); 
	}
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
			//No need to replace, get rid of new id and name
			result_val = DUPE;
			printf("NO NEW, already exists\n"); 
			printf("result is %s\n", result->name); 
		}
		else{
			//NEED TO REPLACE OLD CARD W/ NEW 
			//WAY TO HOLD PLACE STILL NEEDED
			result_val = 0; 
			printf("NO NEW need replace\n"); 
			printf("result is %s\n", result->name); 
		}
	}
	else{
		// Need to add card	 
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
	char *substring = "</b>"; 
	char *alt_text = text; 
	
	char *replace = strstr(alt_text, substring); 
	
	char *new_text = NULL; 	
	if (replace != NULL){
	do {
		memmove(replace,END,strlen(END)); 
		replace = strstr(alt_text,substring); 
	
	} while(replace != NULL); 	
	}
	 
	//next string to replace
	substring = "</i>"; 
	replace = strstr(alt_text,substring); 
	if (replace != NULL){
	do { 
		memmove(replace, END, strlen(END)); 
		replace = strstr(alt_text, substring); 
	} while(replace != NULL); 
	}
	substring = "\\n";
        replace = strstr(alt_text, substring);
	if (replace != NULL){
	do {
		memmove(replace + strlen(substring)-1,replace + strlen(substring), strlen(replace) - strlen(substring)); 
		memmove(replace, "\n", 1); 
	        replace = strstr(alt_text, substring); 	
	} while (replace != NULL); 
	}

	substring = "\"\""; 
        replace = strstr(alt_text, substring);
	int tempcount = 0; 
	if (replace != NULL){
	do {
		memmove(replace + strlen(substring)-1, replace + strlen(substring), strlen(replace) -strlen(substring)); 
		strncpy(alt_text + strlen(alt_text)-1, "", 1); 
		memmove(replace, "\"", 1);
		
		replace = strstr(alt_text, substring); 	
		tempcount+=1; 
	 
	} while (replace != NULL); 	
	}
	 
	//HARD PART, adding characters, requires realloc
	substring = "<b>";  
       	//allocate new text space
	size_t counter = strlen(alt_text); 
	 
	//new_text = realloc(new_text, counter *sizeof(char) );
	new_text = realloc(new_text, counter+1); 
	 
	  
	//new_text = strncpy(new_text, alt_text, counter-1);
	new_text = strncpy(new_text, alt_text, counter+1); 
	 
	replace = strstr(new_text, substring); 	 	
        if (replace != NULL){
       		do {
			counter += 1;  
			//realloc more space
			new_text = realloc(new_text, counter+1);
		        	
			//Initialize new text values? 
			strncpy(new_text+counter-1, "", 2); 
		
			replace = strstr(new_text, substring);  	
			memmove(replace + (strlen(substring)+1),
				replace+strlen(substring), 
				strlen(replace)-strlen(substring)); 

			
			memmove(replace, BOLD, strlen(BOLD));
			
			replace = strstr(new_text, substring); 

		} while (replace != NULL); 	
        }	 
   	
        substring = "<i>";
        counter = strlen(new_text); 
	
	replace = strstr(new_text, substring); 
	
	if (replace != NULL){
		do{
			counter += 1; 
			new_text = realloc(new_text, counter+1); 
			strncpy(new_text+counter-1, "", 2); 
			replace = strstr(new_text, substring); 

			memmove(replace + (strlen(substring)+1), 
				replace + strlen(substring), 
				strlen(replace)-strlen(substring)); 

			memmove(replace, ITALIC, strlen(ITALIC)); 
			replace = strstr(new_text, substring); 
		
		}while (replace != NULL); 
	
	}	

	//alt_text = memmove(alt_text, new_text, strlen(new_text)); 
	//strncpy(alt_text, new_text, strlen(new_text)+1);  
	//alt_text = strsep(&new_text, "");  
	alt_text = new_text;
	//alt_text = new_text; 
        //free(new_text); 	
	printf("ALT TEXT: %s\n", alt_text); 
	return alt_text; 
}

/*
 * This short function simply frees both fields
 * and then the card itself
 */
void free_card(CARD_T *card) {
	//free(card->name);
	free(card->name); 
        if (strlen(card->text) != 0){	
        free(card->text); 
	}
	free(card); 	
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
	CARD_T * parsedcard = NULL; 
	char *stringp = line; 
	unsigned id = atoi(strsep(&stringp, ",")); 
	stringp++; 
	char *name = strsep(&stringp, "\""); 
	stringp++; 
	char *token; 
	
	int result = dupe_check(id, name); 
	
	if (result == DUPE){
		//Do nothing, toss out stuff 	
	}
	else if (result == NO_DUPE){
		parsedcard = realloc(parsedcard, sizeof(CARD_T)); 
		parsedcard->id = id; 
		parsedcard->name = strdup(name); 

		//Parse cost
		token = strsep(&stringp, ","); 
		if (strlen(token) != 0 ){
			parsedcard->cost = atoi(token); 
		}
		else{
			parsedcard->cost = 0; 
		}
		int length = strlen(stringp); 
		char *back ; 
		int comma_count = 0; 
		char *temp = ""; 	
		// Count 5 commas backwards from the end. 
		for (int i = 0; i<length; i++){
			back = &(stringp[(length-1)-i]);  
		       	if (back[0] == ','){
				comma_count += 1; 
			
			}
			if (comma_count ==5){
			break; 
			}
			
		}
		// Now back should point only to the end.....
	//	back -=1;
		//size_t index = back - stringp;
		//replace the \" where back points to with null pointer and save to token
	        strcpy(back,"");
		token = strsep(&stringp, "\0"); 
		if (total_cards > 3451){
			printf("test\n"); 
		}
		//increment back past the null pointer
		//Save the text into token and set stringp to after the text
	        back += 1;
	        //if (strcmp(stringp, back ) != 0){	
		//if (stringp != back){
		if (strlen(token)>0){
			
			//token = stringp; 
			//token = strdup(stringp);  
			 
			
			stringp = back;
			 
		//Fixtext for text in token
			printf("TOKEN BEFORE: %s\n", token); 
		//if (strlen(token) != 0){
			token = fix_text(token);
		//	printf("Strlen is %ld\n", strlen(token));
	       		parsedcard->text = strdup(token);   	 
			free(token);
			char *token; 
			token = strsep(&stringp, ",");  
	       		printf("text is: %s\n", parsedcard->text); 	
			printf("TOKEN: %s\n", token); 
		//}
		}
		else{
		//Perhaps allocate space here for text anyways, so it can be freed.  
		//temp = realloc(temp, sizeof(char));  
		//temp = ""; 
		//parsedcard->text = temp; 
		parsedcard->text = temp; 
		}
		//memmove(parsedcard->text, token, strlen(token));
			
	        //free(token); 	

	}
		
		//Take string length remaining, for i = 0 to strlen: 
		//Go backwards and strcmp(",") and if so add to count
		//Once counting 5 of them record the address
		//address found - current address = token
		//stringp = addressfound 
	 
	return parsedcard;
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

