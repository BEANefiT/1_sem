#ifdef OPER_DEF

OPER_DEF( add_cmd, bin, ADD, +, ADD( tree, dL, dR ) )
OPER_DEF( sub_cmd, bin, SUB, -, SUB( tree, dL, dR ) )
OPER_DEF( mul_cmd, bin, MUL, *, ADD( tree, MUL( tree, dL, R ), MUL( tree, L, dR ) ) )
OPER_DEF( div_cmd, bin, DIV, /, DIV( tree, SUB( tree, MUL( tree, dL, R ), MUL( tree, L, dR ) ), MUL( tree, R, R ) ) )

#endif /*OPER_DEF*/
