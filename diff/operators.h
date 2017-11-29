#ifdef OPER_DEF

OPER_DEF( add_cmd, +, ADD( tree, dL, dR ) )
OPER_DEF( sub_cmd, -, SUB( tree, dL, dR ) )
OPER_DEF( mul_cmd, *, ADD( tree, MUL( tree, dL, R ), MUL( tree, L, dR ) ) )
OPER_DEF( div_cmd, /, DIV( tree, SUB( tree, MUL( tree, dL, R ), MUL( tree, L, dR ) ), MUL( tree, R, R ) ) )

#endif /*OPER_DEF*/
