/* Definition Area
    
*/
%{  /* definiciones de lenguaje C */
    /* Lenguaje C */
    #include <stdio.h>
    /*
        Los analizadores léxicos se definen con expresiones
        regulares, las expresiones regulares son una notación
        simplificada de conjuntos regulares y los conjuntos
        regulares están hechos de dos cosas, conjuntos
        finitos + operaciones: unión, concatenación,
        cerradura ( + , * )
    */
%}
/*definiciones regulares*/
letters         [A-Za-z]
digits          [0-9]
spaces          [ \t]
operator        [+\-*/=!&|^]
identifier      {letters}({letters}|{digits}|"_")*

%%
"int"|"unsigned int"|reg|mem	       { printf("<reserved_word>"); }
"#"|":"                                { printf("<definition>"); }
"("|"["                                { printf("<left_agrup>"); }
")"|"]"                                { printf("<right_agrup>"); }
"{"                                    { printf("<codeblock_start>"); }
"}"                                    { printf("<codeblock_end>"); }
","                                    { printf("<separator>"); }
";"                                    { printf("<end_of_line>"); }
"\""                                   { printf("<double_quote>"); }
"\'"                                   { printf("<quote>"); }
"."                                    { printf("<dot>"); }
\n                                     { printf("<return>\n"); }
{spaces}                               { printf("<space>"); }
{digits}*                              { printf("<const>"); }
{operator}                             { printf("<operator>"); }
{letters}({letters}|{digits}|"_")*     { printf("<id>"); }
(letters|digits)                       { printf("<unrecognized>"); }
%%
/*auxiliar*/
