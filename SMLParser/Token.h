// Token.cpp : Defines the token information.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <set>
using namespace std;

#define NTOKEN 1000

// 标签和标签的值
typedef struct _Token {
	int type;
	int line;
	string str;
} Token;
