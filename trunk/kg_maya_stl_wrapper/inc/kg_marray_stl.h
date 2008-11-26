#if !defined (KG_MARRAY_STL_H_)
#define KG_MARRAY_STL_H_
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <iterator>
#include <xmemory>
#include <functional>

#include "kg_mheaders.h"
#include "kg_kernel.h"


template < >
struct std::less< MFloatPoint >{
	bool operator ()( const MFloatPoint &left, const MFloatPoint &right )
	{
		return (left.x < right.x || left.x == right.x && left.y < right.y || left.x == right.x && left.y == right.y && left.z < right.z);
	}
};


template < >
struct std::less< MFloatVector >{
	bool operator ()( const MFloatVector &left, const MFloatVector &right )
	{
		return (left.x < right.x || left.x == right.x && left.y < right.y || left.x == right.x && left.y == right.y && left.z < right.z);
	}
};

template < >
struct std::less< MPoint >{
	bool operator ()( const MPoint &left, const MPoint &right )
	{
		return (left.x < right.x || left.x == right.x && left.y < right.y || left.x == right.x && left.y == right.y && left.z < right.z);
	}
};



template < >
struct std::less< MVector >{
	bool operator ()( const MVector &left, const MVector &right )
	{
		return (left.x < right.x || left.x == right.x && left.y < right.y || left.x == right.x && left.y == right.y && left.z < right.z);
	}
};




namespace kg{

	template<>
	struct BaseType<MPoint> {	
		static  MPoint zero(){ return MPoint(); }
		static MPoint one() { MPoint one(1.0, 1.0, 1.0); return one; }
		static std::pointer_to_unary_function< MPoint, MPoint > fabsfun;
	};


	template<  >
	struct BaseType< MFloatPoint > {
		static MFloatPoint zero(){ return  ( MFloatPoint::origin ); }
	};

	template<  >
	struct BaseType< MFloatVector > {
		static MFloatVector zero(){ return  ( MFloatVector::zero ); }
	};

	template<  >
	struct BaseType< MVector > {
		static MVector zero(){ return ( MVector::zero ); }
	};


	template< typename _Ty, class _Dift, class _cTptr, class _cReft, class _Tptr, class _Reft>
	class kg_const_iterator
		: public std::_Ranit<_Ty, _Dift, _cTptr, _cReft>
	{	// iterator for nonmutable vector
	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef _Ty value_type;
		typedef _Dift difference_type;
		typedef _cTptr pointer;
		typedef  _cReft reference;
		typedef _cReft const_reference;
		typedef _cTptr const_pointer;

		kg_const_iterator()
		{	// construct with null pointer
			_Myptr = 0;
		}

		kg_const_iterator(_Tptr _Ptr)
		{	// construct with pointer _Ptr
			_Myptr = _Ptr;
		}

		const_reference operator*() const
		{	// return designated object
			return (*_Myptr);
		}

		const_pointer operator->() const
		{	// return pointer to class object
			return (&**this);
		}

		kg_const_iterator& operator++()
		{	// preincrement
			++_Myptr;
			return (*this);
		}

		kg_const_iterator operator++(int)
		{	// postincrement
			kg_const_iterator _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		kg_const_iterator& operator--()
		{	// predecrement
			--_Myptr;
			return (*this);
		}

		kg_const_iterator operator--(int)
		{	// postdecrement
			kg_const_iterator _Tmp = *this;
			--*this;
			return (_Tmp);
		}

		kg_const_iterator& operator+=(difference_type _Off)
		{	// increment by integer
			_Myptr += _Off;
			return (*this);
		}

		kg_const_iterator operator+(difference_type _Off) const
		{	// return this + integer
			kg_const_iterator _Tmp = *this;
			return (_Tmp += _Off);
		}

		kg_const_iterator& operator-=(difference_type _Off)
		{	// decrement by integer
			return (*this += -_Off);
		}

		kg_const_iterator operator-(difference_type _Off) const
		{	// return this - integer
			kg_const_iterator _Tmp = *this;
			return (_Tmp -= _Off);
		}

		difference_type operator-(const kg_const_iterator& _Right) const
		{	// return difference of iterators
			return (_Myptr - _Right._Myptr);
		}

		const_reference operator[](difference_type _Off) const
		{	// subscript
			return (*(*this + _Off));
		}

		bool operator==(const kg_const_iterator& _Right) const
		{	// test for iterator equality
			return (_Myptr == _Right._Myptr);
		}

		bool operator!=(const kg_const_iterator& _Right) const
		{	// test for iterator inequality
			return (!(*this == _Right));
		}

		bool operator<(const kg_const_iterator& _Right) const
		{	// test if this < _Right
			return (_Myptr < _Right._Myptr);
		}

		bool operator>(const kg_const_iterator& _Right) const
		{	// test if this > _Right
			return (_Right < *this);
		}

		bool operator<=(const kg_const_iterator& _Right) const
		{	// test if this <= _Right
			return (!(_Right < *this));
		}

		bool operator>=(const kg_const_iterator& _Right) const
		{	// test if this >= _Right
			return (!(*this < _Right));
		}

		friend kg_const_iterator operator+(difference_type _Off,
			const kg_const_iterator& _Right)
		{	// return iterator + integer
			return (_Right + _Off);
		}
		_Tptr _Myptr;	// offset of element in vector
	};


	template< typename _Ty, class _Dift, class _cTptr, class _cReft, class _Tptr, class _Reft >             		class kg_iterator
		: public kg_const_iterator< _Ty, _Dift, _cTptr, _cReft, _Tptr, _Reft>
	{	// iterator for mutable vector
	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef _Ty value_type;
		typedef _Dift difference_type;
		typedef _Tptr pointer;
		typedef _Reft reference;
		typedef kg_const_iterator< _Ty, _Dift, _cTptr, _cReft, _Tptr, _Reft> kg_const_iterator;
		kg_iterator()
		{	// construct with null vector pointer
		}

		kg_iterator(pointer _Ptr)
			: kg_const_iterator(_Ptr)
		{	// construct with pointer _Ptr
		}

		reference operator*() const
		{	// return designated object
			return ((reference)**(kg_const_iterator *)this);
		}

		_Tptr operator->() const
		{	// return pointer to class object
			return (&**this);
		}

		kg_iterator& operator++()
		{	// preincrement
			++this->_Myptr;
			return (*this);
		}

		kg_iterator operator++(int)
		{	// postincrement
			kg_iterator _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		kg_iterator& operator--()
		{	// predecrement
			--this->_Myptr;
			return (*this);
		}

		kg_iterator operator--(int)
		{	// postdecrement
			kg_iterator _Tmp = *this;
			--*this;
			return (_Tmp);
		}

		kg_iterator& operator+=(difference_type _Off)
		{	// increment by integer
			this->_Myptr += _Off;
			return (*this);
		}

		kg_iterator operator+(difference_type _Off) const
		{	// return this + integer
			kg_iterator _Tmp = *this;
			return (_Tmp += _Off);
		}

		kg_iterator& operator-=(difference_type _Off)
		{	// decrement by integer
			return (*this += -_Off);
		}

		kg_iterator operator-(difference_type _Off) const
		{	// return this - integer
			kg_iterator _Tmp = *this;
			return (_Tmp -= _Off);
		}

		difference_type operator-(const kg_const_iterator& _Right) const
		{	// return difference of iterators
			return ((kg_const_iterator)*this - _Right);
		}

		reference operator[](difference_type _Off) const
		{	// subscript
			return (*(*this + _Off));
		}

		friend kg_iterator operator+(difference_type _Off,
			const kg_iterator& _Right)
		{	// return iterator + integer
			return (_Right + _Off);
		}
	};



	template<typename B,  class T, class TArray>
	class MArray_stl

	{	
	public:
		//dummy alloc structure 
		typedef TArray _TArray;
		typedef MArray_stl<B, T, TArray > _Myt;
		typedef typename std::allocator< T > allocator_type;
		typedef typename allocator_type::size_type size_type;
		typedef std::ptrdiff_t _Dift;
		typedef _Dift difference_type;
		typedef T* _Tptr;
		typedef const T* _Ctptr;
		typedef _Tptr pointer;
		typedef _Ctptr const_pointer;
		typedef T & _Reft;
		typedef _Reft reference;
		typedef const T& const_reference;
		typedef T value_type;
		typedef T _Ty;
		typedef kg_const_iterator<T, _Dift, _Ctptr, const_reference, _Tptr, _Reft> const_iterator;
		typedef kg_iterator<T, _Dift, _Ctptr, const_reference, _Tptr, _Reft> iterator;
		friend class const_iterator;
		friend class iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;



		MArray_stl( TArray *base)
			: _MyBase(base)
		{	
			assert( _MyBase != NULL );
		}

		MArray_stl( const MArray_stl &rhs)
		{	
			_MyBase = rhs._MyBase;
			assert( _MyBase != NULL );
		}


		~MArray_stl()
		{	
			assert(_MyBase != NULL);
		}

		_Myt& operator=(const _Myt& _Right)
		{
			_MyBase = _Right._MyBase;
			assert(_MyBase != NULL);
			return *this;
		}


		void reserve(size_type _Count)
		{
			//disreagrd the reserve call
		}

		size_type capacity() const
		{
			return _MyBase->length();
		}

		iterator begin()
		{	// return iterator for beginning of mutable sequence
			assert( _MyBase != NULL );
			return (iterator(&(*_MyBase)[0]));
		}

		const_iterator begin() const
		{	// return iterator for beginning of nonmutable sequence
			assert( _MyBase != NULL );
			return (const_iterator(&(*_MyBase)[0]));
		}

		iterator end()
		{	// return iterator for end of mutable sequence
			assert( _MyBase != NULL );
			iterator _Tmp = begin();
			size_type len = static_cast<size_type>( _MyBase->length() );
			if (0 < len)
				_Tmp += len;
			return (_Tmp);
		}

		const_iterator end() const
		{	
			assert( _MyBase != NULL );
			const_iterator _Tmp = begin();
			size_type len = static_cast<size_type>( _MyBase->length() );
			if (0 < len )
				_Tmp += len ;
			return (_Tmp);
		}

		reverse_iterator rbegin()
		{	
			assert( _MyBase != NULL );
			// return iterator for beginning of reversed mutable sequence
			return (reverse_iterator(end()));
		}

		const_reverse_iterator rbegin() const
		{	
			assert( _MyBase != NULL );
			// return iterator for beginning of reversed nonmutable sequence
			return (const_reverse_iterator(end()));
		}

		reverse_iterator rend()
		{	
			assert( _MyBase != NULL );
			// return iterator for end of reversed mutable sequence
			return (reverse_iterator(begin()));
		}

		const_reverse_iterator rend() const
		{	
			assert( _MyBase != NULL );
			// return iterator for end of reversed nonmutable sequence
			return (const_reverse_iterator(begin()));
		}

		void resize(size_type _Newsize)
		{	
			assert( _MyBase != NULL );
			unsigned int oldSize = _MyBase->length();
			_MyBase->setLength( static_cast<unsigned int> (_Newsize) );
			for( unsigned int u=oldSize; u < _Newsize ; ++u )
				_MyBase->set( BaseType< T >::zero(), u );
		}

		void resize(size_type _Newsize, _Ty _Val)
		{
			assert( _MyBase != NULL );
			unsigned int origLen = _MyBase->length();
			_MyBase->setLength( static_cast<unsigned int> (_Newsize) );
			for( size_type _idx= origLen; _idx < _Newsize ; ++_idx)
			{
				_MyBase->set( _Val, static_cast<unsigned int>(_idx) );
			}
		}

		size_type size() const
		{	
			assert( _MyBase != NULL );
			return _MyBase->length();
		}

		size_type max_size() const
		{	
			assert( _MyBase != NULL );
			return static_cast< size_type> (_Alval.max_size());
		}

		bool empty() const
		{
			assert( _MyBase != NULL );
			return (size() == 0);
		}

		allocator_type get_allocator() const
		{	
			assert( _MyBase != NULL );
			return _Alval;
		}

		const_reference at(size_type _Pos) const
		{	
			assert( _MyBase != NULL );
			// subscript nonmutable sequence with checking
			if (size() <= _Pos)
				_Xran();
			return (*(begin() + _Pos));
		}

		reference at(size_type _Pos)
		{	
			assert( _MyBase != NULL );
			// subscript mutable sequence with checking
			if (size() <= _Pos)
				_Xran();
			return (*(begin() + _Pos));
		}

		const_reference operator[](size_type _Pos) const
		{	
			assert( _MyBase != NULL );
			// subscript nonmutable sequence
			return (*(begin() + _Pos));
		}

		reference operator[](size_type _Pos)
		{	
			assert( _MyBase != NULL );
			// subscript mutable sequence
			return (*(begin() + _Pos));
		}

		reference front()
		{	
			assert( _MyBase != NULL );
			// return first element of mutable sequence
			return (*begin());
		}

		const_reference front() const
		{	
			assert( _MyBase != NULL );
			// return first element of nonmutable sequence
			return (*begin());
		}

		reference back()
		{	
			assert( _MyBase != NULL );
			// return last element of mutable sequence
			return (*(end() - 1));
		}

		const_reference back() const
		{	
			assert( _MyBase != NULL );
			// return last element of nonmutable sequence
			return (*(end() - 1));
		}

		void push_back(const _Ty& _Val)
		{	
			assert( _MyBase != NULL );
			_MyBase->append( _Val );
		}

		void pop_back()
		{	
			assert( _MyBase != NULL );
			// erase element at end
			if (!empty())
			{
				_MyBase->setLength( static_cast<unsigned int>( size()-1) );
			}
		}

		template<class _Iter>
			void assign(_Iter _First, _Iter _Last)
		{	
			assert( _MyBase != NULL );
			// assign [_First, _Last)
			_Assign(_First, _Last, _Iter_cat(_First));
		}


		void assign(size_type _Count, const _Ty& _Val)
		{	
			assert( _MyBase != NULL );
			// assign _Count * _Val
			_Assign_n(_Count, _Val);
		}

		iterator insert(iterator _Where, const _Ty& _Val)
		{	
			assert( _MyBase != NULL );
			// insert _Val at _Where
			size_type _Off = size() == 0 ? 0 : _Where - begin();
			_MyBase->insert( _Val, static_cast<unsigned int> (_Off) );
			return (begin() + _Off);
		}

		void insert(iterator _Where, size_type _Count, const _Ty& _Val)
		{
			assert( _MyBase != NULL );
			size_type _Off = size() == 0 ? 0 : _Where - begin();
			for( size_type _idx=0; _idx < _Count; ++_idx) {
				_MyBase->insert( _Val, static_cast<unsigned int> ( _Off + _idx) );
			}
		}		

		template<class _Iter>
			void insert(iterator _Where, _Iter _First, _Iter _Last)
		{	
			assert( _MyBase != NULL );
			// insert [_First, _Last) at _Where
			_Insert(_Where, _First, _Last, _Iter_cat(_First));
		}

		iterator erase(iterator _Where)
		{	
			assert( _MyBase != NULL );
			_Dift _idx=0;
			if( _Where != end() )
			{
				_Distance(begin(), _Where, _idx);
				_MyBase->remove( static_cast<unsigned int>(_idx) );
				_Where = begin() + _idx;
			} else 
			{
				_Where = end();
			}
			return _Where;
		}

		iterator erase(iterator _First, iterator _Last)
		{	
			assert( _MyBase != NULL );
			// erase [_First, _Last)
			if (_First != _Last)
			{	
				_Dift _idx=0;
				_Distance( begin(), _First, _idx);
				for( iterator it = _First; it != _Last; ++it )
				{
					_MyBase->remove( static_cast<unsigned int>( _idx ) );
				}
				_First = begin() + _idx;
			}
			return (_First);
		}

		void clear()
		{	
			assert( _MyBase != NULL );
			// erase all
			_MyBase->clear();
		}

	protected:		


		//disallow the assignment operator
		

		template<class _Iter>
			void _Assign(_Iter _First, _Iter _Last, std::input_iterator_tag)
		{	
			assert( _MyBase != NULL );
			// assign [_First, _Last), input iterators
			erase(begin(), end());
			insert(begin(), _First, _Last);
		}

		template<class _Iter>
			void _Insert(iterator _Where, _Iter _First, _Iter _Last,
			std::_Int_iterator_tag)
		{	
			assert( _MyBase != NULL );
			// insert _Count * _Val at _Where
			_Insert_n(_Where, (size_type)_First, (_Ty)_Last);
		}

		template<class _Iter>
			void _Insert(iterator _Where, _Iter _First, _Iter _Last,
			std::input_iterator_tag)
		{	
			assert( _MyBase != NULL );
			// insert [_First, _Last) at _Where, input iterators
			for (; _First != _Last; ++_First, ++_Where)
				_Where = insert(_Where, *_First);
		}

		template<class _Iter>
			void _Insert(iterator _Where,
			_Iter _First, _Iter _Last, std::forward_iterator_tag)
		{
			assert( _MyBase != NULL );
			_Dift _Count = 0;		
			_Distance(begin(), _Where, _Count);
			size_type t= static_cast<size_type>(_Count);
			for( _Iter it = _First; it != _Last; ++it, ++t )
			{
				_MyBase->insert( *it, static_cast<unsigned int>(t) );			
			}	
		}

		void _Assign_n(size_type _Count, const _Ty& _Val)
		{	
			assert( _MyBase != NULL );
			// assign _Count * _Val
			_Ty _Tmp = _Val;	// in case _Val is in sequence
			erase(begin(), end());
			insert(begin(), _Count, _Tmp);
		}


		void _Insert_n(iterator _Where, size_type _Count, const _Ty& _Val)
		{	
			assert( _MyBase != NULL );
			size_type _idx;				
			_Distance( begin(), _Where, _idx);
			for( size_type t=0; t < _idx; ++t )
			{
				_MyBase->insert( _Val, static_cast<unsigned int>(t));
			}
		}

		void _Xnoa() const
		{
			_THROW( std::runtime_error, "not allowed operation" ); 
		}

		void _Xnoi() const
		{
			_THROW( std::runtime_error, "not implemeted");
		}

		void _Xlen() const
		{	// report a length_error
			_THROW(length_error, "vector<T> too long");
		}

		void _Xran() const
		{	// report an out_of_range error
			_THROW(out_of_range, "invalid vector<T> subscript");
		}

	public:
		TArray *_MyBase;
		allocator_type _Alval;
	};





	// MFloat_POrV_Array template functions
	template <typename B, class T , class TArray>
		inline bool operator==(const MArray_stl< B, T, TArray >& _Left,
		const MArray_stl< B, T, TArray > & _Right)
	{	 // test for equality
		return (  _Left.size() == _Right.size()
			&& equal(_Left.begin(), _Left.end(), _Right.begin()));
	}


	template <typename B, class T , class TArray>
		inline bool operator!=(const MArray_stl< B, T, TArray > & _Left,
		const MArray_stl< B, T, TArray > & _Right)
	{	// test for vector inequality
		return (!(_Left == _Right));
	}

	template < typename B, class T , class TArray>
		inline bool operator<(const MArray_stl< B, T, TArray > &_Left, const MArray_stl< B, T, TArray > &_Right)
	{	// test if _Left < _Right for vectors
		typedef MArray_stl< B, T, TArray > MArray_stl;
		typedef less< T> less;
		less pred;		
		return (lexicographical_compare<MArray_stl::const_iterator,  MArray_stl::const_iterator, less >(_Left.begin(), _Left.end(),
			_Right.begin(), _Right.end(), pred));
	}

	template < typename B,  class T , class TArray>
		inline bool operator>(const MArray_stl< B, T, TArray >& _Left,
		const MArray_stl< B, T, TArray >& _Right)
	{	// test if _Left > _Right for vectors
		return (_Right < _Left);
	}

	template < typename B, class T , class TArray>
		inline bool operator<=(const MArray_stl< B, T, TArray >& _Left, const MArray_stl< B, T, TArray >& _Right)
	{	// test if _Left <= _Right for vectors
		return (!(_Right < _Left));
	}

	template < typename B, class T , class TArray>
		inline bool operator>=(const MArray_stl< B, T, TArray > & _Left,
		const MArray_stl< B,  T, TArray >& _Right)
	{	// test if _Left >= _Right for vectors
		return (!(_Left < _Right));
	}


	typedef MArray_stl< float , MFloatPoint, MFloatPointArray > MFloatPointArray_stl;
	typedef MArray_stl< float, MFloatVector, MFloatVectorArray > MFloatVectorArray_stl;
	typedef MArray_stl< double , MPoint, MPointArray > MPointArray_stl;
	typedef MArray_stl< double, MVector, MVectorArray > MVectorArray_stl;
	typedef MArray_stl< int, int, MIntArray > MIntArray_stl;
	typedef MArray_stl< float, float, MFloatArray > MFloatArray_stl;
	typedef MArray_stl< double, double, MDoubleArray > MDoubleArray_stl;


} //namespace kg
#endif // KG_MARRAY_STL_H_