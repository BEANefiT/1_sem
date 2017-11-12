struct aki_structure
{
	char *src;
	size_t src_sz;
	struct tree_t *tree;
	size_t src_cur;
};
int aki_constr( struct aki_structure *akinator );
char *getsrc( char *src_name, struct aki_structure *akinator );
size_t src_sz( FILE *src );
int make_tree( struct aki_structure *akinator );
