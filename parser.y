%{
#include "lexer.h"
#include <QString>
extern int lines;
extern char *yytext;
extern void printError(QString val);
int yyerror(const char* mens)
{
    printError(QString("Error Sintactico en la linea: %1, en %2").arg(lines).arg(yytext));
    return 0;
}
%}


%token INICIO 
%token NOMBREVARIABLE
%token ENTERO 
%token IMPRIMIR
%token LEER
%token DECIMAL 
%token BOLEANO 
%token CADENA 
%token ITGR
%token DOU
%token T_F
%token TEXT 
%token ASIGNADOR
%token SUMA
%token RESTA
%token MULTIPLICACION 
%token DIVISION 
%token AUMENTAR 
%token DISMINUIR 
%token MAYOR 
%token MENOR 
%token IGUAL 
%token MAYORIGUAL
%token MENORIGUAL 
%token DIFERENTE
%token IF
%token ELSE
%token ELSEIF 
%token FOR 
%token WHILE


%start inicio

%%

inicio			:	principal funciones | principal;
principal		: 	INICIO '(' ')' '{' lineascodigos '}';
lineascodigos		: 	lineacodigo | ;
lineacodigo		:	lineacodigo linea | linea;
linea			:	invocarmetodo ';' | crearvariable ';' | cambiarvalor ';' | ciclo_condicion | imprimir ';'| leer ';' ;
imprimir 		:	IMPRIMIR '(' CADENA ')';
leer			:	LEER '#' NOMBREVARIABLE;
invocarmetodo		:	NOMBREVARIABLE '(' enviar_parametros ')';
enviar_parametros	:	enviaparametros | ;
enviaparametros		:	enviaparametros ',' envia_paramtro | envia_paramtro;
envia_paramtro		:	valor | NOMBREVARIABLE;
valor			:	ENTERO | DECIMAL | BOLEANO | CADENA;  
crearvariable		:	tipodato NOMBREVARIABLE | tipodato NOMBREVARIABLE asignarvalor;
tipodato		:	ITGR | DOU | T_F | TEXT;
asignarvalor		:	ASIGNADOR operacion_asignacion | ASIGNADOR valor | ASIGNADOR NOMBREVARIABLE;
operacion_asignacion	:	aritmetica | invocarmetodo | incre_dismin_variable;
aritmetica		:	operacion_comun | operacion_comun operacion_complemento;
operacion_comun		:	valor tipo_operacion valor | valor tipo_operacion NOMBREVARIABLE | NOMBREVARIABLE tipo_operacion valor | NOMBREVARIABLE tipo_operacion NOMBREVARIABLE;
tipo_operacion		:	SUMA | RESTA | MULTIPLICACION | DIVISION;
operacion_complemento	:	operacion_complemento operacion_complemto | operacion_complemto;
operacion_complemto	:	tipo_operacion valor | tipo_operacion NOMBREVARIABLE;
incre_dismin_variable	:	NOMBREVARIABLE incre_dismin;
incre_dismin		:	AUMENTAR | DISMINUIR;
cambiarvalor		:	NOMBREVARIABLE ASIGNADOR cambio_valor;
cambio_valor		:	valor | operacion_asignacion | NOMBREVARIABLE;
ciclo_condicion		:	ifcondicion | ciclofor | ciclowhile;
ifcondicion		:	condicion_si | condicion_si condicion_no | condicion_si condiciones_sino condicion_no;
condicion_si		:	IF '(' condicion ')' '{' lineascodigos '}';
condicion		:	valor condicional valor | valor condicional NOMBREVARIABLE | NOMBREVARIABLE condicional valor | NOMBREVARIABLE condicional NOMBREVARIABLE;
condicional		:	MAYOR | MENOR | IGUAL | MAYORIGUAL | MENORIGUAL | DIFERENTE;
condicion_no		:	ELSE '{' lineascodigos '}';
condiciones_sino	:	condiciones_sino condicion_sino | condicion_sino;
condicion_sino		:	ELSEIF '(' condicion ')' '{' lineascodigos '}';
ciclofor		:	FOR '(' inicia_for ';' condicion ';' incre_dismin_variable ')' '{' lineascodigos '}';
inicia_for		:	tipodato NOMBREVARIABLE asignarvalor;
ciclowhile		:	WHILE '(' condicion ')' '{' lineascodigos '}';
funciones		:	funciones funcion | funcion;
funcion			:	tiporetorno NOMBREVARIABLE '(' parametrosentrada ')' '{' lineascodigos '}';
tiporetorno		:	tipodato;
parametrosentrada	:	parametros;
parametros		:	parametros ',' parametro | parametro;
parametro		:	tipodato NOMBREVARIABLE;

%%

void set_input_string(const char* in);
void end_lexical_scan(void);

/* This function parses a string */
int parse_string(const char* in) {
  set_input_string(in);
  int rv = yyparse();
  end_lexical_scan();
  return rv;
}