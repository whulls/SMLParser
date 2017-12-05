// SMLParser.cpp : implement a compilier for SML
#include "stdafx.h"
#include "Token.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>
using namespace std;

// token name
enum TokenValue
{
	TOK_COMMA, TOK_DIV, TOK_END, TOK_EQUAL,
	TOK_FOR, TOK_ID, TOK_IF, TOK_INTEGER,
	TOK_DOUBLE, TOK_MINUS, TOK_MULT, TOK_PLUS,
	TOK_SEMICOLON, TOK_UNDEFINED, TOK_WHILE,
};
// token description
string TOKEN_VALUE_DESCRIPTION[] = {
	"TOK_COMMA", "TOK_DIV", "TOK_END", "TOK_EQUAL",
	"TOK_FOR", "TOK_ID", "TOK_IF", "TOK_INTEGER",
	"TOK_DOUBLE", "TOK_MINUS", 	"TOK_MULT", "TOK_PLUS",
	"TOK_SEMICOLON", "TOK_UNDEFINED", "TOK_WHILE",
};

// token to keep
string KEEP_TOKEN[] = {
	"abstype", "and", "andalso", "as",
	"case", "datatype", "do", "else",
	"end", "eqtype", "exception", "fn",
	"fun", "functor", "handle", "if",
	"in", "include", "infix", "infixr",
	"let", "local", "nonfix", "of",
	"op", "open", "orelse", "raise",
	"rec", "sharing", "sig", "signatrue",
	"struct", "structure", "then", "type",
	"val", "where", "while", "with",
	"withtype",
};
// set of keep tokens
static const std::set<string> KEEP_TOKEN_SET(KEEP_TOKEN, KEEP_TOKEN + sizeof(KEEP_TOKEN) / sizeof(KEEP_TOKEN[0]));

Token* token = new Token[NTOKEN];

int pritnToken(Token token) {
	string str = token.str;
	if (KEEP_TOKEN_SET.find(str) != KEEP_TOKEN_SET.end())
		return 0;
	cout << TOKEN_VALUE_DESCRIPTION[token.type] << ": " << token.str << "; Line: " << token.line << endl;
	return 1;
}

int lexer(char *path) {
	FILE *fin;
	int ch;
	char buf[256], *p;
	int n = 0;
	int linenum = 1;
	if ((fin = fopen(path, "r")) == NULL) {
		fprintf(stderr, "Can't open the file... :\n");
	}
	string line;
	ifstream infile(path);
	while (getline(infile, line)) {
		int iSize = line.size();
		//cout << iSize << endl;
		for (int i = 0; i < iSize; i++) {
			ch = line[i];
			if (ch <= ' ') continue;
			p = buf;
			//cout << ch << endl;
			switch (ch)
			{
			case '+':
				token[n].type = TOK_PLUS;
				token[n].line = linenum;
				token[n++].str = '+';
				break;
			case '-':
				token[n].type = TOK_MINUS;
				token[n].line = linenum;
				token[n++].str = '-';
				break;
			case '*':
				token[n].type = TOK_MULT;
				token[n].line = linenum;
				token[n++].str = '*';
				break;
			case '/':
				token[n].type = TOK_DIV;
				token[n].line = linenum;
				token[n++].str = '/';
				break;
			case ';':
				token[n].type = TOK_SEMICOLON;
				token[n].line = linenum;
				token[n++].str = ';';
				break;
			case '=':
				token[n].type = TOK_EQUAL;
				token[n].line = linenum;
				token[n++].str = '=';
				break;
			case ',':
				token[n].type = TOK_COMMA;
				token[n].line = linenum;
				token[n++].str = ',';
				break;
			default:
				break;
			}
			// [A-Za-z][A-Za-z0-9], 以字母开头为真, 是一个标识符
			if (isalpha(ch)) {
				token[n].type = TOK_ID;
				*p++ = ch;
				while (isalnum(ch = line[++i])) *p++ = ch;
				--i;
				*p++ = '\0';
				// label value
				token[n].line = linenum;
				token[n++].str = buf;
				// print token
				pritnToken(token[n - 1]);
			}
			// [0-9]+
			if (isdigit(ch)) {
				*p++ = ch;
				// [0-9]+
				while (isdigit(ch = line[++i])) *p++ = ch;
				// double value 
				if (line[i] == '.') {
					*p++ = line[i];
					while (isdigit(ch = line[++i])) *p++ = ch;
					--i;
					*p++ = '\0';
					token[n].type = TOK_DOUBLE;
					token[n].line = linenum;
					token[n++].str += buf;
					// print token
					pritnToken(token[n - 1]);
				}
				else { // integer value
					--i;
					*p++ = '\0';
					token[n].type = TOK_INTEGER;
					token[n].line = linenum;
					token[n++].str = buf;
					// print token
					pritnToken(token[n - 1]);
				}
			}
		}
		linenum++;
	}
	fclose(fin);
	return n;
}

int main(int argc, char* argv[]) {
	int n;
	char* input = "C:\\Users\\admin\\Desktop\\2015301500259-李彦-词法分析器打包\\test.txt";
	int nToken = lexer(input);
	for (n = 0; n < nToken; n++) {
		//pritnToken(token[n]);
	}
	system("pause");

	return 1;
}