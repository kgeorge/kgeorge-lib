//============================================================
//	This code illustrates a generic framework 
//  for  the obesrver pattern
//  This is covered by the GPL license.
//  http://www.gnu.org/copyleft/gpl.html
//============================================================
#if !defined(KG_OBSERVER_H_)
#define KG_OBSERVER_H_

#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>
#include <cassert>

namespace kgObserver
{

//============================================================
// template structure to detect a void type at compile time
//============================================================
	template< typename T >
	struct TIsVoid
	{
		enum { m_value=0};
	};

	template<>
	struct TIsVoid< void >
	{
		enum { m_value=1};
	};



//============================================================
// compile time switch which returns one of the types as its 
// TValue
//============================================================


	//If the value of the template parameter i passed in
	//is non-zero TValue is A, else TValue is B
	template< int i, typename A, typename B >
	struct TSwitch
	{
		typedef typename  A TValue;

	};
	template< typename A, typename B>
	struct TSwitch<0, A, B>
	{
		typedef typename B TValue;
	};




//============================================================
// A four way switch that takes two int parameters 1, and j
// If  i == 0, this reduces to TSwitch<j, D, B>
// If  i != 0, this reduces to TSwitch<j, C, A>
//============================================================

	
	template< int i, int j, typename A, typename B , typename C, typename D>
	struct TDoubleSwitch
	{
		typedef typename TSwitch<j, C, A>::TValue TValue;
	};

	template<int j,  typename A, typename B, typename C, typename D>
	struct TDoubleSwitch<0,j, A, B, C, D>
	{
		typedef typename TSwitch<j, D, B>::TValue TValue;
	};


//============================================================
// DummyType
//============================================================


	//Since we support call backs with up to 5 arguments,
	//DummyType will serve as the template parameter for call backs
	//with less than 5 arguments
	struct DummyType{};

	
	//template structure to detect DummyType
	template< typename T >
	struct TIsDummy
	{
		enum { m_value=0};
	};

	template< >
	struct TIsDummy< DummyType >
	{
		enum { m_value=1};
	};

	


//============================================================
// Abstract Base classes for Subscription-implementation
//============================================================

	//abstract base class for a Subscription 
	//Observed-s need to know only this interface
    
	class ISubscription
	{
	public:
		ISubscription(){}
		virtual ~ISubscription(){};
		virtual void DeleteMe ()
		{
			delete this;
		}
	};

	template< typename R  >
	class ISubscription0 : public ISubscription
	{
	public:
		typedef ISubscription0< R > TThisType;
		ISubscription0(){}
		virtual ~ISubscription0(){};
		virtual R operator()()=0;
		virtual bool Equals( const TThisType *prhs )=0;
	};
	template< typename R , typename A1 >
	class ISubscription1: public ISubscription
	{
	public:
		typedef ISubscription1< R, A1 > TThisType;
		ISubscription1(){}
		virtual ~ISubscription1(){};
		virtual R operator()(A1 arg1 )=0;
		virtual bool Equals( const TThisType *prhs )=0;
	};
	template< typename R , typename A1, typename A2 >
	class ISubscription2: public ISubscription
	{
	public:
		typedef ISubscription2< R, A1, A2 > TThisType;
		ISubscription2(){}
		virtual ~ISubscription2(){};
		virtual R operator()(A1 arg1, A2 arg2)=0;
		virtual bool Equals( const TThisType *prhs )=0;
	};

	template< typename R , typename A1, typename A2, typename A3 >
	class ISubscription3: public ISubscription
	{
	public:
		typedef ISubscription3< R, A1, A2, A3 > TThisType;
		ISubscription3(){}
		virtual ~ISubscription3(){};
		virtual R operator()(A1 arg1, A2 arg2, A3  arg3)=0;
		virtual bool Equals( const TThisType *prhs )=0;
	};


	//============================================================
	// Observed class
	//============================================================

	//Observed class
	//knows only about ISubscription classes
	//It doesnt know wehther the Subscription is 
	//a member function, a const member function or a global static function
	//Observed class is templatized after the parameters of the
	//signature of the subscription function, this will give type safety
	//ie: only subscription-s of appropriate template - parameters can be
	//hitched to the Observed.

	template< 
		typename R,  
		typename A1=DummyType, 
		typename A2=DummyType,		
		typename A3=DummyType
	>
	class Observed
	{
	public:
		//get all possible function signatures here
		typedef ISubscription0<R>  ISubscription_0;
		typedef ISubscription1<R, A1> ISubscription_1;		
		typedef ISubscription2<R, A1, A2> ISubscription_2;			
		typedef ISubscription3<R, A1, A2, A3> ISubscription_3;	
		//Now based on the template parameter, choose the correct
		//function signature as 'TSubscriptionFun'
		typedef typename TSwitch< TIsDummy< A1 >::m_value,  ISubscription_0, ISubscription_1  >::TValue TSubscription_01 ;
		typedef typename TSwitch< TIsDummy< A2 >::m_value,  TSubscription_01, ISubscription_2  >::TValue TSubscription_012;
		typedef typename TSwitch< TIsDummy< A3 >::m_value,  TSubscription_012, ISubscription_3  >::TValue TSubscription;

		typedef std::vector< TSubscription * > TSubscriptionCont;
	public:
		Observed() 
		{
		}

		virtual ~Observed()
		{
		
			TSubscriptionCont::iterator tit;
    		for(tit = m_subscriptions.begin(); tit !=  m_subscriptions.end(); ++tit )
			{
				TSubscription *psubscription = *tit;
				delete psubscription;
			}
			m_subscriptions.clear();
			//for_each( m_subscriptions.begin(), m_subscriptions.end(), std::mem_fun( &ISubscription::DeleteMe ) );
		}

		void Fire(  )
		{				  
			TSubscriptionCont::iterator tit;
			for(tit = m_subscriptions.begin(); tit !=  m_subscriptions.end(); ++tit )
			{
				TSubscription *psubscription = *tit;
				(*psubscription)( );
			}
		}


		void Fire(   ) const
		{			
			std::cout << "costant fire" << std::endl;
			TSubscriptionCont::const_iterator tit;
			for(tit = m_subscriptions.begin(); tit !=  m_subscriptions.end(); ++tit )
			{
				TSubscription  *  psubscription = *tit;
				psubscription->operator()( );
			}
		}

		template< typename A >
		void Fire( A &a )
		{					  
			TSubscriptionCont::iterator tit;
			for(tit = m_subscriptions.begin(); tit !=  m_subscriptions.end(); ++tit )
			{
				TSubscription *psubscription = *tit;
				(*psubscription)( a  );
			}
		}

		template < typename A  >
		void Fire( A &a  ) const
		{	
			std::cout << "costant fire" << std::endl;
			TSubscriptionCont::const_iterator tit;
			for(tit = m_subscriptions.begin(); tit !=  m_subscriptions.end(); ++tit )
			{
				TSubscription  *  psubscription = *tit;
				(*psubscription)( a  );
			}
		}

		template< typename A, typename B>
		void Fire( A &a , B &b )
		{					  
			TSubscriptionCont::iterator tit;
			for(tit = m_subscriptions.begin(); tit !=  m_subscriptions.end(); ++tit )
			{
				TSubscription *psubscription = *tit;
				(*psubscription)( a, b );
			}
		}


		template < typename A, typename B >
		void Fire( A &a , B &b ) const
		{		
			std::cout << "costant fire" << std::endl;
			TSubscriptionCont::const_iterator tit;
			for(tit = m_subscriptions.begin(); tit !=  m_subscriptions.end(); ++tit )
			{
				TSubscription  *  psubscription = *tit;
				(*psubscription)( a, b );
			}
		}

		template< typename A, typename B, typename C >
		void Fire( A &a , B &b , C &c)
		{					  
			TSubscriptionCont::iterator tit;
			for(tit = m_subscriptions.begin(); tit !=  m_subscriptions.end(); ++tit )
			{
				TSubscription *psubscription = *tit;
				(*psubscription)( a, b, c );
			}
		}


		template < typename A, typename B, typename C >
		void Fire( A &a , B &b, C &c ) const
		{		
			std::cout << "costant fire" << std::endl;
			TSubscriptionCont::const_iterator tit;
			for(tit = m_subscriptions.begin(); tit !=  m_subscriptions.end(); ++tit )
			{
				TSubscription  *  psubscription = *tit;
				(*psubscription )( a, b, c );
			}
		}

		void Subscribe( TSubscription &s )
		{
			//Note that the connected subscriptions are owned by the sugnal
			//and destroyed along withe the Observed
			TSubscriptionCont::iterator sit;
			for( sit = m_subscriptions.begin(); sit != m_subscriptions.end(); ++sit )
			{
				TSubscription * psub = *sit;
				if ( s.Equals( psub ) )
				{
					break;
				}
			}
			if( sit == m_subscriptions.end() )
			{
				m_subscriptions.push_back( &s );
			}

		}

		void UnSubscribe(  TSubscription &s )
		{
			TSubscriptionCont::iterator sit;
			for( sit = m_subscriptions.begin(); sit != m_subscriptions.end(); ++sit )
			{
				TSubscription * psub = *sit;
				if ( s.Equals( psub ) )
				{
					break;
				}
			}
			m_subscriptions.erase( sit );
			s.DeleteMe();
		}
		TSubscriptionCont  m_subscriptions;
	};

    //============================================================
	// Subscription classes that represents
	// const and non-const  class member functions
	//============================================================


	//IsConst = is the subscription function 
	//  a constant member function, 
	//  as in 'R (H::*Fun) (A1, A2 ) const'
	//R = return type of the member function
	//H = holder class of the member function
	template< int IsConst, typename R, typename H>
	class Subscription0 : public ISubscription0<R >
	{
	public:
		typedef ISubscription0<R > TBaseType;
		//get all possible function signatures here
		typedef R  (H:: * TSubscriptionFun0 )();
		typedef R  (H:: * TSubscriptionFun0C ) () const;
		//Now based on the template parameter, choose the correct
		//function signature as 'TSubscriptionFun'
		typedef typename TSwitch< IsConst,  TSubscriptionFun0C, TSubscriptionFun0  >::TValue TSubscriptionFun;
	
	public:
		//Subscription0 initializes with an instance of the holder class,
		//and a subscription function.
		//Eg:
		//class Example
		//{
		//	public:
		//	Example(int id):m_id(id){}
		//	int OnClick( )
		//	{
		//		std::cout << "Example::OnClick subscription: " << " observed: "  << observedId << std::endl; 	
		//		return 0;
		//	}
		//const int m_id;
		//};
		//Example example(67);
		//We can make a subscription out of it as follows
		//Subscription0<0 /*not constant member fun*/, 
		//           int /*return type of member fun*/,
		//	         Example /* holder class of the member function */,
		//	          >  subscription ( example, &Example::OnClick );
	
		Subscription0(H &holder,  TSubscriptionFun slFun ): 
		  m_pHolder( &holder),
		  m_subscriptionFun( slFun )
		  {
		  }

		  //main execute function 
		  R operator()()
		  {	
			  return( m_pHolder->*m_subscriptionFun)();
		  }


		 //trivial destructor
		  ~Subscription0()
		  {
		  }
		  
		  bool Equals( const TBaseType *prhs )
		  {
			const Subscription0 *prhs2 = dynamic_cast< const Subscription0 * > ( prhs );
			return	NULL != prhs2 && 
					m_pHolder == prhs2->m_pHolder && 
					m_subscriptionFun == prhs2->m_subscriptionFun;
		  }

		  H *m_pHolder;
		  TSubscriptionFun m_subscriptionFun;
	};


	//IsConst = is the subscription function 
	//    a constant member function, 
	//    as in 'R (H::*Fun) (A1, A2 ) const'
	//R = return type of the member function
	//H = holder class of the member function
	//A1 = First Argument of the member function
	template< int IsConst, typename R, typename H, typename A1 >
	class Subscription1 : public ISubscription1<R, A1 >
	{
	public:
		typedef ISubscription1<R, A1 >			TBaseType;
		typedef Subscription1< IsConst, R, H, A1> TThisType;

		typedef R  (H:: * TSubscriptionFun1 )( A1 );
		typedef R  (H:: * TSubscriptionFun1C ) ( A1 ) const;		
		//Now based on the template parameter, choose the correct
		//function signature as 'TSubscriptionFun'
		typedef typename TSwitch< IsConst,  TSubscriptionFun1C, TSubscriptionFun1  >::TValue TSubscriptionFun;
	
	public:
		//Subscription initializes with an instance of the holder class,
		//and a subscription function.
		//Eg:
		//class Example
		//{
		//	public:
		//	Example(int id):m_id(id){}
		//	int OnClick( int observedId )
		//	{
		//		std::cout << "Example::OnClick subscription: " << m_id << " observed: "  << observedId << std::endl; 	
		//		return 0;
		//	}
		//const int m_id;
		//};
		//Example example(67);
		//We can make a subscription out of it as follows
		//Subscription1<
		//            0 /*not constant member fun*/, 
		//            int /*return type of member fun*/,
		//	          Example /* holder class of the member function */,
		//	          int /*argument 1 */
		//	           >  subscription ( example, &Example::OnClick );
	
		Subscription1(H &holder,  TSubscriptionFun slFun ): 
		  m_pHolder( &holder ),
		  m_subscriptionFun( slFun )
		  {
		  }

		 //trivial destructor
		  ~Subscription1()
		  {
			  int i=0;
		  }

		  //main execute function 
		  R operator()(A1 a )
		  {	
			  return( m_pHolder->*m_subscriptionFun)(a );
		  }
		  
		  bool Equals( const TBaseType *prhs )
		  {
			const TThisType *prhs2 = dynamic_cast<const TThisType * > ( prhs );			
			return NULL != prhs2 && 
				   m_pHolder == prhs2->m_pHolder && 
				   m_subscriptionFun == prhs2->m_subscriptionFun;
	  	  }
 
		  H *m_pHolder;
		  TSubscriptionFun m_subscriptionFun;
	};


	//IsConst = is the subscription function 
	//	a constant member function, 
	//	as in 'R (H::*Fun) (A1, A2 ) const'
	//R = return type of the member function
	//H = holder class of the member function
	//A1 = First Argument of the member function
	//A2 = Second Argument of the member function
	template< int IsConst, typename R, typename H, typename A1, typename A2 >
	class Subscription2 : public ISubscription2<R, A1, A2>
	{
	public:
		typedef  ISubscription2<R, A1, A2> TBaseType;
		typedef  Subscription2< IsConst, R, H, A1, A2 > TThisType;

		typedef R  (H:: * TSubscriptionFun2 )( A1 , A2 );	
		typedef R  (H:: * TSubscriptionFun2C ) ( A1, A2  ) const;
		//Now based on the template parameter, choose the correct
		//function signature as 'TSubscriptionFun'
		typedef typename TSwitch< IsConst,  TSubscriptionFun2C, TSubscriptionFun2  >::TValue TSubscriptionFun;
	
	public:
		//Subscription initializes with an instance of the holder class,
		//and a subscription function.
		//Eg:
		//class Example
		//{
		//	public:
		//	Example(int id):m_id(id){}
		//	int OnClick( int observedId, int id2 )
		//	{
		//		std::cout << "Example::OnClick subscription: " << m_id << " observed: "  << observedId << std::endl; 	
		//		return 0;
		//	}
		//const int m_id;
		//};
		//Example example(67);
		//We can make a subscription out of it as follows
		//Subscription2<0 /*not constant member fun*/, 
		//              int /*return type of member fun*/,
		//	            Example /* holder class of the member function */,
		//	            int /*argument 1 */
		//	            int /*argument 2*/
		//	   >  subscription ( example, &Example::OnClick );
	
		Subscription2(H &holder,  TSubscriptionFun slFun ): 
		  m_pHolder( &holder ),
			  m_subscriptionFun( slFun )
		  {
		  }
		 
		  //trivial destructor
		  ~Subscription2()
		  {
		  }
		  
		  //main execute function 
		  R operator()(A1 a, A2 b)
		  {	
			  return( m_pHolder->*m_subscriptionFun)(a , b );
		  }
		  
		  bool Equals( const TBaseType *prhs )
		  {
			const TThisType *prhs2 = dynamic_cast< const TThisType * > ( prhs );
			return	NULL != prhs2 && 
					m_pHolder == prhs2->m_pHolder && 
					m_subscriptionFun == prhs2->m_subscriptionFun;
		  }

		  H *m_pHolder;
		  TSubscriptionFun m_subscriptionFun;
	};

    //IsConst = is the subscription function 
	//   a constant member function, 
	//   as in 'R (H::*Fun) (A1, A2, A3 ) const'
	//R = return type of the member function
	//H = holder class of the member function
	//A1 = First Argument of the member function
	//A2 = Second Argument of the member function
	//A3 = THird Argument of the member function
	template< int IsConst, typename R, typename H, typename A1, typename A2, typename A3  >
	class Subscription3 : public ISubscription3<R, A1, A2, A3 >
	{
	public:
		typedef ISubscription3<R, A1, A2, A3 > TBaseType;
		typedef Subscription3< IsConst, R, H, A1, A2, A3 > TThisType;

		typedef R  (H:: * TSubscriptionFun3 )( A1 , A2, A3  );	
		typedef R  (H:: * TSubscriptionFun3C ) ( A1, A2, A3   ) const;
		//Now based on the template parameter, choose the correct
		//function signature as 'TSubscriptionFun'
		typedef typename TSwitch< IsConst,  TSubscriptionFun3C, TSubscriptionFun3  >::TValue TSubscriptionFun;
	
	public:
		//Subscription initializes with an instance of the holder class,
		//and a subscription function.
		//Eg:
		//class Example
		//{
		//	public:
		//	Example(int id):m_id(id){}
		//	int OnClick( int observedId, int id2, char *pszName  )
		//	{
		//		std::cout	<< "Example::OnClick subscription: " 
		//					<< m_id << " observed: "  
		//					<< observedId << << " id2 " 
		//					<< id2 << " name: " 
		//					<< pszName << std::endl; 	
		//		return 0;
		//	}
		//const int m_id;
		//};
		//Example example(67);
		//We can make a subscription out of it as follows
		//Subscription3<0 /*not constant member fun*/, 
		//			int /*return type of member fun*/,
		//			Example /* holder class of the member function */,
		//			int /*argument 1 */
		//			int /*argument 2*/
		//			char  */*argument 3*/
		//	   >  subscription ( example, &Example::OnClick );
	
		Subscription3(H &holder,  TSubscriptionFun slFun ): 
		  m_pHolder( &holder ),
			  m_subscriptionFun( slFun )
		  {
		  }

		  //trivial destructor
		  ~Subscription3()
		  {
		  }

		  //main execute function 
		  R operator()(A1 a, A2 b, A3 c)
		  {	
			  return( m_pHolder->*m_subscriptionFun)(a , b, c);
		  }
		  bool Equals( const TBaseType *prhs )
		  {
			const TThisType *prhs2 = dynamic_cast< const TThisType * > ( prhs );
			return	NULL != prhs2 &&
					m_pHolder == pRhs->m_pHolder && 
					m_subscriptionFun == pRhs->m_subscriptionFun;
		  }

		  H *m_pHolder;
		  TSubscriptionFun m_subscriptionFun;
	};


    //============================================================
	// Subscription classes that represents
	// global static functions
	//============================================================


	//Specialization of Subscription function  for supporting
	//global static functions
	// template parameter isConst is specialized to '0'
	// template parameter H is specialized to void
	template<  typename R  >
	class Subscription0< 0, R, void > : public ISubscription0<R  >
	{
	public:
		typedef  ISubscription0<R  >			TBaseType;
		typedef  Subscription0< 0, R, void >	TThisType;

		typedef R  ( * TSubscriptionFun ) ();;
	

	public:
		//The constructor takes in a global static function
		//of the matching signature
		Subscription0(TSubscriptionFun slFun ): 
			  m_subscriptionFun( slFun )
		  {
		  }

		  ~Subscription0()
		  {
		  }
	    
		  R operator()( )
		  {	
			  return(*m_subscriptionFun)( );
		  }
	
		  bool Equals( const TBaseType *prhs )
		  {
			const TThisType *prhs2 = dynamic_cast< const TThisType * > ( prhs );
			return  NULL != prhs2 && 
					m_subscriptionFun == prhs2->m_subscriptionFun;
		  }

		  TSubscriptionFun m_subscriptionFun;
	};


	//Specialization of Subscription function  for supporting
	//global static functions
	//template parameter isConst is specialized to '0'
	//template parameter H is specialized to void
	template<  typename R, typename A1 >
	class Subscription1< 0, R, void, A1 > : public ISubscription1<R, A1 >
	{
	public:
		typedef ISubscription1<R, A1 > TBaseType;
		typedef Subscription1< 0, R, void, A1 > TThisType;

		typedef R  ( * TSubscriptionFun ) ( A1 ) ;	


	public:
		//The constructor takes in a global static function
		//of the matching signature
		Subscription1(TSubscriptionFun slFun ): 
		  m_subscriptionFun( slFun )
		  {
		  }		
		  ~Subscription1()
		  {
		  }
		  R operator()(A1 a  )
		  {	
			  return m_subscriptionFun ( a  );
		  }	
		  bool Equals( const TBaseType *prhs )
		  {
			  const TThisType *prhs2 = dynamic_cast<const TThisType * > ( prhs );
			  return	NULL != prhs2 &&
						m_subscriptionFun == prhs2->m_subscriptionFun;
		  }

		  TSubscriptionFun m_subscriptionFun;
	};

	//Specialization of Subscription function  for supporting
	//global static functions
	//template parameter isConst is spacialized to '0'
	//template parameter H is specialized to void
	template<  typename R, typename A1, typename A2 >
	class Subscription2< 0, R, void, A1, A2 > : public ISubscription2<R, A1, A2 >
	{
	public:
		typedef ISubscription2<R, A1, A2 > TBaseType;
		typedef Subscription2< 0, R, void, A1, A2 > TThisType;

		typedef R  ( * TSubscriptionFun ) ( A1, A2  ) ;
	
	public:
		//The constructor takes in a global static function
		//of the matching signature
		Subscription2(TSubscriptionFun slFun ): 
			  m_subscriptionFun( slFun )
		  {
		  }	
		  ~Subscription2()
		  {
		  }
		  R operator()(A1 a , A2 b )
		  {	
			  return(*m_fun)(a, b);
		  }
	 
		  bool Equals( const TBaseType *prhs )
		  {
			  const TThisType *prhs2 = dynamic_cast<const TThisType * > ( *prhs );
			  return NULL != prhs2 &&
				     m_subscriptionFun == prhs2->m_subscriptionFun;
		  }
		  TSubscriptionFun m_subscriptionFun;
	};

	//Specialization of Subscription function  for supporting
	//global static functions
	//template parameter isConst is specialized to '0'
	//template parameter H is specialized to void
	template<  typename R, typename A1, typename A2, typename A3 >
	class Subscription3< 0, R, void, A1, A2, A3 > : public ISubscription3<R, A1, A2, A3  >
	{
	public:
		typedef ISubscription3<R, A1, A2, A3  > TBaseType;
		typedef Subscription3< 0, R, void, A1, A2, A3 > TThisType;

		typedef R  ( * TSubscriptionFun ) ( A1, A2, A3  ) ;
	
	public:
		//The constructor takes in a global static function
		//of the matching signature
		Subscription3(TSubscriptionFun slFun ): 
			  m_subscriptionFun( slFun )
		  {
		  }
	
		  ~Subscription3()
		  {
		  }

		  R operator()(A1 a , A2 b, A3 c  )
		  {	
			  return(*m_fun)(a, b, c);
		  }

		  bool Equals( const TBaseType *prhs )
		  {
			const TThisType *prhs2 = dynamic_cast< const TThisType * > ( *prhs );
			return	NULL != prhs2 &&
					m_subscriptionFun == prhs2->m_subscriptionFun;
		  }

		  TSubscriptionFun m_subscriptionFun;
	};


	// SubscriptionMaker functions are utility functions
	// that make Subscription classes.
	// You supply the SubcriptionMaker with the correct arguments
	// and it produces an appropriate  Subscription.
	// Through the use of SubscriptionMaker much of the
	// template complexity is hidden from the user.


    //============================================================
	// SubscriptionMaker functions for making Subscription-s
	// that accept non-constant class functions
	//============================================================

	

	
	//Make a Subscription0 class instance
	//where the member function signature accepts no arguments
	//and the member function is not a constant.
	template< typename R, typename H  >
	Subscription0<0, R, H > *SubscriptionMaker(H  &holder, R (H::*TSubscriptionFun ) ()  )
	{
		return new Subscription0< 0, R, H > ( holder, TSubscriptionFun ); 
	}

	//Make a Subscription1 class instance
	//where the member function signature accepts one argument
	//and the member function is not a constant.
	template< typename R, typename H, typename A1 >
	Subscription1<0, R, H,A1 > *SubscriptionMaker(H  &holder, R (H::*TSubscriptionFun ) (A1 )  )
	{
		return new Subscription1< 0, R, H, A1 > ( holder, TSubscriptionFun ); 
	}

	//Make a Subscription2 class instance
	//where the member function signature accepts two arguments
	//and the member function is not a constant.
	template< typename R, typename H, typename A1 , typename A2 >
	Subscription2<0, R, H,A1, A2 > *SubscriptionMaker(H  &holder, R (H::*TSubscriptionFun ) (A1, A2 )  )
	{
		return new Subscription2< 0, R, H, A1, A2 > ( holder, TSubscriptionFun ); 
	}

	//Make a Subscription3 class instance
	//where the member function signature accepts three arguments
	//and the member function is not a constant.
	template< typename R, typename H, typename A1 , typename A2, typename A3 >
	Subscription3<0, R, H,A1, A2, A3 > *SubscriptionMaker(H  &holder, R (H::*TSubscriptionFun ) (A1, A2, A3 )  )
	{
		return new Subscription3< 0, R, H, A1, A2,A3 > ( holder, TSubscriptionFun ); 
	}	

	//============================================================
	// SubscriptionMaker functions for making Subscription-s
	// that accept constant class member functions
	//============================================================


	//Make a Subscription0 class instance
	//where the member function signature accepts no arguments
	//and the member function is a constant.
	template< typename R, typename H  >
	Subscription0<1, R, H > *SubscriptionMaker(H  &holder, R (H::* TSubscriptionFun ) () const )
	{
		return new Subscription0< 1, R, H  > ( holder, TSubscriptionFun ); 
	}
    template< typename R, typename H  >
	Subscription0<1, R, const H > *SubscriptionMaker( const H  &holder, R (H::* TSubscriptionFun ) () const )
	{
		return new Subscription0< 1, R, const H  > ( holder, TSubscriptionFun ); 
	}
	//Make a Subscription1 class instance
	//where the member function signature accepts one argument
	//and the member function is a constant.
	template< typename R, typename H, typename A1  >
	Subscription1<1, R, H,A1  > *SubscriptionMaker(H  &holder, R (H::* TSubscriptionFun ) (A1 ) const )
	{
		return new Subscription1< 1, R, H, A1 > ( holder, TSubscriptionFun ); 
    }

	template< typename R, typename H, typename A1  >
	Subscription1<1, R, const H,A1  > *SubscriptionMaker(const H  &holder, R (H::* TSubscriptionFun ) (A1 ) const )
	{
		return new Subscription1< 1, R, const H, A1 > ( holder, TSubscriptionFun ); 
    }
	
	//Make a Subscription2 class instance
	//where the member function signature accepts two arguments
	//and the member function is a constant.
	template< typename R, typename H, typename A1 , typename A2 >
	Subscription2<1, R, H,A1, A2 > *SubscriptionMaker(H  &holder, R (H::* TSubscriptionFun ) (A1, A2 ) const )
	{		
		return new Subscription2< 1, R, H, A1, A2 > ( holder, TSubscriptionFun ); 
	}
	template< typename R, typename H, typename A1 , typename A2 >
	Subscription2<1, R, const H, A1, A2 > *SubscriptionMaker( const H  &holder, R (H::* TSubscriptionFun ) (A1, A2 ) const )
	{		
		return new Subscription2< 1, R, const H, A1, A2 > ( holder, TSubscriptionFun ); 
	}
    //Make a Subscription3 class instance
	//where the member function signature accepts three arguments
	//and the member function is a constant.
	template< typename R, typename H, typename A1 , typename A2, typename  A3 >
	Subscription3<1, R, H, A1, A2, A3 > *SubscriptionMaker(H  &holder, R (H::* TSubscriptionFun ) (A1, A2, A3 ) const )
	{		
		return new Subscription3< 1, R, H, A1, A2, A3 > ( holder, TSubscriptionFun ); 
	}
	template< typename R, typename H, typename A1 , typename A2, typename  A3 >
	Subscription3<1, R, const H, A1, A2, A3 > *SubscriptionMaker( const H  &holder, R (H::* TSubscriptionFun ) (A1, A2, A3 ) const )
	{		
		return new Subscription3< 1, R, const H, A1, A2, A3 > ( holder, TSubscriptionFun ); 
	}
	//============================================================
	// SubscriptionMaker functions for making Subscription-s
	// that accept global static functions
	//============================================================

	//Make a Subscription0 class instance
	//where the global static function signature accepts no argument
	//and the global static function is not a  constant. (global static funs cannot be constants)
	template< typename R  >
	Subscription0<0, R, void > *SubscriptionMaker( R (*TSubscriptionFun ) ()  )
	{
		return new Subscription0< 0, R, void > ( TSubscriptionFun ); 
	}
	//Make a Subscription1 class instance
	//where the global static function signature accepts one argument
	//and the global static function is not a  constant. (global static funs cannot be constants)
	template< typename R,  typename A1 >
	Subscription1<0, R, void, A1  > *SubscriptionMaker( R (*TSubscriptionFun ) (A1)  )
	{
		return new Subscription1< 0, R, void, A1 > ( TSubscriptionFun ); 
	}
	//Make a Subscription2 class instance
	//where the global static function signature accepts two arguments
	//and the global static function is not a constant. (global static funs cannot be constants)
	template< typename R,  typename A1, typename A2 >
	Subscription2<0, R, void, A1, A2 > *SubscriptionMaker( R (*TSubscriptionFun ) (A1, A2 )  )
	{
		return new Subscription2< 0, R, void, A1, A2 > ( TSubscriptionFun ); 
	}
	//Make a Subscription3 class instance
	//where the global static function signature accepts two arguments
	//and the global static function is not a constant. (global static funs cannot be constants)
	template< typename R,  typename A1, typename A2, typename A3 >
	Subscription3<0, R, void, A1, A2, A3 > *SubscriptionMaker( R (*TSubscriptionFun ) (A1, A2, A3 )  )
	{
		return new Subscription2< 0, R, void, A1, A2, A3 > ( TSubscriptionFun ); 
	}
}



#endif