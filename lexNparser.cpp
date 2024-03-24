#include <iostream>
#include <string>
#include <string.h>
#include <fstream>

using namespace std;

/* variables */
int charClass;
char lexeme[100];
char nextChar;
int lexLength;
int token;
int nextToken;
fstream fvar;

/* Function declarations */

/*Lex analyzer/parser functions*/
void addCh();
void getCh();
void getNonBlank();
int lex();
void expr();
void term();
void exprTail();
void termTail();
void factor();
void err();


/* Chracater classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token Codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define TERMINATOR -1

/***********************************************************************/
/* MAIN FUNCTION*/
/* file will be opened HERE*/

int main(){
	char ch = ' ';
	string filename = "C:\\Braxton\\testing.txt"; // <--- put YOUR file name here (use DOUBLE (\\) slashes)
    fvar.open(filename.c_str());
    if(!fvar){
    	cout << "Error - cannot open " + filename + ". File doesnt exist." << endl;
    }
	else{
		cout << "success!...begin lexical analysis...\n\nSTART\n-----------\n";
		getCh();
		do{
		lex(); //call analyzer
		expr(); //parser function
		}while(nextToken != TERMINATOR);	
		cout << "\n-----------\nFINISH" << endl;
	}
	return 0;
}

/***********************************************************************/

/* lookup - This function will determine if characters
are integers, operators or parentheses and return the token */
int lookup(char ch) {
    switch (ch) {
        case '(':
            addCh();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addCh();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addCh();
            nextToken = ADD_OP;
            break;
        case '-':
            addCh();
            nextToken = SUB_OP;
            break;
        case '*':
            addCh();
            nextToken = MULT_OP;
            break;
        case '/':
            addCh();
            nextToken = DIV_OP;
            break;
        default:
            addCh();
            nextToken = MULT_OP;
            break;
    }
    return nextToken;
}
/* addCh - This function will add whatever nextChar is to lexeme array */
void addCh() {
	//check length of lexeme
    if (lexLength <= 98) {
        lexeme[lexLength++] = nextChar;
        lexeme[lexLength] = 0;
    } 
	else
    cout << "Error - lexeme is too long" << endl;
}
/* getCh - This function classifies if a value is a Letter, number or other */
void getCh() {
	nextChar = fvar.get();
    if (nextChar != EOF) {	
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar)){
            charClass = DIGIT;
    	}
        else
            charClass = UNKNOWN;
    } else
        charClass = EOF;
}

/* getNonBlank - This function will continue to call getCh() until nextChar isnt a space */
void getNonBlank() {
    while(isspace(nextChar))
        getCh();
}

/* lex - This function serves as the overall analyzer */
int lex(){
lexLength = 0;
getNonBlank();

	switch (charClass){
	/* Parse identifiers */
		case LETTER:
			addCh();
			getCh();
			while(charClass == LETTER || charClass == DIGIT){
				addCh();
				getCh();
			}
			nextToken = IDENT;
			break;
		/* Parse integer literals */
		case DIGIT:
			addCh();
			getCh();
			while(charClass == DIGIT){
				addCh();
				getCh();
			} // end of while loop
		nextToken = INT_LIT;
		break;
		/* Parentheses and operators */
		case UNKNOWN:
			lookup(nextChar);
			getCh();
		break;
		/* EOF */
		case EOF:
		nextToken = EOF;
		lexeme[0] = 'E';
		lexeme[1] = 'O';
		lexeme[2] = 'F';
		lexeme[3] = 0;
		break;
} /* End of switch */


cout << "Next token is: " << nextToken << ", Next lexeme is: " << lexeme << endl;
	return nextToken;
} /* End of function lex */


/******************************************************/
/* GRAMMAR for Parser
<Expr>        -> <Term> <ExprTail>
<ExprTail>     ->  + <Term> <ExprTail>
                  | - <Term> <ExprTail>
                  | e
<Term>         -> <Factor> <TermTail>
<TermTail>    -> * <Factor> <TermTail>
                  | / <Factor> <TermTail>
                 | e
<Factor>    -> NUMBER
                   | ( <Expr>)

/*****************************************************/
/* expr - parses strings in the language generated by the rule:
* <Expr> -> <Term> <ExprTail>
*/
void expr() {
	cout << "Enter <expr>" << endl;
	/* parse the first term*/
	term();
	exprTail();
	cout << "Exit <expr>" << endl;
}
/*****************************************************/
/* exprTail - parses strings in the language generated by the rule:
* <ExprTail> -> + <Term> <ExprTail> | - <Term> <ExprTail> | e
*/
void exprTail() {
	cout << "Enter <exprtail>" << endl;
	/*If the next token is * or /, get the next token and parse the next factor */
	if(nextToken == ADD_OP || nextToken == SUB_OP) {  
	   lex();
	   factor();
	   exprTail();
	}
	else
	cout << "Exit <exprtail>" << endl;
}

/*****************************************************/
/* term - parses strings in the language generated by the rule:
* <Term> -> <Factor> <TermTail>
*/
void term() {
	cout << "Enter <term>" << endl;	
	/*Parse first factor*/
	factor();
	termTail();
	cout << "Exit <term>" << endl;
}
	
/*****************************************************/
/* termTail - parses strings in the language generated by the rule:
* <TermTail> -> * <Factor> <TermTail> | / <Factor> <TermTail> | e
*/
void termTail() {
	cout << "Enter <termtail>" << endl;
	/*If the next token is * or /, get the next token and parse the next factor */
	if(nextToken == MULT_OP || nextToken == DIV_OP) {  
	   lex();
	   factor();
	   termTail();
	}
	else
	cout << "Exit <termtail>" << endl;
}

/*****************************************************/
/* factor - parses strings in the language generated by the rule:
* <Factor> -> NUMBER | ( <Expr> )
*/
void factor() {
cout << "Enter <factor>" << endl;
	if (nextToken == LEFT_PAREN) {
		lex();
		expr();
		if (nextToken == RIGHT_PAREN)
			lex();
		else
			err();
	} /* End of if (nextToken == ... */
	else if(nextToken == INT_LIT){
			lex();
	}
	else
		err(); /* End of else */
	cout << "Exit <factor>" << endl;
 /* End of function factor */
} 
/* err function for when all else fails...or if input is improper */
void err(){
cout << "Error with input, please check and try again";
exit(1);
}