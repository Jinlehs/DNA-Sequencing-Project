/*
Assignment 2
Author: Jin Lee 
Instructor: Guohui Lin 
Date: November 3, 2019
*/

/*directives: libraries that are in use in this program*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/*macros*/
#define max(a,b)  (((a) > (b)) ? (a) : (b)) //determines max value 

/*prototypes*/
void lcs(char P1[], char P2[], int a, int b);
void lts(char P1[], char P2[], int a, int b);
void lps(char P1[], char P2[], int a, int b);

/*global variable*/
int print_counter = 0; //counter to determine print sequence
int maxlen = 0;
int lts_counter = 0;
int P1len = 0;
char *maxar = NULL;

/*main function: user interface and first order error checking*/
int main(void) {

	int i = 0; //declare iterator variable for array1
	int n = 0; //declare iterator variable for array2

	//char input1[2048]; // oirginal implementation
	//char input2[2048]; // original implementation

	char *input1 = malloc(10001*sizeof(char)); //dynamically allocates memory 
	char *input2 = malloc(10001*sizeof(char)); //dynamically allocates memory 

	printf("Enter the first sequence: ");   
	
	//Note: Character '0' == '0\0' or 'A' == 'A\0'
	//scanf("%s", input1);//scan input as a string value (char array)
	fgets(input1,10001,stdin);
	while ((i < strlen(input1)) && input1[i] != '\n'){//condition: iterate as long as i is less that strlen
		if (isdigit(input1[i]))//condition: as long as input[i] is digit  
			//printf("%c", input1[i]); #checking purpose
			i++;
		else{
			printf("Error, non-digit character detected!\n\n");//FUNCTIONALITY 2
			main();//calling back to main function
		}
	} 

	printf("Enter the second sequence: ");
	//scanf("%s", input2);
	fgets(input2,10001,stdin);
	while ((n < strlen(input2)) && input2[n] != '\n'){//condition: iterate as long as i is less that strlen
		if (isdigit(input2[n]))//condition: as long as input[i] is digit  
			//printf("%c", input2[n]); #checking purpose
			n++;
		else{
			printf("Error, non-digit character detected!\n\n");//FUNCTIONALITY 2
			main();//calling back to main function
		}
	}

	printf("\n# Two input sequences (length = %d,%d) are: \n", i, n);
	printf("%s", input1);
	printf("%s\n", input2);
	lcs(input1, input2, i, n);
	printf("\n");
	lps(input1, input2, i, n); 
	printf("\n");
	lts(input1, input2, i, i);

	exit(0); //to prevent the program running when use intially inputs nondigit char 

	return 0;

}

/*creation of two dimensional array to find LCS length from Geeksforgeeks*/
void lcs(char *P1, char *P2, int a, int b) { 	
	//int L[a+1][b+1]; //original declaration of 2d array 

	int m = a+1, n = b+1, k; 
	int **L = (int**)malloc(m*sizeof(int*));//pointer to pointer that dynamically allocates memory for 2d array
	for (k=0; k<m; k++)
		L[k] = (int*)malloc(n*sizeof(int));

	for (int p = 0; p <= a; p++) { 
		for (int j = 0; j <= b; j++) {
			if (p == 0 || j == 0) //initialize 2d array by setting first column n row = 0 
				L[p][j] = 0; 
			else if (P1[p-1] == P2[j-1]) //since char of p,j in the arrays match we increment by 1 
				L[p][j] = L[p-1][j-1] + 1;
			else //otherwise, we find the max of square right above and to the left and set L[p][j] to it 
				L[p][j] = max(L[p-1][j], L[p][j-1]);
		}
	}
	int len = L[a][b]; //LCS length 
	//char *longest[len+1];  //original implementation 
	char *longest = malloc(10001*sizeof(char));//dynamically allocating memory for longest sequence 
	longest[len] = '\0';// essential for indicating termination of array
	//storing characters in lcs[] 

	while (a > 0 && b > 0) { 
		if (P1[a-1] == P2[b-1]){ //if char in X[] and Y[] are same, then part of LCS
			longest[len-1] = P1[a-1];
			len--; a--; b--; //move diagnoally up left
		} 
		else if (L[a-1][b] > L[a][b-1])
			a--; 
		else
			b--;
	}
		
	int lenlcs = strlen(longest); //length of lcs 
	
	if (print_counter == 0){ //if first iteration, print lcs
		printf("# an LCS (length = %d) is:\n%s\n", lenlcs, longest);
		print_counter++;
		P1len = strlen(P1); // len of the initial P1 array
	}
	else if (print_counter == 1){ //if second iteration, print lps
		printf("# an LPS (length = %d) is:\n%s\n", lenlcs, longest);
		print_counter++;
	}
	else { 
		if (print_counter > 2){ //if more than second iteration, access lts step 
			if (lenlcs > maxlen){ //if length of lcs is greater than the stored max length, store lcs length into maxlen and the array into maxar
				maxlen = lenlcs;
				maxar = longest;
			}
			lts_counter++; // iterates from 2 to the end of the array 
		}
		//printf("%d", lts_counter); //testcases
		//printf("%d", P1len);

		if (lts_counter >= P1len - 2){ //if we finish iterating through all the elements in P1
			printf("# an LTS (length = %d) is:\n%s%s\n", maxlen*2, maxar, maxar); //doubling up on maxar to output correct representation of lts
		}
		print_counter++;
	}
}

/*LPS: longest palindrome sequence - longest subsequence that exists going
left to right and right to left*/
void lps(char *P1, char *P2, int a, int b){

	char revP2[b];//declaring reversal array 
	int end = b; 
	int counter = 0; 

	//reversal of the array
	for(int i = end - 1; i >= 0; i--){
		revP2[counter] = P2[i];
		counter++;
	}
	//printf("\n%s\n", revP2); //test cases
	//printf("\n%s\n", revarray(&P2, b)); 

	lcs(P2, revP2, b, b); //calling on lcs with oirginal array and reversed array input
}

/*LTS: longest tandem sequence - longest subsequence that exists on 
both sides of a sequence*/
void lts(char *P1, char *P2, int a, int b) {

	char *rhs = malloc(10001*sizeof(char)); //dynamically allocate memory for rhs array 
	char *lhs = malloc(10001*sizeof(char)); //dynamically allocate memory for lhs array 

	for (int i = 0; i < a; i++){
		//partition lhs and rhs of array and input them into lcs to get the lcs 

		for (int n = 0; n < i; n++){ //put leftside of i index into lhs 
			lhs[n] = P1[n];
			
		}
		int p = 0; //declare rhs iterator

		for (int m = i; m <= a; m++){ //put rightside of i index into rhs
			rhs[p] = P1[m];
			p++; //increment rhs iterator
		}

		/*printf("%s",lhs); //test cases 
		printf("\n");
		printf("%s",rhs);
		printf("\n");
		*/

		lcs(lhs,rhs,strlen(lhs),strlen(rhs));
	}

}