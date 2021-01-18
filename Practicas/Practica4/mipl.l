%{
    #include <stdio.h>
%}

letras          [A-Za-z]
digitos         [0-9]
digitos_hex     [0-9A-Fa-f]
espacios        [ \t]
operador        [+\-*/=!&|^~]|">>"|"<<"
identificador   {letras}({letras}|{digitos}|"_")*

%%
"reg"|"mem" 					       { printf("<palabra_reservada>"); }
"if"|"else"							   { printf("<condicional>"); }
"while"								   { printf("<ciclo>"); }
{letras}+({letras}|{digitos}|"_")*	   { printf("<identificador>"); }
"("|")"|"["|"]"|"{"|"}"                { printf("<agrupador>"); }
","                                    { printf("<separador>"); }
";"                                    { printf("<fin_de_instrucción>"); }
\n                                     { printf("<retorno>\n"); }
{espacios}                             { printf(""); }
{digitos}+                             { printf("<constante_decimal>"); }
"0x"{digitos_hex}+                     { printf("<constante_hexadecimal>"); }
"\""(0|1)+"\""                         { printf("<constante_binaria>"); }
"&&"|"||"|"=="|"<="|">="|"<"|">"       { printf("<operador_logico>"); }
{operador}	                           { printf("<operador>"); }
%%