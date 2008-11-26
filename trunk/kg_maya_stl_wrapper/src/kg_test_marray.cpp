#include <windows.h>
#include <iostream>
#include <vector>
#include <deque>

#include "kg_mheaders.h"
#include "kg_marray_stl.h"



using namespace std;





template < typename B, typename T , typename TArray>
struct test_M_Array_traits {
	static T two() { 
		B t = static_cast< B > ( 2 );
		return T( t, t, t );
	}

	static T three() {
		B t = static_cast< B > ( 3 );
		return T ( t, t, t );
	}

	static T six() {
		B t = static_cast< B > ( 6 );
		return T ( t, t, t);
	}

	static void init_M_Array(unsigned int len,  TArray &fpa)
	{
		fpa.setLength( len );
		for( unsigned int i=0; i < len; ++ i)
		{
			B f( static_cast<B>(i));
			T p( f, f, f);
			fpa.set( p, i);
		}
	}

};

template < typename B, typename TArray  >
struct test_M_Array_traits<B,B, TArray> {
	static B two() { 
		B t = static_cast< B > ( 2 );
		return t;
	}

	static B three() {
		B t = static_cast< B > ( 3 );
		return t;
	}

	static B six() {
		B t = static_cast< B > ( 6 );
		return t;
	}

	static void init_M_Array(unsigned int len,  TArray &fpa)
	{
		fpa.setLength( len );
		for( unsigned int i=0; i < len; ++ i)
		{
			B f( static_cast<B>(i));
			fpa.set( f, i);
		}
	}
};




template<typename B, typename T,  typename TArray >
bool test_M_Array ()
{

	typedef typename kg::MArray_stl<B, T, TArray > kgWrapper;
	typedef typename kgWrapper::size_type size_t;
	typedef test_M_Array_traits< B, T, TArray > test_traits;
	size_t i;		

	const T two( test_traits::two() );
	const T three (test_traits::three() );
	const T six ( test_traits::six() );

	bool bVal = true;
	size_t l1, l2;

	TArray pa_1;
	test_traits:: init_M_Array (10,  pa_1);
	TArray pa_2( 10 ); 
	TArray pa_3 = pa_1; //get a copy of pa_1

	{

		kgWrapper mfpa_1( &pa_1 );

		//check reserve
		mfpa_1.reserve( 20);

		//check capacity
		bVal = pa_1.length() == mfpa_1.capacity();

		//check begin
		kgWrapper::iterator mit , mbegin, mend;
		mbegin = mfpa_1.begin();
		T &p_10 = pa_1[0];
		bVal = bVal &&  ( p_10 == *mbegin );

		//check const begin
		kgWrapper::const_iterator cmit, cmbegin, cmend;
		cmbegin = mfpa_1.begin();
		const T &cp_10 = pa_1[0];
		bVal = bVal && ( cp_10 == *cmbegin);

		//check size
		kgWrapper::size_type sz = mfpa_1.size();
		bVal = bVal &&  sz == pa_1.length();

		//check end != begin
		//check const end != begin
		mend = mfpa_1.end();
		bVal = bVal && ( mend != mbegin);
		cmend = mfpa_1.end();
		bVal = bVal && ( cmend != cmbegin );

		mit = mend - 1;

		cmit = cmend - 1;

		T &p_1last = *mit;
		const T &cp_1last = *cmit;
		//check decrement
		//check end == begin + sz
		//check (end-1) values
		bVal = bVal && ( &p_10 + (sz  - 1) == &p_1last );
		bVal = bVal && ( &cp_10 + ( sz - 1) == &cp_1last );
		bVal = bVal && ( p_1last ==  pa_1[ pa_1.length() -1 ] ) ;
		bVal = bVal && ( cp_1last == ( pa_1[ pa_1.length() -1 ] ) );	


		kgWrapper::reverse_iterator rit, rbegin, rend;
		kgWrapper::const_reverse_iterator crit, crbegin, crend;

		//check iterator ==
		//check const_iterator ==
		//check reverse_iterator
		//check const_rverse_iterator 

		rbegin = mfpa_1.rbegin();
		crbegin = mfpa_1.rbegin();
		//bVal = bVal && ( rbegin == mit );
		//bVal = bVal && ( crbegin == cmit );
		bVal = bVal && ( *rbegin == pa_1[ pa_1.length() -1 ] );
		bVal = bVal && ( *crbegin == pa_1[ pa_1.length() -1 ] );

		mit = --mbegin;
		cmit = --cmbegin;

		//check rend
		rend = mfpa_1.rend();
		crend = mfpa_1.rend();
		//bVal = bVal && ( mit == rend );
		//bVal = bVal && ( cmit == crend );
	}

	//check copy constructor
	//check const_iterator

	{

		kgWrapper mfpa_1( &pa_1 );
		kgWrapper mfpa_2( mfpa_1 );
		kgWrapper::const_iterator cmit_1, cmit_2;

		bVal = bVal && ( mfpa_1.size() == mfpa_2.size() );

		for( cmit_1 = mfpa_1.begin(), cmit_2 = mfpa_2.begin();
			cmit_1 != mfpa_1.end(); ++ cmit_1, ++ cmit_2 )
		{
			const T & cp1( *cmit_1);
			const T & cp2( *cmit_2);
			bVal = bVal && ( cp1 == cp2 );
		}
	}

	{
		//check resize
		//check opeator =
		pa_2 = TArray ( 10, two);
		kgWrapper mfpa_2 ( &pa_2 );	
		mfpa_2.resize( 12 );
		bVal = bVal && mfpa_2.size() == 12 ;

		l2 = mfpa_2.size();
		//check resize 
		//check operator []
		for ( i =0; i != l2; ++i )
		{
			T & p ( mfpa_2[i]);
			const T & cp ( mfpa_2[i] );
			if ( i < 10 )
			{
				bVal = bVal && (p == two );
				bVal = bVal && ( cp == two );
			} 
		}
	}

	//check resize with initial value
	//check operator =		
	{
		pa_2 = TArray( 10, two );
		kgWrapper mfpa_2( &pa_2 );
		mfpa_2.resize( 12, two );
		l2 = mfpa_2.size();
		//check resize  with initial value
		//check operator []
		for ( i =0; i != l2; ++i )
		{
			T & p ( mfpa_2[i]);
			const T & cp ( mfpa_2[i]);
			bVal = bVal && (p == two );
			bVal = bVal && ( cp == two );
		}
		l2 = pa_2.length();
	}

	{
		kgWrapper mfpa_1( &pa_1 );
		//check max size()
		l1 = mfpa_1.size();
		size_t m_sz = mfpa_1.max_size();
		bVal= bVal && l1 <=  m_sz;
	}

	//check empty
	{
		TArray pa_3;
		kgWrapper mfpa_2 (&pa_3);
		bVal = bVal && mfpa_2.empty();
	}
	//check at
	//pa_2 now contains 12 elements all of which are <2,2,2> or 2
	{
		kgWrapper mfpa_1( &pa_1);
		l1 = pa_1.length();
		T & p = mfpa_1.at( 6);
		bVal= bVal && p == T(six);

		try {
			p = mfpa_1.at( 10 );
			bVal = bVal && false;
		}catch ( std::out_of_range &xran )
		{
			xran = xran;
			bVal = bVal && true;
		}
		const T & cp = mfpa_1.at( 6 );
		bVal = bVal && cp == T(six);

		try {
			const T cp_2 = mfpa_1.at( 10 );
			bVal = bVal && false;
		}catch ( std::out_of_range &xran )
		{
			xran = xran;
			bVal = bVal && true;
		}
	}
	//front
	{
		const T &p_10 = pa_1[ 0 ];
		kgWrapper mfpa_1( &pa_1);
		T p = mfpa_1.front();
		const T &cp_3 = mfpa_1.front();

		bVal = bVal && p == p_10;
		bVal = bVal && cp_3 == p_10;

		//back
		const T &p_1last = pa_1[ pa_1.length() -1 ];
		p = mfpa_1.back();
		const T &cp_4 = mfpa_1.back();
		const T &cp_1last_2 = pa_1[ pa_1.length() -1 ];
		bVal = bVal && p_1last == p;
		bVal = bVal && cp_1last_2 == cp_4;
	}

	//check push_back
	{
		pa_2 = TArray( 10, two );
		kgWrapper mfpa_2( &pa_2);
		l2 = mfpa_2.size();
		mfpa_2.push_back( three);
		//pa_2 now contains 11 elements  all but last are <2,2,2> or 2
		//and the last one is <3,3,3 > or 3
		bVal = bVal && ( mfpa_2[ mfpa_2.size() -1 ] == three );
		bVal = bVal && (mfpa_2.size() == (l2 +1));

		//pop_back
		mfpa_2.pop_back();
		bVal = bVal && ( mfpa_2[ mfpa_2.size() -1 ] == two );
		bVal = bVal && ( mfpa_2.size() == l2 );
	}

	//check assign;
	//check opeator []
	pa_1 = TArray ( 10, two);
	pa_2 = TArray ( 12, three);
	{ 

		kgWrapper mfpa_2( &pa_2);
		kgWrapper mfpa_1( &pa_1);
		mfpa_2.assign( mfpa_1.begin(), mfpa_1.end() );
		l1 = mfpa_1.size();
		bVal = bVal && mfpa_2.size() == l1;
		for( i =0; i < l1; ++i )
		{
			const T &p1 = mfpa_1[i ];
			const T &p2 = mfpa_2[i];
			bVal = bVal && ( p1 == p2 );
			bVal = bVal && ( p2 == two );
		}

		std::vector< T > fpvec( 10, three);
		mfpa_2.assign( fpvec.begin(), fpvec.end() );
		bVal = bVal && mfpa_2.size() == fpvec.size();
		for( i =0; i < fpvec.size(); ++i )
		{	
			const T &p1 = fpvec[i ];
			const T &p2 = mfpa_2[i];
			bVal = bVal && ( p2 == three );

		}
	}

	//assign with init value
	//check operator []
	{
		kgWrapper  mfpa_2( &pa_2);		
		kgWrapper::iterator mit , mbegin, mend;		
		kgWrapper::const_iterator cmit , cmbegin, cmend;
		size_t sz(12);
		mfpa_2.assign( sz, two );
		bVal = bVal && mfpa_2.size() == 12;
		for( i =0; i < mfpa_2.size(); ++i )
		{
			const T &p2 = mfpa_2[i];
			bVal = bVal && ( p2 == two );		
		}


		//mfpa_2 now contains 12 elements all of which are two-s
		for( mit = mfpa_2.begin(), i=0;  i < 6; ++i, ++mit );
		//mit now points to the sixth element
		mit = mfpa_2.insert( mit, three);		
		bVal = bVal && ( *mit == three);
		for( mit = mfpa_2.begin(), i=0; mit != mfpa_2.end(); ++mit, ++i )
		{
			const T &p = *mit;
			if ( i != 6 )
			{
				bVal == bVal && ( p	== two );	
			} else {
				bVal == bVal && ( p == three);
			}
		}

	}

	//reset mfpa_2 to 10 elemen aray of  two-s
	pa_2 = TArray( 10, two );
	{
		kgWrapper mfpa_2( &pa_2);		
		kgWrapper::iterator mit , mbegin, mend;		
		kgWrapper::const_iterator cmit , cmbegin, cmend;
		//l2 == original length of mfpa_2
		l2 = mfpa_2.size();
		for( mit = mfpa_2.begin(), i=0;  i < 6; ++i, ++mit );
		//mit now points to the sixth element

		//check insert (iterator, count, val)
		size_t sz(4);
		mfpa_2.insert( mit, sz, three );
		bVal = bVal && ( (l2 + 4 ) == mfpa_2.size());
		for( mit = mfpa_2.begin(), i=0; mit != mfpa_2.end(); ++mit, ++i )
		{
			const T &p = *mit;
			if ( i < 6 && i > 9 )
			{
				bVal == bVal && ( p	== two );	
			} else {
				bVal == bVal && ( p == three);
			}
		}

	}

	//reset mfpa_2 to 10 elemen aray of  two-s
	pa_2 = TArray( 10, two );
	{

		kgWrapper mfpa_2( &pa_2 );		
		kgWrapper::iterator mit , mbegin, mend;		
		kgWrapper::const_iterator cmit , cmbegin, cmend;
		//l2 now has he origianl size of mfpa_2
		l2 = mfpa_2.size();

		std::deque< T > dq( 10, three);

		for( mit = mfpa_2.begin(), i=0;  i < 6; ++i, ++mit );
		//mit now points to the sixth element

		//check insert ( iterator, first , last)
		mfpa_2.insert( mit, dq.begin(), dq.begin() + 4);				
		bVal = bVal && ( (l2 + 4 ) == mfpa_2.size());
		for( mit = mfpa_2.begin(), i=0; mit != mfpa_2.end(); ++mit, ++i )
		{
			const T &p = *mit;
			if ( i < 6 && i > 9 )
			{
				bVal == bVal && ( p	== two );	
			} else {
				bVal == bVal && ( p == three);
			}
		}
	}
	//check erase

	//reset mfpa_2 to 10 elemen aray of  two-s
	pa_2 = TArray( 10, two );
	{ 
		kgWrapper mfpa_2( &pa_2);		
		kgWrapper::iterator mit , mbegin, mend;		
		kgWrapper::const_iterator cmit , cmbegin, cmend;
		//l2 now has he origianl size of mfpa_2
		l2 = mfpa_2.size();
		for( mit = mfpa_2.begin(), i=0;  i < 6; ++i, ++mit );
		//mit now points to the sixth element
		//inset a three at the sizeth element 
		mfpa_2.insert( mit, three);
		for( mit = mfpa_2.begin(), i=0;  i < 6; ++i, ++mit );
		//mit now points to the sixth element
		//now ease the sixth element
		bVal = bVal  && ( *mit == three );
		mit = mfpa_2.erase( mit );
		bVal = bVal && ( *mit == two );
		for( mit = mfpa_2.begin(), i=0;  i < 6; ++i, ++mit );
		//mit now points to the sixth element
		bVal = bVal && ( *mit == two);
	}

	//check erase ( first, last )

	//reset mfpa_2 to 10 elemen aray of  two-s
	pa_2 = TArray( 10, two );
	{

		kgWrapper mfpa_2( &pa_2 );		
		kgWrapper::iterator mit , mbegin, mend;		
		kgWrapper::const_iterator cmit , cmbegin, cmend;
		l2 = mfpa_2.size();
		for( mit = mfpa_2.begin(), i=0;  i < 6; ++i, ++mit );
		//mit now points to the sixth element
		mfpa_2.erase( mit, mit + 3 );
		bVal = bVal && ( l2 == mfpa_2.size() + 3 );

		//check clear
		mfpa_2.clear();
		bVal = bVal && ( mfpa_2.size() == 0);
		bVal = bVal && ( mfpa_2.begin() == mfpa_2.end() );
	}


	//check operator ==. !=

	pa_1 = TArray( 10, three);
	pa_2 = TArray( 10, two );
	pa_3 = TArray(10, two);
	{
		kgWrapper  mfpa_1( &pa_1 );
		kgWrapper mfpa_2( &pa_2 );
		kgWrapper mfpa_11( &pa_1);
		kgWrapper mfpa_3( &pa_3); 
		bVal = bVal && ( mfpa_1 != mfpa_2 );
		bVal = bVal && ( mfpa_1 == mfpa_1 );
		bVal = bVal && ( mfpa_11 == mfpa_1 );

		bVal = bVal && (mfpa_3 != mfpa_1);
		bVal = bVal && (mfpa_3 == mfpa_2);


		//check operator <, > <=, >=

		//mfpa_2 is 13 twos
		size_t t3(3);
		mfpa_2.insert( mfpa_2.end(), t3, two);
		//mfpa_3 is 10 twos folowed by 3 threes
		mfpa_3.insert( mfpa_3.end(), t3, three);

		bVal = bVal && ( mfpa_2 < mfpa_3 );
		bVal = bVal && ( mfpa_3 > mfpa_2);
		bVal = bVal && ( mfpa_2 <= mfpa_3);
		bVal = bVal && (mfpa_3 >= mfpa_2);
	}

	{
		//mfpa_3 is now numerically equal to mfpa_2
		pa_3 = TArray( 13, two);

		kgWrapper  mfpa_1( &pa_1 );
		kgWrapper  mfpa_2( &pa_2 );
		kgWrapper  mfpa_3( &pa_3);

		bVal = bVal && (mfpa_2 <= mfpa_3 );
		bVal = bVal && (mfpa_2 >= mfpa_3);

	}

	return bVal;


}


#define KG_REPORT_TEST_MARRAY(b, s ) \
	if ( b ) \
{ \
	cout << s##" succeeded" << endl; \
} else \
{ \
	cout << s##" failed" << endl; \
}

int main(int argc, char **argv)
{
	MStatus	stat;

	stat = MLibrary::initialize (argv[0]);
	if (!stat) {
		stat.perror("MLibrary::initialize");
		return 1;
	}


	bool bVal = test_M_Array<float, MFloatPoint, MFloatPointArray> ();
	KG_REPORT_TEST_MARRAY( bVal , "test_M_Array<float, MFloatPoint, MFloatPointArray> ()" );	
	bVal = bVal &&  test_M_Array<float,  MFloatVector, MFloatVectorArray> ();
	KG_REPORT_TEST_MARRAY( bVal , "test_M_Array<float,  MFloatVector, MFloatVectorArray> ()" );		
	bVal = bVal && test_M_Array<double, MPoint, MPointArray > ();
	KG_REPORT_TEST_MARRAY( bVal , "test_M_Array<double, MPoint, MPointArray > ()" );		
	bVal = bVal && test_M_Array< double, MVector, MVectorArray > ();
	KG_REPORT_TEST_MARRAY( bVal , "test_M_Array< double, MVector, MVectorArray > ()" );
	bVal = bVal && test_M_Array<float, float, MFloatArray > ();
	KG_REPORT_TEST_MARRAY( bVal , "test_M_Array<float, float, MFloatArray > ()" );
	bVal = bVal && test_M_Array< double, double, MDoubleArray > ();
	KG_REPORT_TEST_MARRAY( bVal , "test_M_Array< double, double, MDoubleArray > ()"  );
	bVal = bVal && test_M_Array< int, int, MIntArray > ();
	KG_REPORT_TEST_MARRAY( bVal , "test_M_Array< int, int, MIntArray > ()"  );
	MLibrary::cleanup();
	return (bVal) ? 0 : 1;
}

