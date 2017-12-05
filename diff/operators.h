#ifdef OPER_DEF

OPER_DEF( add_cmd, bin, ADD, +,   ADD( tree, dL, dR ) )
OPER_DEF( sub_cmd, bin, SUB, -,   SUB( tree, dL, dR ) )
OPER_DEF( mul_cmd, bin, MUL, *,   ADD( tree, MUL( tree, dL, R ), MUL( tree, L, dR ) ) )
OPER_DEF( div_cmd, bin, DIV, /,   DIV( tree, SUB( tree, MUL( tree, dL, R ), MUL( tree, L, dR ) ), MUL( tree, R, R ) ) )
OPER_DEF( ln_cmd,  uno, LN,  ln,  DIV( tree, dL, L ) )
OPER_DEF( sin_cmd, uno, SIN, sin, MUL( tree, dL, COS( tree, L ) ) )
OPER_DEF( cos_cmd, uno, COS, cos, MUL( tree, num( tree, -1 ), MUL( tree, dL, SIN( tree, L ) ) ) )
OPER_DEF( pow_cmd, bin, POW, ^,   diff( tree, EXP( tree, MUL( tree, R, LN( tree, L ) ) ), dvar ) )
OPER_DEF( tan_cmd, uno, TAN, tan, DIV( tree, dL, POW( tree, COS( tree, L ), num( tree, 2 ) ) ) )
OPER_DEF( cot_cmd, uno, COT, cot, MUL( tree, num( tree, -1 ), DIV( tree, dL, POW( tree, SIN( tree, L ), num( tree, 2 ) ) ) ) )
OPER_DEF( exp_cmd, uno, EXP, exp, MUL( tree, dL, EXP( tree, L ) ) )

#endif /*OPER_DEF*/
