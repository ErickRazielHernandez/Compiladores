%{
/* MIPS */
    /* Manejador de errores */
    #include <stdlib.h>
    #include <stdio.h>

    void yyerror( char * message ) {
        printf( "Error de sintaxis: %s\n", message );
        exit(0);
    }
%}

%token MAIN
%token IF
%token ELSE
%token WHILE
%token RETURN
%token MEM
%token R_ID
%token COMENTARIO
%token C_IZQ
%token C_DER
%token IGUAL
%token DIFERENTE
%token MAYORI
%token MENORI
%token CONSTANTE
%start programa

%%

programa
:	MAIN '{' codigo '}'
;

codigo
:	instruccion
|	instruccion codigo
;

instruccion
:	expresion ';'
|	COMENTARIO
;

expresion
:	while
|	if
|	RETURN R_ID
|	RETURN valor_en_memoria
|	RETURN CONSTANTE
|	asignacion
;

operacion
:	operando operador operando
;

asignacion
:	operando '=' operando
;

operador
:	'+'
|	'-'
|	'*'
|	'/'
|	'&'
|	'|'
|	'~'
|	'^'
;

operando
:	valor_en_memoria
|	R_ID
|	operacion
|	'(' operando ')'
;

valor_en_memoria
:	MEM '[' CONSTANTE ']'
|	MEM '[' R_ID '+' CONSTANTE ']'
|	MEM '[' CONSTANTE '+' R_ID ']'
;

operador_logico
:	IGUAL
|	DIFERENTE
|	'<'
|	'>'
|	MAYORI
|	MENORI
;

condicion
:	operando operador_logico operando
;

while
:	WHILE '(' condicion ')' '{' codigo '}'
;

if
:	IF '(' condicion ')' '{' codigo '}'
|	IF '(' condicion ')' '{' codigo '}' ELSE '{' codigo '}'
;

%%