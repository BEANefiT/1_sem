enum seprs
{
	mono,
	duo
};

struct aki_structure
{
	struct tree_t *tree;
	char *src;
	size_t src_sz;
	size_t src_cur;
	enum seprs mode;
};
int aki_constr( struct aki_structure *akinator );
char *getsrc( char *src_name, struct aki_structure *akinator );
size_t src_sz( FILE *src );
int make_tree( struct aki_structure *akinator );
int start( struct aki_structure *akinator );
int base_edit( struct aki_structure *akinator, char *base_name );
int determine( struct aki_structure *akinator );
int get_command( struct aki_structure *akinator, char *base_name );
