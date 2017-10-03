struct stack_t{

	typedef int value_type;

	stack_t ();
	~stack_t ();

	int			resize ( int capasity_new );
	bool			push ( value_type value );
	bool			clear ();
	bool			empty ();
	value_type 		pop ();
	value_type		top ();
	int			size ();
	int			capasity ();
	static const int	noerror = 1010;
	static const int	error_emptystack = 1011;
	static const int	error_alloc = 1012;
	static const int	error_loss = 1013;

};
