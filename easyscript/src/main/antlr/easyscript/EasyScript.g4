grammar EasyScript;

@header {
package easyscript;
}

start   : expr EOF
        ;
expr    : left=expr '+' right=expr # AddExpr
        | literal                  # LiteralExpr
        ;
literal : INT | DOUBLE
        ;

fragment DIGIT : [0-9] ;
           INT : DIGIT+ ;
        DOUBLE : DIGIT+ '.' DIGIT+ ;


WS : (' ' | '\r' | '\t' | '\n' | '\f')+ -> skip ;
