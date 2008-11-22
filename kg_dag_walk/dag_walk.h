#if !defined(KG_DAGWALK_H_)
#define KG_DAGWALK_H_
#include <cassert>
/**
* dag_walk exemplifies a generic dag walk, 
* The nodes which the dag is built up of can be any user defined  Node data structure.
* Its only requirement is that it should have a way to set/unset a visit flag, 
* which can be used to trackn whrether the node had been already visited or not.
* The nodes can be NULL nodes ( hence we are passing them as pointers)
* Also, the nodes thus passed are mutable( ie: not const nodes).
* The node_trait class defines access functions for the node.
* 
* The actual operation done upon visiting each dag node is encapsulated by a walk
* template class W. The walk_traits accessor class defines accssors to get and set the 
* required information from W.
*
**/



namespace kg {

template < typename N >
struct node_traits {
	
	
	//get the visit tag from the node
	static bool get_visit_tag(const  N * pNode ) { return false;}
	//set the visit flag
	static void set_visit_tag( N *pNode, bool b ) { return; }


	//how the children of the nodes are defined
	typedef N * node_container;
	typedef N ** node_iterator;
	typedef const N ** node_const_iterator;

	//get the begin and end iterators for the children
	static typename node_iterator get_node_container_begin( N *pNode ) { return NULL;}
	static typename node_const_iterator get_node_container_begin(const  N *pNode ) { return NULL;}
	static typename node_iterator get_node_container_end( N *pNode ) { return NULL;}
	static typename node_const_iterator get_node_container_end(const N *pNode ) { return NULL;}	

};


struct walk_traits_base {
	/**
	*these specify the different return types 
	*for the dag walk
	* eOK = the result of the walk was a success
	* ePrune = further traversal of the descenadnts of this node is disallowed
	* eStop = stop the traversal and return immediately
	* eError = same as eStop, but signifies an error
	**/
	typedef enum { eOK, ePrune, eStop, eError } walk_result;
};




template < typename N, typename  W>
struct walk_traits : public walk_traits_base {
//W should have a defined an internal class or struct 
//called stack_type. Further it should have the member variables
//	stack_type & get_stack()
//	const stack_type &get_stack() const;
//THis is not much of a problem usually since,
//W is a class that the user will be makin exclusively for
//the purpose of walk unlike N. N  may be more
//or a less predfined class, and its definition 
//may be not modifiable by the user.
	typedef typename W::stack_type stack_type;

	//does this walk allows multiple visits for
	//the dag node. generally during a dag walk, the nodes
	// visisted once are not visited again
	static const bool is_multiple_visits_allowed = false;

	//main operation that need be performed on the node
	//THis is called in dag_walk, for each node,
	//before we recurse into the dscendents 
	static walk_result pre(  N *pNode, W &w ) { return eOK;}

	//get the curent stack variable of W
	static stack_type & get_stack( W & w){ return w.get_stack(); }
	static const stack_type &get_stack( const W & w) { return w.get_stack();}

	//set the curent stack on W
	static void set_stack( W &w, const stack_type &s ){ return ; }

	//main operatrion that will be called after visiting the descendants
	//this function is called after visiting the children
	static walk_result post( N * pNode, W &w, const walk_result &res ){ return res;}
};

/**
*	Generic dag_walk procedure
*
*   - do the pre operation on the node
*   - visit the children and call dag_walk recursively
*   - do the post operation on th node
*   @param pNode[in, out] pointer to node.
*          Note that pNode is a not a const pointer and is mutable
*		
*   @param w [in, out] the walk operation on the node
*   @return walk_result 
**/

template < typename N, typename W >
walk_traits_base::walk_result dag_walk( N *pNode, W &w)
{
	typedef typename walk_traits<N, W> wtraits;
	typedef typename node_traits< N > ntraits;
	typedef walk_traits_base::walk_result walk_result;

	walk_result res = wtraits::eOK;
	bool b_visit_children = true;

	//there is no need to visit this node 
	//if multiple visits are not allowed and it has already been visited earlier
	bool b_should_do_main_operations = wtraits::is_multiple_visits_allowed || !ntraits::get_visit_tag( pNode);
	if( b_should_do_main_operations)
	{
		res = wtraits::pre( pNode, w );

		//set the visit flag for this node
		ntraits::set_visit_tag( pNode, true );
		//if the result of the main operation 
		//is prune, stop or error, the not visit children
		if ( res > wtraits::eOK)
		{
			b_visit_children = false;
		}
	} else {
		//there is no need to continue
		//this  node has been visited once
		return res;
	}
	//if the children ought to be visisted
	if ( b_visit_children )
	{
		//copy the curreht stack
		//we need to rstore it after visiting chilren
		wtraits::stack_type s_copy( wtraits::get_stack( w) );
		ntraits::node_iterator nit, nbegin, nend;
		nbegin = ntraits::get_node_container_begin( pNode );
		nend = ntraits::get_node_container_end( pNode );
		for( nit = nbegin; nit != nend; ++ nit )
		{
			N * pChild = *nit;
			assert( pChild );

				res = dag_walk( pChild, w );

			//restore the stack on w
			//so that the next child gets the same context
			wtraits::set_stack( w, s_copy);

			//if the rsult of this walk is more serious
			//than ePrune, let us quit
			if ( res >= wtraits::eStop )
			{
				break;
			}
		}		
	}	
	//finally do the post operation
	//The post operation is the one done after vsisting the children.
	//It is passed the previous value of res.
	//So the first step in post, has to check the result so far,
	//and treat it accordingly 
	if( res <= wtraits::ePrune )
	{
		res = wtraits::post( pNode, w, res );
	}
	return res;
}




/**
*	Generic tree_walk procedure
*
*   - do the pre operation on the node
*   - visit the children and call tree_walk recursively
*   - do the post operation on th node
* tree walk is a simplified form of dag walk
* in that the nodes need nothave a visit flag mamber
*   @param pNode[in, out] pointer to node.
*          Note that pNode  not a const pointer and is mutable
*		
*   @param w [in, out] the walk operation on the node
*   @return walk_result 
**/

template < typename W, typename N >
walk_traits_base::walk_result tree_walk( W & w, N *pNode)
{
	typedef typename walk_traits<W, N> wtraits;
	typedef typename node_traits< N > ntraits;
	typedef walk_traits_base::walk_result walk_result;
	using walk_traits_base::eOK;
	using walk_traits_base::ePrune;
	using walk_traits_base::eStop;
	using walk_traits_base::eError;

	walk_result res = wtraits::pre( w, pNode );
	//if the result of the main operation 
	//is prune, stop or eror, the not visist children
	bool b_visit_children = true;
	if ( res > eOK)
	{
		b_visit_children = false;
	}

	//if the children ought to be visisted
	if ( b_visit_children )
	{
		//copy the curreht stack
		//we need to rstore it after vssiting chilren
		wtraits::stack_type s_copy( wtraits::get_stack( w) );
		ntraits::node_iterator nit, nbegin, nend;
		nbegin = ntraits::get_node_container_begin( pNode );
		nend = ntraits::get_node_container_end( pNode );
		for( nit = nbegin; nit != nend; ++ nit )
		{
			N * pChild = *nit;
			assert( pChild );

			res = tree_walk( w, pChild );

			//restore the stack on w
			//so that the next child gets the same context
			wtraits::set_stack( w, s_copy);

			//if the rsult of this walk is more serious
			//than ePrune, let us quit
			if ( res >= wtraits::eStop )
			{
				break;
			}
		}
	}	
	//finally do the post operation
	//The pst operation is the one done after vsisting the children.
	//It is passed the previous value of res.
	//So the first step in post, has to check the result so far,
	//and treat it accordingly
	if ( res <= ePrune )
	{
		res = wtraits::post( w, pNode, res );
	}
	return res;
}

}; //namespace kg
#endif //KG_DAGWALK_H_