/**

	test_dag_walk





*/
#include <limits>
#include <list>
#include <iostream>
#include "dag_walk.h"
using namespace std;

struct Node {
	Node( int data):_data(data),_visited(false){}
	int _data;
	list< Node *> _children;
	mutable bool _visited;
};

struct Walk {
	Walk():_level(0){}
	typedef int stack_type;
	stack_type _level;
	stack_type & get_stack() { return _level;}
	const stack_type & get_stack() const { return _level;}
	bool pre( Node *pNode ) {
		print( pNode);		
		inc_level();
		return true;
	}
	bool post (Node * pNode ) {
		dec_level();
		return true;
	}
	void print( Node * pNode ) {  cout << _level << " " << pNode->_data << endl; }
	void inc_level() { ++_level;}
	void dec_level() {--_level;}
};


template < >
struct kg::node_traits<Node> {
	typedef Node N;
	
	//get the visit tag from the node
	static bool get_visit_tag(const N * pNode ) { return pNode->_visited;}
	//set the visit flag
	static void set_visit_tag( N *pNode, bool b ) { pNode->_visited= b; }


	//how the children of the nodes are defined
	typedef list< Node *> node_container;
	typedef list<Node *>::iterator node_iterator;
	typedef list<Node *>::const_iterator node_const_iterator;

	//get the begin and end iterators for the children
	static node_iterator get_node_container_begin( N *pNode ) { return pNode->_children.begin();}
	static node_const_iterator get_node_container_begin(const  N *pNode ) { return pNode->_children.begin();}
	static node_iterator get_node_container_end( N *pNode ) { return pNode->_children.end();}
	static node_const_iterator get_node_container_end(const N *pNode ) { return pNode->_children.end();}	

};


template < >
struct kg::walk_traits< Node, Walk> : public walk_traits_base {
//W should have a defined an internal class or struct 
//called stack_type. Further it should have the member variables
//	stack_type & get_stack()
//	const stack_type &get_stack() const;
//THis is not much of a problem usually since,
//W is a class that the user will be makin exclusively for
//the purpose of walk unlike N. N  may be more
//or a less predfined class, and its definition 
//may be not modifiable by the user.
	typedef Walk W;
	typedef Node N;
	typedef W::stack_type stack_type;

	//does this walk allows multiple visits for
	//the dag node. generally during a dag walk, the nodes
	// visisted once are not visited again
	static const bool is_multiple_visits_allowed = false;

	//main operation that need be performed on the node
	//THis is called in dag_walk, for each node,
	//before we recurse into the dscendents 
	static walk_result pre( N *pNode, W &w ) {
		w.pre( pNode );
		return eOK;
	}

	//get the curent stack variable of W
	static stack_type & get_stack( W & w){ return w.get_stack(); }
	static const stack_type &get_stack( const W & w) { return w.get_stack();}

	//set the curent stack on W
	static void set_stack( W &w, stack_type &s ){ w._level = s; }

	//main operatrion that will be called after visiting the descendants
	//this function is called after visiting the children
	static walk_result post( N * pNode, W &w, const walk_result &res ){
		//if the current status 'res' is graver than ePrune then dont do anything.
		if( res <= ePrune )
		{ 
			w.post( pNode );
		}
		return res;
	}
};

int fun()
{

	/**
		The following will make a dag which look like
		 

		            0
			     ,/   \,
			  ,/        \,
			 /            \
            1               2
		 ,/   \,           ,/ \,
       ,/       \,       ,/     \,
      /           \     /         \
	 11             12             22   




	*/

	Node * pRoot = new Node(0);
	Node *pc1 = new Node(1); 
	Node *pc2 = new Node(2);
	pRoot->_children.push_back( pc1 );
	pRoot->_children.push_back( pc2 );
	Node *pc11 = new Node( 11 );
	Node *pc12 = new Node( 12 );
	pc1->_children.push_back( pc11);
	pc1->_children.push_back( pc12);
	Node *pc22 = new Node( 22);
	pc2->_children.push_back( pc12);
	pc2->_children.push_back( pc22);
	


	
	Walk w;
	kg::walk_traits<const Node, Walk>::walk_result res = kg::dag_walk< Node, Walk >( pRoot, w);
	/**
		This whould print some output like
		 (level)  (node data)
		 0			0
		 1			1
		 2			11
		 2			12
		 1			2
		 2			22


	*/



	
	return 0;

}

int main() {

	fun();
	return 0;
}