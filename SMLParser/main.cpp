#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_DEPRECATE
using namespace std;
/*
* ��һ��ָ��line��place��pos��ʾ�������Ĵ���λ�á������µĴ��أ����˳��������ǰ�����µ�״̬����ָ��sline��splace��ʾ������صĵ�һ���ַ���λ�ã���������һ�����غ���ʾ����λ�á�
* �ú���lsmread��posָ���ĵ�ǰλ�ã��ú���smlreadahead���һλ����ʵsmlreadaheadҲ�Ƕ�posָ���ĵ�ǰλ�ã���ÿ��lsmread֮��pos����һλ����lsmreadahead����lsmread֮��ʹ�ã��Ҳ����ƶ�pos��
* ��һ��prev_element��¼��һ��Ԫ�أ����ڸ���֮���
* �ڱ���SML����Ĺ����У���������´���״̬��
* ��������״̬��״̬���������о������ŵ�˳�򲻺ã�����ֻд����Щ״̬��������š�
* ע��״̬�����ֶ������ַ���(��Ԥ��һλΪ*ʱ�������״̬��֮�������ȫ��������ֱ������*��Ԥ��һλΪ)ʱ�˳���״̬�ص�״̬3��
* �ַ�״̬�����ֶ������ַ���#��Ԥ��һλΪ"ʱ�������״̬��ֱ��������һ��"�˳���״̬����֮�������⴦����ַ���״̬��ͬ���������ַ�״ֻ̬�ܰ���1���ַ������Ի���м������������˳�״ֻ̬��"Ϊ�жϱ�׼����#��ʼ�����뻺����������ӡֵ����Ϊchar����������������ַ�����1������ʱ����ʾ������Ϣ��
* �ַ���״̬��һ�����ֶ���"ʱ�������״̬��ֱ��������һ��"�˳���״̬���������������\��֮�����һ���ַ������⴦�������ж����Ƿ���"��������һ��"�����һ��"���ڵ����ݶ����ڱ���ʱ���뻺����������ӡ������ֵ������Ϊstring��
* ��Ԫ��״̬��ʲô��û�н���ʱ�������ո�����Ʊ�����߻��з��Լ�,;()[]{}ʱ����������״̬���ȴ���һ�ζ������������ݡ��ʷ������������ж�(�Բ���Ӧ��
* ����״̬1���ڷ�Ԫ��״̬�»��߷��ű�ʶ��״̬���������ֺ�����״̬������Ƿ�Ԫ��״̬������~��Ԥ��1λ������Ҳ���������״̬������Ҫ�ŵ�������������~�Ļ�~Ҳ�Ž�ȥ�����������Ԫ��״̬��ע��״̬�����߶������ű�ʶ�����õķ��ţ����ӡ����Ϊdigit�������˳���֮���ܹ�һֱ֧�ֱ������״̬�ľ��Ƕ������֣��ٶ�~��Ҫ�����ű�ʶ��״̬�ˡ��������.����������״̬2���������E����������״̬3��������������ĸ���»��ߡ�Ʋ��֮��ģ���������״̬������״̬Ӧ�û����һֱ������Ԫ���ࡢע���ࡢ�ַ����ַ�������ͣ�����ɣ�
* ����״̬2���������״̬������ջ���������ʼλ�ò��䣬���ǻ���������ֽ��뻺����������E��������״̬3�����Ԥ������E��.�����Ž���������������ٶ�.Ҳ�ǽ�����״̬�ġ���������ת���ж�������״̬1����ˡ�1��.֮��ֱ�Ӹ������ķ���Ӧ�ò�����̫�����⣬���������Ļ���ӡ��digit���ȥ������Ǹ�.��
* ����״̬3���������״̬Ҳ������ջ����������俪ʼλ�á����״̬����֮�������ͷ��~��Ԥ��һλ�������������������������ֻ�ܽ�����״̬�ˡ�֮����Ǽ��������֡������.��������״̬4������ٶ���E��Ҳֻ�ܽ�����״̬�ˡ�����������Ǻ�һ�������״̬��ࡣ
* ����״̬4���������״̬������ջ���������ʼλ�ò��䡣֮��Ҫά������״ֻ̬�ܶ������ˣ�����.��E����������״̬��Ԥ��һλ�����������ô.�ڻ����������ʲô��û�н���ֱ���˳�����״̬4����ô.�����뻺��������ӡʱȥ���Ǹ�.��������һ������״̬��ࡣ
* ���ű�ʶ��״̬���ڷ�Ԫ��״̬������״̬����ĸ��ʶ��״̬��������ʶ���Ͽɵ�!%&$#+-*\/:<=>?@~`^| ���֮ǰû��ƥ�����ַ�״̬�������ͷ��~��Ԥ����������֮�࣬�Ǿͽ������״̬�����ݶ����뻺�������������ͬ���ķ��Ż�һֱ����ȥ���������.����Ӧ����Ҫ������һ����ʶ���ˣ�����֮�����ȴ�ӡ���ط�Ԫ��״̬��������ĸ"���ֺͷ�Ԫ��״̬���������˳�������_'��������״̬���˳�ʱ�жϻ������Ƿ�͹ؼ�����ƥ�䣬������򲻴�ӡ�������ӡsymbolic name�ͻ��������ݡ�
* ��ĸ��ʶ��״̬����Ԫ��״̬�����ű�ʶ��״̬��������ĸ�����״̬��֮����Խ�����ĸ���ֺ�_' ���������Ԫ��״̬�����ű�ʶ����.���˳���ĸ��ʶ�����˳�ʱ�ж��Ƿ��ǹؼ��֣�������ǣ���ӡalphabatic name�ͻ��������ݡ�
* ����״̬����Ԫ��״̬��������_'.�����ķ��ţ��Լ���������֮���û���ֵķ��š�����״̬������ĸ��_'�����ű�ʶ��״̬����_'����֮���һϵ�ж��ᵱ�ɴ������ݵ�һ���֣�ֱ������ע�͡��ַ�������Ԫ�صȡ�
* �������������κ�״̬������'\0'������������״̬��endflag��1��
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
//#define T_RETURN 3 ���ֶ����Ĵ�����Զ�ת��\n���������\r
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

char smlreadahead(char code[], long *pos)//���������������룬��������д��������ֱ��дcode[*pos]�������Ժ󲻻����ˡ�
{
	char ch;
	ch = code[*pos];
	return ch;
}

int gettype(char ch)
{
	if (ch == 0 || ch == -1)//��code����ASCII����ʾ�����������������ķ�����-1��
		return T_END;
	else if (ch == 9 || ch == 32 || ch == 44 || ch == 59 || ch == 91 || ch == 93 || ch == 123 || ch == 125)
		return T_SEPERATE;//�ָ����������ո��Ʊ����š��ֺá������š������ŵȡ�
	else if (ch == 10)
		return T_NEWLINE;//���з�����Ϊ�ᵼ��line�ı仯���ӷָ����е����г���
						 /*else if(ch==13)
						 return T_RETURN;//�س������ص���һλ��*/
	else if (ch == 33 || (ch >= 36 && ch <= 38) || ch == 43 || ch == 45 || ch == 47 || ch == 58 || (ch >= 60 && ch <= 64) || ch == 94 || ch == 96 || ch == 124)
		return T_OTHERSYM;//��ͨ�ķ���
	else if (ch == 34)
		return T_QUOT;//˫����
	else if (ch == 35)
		return T_POUND;//���ţ���Ϊ���������ַ��Ͷ�����ͨ�����е����г���
					   //else if(ch==39 || ch==95)
					   //    return T_LSYM;//����ĸ��ʹ�õķ��ţ�Ʋ�ź��»���
	else if (ch == 39)
		return T_PRIME;
	else if (ch == 40)
		return T_LBRAKET;//�����ţ���Ϊ��ע����ɲ��ֶ��ӷָ������г���
	else if (ch == 41)
		return T_RBRAKET;//�����ţ���Ϊ��ע����ɲ��ֶ��ӷָ������г�
	else if (ch == 42)
		return T_STAR;//�Ǻţ���Ϊ��ע����ɲ��ֶ�����ͨ�����е����г���
	else if (ch == 46)
		return T_DOT;//���
	else if (ch >= 48 && ch <= 57)
		return T_NUM;//����
	else if ((ch >= 65 && ch <= 68) || (ch >= 70 && ch <= 90) || (ch >= 97 && ch <= 122))
		return T_OTHERLTR;//�������ĸ����E
	else if (ch == 69)
		return T_E;//��д��ĸE�������ܹ������ֱ������г�
	else if (ch == 92)
		return T_SLANT;//��б�ߣ���Ϊ������Ϊת���ַ�������ͨ�����е����г���
	else if (ch == 95)
		return T__;
	else if (ch == 126)
		return T_NEG;//���˺ţ���Ϊ�������ΪһԪ���Ŷ�����ͨ�����е����г���
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
		else if (cha == '1')//1��ͷ������1__Ҳ������1_����1��
		{
			smlread(code, pos, place);
			char chaa = smlreadahead(code, pos);
			if (chaa >= 48 && chaa <= 55)//chaaΪ0~7��ע��˽���
			{
				smlread(code, pos, place);
				char chaaa = smlreadahead(code, pos);
				if (chaaa >= 48 && chaaa <= 55)//1__�����
				{
					smlread(code, pos, place);
					buffer[*bufptr] = 64 + 8 * (chaa - 48) + chaaa - 48;
					*bufptr = *bufptr + 1;
				}
				else//1_�����
				{
					buffer[*bufptr] = 8 + chaa - 48;
					*bufptr = *bufptr + 1;
				}
			}
			else//1�����
			{
				buffer[*bufptr] = 1;
				*bufptr = *bufptr + 1;
			}
		}
		else if (cha >= 50 && cha <= 55)//2~7�ģ���ȥ�����ӡ�
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
		else if (cha == ' ' || cha == '\t' || cha == '\n')//\֮���һ���ո�����Ʊ�����߻��У�֮���ٸ�\�������κζ�������Ȼ������֮���ַ������š�
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

		}��ǰ���\n�����������ո�ֿ������ǿո������̫���ˣ����Ի�����һ��else�����if�ж�д��������*/
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
		else if (cha == '1')//1��ͷ������1__Ҳ������1_����1��
		{
			smlread(code, pos, place);
			char chaa = smlreadahead(code, pos);
			if (chaa >= 48 && chaa <= 55)//chaaΪ0~7��ע��˽���
			{
				smlread(code, pos, place);
				char chaaa = smlreadahead(code, pos);
				if (chaaa >= 48 && chaaa <= 55)//1__�����
				{
					smlread(code, pos, place);
					buffer[*bufptr] = 64 + 8 * (chaa - 48) + chaaa - 48;
					*bufptr = *bufptr + 1;
				}
				else//1_�����
				{
					buffer[*bufptr] = 8 + chaa - 48;
					*bufptr = *bufptr + 1;
				}
			}
			else//1�����
			{
				buffer[*bufptr] = 1;
				*bufptr = *bufptr + 1;
			}
		}
		else if (cha >= 50 && cha <= 55)//2~7�ģ���ȥ�����ӡ�
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
		else if (cha == ' ' || cha == '\t' || cha == '\n')//\֮���һ���ո�����Ʊ�����߻��У�֮���ٸ�\�������κζ�������Ȼ������֮���ַ������š�
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

		}��ǰ���\n�����������ո�ֿ������ǿո������̫���ˣ����Ի�����һ��else�����if�ж�д��������*/
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
	case T_STAR://*��\�������Ǻ���������û������ġ�
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
	case T_DOT://._'����������¶��Ǵ����д����
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
	case T_OTHERLTR://E��������ĸ��ʱ������ͨ����ĸ��
	case T_E:
		*nstate = LETTER;
		*sline = *line;
		*splace = *place;
		buffer[0] = ch;
		*bufptr = 1;
		break;
	case T__:
		if (gettype(cha) == T_PRIME || gettype(cha) == T_NUM || gettype(cha) == T_OTHERLTR || gettype(cha) == T_E
			|| gettype(cha) == T__)//��ͨ�����̫��Ϥ����ʱ��Ϊһ�������������治��ֱ�Ӹ���ĸ��Ʋ�š����֡���һ���»��ߡ�
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
		if (gettype(cha) == T_NUM)//���.���治�������ˣ��ɴ�Ͳ��ŵ����������ˡ�
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
	case T_DOT://����һ��С���㣬�Ҳ���E���棬�����ٳ���С���㡣
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
	case T_E://����һ��E�Ժ����ٳ���E��
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
		if (gettype(cha) == T_NUM)//���.���治�������ˣ��ɴ�Ͳ��ŵ����������ˡ�
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
	case T_DOT://������E�ֳ���С���㣬��������С�����E��
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
			|| gettype(cha) == T__)//��ͨ�����̫��Ϥ����ʱ��Ϊһ�������������治��ֱ�Ӹ���ĸ��Ʋ�š����֡���һ���»��ߡ�
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

	/*�����и�ʽ����*/
	if (argc<2)
	{
		printf("No input.");
		exit(1);
	}

	/*��Դ�ļ�*/
	if ((in = fopen(argv[1], "r")) == NULL)
	{
		printf("Open file failed.");
		exit(1);
	}
	printf("Open file successed.Print the code:\n");

	/*�ж�Դ�ļ�����*/
	long len = 0L;
	fseek(in, 0L, SEEK_END);
	len = ftell(in);

	//printf("%ld",len);

	/*��Դ�ļ�*/
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
