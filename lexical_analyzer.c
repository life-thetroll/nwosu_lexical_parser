#include <iostream>
#include <cctype>

/* Global declarations */
/* Variables */

int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;

void addChar();
void getChar();
void getNonBlank();
int lex();

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OF 20
#define ADD_OP 21
#define SUB_OP 22 // Corrected typo: Changed 21 to 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

int main() {
    std::cout << "Enter your code: (type 'exit' to quit)\n";
    do {
        getChar();
        do {
            lex();
        } while (nextToken != EOF);
        std::cout << "\nEnter more code or type 'exit' to quit:\n";
    } while (std::cin.peek() != 'e' && std::cin.peek() != EOF);
    return 0;
}

/* lookup - a function to lookup operators and parentheses and return the token */

int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        default:
            addChar();
            nextToken = MULT_OP;
            break;
    }
    return nextToken;
}

void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else
        std::cout << "Error - lexeme is too long" << std::endl;
}

void getChar() {
    std::cin.get(nextChar);
    if (nextChar != '\n')
        std::cin.ignore(256, '\n'); // Ignore extra characters in the input buffer
    if (nextChar != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    } else
        charClass = EOF;
}

void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    }

    std::cout << "Next token is: " << nextToken << ", Next lexeme is " << lexeme << std::endl;
    return nextToken;
}
