/* Definition Area
    
*/
%{
    #include <stdio.h>
    #include "mips.tab.h"
    int hextoi(char *);
    int bintoi(char *);
%}
/*definiciones regulares*/
minus           [a-z]
mayus           [A-Z]
letra           {minus}|{mayus}
digito          [0-9]
hex             [0-9A-Fa_f]
bin             [01]
espacio         [ \t\n]
identificador   {hex}|["10""11""12""13""14""15"]
%%
"main"              { return MAIN; }
"if"                { return IF; }
"else"              { return ELSE; }
"while"             { return WHILE; }
"return"            { return RETURN; }
"mem"               { return MEM; }
"R"{identificador}  { return R_ID; }
"/*"(.*)"*/"        { return COMENTARIO; }
"{"                 { return '{'; }
"}"                 { return '}'; }
"("                 { return '('; }
")"                 { return ')'; }
"["                 { return '['; }
"]"                 { return ']'; }
";"                 { return ';'; }
","                 { return ','; }
"<<"                { return C_IZQ; }
">>"                { return C_DER; }
"=="                { return IGUAL; }
"!="                { return DIFERENTE; }
"<="                { return MENORI; }
">="                { return MAYORI; }
"<"                 { return '<'; }
">"                 { return '>'; }
"+"                 { return '+'; }
"-"                 { return '-'; }
"|"                 { return '|'; }
"^"                 { return '^'; }
"&"                 { return '&'; }
"="                 { return '='; }
(("-")?{digito}+)   { yylval = atoi(yytext); return CONSTANTE; }
("0x"{hex}+)        { yylval = hextoi(yytext); return CONSTANTE; }
"\""{bin}+"\""      { yylval = bintoi(yytext); return CONSTANTE; }
espacio

%%

/*auxiliar*/
int bintoi(char *cadena){
    int i = 0, entero = 0, potencia = 1;
    for (i = 0; cadena[i] != '\0'; i++){
        entero += cadena[i] - '0';
        entero *= potencia;
        potencia *= 2;
    }
    return entero;
}
int hextoi(char *cadena){
    int i = 0, entero = 0, potencia = 1;
    for (i = 0; cadena[i] != '\0'; i++){
        entero += cadena[i] - '0';
        entero *= potencia;
        potencia *= 16;
    }
    return entero;
}