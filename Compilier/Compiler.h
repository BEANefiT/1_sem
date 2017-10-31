enum Compilier_modes
{
	first,
	second
};

struct Compilier_structure
{
	char *src;
	size_t src_sz;
	size_t *labels;
	void *exe;
	size_t exe_sz;
	enum Compilier_modes mode;
};

int Compilier_constr( struct Compilier_structure *Compilier );
char *get_src( char *src_file_name, struct Compilier_structure *Compilier );
size_t srcSize( FILE *src );
int compile( struct Compilier_structure *Compilier );
int makecode( struct Compilier_structure *Compilier );
