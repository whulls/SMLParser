// Token.cpp : Defines the token information.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <set>
using namespace std;

#define NTOKEN 1000

// ��ǩ�ͱ�ǩ��ֵ
typedef struct _Token {
	int type;
	int line;
	string str;
} Token;
