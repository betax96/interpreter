%option noyywrap
%{
#include "parser.h"
#include <QString>
#include <QPlainTextEdit>
int lines =1;
int errorLine = 1;
QPlainTextEdit* error;
int errorLineNumber(){return errorLine;};
void setTextEditError(QPlainTextEdit* edit1)   {lines=1; error=edit1; }
void printError(QString val){error->appendPlainText(val); errorLine=lines;}
%}

ignore " "|\t|\r
caracter [A-Za-z]
digito [0-9]
boleano "true"|"false"

%%

{ignore}				{;}
"\n"				{lines++;}
"."					{return ('.');}
"{"					{return ('{');}
"}"					{return ('}');}
";"					{return (';');}
","					{return (',');}
"("					{return ('(');}
")"					{return (')');}
"["					{return ('[');}
"]"					{return (']');}
"#"					{return ('#');}
"si"					{return IF;}
"sino_si"				{return ELSEIF;}
"sino"					{return ELSE;}
"inicio"				{return INICIO;}
"integer"				{return ITGR;}
"decimal"				{return DOU;}
"bool"					{return T_F;}
"text"					{return TEXT;}
"para"					{return FOR;}
"mientras"				{return WHILE;}
"imprimir"				{return IMPRIMIR;}
"leer"					{return LEER;}
"+"					{return SUMA;}
"-"					{return RESTA;}
"*"					{return MULTIPLICACION;}
"/"					{return DIVISION;}
"++"					{return AUMENTAR;}
"--"					{return DISMINUIR;}
":="					{return ASIGNADOR;}
">"					{return MAYOR;}
"<"					{return MENOR;}
"=="					{return IGUAL;}
">="					{return MAYORIGUAL;}
"<="					{return MENORIGUAL;}
"<>"					{return DIFERENTE;}
{digito}+				{return ENTERO;}
{digito}+"."{digito}			{return DECIMAL;}
{boleano}				{return BOLEANO;}
"\""({digito}|{caracter}|" ")*"\""	{return CADENA;}
{caracter}({caracter}|(digito))*	{return NOMBREVARIABLE;}
.					{printError(QString("Error Lexico en la linea: %1, en %2").arg(lines).arg(yytext)); }

%%

void set_input_string(const char* in) {
  yy_scan_string(in);
}

void end_lexical_scan(void) {
  yy_delete_buffer(YY_CURRENT_BUFFER);
}