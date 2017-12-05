#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_DEPRECATE
using namespace std;
/*
* 用一个指针line、place和pos表示遍历到的代码位置。读到新的词素，在退出这个词素前进入新的状态。用指针sline和splace表示这个词素的第一个字符的位置，用来读完一个词素后显示它的位置。
* 用函数lsmread读pos指代的当前位置，用函数smlreadahead多读一位。其实smlreadahead也是读pos指代的当前位置，但每次lsmread之后pos会移一位，而lsmreadahead总在lsmread之后使用，且不会移动pos。
* 有一个prev_element记录上一个元素，用于辅助之后的
* 在遍历SML代码的过程中，会进入以下词素状态：
* 最初设计了状态几状态几，后来感觉现在排的顺序不好，所以只写有哪些状态，不编序号。
* 注释状态：发现读到的字符是(且预读一位为*时，进入此状态。之后的内容全部不处理。直到读到*且预读一位为)时退出此状态回到状态3。
* 字符状态：发现读到的字符是#且预读一位为"时，进入此状态，直到读到下一个"退出此状态，总之里面特殊处理和字符串状态相同。理论上字符状态只能包括1个字符，所以会进行计数，但这里退出状态只以"为判断标准。从#开始都进入缓冲区，最后打印值类型为char。如果计数包括的字符超过1个返回时会显示错误信息。
* 字符串状态：一旦发现读到"时，进入此状态，直到读到下一个"退出此状态。过程中如果读到\，之后的下一个字符会特殊处理，不会判断它是否是"。包括第一个"和最后一个"在内的内容都会在遍历时进入缓冲区，最后打印出它的值，类型为string。
* 非元素状态：什么都没有进入时，读到空格或者制表符或者换行符以及,;()[]{}时，都会进入此状态，等待下一次读入有意义内容。词法分析器懒得判断(对不对应。
* 数字状态1：在非元素状态下或者符号标识符状态下遇到数字后进入的状态。如果是非元素状态，读到~后预读1位是数字也会进入数字状态。内容要放到缓冲区里，如果有~的话~也放进去。如果读到非元素状态、注释状态，或者读到符号标识符可用的符号，会打印类型为digit的内容退出。之后能够一直支持保留这个状态的就是读到数字，再读~就要进符号标识符状态了。如果读到.，进入数字状态2。如果读到E，进入数字状态3。读到其他的字母、下划线、撇号之类的，会进入错误状态（错误状态应该会继续一直读到非元素类、注释类、字符及字符串类再停下来吧）
* 数字状态2：进入这个状态不会清空缓冲区，开始位置不变，而是会继续读数字进入缓冲区。读到E进入数字状态3（如果预读就是E那.都不放进缓冲区）。如果再读.也是进错误状态的。其他读跳转的判定和数字状态1差不多了。1个.之后直接跟其他的符号应该不会有太多问题，但是这样的话打印的digit最好去除最后那个.。
* 数字状态3：进入这个状态也不会清空缓冲区，不变开始位置。这个状态进入之后，如果开头是~，预读一位，如果是数字则继续，如果不是只能进错误状态了。之后就是继续读数字。如果有.进入数字状态4，如果再读到E，也只能进错误状态了。其他情况还是和一般的数字状态差不多。
* 数字状态4：进入这个状态不会清空缓冲区，开始位置不变。之后要维持数字状态只能读数字了，读到.和E都会进入错误状态。预读一位如果是数字那么.在缓冲区，如果什么都没有将会直接退出数字状态4，那么.不进入缓冲区，打印时去掉那个.。其他和一般数字状态差不多。
* 符号标识符状态：在非元素状态、数字状态、字母标识符状态，遇到标识符认可的!%&$#+-*\/:<=>?@~`^| 如果之前没有匹配上字符状态，如果开头是~而预读不是数字之类，那就进入这个状态。内容都进入缓冲区，如果读到同样的符号会一直读下去。如果读到.，那应该是要进入下一个标识符了，但总之还是先打印返回非元素状态。遇到字母"数字和非元素状态等内容则退出，遇到_'会进入错误状态。退出时判断缓冲区是否和关键符号匹配，如果是则不打印，否则打印symbolic name和缓冲区内容。
* 字母标识符状态：非元素状态、符号标识符状态，遇到字母进入此状态。之后可以接纳字母数字和_' 如果遇到非元素状态、符号标识符、.会退出字母标识符，退出时判断是否是关键字，如果不是，打印alphabatic name和缓冲区内容。
* 错误状态：非元素状态，遇到了_'.这样的符号，以及其他中文之类的没出现的符号。数字状态遇到字母和_'。符号标识符状态遇到_'。这之后的一系列都会当成错误内容的一部分，直到碰到注释、字符串、非元素等。
* 最后的最后，如果在任何状态读到了'\0'，都会进入结束状态，endflag置1。
*/
#define NOTE 0
#define CHAR 1
#define STRING 2
#define NOVALUE 3
#define NUM1 4
#define NUM2 5
#define NUM3 6
#define NUM4 7
#define LETTER 8
#define SYMBOL 9
#define TYPE 10
#define ERR 11
#define END 12

#define T_END 0
#define T_SEPERATE 1
#define T_NEWLINE 2
//#define T_RETURN 3 发现读出的代码会自动转成\n，不会出现\r
#define T_OTHERSYM 3
#define T_QUOT 4
#define T_POUND 5
#define T_PRIME 6
#define T_LBRAKET 7
#define T_RBRAKET 8
#define T_STAR 9
#define T_DOT 10
#define T_NUM 11
#define T_OTHERLTR 12
#define T_E 13
#define T_SLANT 14
#define T__ 15
#define T_NEG 16

int issavedsymbol(char buffer[])
{
	char *str1 = ":";
	char *str2 = "|";
	char *str3 = "=";
	char *str4 = "=>";
	char *str5 = "->";
	char *str6 = "#";
	char *str7 = ":>";
	if (strcmp(buffer, str1) == 0 ||
		strcmp(buffer, str2) == 0 ||
		strcmp(buffer, str3) == 0 ||
		strcmp(buffer, str4) == 0 ||
		strcmp(buffer, str5) == 0 ||
		strcmp(buffer, str6) == 0 ||
		strcmp(buffer, str7) == 0)
		return 1;
	else
		return 0;
}

int issavedletter(char buffer[])
{
	char *str1 = "abstype";
	char *str2 = "and";
	char *str3 = "andalso";
	char *str4 = "as";
	char *str5 = "case";
	char *str6 = "datatype";
	char *str7 = "do";
	char *str8 = "else";
	char *str9 = "end";
	char *str10 = "eqtype";
	char *str11 = "exception";
	char *str12 = "fn";
	char *str13 = "fun";
	char *str14 = "functor";
	char *str15 = "handle";
	char *str16 = "if";
	char *str17 = "in";
	char *str18 = "include";
	char *str19 = "infix";
	char *str20 = "infixr";
	char *str21 = "let";
	char *str22 = "local";
	char *str23 = "nonfix";
	char *str24 = "of";
	char *str25 = "op";
	char *str26 = "open";
	char *str27 = "orelse";
	char *str28 = "raise";
	char *str29 = "rec";
	char *str30 = "sharing";
	char *str31 = "sig";
	char *str32 = "signatrue";
	char *str33 = "struct";
	char *str34 = "structure";
	char *str35 = "then";
	char *str36 = "type";
	char *str37 = "val";
	char *str38 = "where";
	char *str39 = "while";
	char *str40 = "with";
	char *str41 = "withtype";
	if (strcmp(buffer, str1) == 0 ||
		strcmp(buffer, str2) == 0 ||
		strcmp(buffer, str3) == 0 ||
		strcmp(buffer, str4) == 0 ||
		strcmp(buffer, str5) == 0 ||
		strcmp(buffer, str6) == 0 ||
		strcmp(buffer, str7) == 0 ||
		strcmp(buffer, str8) == 0 ||
		strcmp(buffer, str9) == 0 ||
		strcmp(buffer, str10) == 0 ||
		strcmp(buffer, str11) == 0 ||
		strcmp(buffer, str12) == 0 ||
		strcmp(buffer, str13) == 0 ||
		strcmp(buffer, str14) == 0 ||
		strcmp(buffer, str15) == 0 ||
		strcmp(buffer, str16) == 0 ||
		strcmp(buffer, str17) == 0 ||
		strcmp(buffer, str18) == 0 ||
		strcmp(buffer, str19) == 0 ||
		strcmp(buffer, str20) == 0 ||
		strcmp(buffer, str21) == 0 ||
		strcmp(buffer, str22) == 0 ||
		strcmp(buffer, str23) == 0 ||
		strcmp(buffer, str24) == 0 ||
		strcmp(buffer, str25) == 0 ||
		strcmp(buffer, str26) == 0 ||
		strcmp(buffer, str27) == 0 ||
		strcmp(buffer, str28) == 0 ||
		strcmp(buffer, str29) == 0 ||
		strcmp(buffer, str30) == 0 ||
		strcmp(buffer, str31) == 0 ||
		strcmp(buffer, str32) == 0 ||
		strcmp(buffer, str33) == 0 ||
		strcmp(buffer, str34) == 0 ||
		strcmp(buffer, str35) == 0 ||
		strcmp(buffer, str36) == 0 ||
		strcmp(buffer, str37) == 0 ||
		strcmp(buffer, str38) == 0 ||
		strcmp(buffer, str39) == 0 ||
		strcmp(buffer, str40) == 0 ||
		strcmp(buffer, str41) == 0)
		return 1;
	else
		return 0;
}

char smlread(char code[], long* pos, int* place)
{
	char ch;
	ch = code[*pos];
	*pos = *pos + 1;
	*place = *place + 1;
	return ch;
}

char smlreadahead(char code[], long *pos)//本来设计了这个代码，不过好像写起来不如直接写code[*pos]，可能以后不会用了。
{
	char ch;
	ch = code[*pos];
	return ch;
}

int gettype(char ch)
{
	if (ch == 0 || ch == -1)//把code所有ASCII码显示出来，发现最后结束的符号是-1。
		return T_END;
	else if (ch == 9 || ch == 32 || ch == 44 || ch == 59 || ch == 91 || ch == 93 || ch == 123 || ch == 125)
		return T_SEPERATE;//分隔符，包括空格、制表、逗号、分好、中括号、大括号等。
	else if (ch == 10)
		return T_NEWLINE;//换行符，因为会导致line的变化而从分隔符中单独列出。
						 /*else if(ch==13)
						 return T_RETURN;//回车符，回到第一位。*/
	else if (ch == 33 || (ch >= 36 && ch <= 38) || ch == 43 || ch == 45 || ch == 47 || ch == 58 || (ch >= 60 && ch <= 64) || ch == 94 || ch == 96 || ch == 124)
		return T_OTHERSYM;//普通的符号
	else if (ch == 34)
		return T_QUOT;//双引号
	else if (ch == 35)
		return T_POUND;//井号，因为可以引起字符型而从普通符号中单独列出。
					   //else if(ch==39 || ch==95)
					   //    return T_LSYM;//在字母后使用的符号，撇号和下划线
	else if (ch == 39)
		return T_PRIME;
	else if (ch == 40)
		return T_LBRAKET;//左括号，因为是注释组成部分而从分隔符中列出。
	else if (ch == 41)
		return T_RBRAKET;//右括号，因为是注释组成部分而从分隔符中列出
	else if (ch == 42)
		return T_STAR;//星号，因为是注释组成部分而从普通符号中单独列出。
	else if (ch == 46)
		return T_DOT;//点号
	else if (ch >= 48 && ch <= 57)
		return T_NUM;//数字
	else if ((ch >= 65 && ch <= 68) || (ch >= 70 && ch <= 90) || (ch >= 97 && ch <= 122))
		return T_OTHERLTR;//大多数字母除了E
	else if (ch == 69)
		return T_E;//大写字母E，由于能够成数字被单独列出
	else if (ch == 92)
		return T_SLANT;//反斜线，因为可以作为转义字符而从普通符号中单独列出。
	else if (ch == 95)
		return T__;
	else if (ch == 126)
		return T_NEG;//波浪号，因为其可以作为一元负号而从普通符号中单独列出。
	else return -1;
}

void state_note(char code[], int* nstate, long* pos, int* line, int* place, int* sline, int* splace)
{
	char ch = smlread(code, pos, place);
	char cha = smlreadahead(code, pos);
	switch (gettype(ch))
	{
	case T_END:*nstate = END; break;
	case T_SEPERATE:break;
	case T_NEWLINE:*line = *line + 1; *place = 0; break;
		//case T_RETURN:*place=0;break;
	case T_OTHERSYM:break;
	case T_QUOT:break;
	case T_POUND:break;
		//case T_LSYM:break;
	case T_PRIME:break;
	case T_LBRAKET:break;
	case T_RBRAKET:break;
	case T_STAR:
		if (gettype(cha) == T_RBRAKET)
		{
			*nstate = NOVALUE;
			smlread(code, pos, place);
		}
		break;
	case T_DOT:break;
	case T_NUM:break;
	case T_OTHERLTR:break;
	case T_E:break;
	case T_SLANT:break;
	case T__:break;
	case T_NEG:break;
	default:break;
	}
}

void state_char(char code[], int* nstate, char buffer[], int* bufptr, long* pos, int* line, int* place, int* sline, int* splace)
{
	char ch = smlread(code, pos, place);
	char cha = smlreadahead(code, pos);
	switch (gettype(ch)) {
	case T_END:
		*nstate = END;
		printf("ERROR:UNCOMPLETE CHAR\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		break;
	case T_OTHERSYM:
	case T_POUND:
		//case T_LSYM:
	case T_PRIME:
	case T_LBRAKET:
	case T_RBRAKET:
	case T_STAR:
	case T_DOT:
	case T_NUM:
	case T_OTHERLTR:
	case T_E:
	case T__:
	case T_NEG:
	case T_SEPERATE:
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	case T_NEWLINE:
		*line = *line + 1;
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		*place = 0;
		break;
		/*case T_RETURN:
		*place=0;
		buffer[*bufptr]=ch;
		*bufptr=*bufptr+1;
		break;*/
	case T_QUOT:
		*nstate = NOVALUE;
		buffer[*bufptr] = 34;//"
		if (*bufptr != 4)
			printf("ERROR:WRONG CHAR LENGTH\n");
		printf("CHAR\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_SLANT:
		if (cha == 'a')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 7;
			*bufptr = *bufptr + 1;
		}
		else if (cha == 'b')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 8;
			*bufptr = *bufptr + 1;
		}
		else if (cha == 'f')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 12;
			*bufptr = *bufptr + 1;
		}
		else if (cha == 'n')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 10;
			*bufptr = *bufptr + 1;
		}
		else if (cha == 'r')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 13;
			*bufptr = *bufptr + 1;
		}
		else if (cha == 't')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 9;
			*bufptr = *bufptr + 1;
		}
		else if (cha == 'v')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 11;
			*bufptr = *bufptr + 1;
		}
		else if (cha == '\\')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 92;
			*bufptr = *bufptr + 1;
		}
		else if (cha == '\'')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 39;
			*bufptr = *bufptr + 1;
		}
		else if (cha == '"')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 34;
			*bufptr = *bufptr + 1;
		}
		else if (cha == '?')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 63;
			*bufptr = *bufptr + 1;
		}
		else if (cha == '0')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 0;
			*bufptr = *bufptr + 1;
		}
		else if (cha == '1')//1开头可能是1__也可能是1_或者1。
		{
			smlread(code, pos, place);
			char chaa = smlreadahead(code, pos);
			if (chaa >= 48 && chaa <= 55)//chaa为0~7，注意八进制
			{
				smlread(code, pos, place);
				char chaaa = smlreadahead(code, pos);
				if (chaaa >= 48 && chaaa <= 55)//1__的情况
				{
					smlread(code, pos, place);
					buffer[*bufptr] = 64 + 8 * (chaa - 48) + chaaa - 48;
					*bufptr = *bufptr + 1;
				}
				else//1_的情况
				{
					buffer[*bufptr] = 8 + chaa - 48;
					*bufptr = *bufptr + 1;
				}
			}
			else//1的情况
			{
				buffer[*bufptr] = 1;
				*bufptr = *bufptr + 1;
			}
		}
		else if (cha >= 50 && cha <= 55)//2~7的，回去看本子。
		{
			smlread(code, pos, place);
			char chaa = smlreadahead(code, pos);
			if (chaa >= 48 && chaa <= 55)
			{
				smlread(code, pos, place);
				buffer[*bufptr] = 8 * (cha - 48) + chaa - 48;
				*bufptr = *bufptr + 1;
			}
			else
			{
				buffer[*bufptr] = cha - 48;
				*bufptr = *bufptr + 1;
			}
		}
		else if (cha == ' ' || cha == '\t' || cha == '\n')//\之后跟一个空格或者制表符或者换行，之后再跟\不插入任何东西。当然，换行之后字符串连着。
		{
			do
			{
				ch = smlread(code, pos, place);
				cha = smlreadahead(code, pos);
				if (ch == '\n')
					*line = *line + 1;
				if (ch == '\r')
					*place = 0;
			} while (cha == ' ' || cha == '\t' || cha == '\n' || cha == '\r');
			if (cha == '\\')
				smlread(code, pos, place);
			else
				printf("ERROR:INVALID\"\\\" SEQUENCE\tLINE%d\tPLACE%d\n", *line, *place);
		}
		/*else if(cha=='\n')
		{

		}以前想把\n和另外两个空格分开，但是空格的内容太多了，所以还是在一个else里面加if判断写起来轻松*/
		else
		{
			printf("ERROR:INVALID\"\\\" SEQUENCE\tLINE%d\tPLACE%d\n", *line, *place);
		}
		break;
	default:
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	}
}

void state_string(char code[], int* nstate, char buffer[], int* bufptr, long* pos, int* line, int* place, int* sline, int* splace)
{
	char ch = smlread(code, pos, place);
	char cha = smlreadahead(code, pos);
	switch (gettype(ch)) {
	case T_END:
		*nstate = END;
		printf("ERROR:UNCOMPLETE STRING\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		break;
	case T_OTHERSYM:
	case T_POUND:
		//case T_LSYM:
	case T_PRIME:
	case T_LBRAKET:
	case T_RBRAKET:
	case T_STAR:
	case T_DOT:
	case T_NUM:
	case T_OTHERLTR:
	case T_E:
	case T__:
	case T_NEG:
	case T_SEPERATE:
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	case T_NEWLINE:
		*line = *line + 1;
		*place = 0;
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
		/*case T_RETURN:
		*place=0;
		buffer[*bufptr]=ch;
		*bufptr=*bufptr+1;
		break;*/
	case T_QUOT:
		*nstate = NOVALUE;
		buffer[*bufptr] = 34;//"
		printf("STRING\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_SLANT:
		if (cha == 'a')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 7;
			*bufptr = *bufptr + 1;
		}
		else if (cha == 'b')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 8;
			*bufptr = *bufptr + 1;
		}
		else if (cha == 'f')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 12;
			*bufptr = *bufptr + 1;
		}
		else if (cha == 'n')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 10;
			*bufptr = *bufptr + 1;
		}
		else if (cha == 'r')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 13;
			*bufptr = *bufptr + 1;
		}
		else if (cha == 't')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 9;
			*bufptr = *bufptr + 1;
		}
		else if (cha == 'v')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 11;
			*bufptr = *bufptr + 1;
		}
		else if (cha == '\\')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 92;
			*bufptr = *bufptr + 1;
		}
		else if (cha == '\'')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 39;
			*bufptr = *bufptr + 1;
		}
		else if (cha == '"')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 34;
			*bufptr = *bufptr + 1;
		}
		else if (cha == '?')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 63;
			*bufptr = *bufptr + 1;
		}
		else if (cha == '0')
		{
			smlread(code, pos, place);
			buffer[*bufptr] = 0;
			*bufptr = *bufptr + 1;
		}
		else if (cha == '1')//1开头可能是1__也可能是1_或者1。
		{
			smlread(code, pos, place);
			char chaa = smlreadahead(code, pos);
			if (chaa >= 48 && chaa <= 55)//chaa为0~7，注意八进制
			{
				smlread(code, pos, place);
				char chaaa = smlreadahead(code, pos);
				if (chaaa >= 48 && chaaa <= 55)//1__的情况
				{
					smlread(code, pos, place);
					buffer[*bufptr] = 64 + 8 * (chaa - 48) + chaaa - 48;
					*bufptr = *bufptr + 1;
				}
				else//1_的情况
				{
					buffer[*bufptr] = 8 + chaa - 48;
					*bufptr = *bufptr + 1;
				}
			}
			else//1的情况
			{
				buffer[*bufptr] = 1;
				*bufptr = *bufptr + 1;
			}
		}
		else if (cha >= 50 && cha <= 55)//2~7的，回去看本子。
		{
			smlread(code, pos, place);
			char chaa = smlreadahead(code, pos);
			if (chaa >= 48 && chaa <= 55)
			{
				smlread(code, pos, place);
				buffer[*bufptr] = 8 * (cha - 48) + chaa - 48;
				*bufptr = *bufptr + 1;
			}
			else
			{
				buffer[*bufptr] = cha - 48;
				*bufptr = *bufptr + 1;
			}
		}
		else if (cha == ' ' || cha == '\t' || cha == '\n')//\之后跟一个空格或者制表符或者换行，之后再跟\不插入任何东西。当然，换行之后字符串连着。
		{
			do
			{
				ch = smlread(code, pos, place);
				cha = smlreadahead(code, pos);
				if (ch == '\n')
				{
					*line = *line + 1;
					*place = 0;
				}
			} while (cha == ' ' || cha == '\t' || cha == '\n');
			if (cha == '\\')
				smlread(code, pos, place);
			else
				printf("ERROR:INVALID\"\\\" SEQUENCE\tLINE%d\tPLACE%d\n", *line, *place);
		}
		/*else if(cha=='\n')
		{

		}以前想把\n和另外两个空格分开，但是空格的内容太多了，所以还是在一个else里面加if判断写起来轻松*/
		else
		{
			printf("ERROR:INVALID\"\\\" SEQUENCE\tLINE%d\tPLACE%d\n", *line, *place);
		}
		break;
	default:
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	}
}

void state_novalue(char code[], int* nstate, char buffer[], int* bufptr, long* pos, int* line, int* place, int* sline, int* splace)
{
	char ch = smlread(code, pos, place);
	char cha = smlreadahead(code, pos);
	switch (gettype(ch)) {
	case T_END:*nstate = END; break;
	case T_SEPERATE:break;
	case T_NEWLINE:*line = *line + 1; *place = 0; break;
		//case T_RETURN:*place=0;break;
	case T_STAR://*和\在这里是和其他符号没有区别的。
	case T_SLANT:
	case T_OTHERSYM:
		*nstate = SYMBOL;
		*sline = *line;
		*splace = *place;
		buffer[0] = ch;
		*bufptr = 1;
		break;
	case T_QUOT:
		*nstate = STRING;
		*sline = *line;
		*splace = *place;
		buffer[0] = 34;//"
		*bufptr = 1;
		break;
	case T_POUND:
		if (gettype(cha) == T_QUOT)
		{
			*nstate = CHAR;
			*sline = *line;
			*splace = *place;
			smlread(code, pos, place);
			buffer[0] = 35;//#
			buffer[1] = 34;//"
			*bufptr = 2;
		}
		else
		{
			*nstate = SYMBOL;
			*sline = *line;
			*splace = *place;
			buffer[0] = 35;//#
			*bufptr = 1;
		}
		break;
	case T_DOT://._'在这种情况下都是错误的写法。
			   /*case T_LSYM:
			   *nstate=ERR;
			   *sline=*line;
			   *splace=*place;
			   buffer[0]=ch;
			   *bufptr=1;
			   break;*/
	case T_PRIME:
		if (gettype(cha) != T_OTHERLTR && gettype(cha) != T_E)
			*nstate = ERR;
		else
			*nstate = TYPE;
		*sline = *line;
		*splace = *place;
		buffer[0] = 39;
		*bufptr = 1;
		break;
	case T_LBRAKET:
		if (gettype(cha) == T_STAR)
		{
			*nstate = NOTE;
			smlread(code, pos, place);
		}
		break;
	case T_RBRAKET:break;
	case T_NUM:
		*nstate = NUM1;
		*sline = *line;
		*splace = *place;
		buffer[0] = ch;
		*bufptr = 1;
		break;
	case T_OTHERLTR://E和其他字母这时候都是普通的字母。
	case T_E:
		*nstate = LETTER;
		*sline = *line;
		*splace = *place;
		buffer[0] = ch;
		*bufptr = 1;
		break;
	case T__:
		if (gettype(cha) == T_PRIME || gettype(cha) == T_NUM || gettype(cha) == T_OTHERLTR || gettype(cha) == T_E
			|| gettype(cha) == T__)//对通配符不太熟悉，暂时认为一个单独的它后面不能直接跟字母、撇号、数字、另一个下划线。
		{
			*nstate = ERR;
			*sline = *line;
			*splace = *place;
			buffer[0] = 95;
			*bufptr = 1;
		}
		break;
	case T_NEG:
		if (gettype(cha) == T_NUM)
			*nstate = NUM1;
		else
			*nstate = SYMBOL;
		*sline = *line;
		*splace = *place;
		buffer[0] = ch;
		*bufptr = 1;
		break;
	default:
		*nstate = ERR;
		*sline = *line;
		*splace = *place;
		buffer[0] = ch;
		*bufptr = 1;
		break;
	}
}

void state_num1(char code[], int* nstate, char buffer[], int* bufptr, long* pos, int* line, int* place, int* sline, int* splace)
{
	char ch = smlread(code, pos, place);
	char cha = smlreadahead(code, pos);
	switch (gettype(ch)) {
	case T_END:
		*nstate = END;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_RBRAKET:
	case T_SEPERATE:
		*nstate = NOVALUE;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_NEWLINE:
		*nstate = NOVALUE;
		*line = *line + 1;
		*place = 0;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
		/*case T_RETURN:
		*nstate=NOVALUE;
		*place=0;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n",*sline,*splace,buffer);
		memset(buffer,0,255);
		*bufptr=0;
		break;*/
	case T_NEG:
	case T_SLANT:
	case T_STAR:
	case T_OTHERSYM:
		*nstate = SYMBOL;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		*sline = *line;
		*splace = *place;
		buffer[0] = ch;
		*bufptr = 1;
		break;
	case T_QUOT:
		*nstate = STRING;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		*sline = *line;
		*splace = *place;
		buffer[0] = 34;
		*bufptr = 1;
		break;
	case T_POUND:
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		if (gettype(cha) == T_QUOT)
		{
			*nstate = CHAR;
			*sline = *line;
			*splace = *place;
			smlread(code, pos, place);
			buffer[0] = 35;//#
			buffer[1] = 34;//"
			*bufptr = 2;
		}
		else
		{
			*nstate = SYMBOL;
			*sline = *line;
			*splace = *place;
			buffer[0] = 35;//#
			*bufptr = 1;
		}
		break;
	case T__:
	case T_OTHERLTR:
	case T_PRIME:
		*nstate = ERR;
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	case T_LBRAKET:
		if (gettype(cha) == T_STAR)
		{
			*nstate = NOTE;
			smlread(code, pos, place);
		}
		else
			*nstate = NOVALUE;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_DOT:
		*nstate = NUM2;
		if (gettype(cha) == T_NUM)//如果.后面不是数字了，干脆就不放到缓冲区里了。
		{
			buffer[*bufptr] = 46;//.
			*bufptr = *bufptr + 1;
		}
		break;
	case T_NUM:
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	case T_E:
		buffer[*bufptr] = 69;//E
		*bufptr = *bufptr + 1;
		if (gettype(cha) == T_NUM)
		{
			*nstate = NUM3;
			ch = smlread(code, pos, place);
			buffer[*bufptr] = ch;
			*bufptr = *bufptr + 1;
		}
		else if (gettype(cha) == T_NEG)
		{
			ch = smlread(code, pos, place);
			cha = smlreadahead(code, pos);
			buffer[*bufptr] = ch;
			*bufptr = *bufptr + 1;
			if (gettype(cha) == T_NUM)
			{
				ch = smlread(code, pos, place);
				*nstate = NUM3;
				buffer[*bufptr] = ch;
				*bufptr = *bufptr + 1;
			}
			else
				*nstate = ERR;
		}
		else
			*nstate = ERR;
		break;
	default:
		*nstate = ERR;
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	}
}

void state_num2(char code[], int* nstate, char buffer[], int* bufptr, long* pos, int* line, int* place, int* sline, int* splace)
{
	char ch = smlread(code, pos, place);
	char cha = smlreadahead(code, pos);
	switch (gettype(ch)) {
	case T_END:
		*nstate = END;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_RBRAKET:
	case T_SEPERATE:
		*nstate = NOVALUE;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_NEWLINE:
		*nstate = NOVALUE;
		*line = *line + 1;
		*place = 0;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
		/*case T_RETURN:
		*place=0;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n",*sline,*splace,buffer);
		memset(buffer,0,255);
		*bufptr=0;
		break;*/
	case T_NEG:
	case T_SLANT:
	case T_STAR:
	case T_OTHERSYM:
		*nstate = SYMBOL;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		*sline = *line;
		*splace = *place;
		buffer[0] = ch;
		*bufptr = 1;
		break;
	case T_QUOT:
		*nstate = STRING;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		*sline = *line;
		*splace = *place;
		buffer[0] = 34;
		*bufptr = 1;
		break;
	case T_POUND:
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		if (gettype(cha) == T_QUOT)
		{
			*nstate = CHAR;
			*sline = *line;
			*splace = *place;
			smlread(code, pos, place);
			buffer[0] = 35;//#
			buffer[1] = 34;//"
			*bufptr = 2;
		}
		else
		{
			*nstate = SYMBOL;
			*sline = *line;
			*splace = *place;
			buffer[0] = 35;//#
			*bufptr = 1;
		}
		break;
	case T__:
	case T_DOT://出现一个小数点，且不在E后面，不能再出现小数点。
	case T_OTHERLTR:
	case T_PRIME:
		*nstate = ERR;
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	case T_LBRAKET:
		if (gettype(cha) == T_STAR)
		{
			*nstate = NOTE;
			smlread(code, pos, place);
		}
		else
			*nstate = NOVALUE;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_NUM:
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	case T_E:
		buffer[*bufptr] = 69;//E
		*bufptr = *bufptr + 1;
		if (gettype(cha) == T_NUM)
		{
			*nstate = NUM3;
			ch = smlread(code, pos, place);
			buffer[*bufptr] = ch;
			*bufptr = *bufptr + 1;
		}
		else if (gettype(cha) == T_NEG)
		{
			ch = smlread(code, pos, place);
			cha = smlreadahead(code, pos);
			buffer[*bufptr] = ch;
			*bufptr = *bufptr + 1;
			if (gettype(cha) == T_NUM)
			{
				ch = smlread(code, pos, place);
				*nstate = NUM3;
				buffer[*bufptr] = ch;
				*bufptr = *bufptr + 1;
			}
			else
				*nstate = ERR;
		}
		else
			*nstate = ERR;
		break;
	default:
		*nstate = ERR;
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	}
}

void state_num3(char code[], int* nstate, char buffer[], int* bufptr, long* pos, int* line, int* place, int* sline, int* splace)
{
	char ch = smlread(code, pos, place);
	char cha = smlreadahead(code, pos);
	switch (gettype(ch)) {
	case T_END:
		*nstate = END;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_RBRAKET:
	case T_SEPERATE:
		*nstate = NOVALUE;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_NEWLINE:
		*nstate = NOVALUE;
		*line = *line + 1;
		*place = 0;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
		/*case T_RETURN:
		*nstate=NOVALUE;
		*place=0;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n",*sline,*splace,buffer);
		memset(buffer,0,255);
		*bufptr=0;
		break;*/
	case T_NEG:
	case T_SLANT:
	case T_STAR:
	case T_OTHERSYM:
		*nstate = SYMBOL;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		*sline = *line;
		*splace = *place;
		buffer[0] = ch;
		*bufptr = 1;
		break;
	case T_QUOT:
		*nstate = STRING;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		*sline = *line;
		*splace = *place;
		buffer[0] = 34;
		*bufptr = 1;
		break;
	case T_POUND:
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		if (gettype(cha) == T_QUOT)
		{
			*nstate = CHAR;
			*sline = *line;
			*splace = *place;
			smlread(code, pos, place);
			buffer[0] = 35;//#
			buffer[1] = 34;//"
			*bufptr = 2;
		}
		else
		{
			*nstate = SYMBOL;
			*sline = *line;
			*splace = *place;
			buffer[0] = 35;//#
			*bufptr = 1;
		}
		break;
	case T__:
	case T_E://出现一个E以后不能再出现E。
	case T_OTHERLTR:
	case T_PRIME:
		*nstate = ERR;
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	case T_LBRAKET:
		if (gettype(cha) == T_STAR)
		{
			*nstate = NOTE;
			smlread(code, pos, place);
		}
		else
			*nstate = NOVALUE;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_DOT:
		*nstate = NUM4;
		if (gettype(cha) == T_NUM)//如果.后面不是数字了，干脆就不放到缓冲区里了。
		{
			buffer[*bufptr] = 46;//.
			*bufptr = *bufptr + 1;
		}
		break;
	case T_NUM:
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	default:
		*nstate = ERR;
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	}
}

void state_num4(char code[], int* nstate, char buffer[], int* bufptr, long* pos, int* line, int* place, int* sline, int* splace)
{
	char ch = smlread(code, pos, place);
	char cha = smlreadahead(code, pos);
	switch (gettype(ch)) {
	case T_END:
		*nstate = END;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_RBRAKET:
	case T_SEPERATE:
		*nstate = NOVALUE;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_NEWLINE:
		*nstate = NOVALUE;
		*line = *line + 1;
		*place = 0;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
		/*case T_RETURN:
		*nstate=NOVALUE;
		*place=0;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n",*sline,*splace,buffer);
		memset(buffer,0,255);
		*bufptr=0;
		break;*/
	case T_NEG:
	case T_SLANT:
	case T_STAR:
	case T_OTHERSYM:
		*nstate = SYMBOL;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		*sline = *line;
		*splace = *place;
		buffer[0] = ch;
		*bufptr = 1;
		break;
	case T_QUOT:
		*nstate = STRING;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		*sline = *line;
		*splace = *place;
		buffer[0] = 34;
		*bufptr = 1;
		break;
	case T_POUND:
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		if (gettype(cha) == T_QUOT)
		{
			*nstate = CHAR;
			*sline = *line;
			*splace = *place;
			smlread(code, pos, place);
			buffer[0] = 35;//#
			buffer[1] = 34;//"
			*bufptr = 2;
		}
		else
		{
			*nstate = SYMBOL;
			*sline = *line;
			*splace = *place;
			buffer[0] = 35;//#
			*bufptr = 1;
		}
		break;
	case T__:
	case T_DOT://出现了E又出现小数点，不能再有小数点和E。
	case T_E:
	case T_OTHERLTR:
	case T_PRIME:
		*nstate = ERR;
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	case T_LBRAKET:
		if (gettype(cha) == T_STAR)
		{
			*nstate = NOTE;
			smlread(code, pos, place);
		}
		else
			*nstate = NOVALUE;
		printf("DIGIT\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_NUM:
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	default:
		*nstate = ERR;
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	}
}

void state_letter(char code[], int* nstate, char buffer[], int* bufptr, long* pos, int* line, int* place, int* sline, int* splace)
{
	char ch = smlread(code, pos, place);
	char cha = smlreadahead(code, pos);
	switch (gettype(ch)) {
	case T_END:
		*nstate = END;
		if (issavedletter(buffer) == 0)
		{
			printf("ALPHABATIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		}
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_DOT:
	case T_RBRAKET:
	case T_SEPERATE:
		*nstate = NOVALUE;
		if (issavedletter(buffer) == 0)
		{
			printf("ALPHABATIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		}
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_NEWLINE:
		*nstate = NOVALUE;
		*line = *line + 1;
		*place = 0;
		if (issavedletter(buffer) == 0)
		{
			printf("ALPHABATIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		}
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
		/*case T_RETURN:
		*nstate=NOVALUE;
		*place=0;
		if(issavedletter(buffer)==0)
		{
		printf("ALPHABATIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n",*sline,*splace,buffer);
		}
		memset(buffer,0,255);
		*bufptr=0;
		break;*/
	case T_SLANT:
	case T_STAR:
	case T_OTHERSYM:
		*nstate = SYMBOL;
		if (issavedletter(buffer) == 0)
		{
			printf("ALPHABATIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		}
		memset(buffer, 0, 255);
		*bufptr = 0;
		*sline = *line;
		*splace = *place;
		buffer[0] = ch;
		*bufptr = 1;
		break;
	case T_QUOT:
		*nstate = STRING;
		if (issavedletter(buffer) == 0)
		{
			printf("ALPHABATIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		}
		memset(buffer, 0, 255);
		*bufptr = 0;
		*sline = *line;
		*splace = *place;
		buffer[0] = 34;//"
		*bufptr = 1;
		break;
	case T_POUND:
		if (issavedletter(buffer) == 0)
		{
			printf("ALPHABATIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		}
		memset(buffer, 0, 255);
		*bufptr = 0;
		if (gettype(cha) == T_QUOT)
		{
			*nstate = CHAR;
			*sline = *line;
			*splace = *place;
			smlread(code, pos, place);
			buffer[0] = 35;//#
			buffer[1] = 34;//"
			*bufptr = 2;
		}
		else
		{
			*nstate = SYMBOL;
			*sline = *line;
			*splace = *place;
			buffer[0] = 35;//#
			*bufptr = 1;
		}
		break;
	case T__:
	case T_E:
	case T_OTHERLTR:
	case T_NUM:
	case T_PRIME:
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	case T_LBRAKET:
		if (gettype(cha) == T_STAR)
		{
			*nstate = NOTE;
			smlread(code, pos, place);
		}
		else
			*nstate = NOVALUE;
		if (issavedletter(buffer) == 0)
		{
			printf("ALPHABATIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		}
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_NEG:
		if (issavedletter(buffer) == 0)
		{
			printf("ALPHABATIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		}
		memset(buffer, 0, 255);
		*bufptr = 0;
		if (gettype(cha) == T_NUM)
			*nstate = NUM1;
		else
			*nstate = SYMBOL;
		*sline = *line;
		*splace = *place;
		buffer[0] = ch;
		*bufptr = 1;
		break;
	default:
		*nstate = ERR;
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	}
}

void state_symbol(char code[], int* nstate, char buffer[], int* bufptr, long* pos, int* line, int* place, int* sline, int* splace)
{
	char ch = smlread(code, pos, place);
	char cha = smlreadahead(code, pos);
	switch (gettype(ch)) {
	case T_END:
		*nstate = END;
		if (issavedsymbol(buffer) == 0)
		{
			printf("SYMBOLIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		}
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_DOT:
	case T_RBRAKET:
	case T_SEPERATE:
		*nstate = NOVALUE;
		if (issavedsymbol(buffer) == 0)
		{
			printf("SYMBOLIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		}
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_NEWLINE:
		*nstate = NOVALUE;
		*line = *line + 1;
		*place = 0;
		if (issavedsymbol(buffer) == 0)
		{
			printf("SYMBOLIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		}
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
		/*case T_RETURN:
		*nstate=NOVALUE;
		*place=0;
		if(issavedsymbol(buffer)==0)
		{
		printf("SYMBOLIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n",*sline,*splace,buffer);
		}
		memset(buffer,0,255);
		*bufptr=0;
		break;*/
	case T_NEG:
	case T_SLANT:
	case T_STAR:
	case T_POUND:
	case T_OTHERSYM:
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	case T_QUOT:
		*nstate = STRING;
		if (issavedsymbol(buffer) == 0)
		{
			printf("SYMBOLIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		}
		memset(buffer, 0, 255);
		*bufptr = 0;
		*sline = *line;
		*splace = *place;
		buffer[0] = 34;//"
		*bufptr = 1;
		break;
		/*case T_LSYM:
		*nstate=ERR;
		buffer[*bufptr]=ch;
		*bufptr=*bufptr+1;
		break;*/
	case T_PRIME:
		if (gettype(cha) != T_OTHERLTR && gettype(cha) != T_E)
			*nstate = ERR;
		else
			*nstate = TYPE;
		if (issavedsymbol(buffer) == 0)
		{
			printf("SYMBOLIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		}
		memset(buffer, 0, 255);
		*bufptr = 0;
		*sline = *line;
		*splace = *place;
		buffer[0] = 39;
		*bufptr = 1;
		break;
	case T_LBRAKET:
		if (gettype(cha) == T_STAR)
		{
			*nstate = NOTE;
			smlread(code, pos, place);
		}
		else
			*nstate = NOVALUE;
		if (issavedsymbol(buffer) == 0)
		{
			printf("SYMBOLIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		}
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_NUM:
		*nstate = NUM1;
		if (issavedsymbol(buffer) == 0)
		{
			printf("SYMBOLIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		}
		memset(buffer, 0, 255);
		*bufptr = 0;
		*sline = *line;
		*splace = *place;
		buffer[0] = ch;
		*bufptr = 1;
		break;
	case T_E:
	case T_OTHERLTR:
		*nstate = LETTER;
		if (issavedsymbol(buffer) == 0)
		{
			printf("SYMBOLIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		}
		memset(buffer, 0, 255);
		*bufptr = 0;
		*sline = *line;
		*splace = *place;
		buffer[0] = ch;
		*bufptr = 1;
		break;
	case T__:
		if (issavedsymbol(buffer) == 0)
		{
			printf("SYMBOLIC IDENTIFIER\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		}
		memset(buffer, 0, 255);
		*bufptr = 0;
		if (gettype(cha) == T_PRIME || gettype(cha) == T_NUM || gettype(cha) == T_OTHERLTR || gettype(cha) == T_E
			|| gettype(cha) == T__)//对通配符不太熟悉，暂时认为一个单独的它后面不能直接跟字母、撇号、数字、另一个下划线。
		{
			*nstate = ERR;
			*sline = *line;
			*splace = *place;
			buffer[0] = 95;
			*bufptr = 1;
		}
		else
			*nstate = NOVALUE;
		break;
	default:
		*nstate = ERR;
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	}
}

void state_type(char code[], int* nstate, char buffer[], int* bufptr, long* pos, int* line, int* place, int* sline, int* splace)
{
	char ch = smlread(code, pos, place);
	char cha = smlreadahead(code, pos);
	switch (gettype(ch)) {
	case T_END:
		*nstate = END;
		printf("TYPE VARIABLE\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_DOT:
	case T_RBRAKET:
	case T_SEPERATE:
		*nstate = NOVALUE;
		printf("TYPE VARIABLE\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_NEWLINE:
		*nstate = NOVALUE;
		*line = *line + 1;
		*place = 0;
		printf("TYPE VARIABLE\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
		/*case T_RETURN:
		*nstate=NOVALUE;
		*place=0;
		if(issavedletter(buffer)==0)
		{
		printf("TYPE VARIABLE\tLINE%d\tPLACE%d:\n%s\n",*sline,*splace,buffer);
		}
		memset(buffer,0,255);
		*bufptr=0;
		break;*/
	case T_SLANT:
	case T_STAR:
	case T_OTHERSYM:
		*nstate = SYMBOL;
		printf("TYPE VARIABLE\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		*sline = *line;
		*splace = *place;
		buffer[0] = ch;
		*bufptr = 1;
		break;
	case T_QUOT:
		*nstate = STRING;
		printf("TYPE VARIABLE\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		*sline = *line;
		*splace = *place;
		buffer[0] = 34;//"
		*bufptr = 1;
		break;
	case T_POUND:
		printf("TYPE VARIABLE\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		if (gettype(cha) == T_QUOT)
		{
			*nstate = CHAR;
			*sline = *line;
			*splace = *place;
			smlread(code, pos, place);
			buffer[0] = 35;//#
			buffer[1] = 34;//"
			*bufptr = 2;
		}
		else
		{
			*nstate = SYMBOL;
			*sline = *line;
			*splace = *place;
			buffer[0] = 35;//#
			*bufptr = 1;
		}
		break;
	case T__:
	case T_E:
	case T_OTHERLTR:
	case T_NUM:
	case T_PRIME:
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	case T_LBRAKET:
		if (gettype(cha) == T_STAR)
		{
			*nstate = NOTE;
			smlread(code, pos, place);
		}
		else
			*nstate = NOVALUE;
		printf("TYPE VARIABLE\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_NEG:
		printf("TYPE VARIABLE\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		if (gettype(cha) == T_NUM)
			*nstate = NUM1;
		else
			*nstate = SYMBOL;
		*sline = *line;
		*splace = *place;
		buffer[0] = ch;
		*bufptr = 1;
		break;
	default:
		*nstate = ERR;
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	}
}

void state_err(char code[], int* nstate, char buffer[], int* bufptr, long* pos, int* line, int* place, int* sline, int* splace)
{
	char ch = smlread(code, pos, place);
	char cha = smlreadahead(code, pos);
	switch (gettype(ch)) {
	case T_END:
		*nstate = END;
		printf("ERROR\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_RBRAKET:
	case T_SEPERATE:
		*nstate = NOVALUE;
		printf("ERROR\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	case T_NEWLINE:
		*nstate = NOVALUE;
		*line = *line + 1;
		*place = 0;
		printf("ERROR\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
		/*case T_RETURN:
		*nstate=NOVALUE;
		*place=0;
		printf("ERROR\tLINE%d\tPLACE%d:\n%s\n",*sline,*splace,buffer);
		memset(buffer,0,255);
		*bufptr=0;
		break;*/
	case T_NEG:
	case T__:
	case T_SLANT:
	case T_E:
	case T_OTHERLTR:
	case T_NUM:
	case T_DOT:
	case T_STAR:
	case T_PRIME:
	case T_POUND:
	case T_OTHERSYM:
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	case T_QUOT:
		*nstate = STRING;
		printf("ERROR\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		*sline = *line;
		*splace = *place;
		buffer[0] = 34;
		*bufptr = 1;
		break;
	case T_LBRAKET:
		if (gettype(cha) == T_STAR)
		{
			*nstate = NOTE;
			smlread(code, pos, place);
		}
		else
			*nstate = NOVALUE;
		printf("ERROR\tLINE%d\tPLACE%d:\n%s\n", *sline, *splace, buffer);
		memset(buffer, 0, 255);
		*bufptr = 0;
		break;
	default:
		buffer[*bufptr] = ch;
		*bufptr = *bufptr + 1;
		break;
	}
}

void jmpstate(char code[], int* nstate, char buffer[], int* bufptr, long* pos, int* line, int* place, int* sline, int* splace, bool* endflag)
{
	switch (*nstate) {
	case NOTE:state_note(code, nstate, pos, line, place, sline, splace); break;
	case CHAR:state_char(code, nstate, buffer, bufptr, pos, line, place, sline, splace); break;
	case STRING:state_string(code, nstate, buffer, bufptr, pos, line, place, sline, splace); break;
	case NOVALUE:state_novalue(code, nstate, buffer, bufptr, pos, line, place, sline, splace); break;
	case NUM1:state_num1(code, nstate, buffer, bufptr, pos, line, place, sline, splace); break;
	case NUM2:state_num2(code, nstate, buffer, bufptr, pos, line, place, sline, splace); break;
	case NUM3:state_num3(code, nstate, buffer, bufptr, pos, line, place, sline, splace); break;
	case NUM4:state_num4(code, nstate, buffer, bufptr, pos, line, place, sline, splace); break;
	case LETTER:state_letter(code, nstate, buffer, bufptr, pos, line, place, sline, splace); break;
	case SYMBOL:state_symbol(code, nstate, buffer, bufptr, pos, line, place, sline, splace); break;
	case TYPE:state_type(code, nstate, buffer, bufptr, pos, line, place, sline, splace); break;
	case ERR:state_err(code, nstate, buffer, bufptr, pos, line, place, sline, splace); break;
	case END:*endflag = 1; break;
	}
}

int main(int argc, char *argv[])
{

	printf("LSMlexer\nYou need to open it from cmd with filename, or \"no input\" will be printed.\n");
	printf("\"\\t\" in the file will only be counted as one place, so maybe the \"PLACE\" seems not explicit.\n");

	FILE *in;

	/*命令行格式测试*/
	if (argc<2)
	{
		printf("No input.");
		exit(1);
	}

	/*打开源文件*/
	if ((in = fopen(argv[1], "r")) == NULL)
	{
		printf("Open file failed.");
		exit(1);
	}
	printf("Open file successed.Print the code:\n");

	/*判断源文件长度*/
	long len = 0L;
	fseek(in, 0L, SEEK_END);
	len = ftell(in);

	//printf("%ld",len);

	/*读源文件*/
	rewind(in);

	char* code = new char[len];
	memset(code, 0, len);
	long i = 0;
	while (!feof(in))
	{
		*(code + i) = fgetc(in);
		i++;
		//int ascii=int(*(code+i-1));
		//printf("ascii=%d\n",ascii);
	}
	//printf("\n%ld\n",i);

	printf("%s", code);

	printf("\n\n");
	int nstate[1] = { NOVALUE };
	char buffer[255] = {};
	int bufptr[1] = { 0 };
	long pos[1] = { 0 };
	int line[1] = { 1 };
	int place[1] = { 0 };
	int sline[1] = { 0 };
	int splace[1] = { 0 };
	bool endflag[1] = { 0 };

	while (!*endflag)
	{
		jmpstate(code, nstate, buffer, bufptr, pos, line, place, sline, splace, endflag);
	}

	system("pause");

	return 0;
}
