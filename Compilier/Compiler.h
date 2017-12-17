enum Compilier_modes
{
	first,
	second
};

struct labels_t
{
	char*	name;
	size_t	pos;
};

struct Compilier_structure
{
	struct labels_t**    labels;
	size_t               src_sz;
	size_t               exe_sz;
	char*                src;
	void*                exe;
	int                  lbl_count;
	enum                 Compilier_modes mode;
};

int	Compilier_constr( struct Compilier_structure *Compilier );
char*	get_src( char *src_file_name, struct Compilier_structure *Compilier );
size_t	srcSize( FILE *src );
int	compile( struct Compilier_structure *Compilier );
int	makecode( struct Compilier_structure *Compilier );
