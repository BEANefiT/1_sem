#ifndef __BACK_END_H__
#define __BACK_END_H__

enum back_type
{
	recurs = 0,
	normal = 1
};

int	analyser_make_asm( struct analyser_t * );
int	back_node( struct analyser_t *, struct tree_node_t *, FILE *, enum back_type mode );

#define L( node ) tree_get_next( node, left )

#define R( node ) tree_get_next( node, right )

#endif /*__BACK_END_H__*/
