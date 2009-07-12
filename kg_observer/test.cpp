#include "kg_observer.h"


using namespace kgObserver;


namespace test
{
	//======================================================================================/
	//======================================================================================/
	// Make a class which has a member function of type 'int(:: * fun ) (int)'
	// which can be used to act as a subscription
	//
	//This gives an  interface class
	//with a subscription fnction which has the signature 'int (:: *fun)( int )'
	class I_SubscriberExample_R_int_A1_int
	{
	public:
		I_SubscriberExample_R_int_A1_int(){}
		virtual ~I_SubscriberExample_R_int_A1_int(){}
		//core subscription member function of type 'int (:: *fun ) (int )'
		virtual int NotifyMe( int observedId )=0;
	};

    //This gives a concrete class for the above interface
	class SubscriberExample_R_int_A1_int : public  I_SubscriberExample_R_int_A1_int
	{
	public:
		SubscriberExample_R_int_A1_int(int id):m_id(id){}
		//core subscription member function of type 'int (:: *fun ) (int )'
		int NotifyMe( int observedId )
		{
			std::cout << "example member fun 'int Class::NotifyMe( int ), class insance's id = " << m_id << "  observed id: "  << observedId << std::endl; 	
			return 0;
		}
		//core subscription member function of type 'int (:: *fun ) (int )const'
		int NotifyMeC( int observedId )const
		{
			std::cout << "example member fun 'int Class::NotifyMe( int )const, class insance's id = " << m_id << "  observed id: "  << observedId << std::endl; 	
			return 0;
		}
		const int m_id;
	};


	//======================================================================================/
	//======================================================================================/
	// Make a class which has a member function of type 'int (:: * fun ) (int) const'
	// which can be used to act as a subscription
	//
	//This gives an  interface class
	//with a subscription fnction which has the signature 'int (:: *fun)( int ) const'
	class I_SubscriberExample_R_int_A1_int_Const
	{
	public:
		I_SubscriberExample_R_int_A1_int_Const(){}
		virtual ~I_SubscriberExample_R_int_A1_int_Const(){}
		//core subscription member function of type 'int (:: *fun ) (int ) const'
		virtual	int NotifyMe( int observedId ) const=0;
	};

    //This gives a concrete class for the above interface
	class SubscriberExample_R_int_A1_int_Const : public I_SubscriberExample_R_int_A1_int_Const
	{
	public:
		SubscriberExample_R_int_A1_int_Const(int id):m_id(id){}		
		//core subscription member function of type 'int (:: *fun ) (int ) const'
		int NotifyMe( int observedId ) const
		{
			std::cout << "example member fun 'int Class::NotifyMe( int ) const', class insance's id = " << m_id << "  observed id: "  << observedId << std::endl; 	
			return 0;
		}
		const int m_id;
	};
	
	//======================================================================================/
	//======================================================================================/
	// Make a class which has a member function of type 'R (:: * fun ) ()'
	// which can be used to act as a subscription
	//
	//This gives an  interface class
	//with a subscription fnction which has the signature 'int (:: *fun)( )'
	class I_SubscriberExample_R_int
	{
	public:
		I_SubscriberExample_R_int(){}
		virtual ~I_SubscriberExample_R_int(){}
		//core subscription member function of type 'R (:: *fun ) ( )'
		virtual int NotifyMe( )=0;
	};
	
    //This gives a concrete class for the above interface
	class SubscriberExample_R_int : public I_SubscriberExample_R_int
	{
	public:
		SubscriberExample_R_int(int id):m_id(id){}
		//core subscription member function of type 'R (:: *fun ) ( )'
		int NotifyMe( )
		{			
			std::cout << "example member fun 'int Class::NotifyMe(  )', class insance's id = " << m_id << " no provision to get observed's id "   << std::endl; 
			return 0;
		}
		
		//core subscription member function of type 'R (:: *fun ) ( )const'
		//Note that, this is not defined in the base class
		int NotifyMeC( )const
		{			
			std::cout << "example member fun 'int Class::NotifyMe(  )C', class insance's id = " << m_id << " no provision to get observed's id "   << std::endl; 
			return 0;
		}
		const int m_id;
	};

	//======================================================================================/
	//======================================================================================/
	// Make a class which has a member function of type 'void (:: * fun ) ( int)'
	// which can be used to act as a subscription
	//
	//This gives an  interface class
	//with a subscription fnction which has the signature 'void (:: *fun)( int )'
	class I_SubscriberExample_R_void_A1_int
	{
	public:
		I_SubscriberExample_R_void_A1_int(){}
		virtual ~I_SubscriberExample_R_void_A1_int(){}
		//core subscription member function of type 'void (:: *fun ) ( int )'
		virtual void NotifyMe( int observedId )=0;
	};
	
    //This gives a concrete class for the above interface
	class SubscriberExample_R_void_A1_int :public I_SubscriberExample_R_void_A1_int
	{
	public:
		SubscriberExample_R_void_A1_int(int id):m_id(id){}		
		//core subscription member function of type 'void (:: *fun ) ( int )'
		void NotifyMe( int observedId )
		{		
			std::cout << "example member fun 'void Class::NotifyMe( int  )', class insance's id = " << m_id << " observed id = " << observedId << std::endl; 
			return;
		}
		//another core subscription member function of type 'void (:: *fun ) ( int )const'		
		//This is not available in the base class
		void NotifyMeC( int observedId ) const
		{		
			std::cout << "example member fun 'void Class::NotifyMeC( int  ) const', class insance's id = " << m_id << " observed id = " << observedId << std::endl; 
			return;
		}
		const int m_id;
	};
	
	//======================================================================================/
	//======================================================================================/
	//An example complex class
	//that can be passed as an argument of a subscription function
	class ExampleCompleClassWithId 
	{
	public:
		ExampleCompleClassWithId( int id):m_id(id ){}
		virtual ~ExampleCompleClassWithId(){}
		int GetId() const { return m_id; }
	private:
		const int m_id;
	};
	
	//======================================================================================/
	//======================================================================================/
	// Make a class which has a member function of type 'void (:: * fun ) ( ExampleClass & )'
	// which can be used to act as a subscription
	//
	//This gives an  interface class
	//with a subscription fnction which has the signature 'void (:: *fun)( ExampleClass & )'
	class I_SubscriberExample_R_void_A1_Class
	{
	public:
		I_SubscriberExample_R_void_A1_Class(){}
		virtual ~I_SubscriberExample_R_void_A1_Class(){}
		//core  subscription member function of type 'void (:: *fun ) ( ExampleClass & )'		
		virtual void NotifyMe( ExampleCompleClassWithId &object  )=0;
	};

    //This gives a concrete class for the above interface
	class SubscriberExample_R_void_A1_Class : public I_SubscriberExample_R_void_A1_Class
	{
	public:
		SubscriberExample_R_void_A1_Class(int id):m_id(id){}				
		//core  subscription member function of type 'void (:: *fun ) ( ExampleClass & )'
		void NotifyMe( ExampleCompleClassWithId &object  )
		{
			int observedId = object.GetId();
			std::cout << "example member fun 'void Class::NotifyMe( ExampleClass &  )', class insance's id = " << m_id << " observed id = " << observedId <<  std::endl; 
			return;
		}
		//core  subscription member function of type 'void (:: *fun ) ( ExampleClass & )const'	
        //This is absent in the base class	
		void NotifyMeC( ExampleCompleClassWithId &object  )const
		{
			int observedId = object.GetId();
			std::cout << "example member fun 'void Class::NotifyMe( ExampleClass &  )const', class insance's id = " << m_id << " observed id = " << observedId <<  std::endl; 
			return;
		}
		const int m_id;
	};

	//======================================================================================/
	//======================================================================================/
	// Make a class which has a member function of type 'void (:: * fun ) ( const  ExampleClass & )'
	// which can be used to act as a subscription
	//
	//This gives an  interface class
	//with a subscription fnction which has the signature 'void (:: *fun)( const ExampleClass & )'
	class I_SubscriberExample_R_void_A1_Const_Class
	{
	public:
		I_SubscriberExample_R_void_A1_Const_Class(){}
		virtual ~I_SubscriberExample_R_void_A1_Const_Class(){}
		virtual void NotifyMe( const ExampleCompleClassWithId &object  )=0;
	};

	
    //This gives a concrete class for the above interface
	class SubscriberExample_R_void_A1_Const_Class : public I_SubscriberExample_R_void_A1_Const_Class
	{
	public:
		SubscriberExample_R_void_A1_Const_Class(int id):m_id(id){}				
		//core subscription member function of type 'void (:: *fun ) ( const ExampleClass & )'
		void NotifyMe( const ExampleCompleClassWithId &object  )
		{
			int observedId = object.GetId();
			std::cout << "example member fun 'void Class::NotifyMe( cons ExampleClass &  )', class insance's id = " << m_id << " observed id = " << observedId <<   std::endl; 
			return;
		}
		//core subscription member function of type 'void (:: *fun ) ( const ExampleClass & )const'
		//Note that this is not defined in the base class
		void NotifyMeC( const ExampleCompleClassWithId &object  )const
		{
			int observedId = object.GetId();
			std::cout << "example member fun 'void Class::NotifyMeC( cons ExampleClass &  )const', class insance's id = " << m_id << " observed id = " << observedId <<   std::endl; 
			return;
		}
		const int m_id;
	};


	//======================================================================================/
	//======================================================================================/
	// Make a class which has a member function of type 'void (:: * fun ) (  ExampleClass & ) const'
	// which can be used to act as a subscription
	//
	//This gives an  interface class
	//with a subscription fnction which has the signature 'void (:: *fun)( ExampleClass & )const '

	class I_Const_SubscriberExample_R_void_A1_Class
	{
	public:
		I_Const_SubscriberExample_R_void_A1_Class(){}
		virtual ~I_Const_SubscriberExample_R_void_A1_Class(){}
		//core subscription member function of type 'void (:: *fun ) ( class & ) const '
		virtual void NotifyMe( ExampleCompleClassWithId &object  )const=0;
	};

	
    //This gives a concrete class for the above interface
	class Const_SubscriberExample_R_void_A1_Class : public I_Const_SubscriberExample_R_void_A1_Class
	{
	public:
		Const_SubscriberExample_R_void_A1_Class(int id):m_id(id){}				
		//core subscription member function of type 'void (:: *fun ) ( class & ) const '
		void NotifyMe( ExampleCompleClassWithId &object  ) const
		{			
			int observedId = object.GetId();
			std::cout << "example member fun 'void Class::NotifyMe( ExampleClass &  ) const', class insance's id = " << m_id << " observed id = " << observedId <<  std::endl; 
			return;
		}
		const int m_id;
	};

	

	//======================================================================================/
	//======================================================================================/
	// Make a class which has a member function of type 'int (:: * fun ) (  int ) const'
	// which can be used to act as a subscription
	//
	//This gives an  interface class
	//with a subscription fnction which has the signature 'int (:: *fun)( int )const '

	class I_Const_SubscriberExample_R_int_A1_int
	{
	public:
		I_Const_SubscriberExample_R_int_A1_int(){}
		virtual ~I_Const_SubscriberExample_R_int_A1_int(){}
		//core subscription member function of type 'int (:: *fun ) ( class & ) const '
		virtual int NotifyMe(int observedId  )const=0;
	};

	
    //This gives a concrete class for the above interface
	class Const_SubscriberExample_R_int_A1_int : public I_Const_SubscriberExample_R_int_A1_int
	{
	public:
		Const_SubscriberExample_R_int_A1_int(int id):m_id(id){}				
		//core subscription member function of type 'int (:: *fun ) ( class & ) const '
		int NotifyMe( int observedId ) const
		{			
			std::cout << "example member fun 'int Class::NotifyMe( int  ) const', class insance's id = " << m_id << " observed id = " << observedId <<  std::endl; 
			return 0;
		}
		const int m_id;
	};

	//======================================================================================/
	//======================================================================================/
	// Make a global static function of type 'int (* fun ) ( int )'
	// which can be used to act as a subscription
	//

	int NotifyMe( int observedId )
		{	
			std::cout << "example global static fun 'int NotifyMe( int )'" <<  " observed id = " << observedId <<   std::endl; 
			return 0;
		}

	//======================================================================================/
	//======================================================================================/
	// Make a global static function of type 'int (* fun ) ( )'
	// which can be used to act as a subscription
	//
	int NotifyMe( )
		{		
			std::cout << "example global static fun 'int NotifyMe(  )'" <<  " no provision of geting observedId " << std::endl; 
			return 0;
		}
	
	//======================================================================================/
	//======================================================================================/
	// Make a global static function of type 'int (* fun ) ( Class & )'
	// which can be used to act as a subscription
	//

	int NotifyMe( ExampleCompleClassWithId &o )
		{
			int observedId = o.GetId();
			std::cout << "example global static fun 'int NotifyMe( int )'" <<  " observed id = " << observedId <<   std::endl; 
			return 0;
		}

	//======================================================================================/
	//======================================================================================/
	// Make a global static function of type 'int (* fun ) ( const Class & )'
	// which can be used to act as a subscription
	//

	int NotifyMe( const ExampleCompleClassWithId &o )
		{
			int observedId = o.GetId();
			std::cout << "example global static fun 'int NotifyMe( int )'" <<  " observed id = " << observedId <<    std::endl; 
			return 0;
		}
	
	//======================================================================================/
	//======================================================================================/
	//Example Twelve
	// Make a global static function of type 'void (* fun ) ( int )'
	// which can be used to act as a subscription
	//
	// We have to rename all functions that return void
	// otherwise they collide with the 'NotifyMe' functions with the same arguments
	// that return int
	void NotifyMeV( int observedId )
		{		
			std::cout << "example global static fun 'int NotifyMe( int )'" <<  " observed id = " << observedId <<    std::endl; 
			return;
		}

	//======================================================================================/
	//======================================================================================/
	// Make a global static function of type 'void (* fun ) (  )'
	// which can be used to act as a subscription
	//
	// We have to rename all functions that return void
	// otherwise they collide with the 'NotifyMe' functions with the same arguments
	// that return int
	void NotifyMeV(  )
		{			
			std::cout << "example global static fun 'void NotifyMe(  )'" <<  " no provision of geting observedId " << std::endl; 
			return;
		}

	//======================================================================================/
	//======================================================================================/
	// Make a global static function of type 'void (* fun ) ( ExampleClass & )'
	// which can be used to act as a subscription

	//	We have to rename all functions that return void
	// otherwise they collide with the 'NotifyMe' functions with the same arguments
	// that return int
	void NotifyMeV( ExampleCompleClassWithId &o )
		{
			int observedId = o.GetId();
			std::cout << "example global static function NotifyMe of type 'void (*F)( ExampleCompleClassWithId & )'  " << " observed: "  << o.GetId() << std::endl; 	
			return;
		}
	//======================================================================================/
	//======================================================================================/
	// Make a global static function of type 'void (* fun ) ( const ExampleClass & )'
	// which can be used to act as a subscription

	//	We have to rename all functions that return void
	// otherwise they collide with the 'NotifyMe' functions with the same arguments
	// that return int
	
	void NotifyMeV( const ExampleCompleClassWithId &o )
		{			
			int observedId = o.GetId();
			std::cout << "Static function NotifyMe of type 'void (*F)( ExampleCompleClassWithId & )'  " << " observed: "  << o.GetId() << std::endl; 	
			return;
		}
	
	//======================================================================================/
	//======================================================================================/
	//An example of an  'Observed' class 
	// expecting a Subscription from subscriptions of type
	// 'int ( :: *fun ) ( int )' 
	///or 'int ( :: *fun ) ( int ) const ' 
	// or 'int ( * fun ) (int )'

	class ClickExample_R_int_A1_int : public Observed< int, int>
	{
	public:
		ClickExample_R_int_A1_int( int id ):m_id(id){}
		void ClickMe()
		{
			int id = m_id;
			//Fire the event,
			//call the connected subscription funs with this 'int'argument
			//If you use a 'Fire' signature or any other arity,
			//this will result in compilation error.
			Fire( id );
		}		
		const int m_id;
	};
	
	
	//======================================================================================/
	//======================================================================================/	
	//An example of an  'Observed' class expecting a connection from 
	// subscriptions of type
	// 'void ( :: *fun ) ( int )' 
	// or 'void ( :: *fun ) ( int ) const' 
	// or 'void ( * fun ) (int )'

	class ClickExample_R_void_A1_int : public Observed< void, int>
	{
	public:
		ClickExample_R_void_A1_int( int id ):m_id(id){}
		void ClickMe()
		{
			int id = m_id;
			//Fire the event,
			//call the connected subscription funs with this 'int'argument
			//If you use a 'Fire' signature or any other arity,
			//this will result in compilation error.
			Fire( id );
		}		
		const int m_id;
	};
	
	//======================================================================================/
	//======================================================================================/	
	//An example of an  'Observed' class 
	// expecting a connection from subscriptions of type
	// 'void ( :: *fun ) ( ExampleClass & )' 
	// or 'void ( :: *fun ) ( ExampleClass & ) const' 
	// or 'void ( * fun ) ( ExampleClass & )'
	//Note that this class is multiply derived from ExampleCompleClassWithId
	class ClickExample_R_void_A1_Class : public Observed< void, ExampleCompleClassWithId & >, public ExampleCompleClassWithId
	{
	public:
		ClickExample_R_void_A1_Class ( int id ):ExampleCompleClassWithId(id){}
		void ClickMe()
		{
			int id = GetId();
			//Fire the event,
			//call the connected subscription funs with this 'this'argument
			//Note that this class is multiply derived from ExampleCompleClassWithId
			//If you use a 'Fire' signature or any other signature,
			//this will result in compilation error.
			Fire( *this );
		}	
	};
	
	//======================================================================================/
	//======================================================================================/	
	//An example of an  'Observed'  class
	//expecting a connection from subscriptions of type
	// 'void ( :: *fun ) ( const ExampleClass & )' 
	// or 'void ( :: *fun ) ( const ExampleClass & ) const' 
	// or 'void ( * fun ) ( const ExampleClass & )'	
	//Note that this class is multiply derived from ExampleCompleClassWithId
	class ClickExample_R_void_A1_Const_Class : public Observed< void, const ExampleCompleClassWithId & >, public ExampleCompleClassWithId
	{
	public:
		ClickExample_R_void_A1_Const_Class( int id ):ExampleCompleClassWithId(id){}
		void ClickMe()
		{
			int id = GetId();
			
			//Fire the event,
			//call the connected subscription funs with this 'this'argument
			//Note that this class is multiply derived from ExampleCompleClassWithId
			//If you use a 'Fire' signature or any other signature,
			//this will result in compilation error.
			Fire( *this );
		}	
	};
	//======================================================================================/
	//======================================================================================/	
	//An example of an  'Observed'  class
	//expecting a connection from subscriptions of type
	// 'void ( :: *fun ) ( const ExampleClass & )' 
	// or 'void ( :: *fun ) ( const ExampleClass & ) const' 
	// or 'void ( * fun ) ( const ExampleClass & )'	
	//Note that this class is multiply derived from ExampleCompleClassWithId
	class ClickExample_Const_R_void_A1_Const_Class : public Observed< void, const ExampleCompleClassWithId & >, public ExampleCompleClassWithId
	{
	public:
		ClickExample_Const_R_void_A1_Const_Class( int id ):ExampleCompleClassWithId(id){}
		void ClickMe() const
		{
			int id = GetId();
			
			//Fire the event,
			//call the connected subscription funs with this 'this'argument
			//Note that this class is multiply derived from ExampleCompleClassWithId
			//If you use a 'Fire' signature or any other signature,
			//this will result in compilation error.
			//The compiler will automatically select the
			//contant version of the Fire function
			Fire( *this );
		}	
	};
	
	//======================================================================================/
	//======================================================================================/	
	
	//An example of an  'Observed' class
	// expecting a connection from subscriptions of type
	// 'void ( :: *fun ) (  )' 
    // 'void ( :: *fun ) (  )const' 
	// or 'void ( * fun ) ( )'
	class ClickExample_R_int : public Observed< int >
	{
	public:
		ClickExample_R_int( int id ):m_id(id){}
		void ClickMe()
		{
			int id = m_id;
			//Fire the event,
			//call the connected subscription funs with this 'int'argument
			//If you use a 'Fire' signature or any other arity,
			//this will result in compilation error.
			Fire( );
		}		
		const int m_id;
	};

	//======================================================================================/
	//======================================================================================/	
    // An  example of an  'Observed' class
	// expecting a connection from subscriptions of type
	// 'int ( :: *fun ) ( int )' 
	// 'int ( :: *fun ) ( int ) class' 
	// or 'int ( * fun ) (  int )'
	//where the constant version of the fire version is launched
	class ClickExample_Const_R_int_A1_int : public Observed< int, int>
	{
	public:
		ClickExample_Const_R_int_A1_int( int id ):m_id(id){}
		void ClickMe() const
		{
			int id = m_id;
			//Fire the event,
			//call the connected subscription funs with this 'int'argument
			//If you use a 'Fire' signature or any other arity,
			//this will result in compilation error.
			//The compiler will automatically select the
			//contant version of the Fire function
			Fire( id );
		}		
		const int m_id;
	};
	

	
	bool test()
	{
		{
			//Observed class and subscribers whose subscriptions are of type
			// 'int ( :: *fun ) ( int )' 
			///or 'int ( :: *fun ) ( int ) const ' 
			// or 'int ( * fun ) (int )'
			ClickExample_R_int_A1_int click(98); //observed class
			//different kinds of subscribers
			SubscriberExample_R_int_A1_int   subscription1(56), subscription2(76);			
			I_SubscriberExample_R_int_A1_int &isubscription2( subscription2);
			SubscriberExample_R_int_A1_int_Const subscription3(89), subscription4(46);
			I_SubscriberExample_R_int_A1_int_Const &isubscription4( subscription4 );
			click.Subscribe( *( SubscriptionMaker( subscription1,&SubscriberExample_R_int_A1_int::NotifyMe ) ) );			
			click.Subscribe( *( SubscriptionMaker( isubscription2,&I_SubscriberExample_R_int_A1_int::NotifyMe ) ) );
			click.Subscribe( *( SubscriptionMaker( subscription3,&SubscriberExample_R_int_A1_int_Const::NotifyMe ) ) );
			click.Subscribe( *( SubscriptionMaker( isubscription4,&I_SubscriberExample_R_int_A1_int_Const::NotifyMe ) ) );		
			{
				typedef int (*TFun)( int );
				TFun tfun = &NotifyMe;
				click.Subscribe( *( SubscriptionMaker( tfun ) ) );
			}
			//obsereved function calls the subscribers
			click.ClickMe();
		}


		{
			//Observed class and subscribers whose subscriptions are of type
			// 'void ( :: *fun ) ( int )' 
			///or 'void ( :: *fun ) ( int ) const ' 
			// or 'void ( * fun ) (int )'
			SubscriberExample_R_void_A1_int subscription1(57), subscription2(77), subscription3(58);
			I_SubscriberExample_R_void_A1_int &isubscription2( subscription2 );
			ClickExample_R_void_A1_int click(98);			
			click.Subscribe( *(  SubscriptionMaker( subscription1, &SubscriberExample_R_void_A1_int::NotifyMe   )  ) );
			click.Subscribe( *(  SubscriptionMaker(  isubscription2, &I_SubscriberExample_R_void_A1_int::NotifyMe  ) ) );
			click.Subscribe( *(  SubscriptionMaker( subscription3, &SubscriberExample_R_void_A1_int::NotifyMeC   )  ) );		
			{
				typedef void (*TFun)( int );
				TFun tfun = &NotifyMeV;
				click.Subscribe( *(  SubscriptionMaker( tfun ) ) );
			}			
			//obsereved function calls the subscribers
			click.ClickMe();
			//unsubscribing one of the subscriptions
			click.UnSubscribe( *(  SubscriptionMaker( subscription1, &SubscriberExample_R_void_A1_int::NotifyMe   )  ) );
			//calling click me again
			click.ClickMe();	
		}
	
		{
			//Observed class and subscribers whose subscriptions are of type
			// 'void ( :: *fun ) ( ExampleClass &c )' 
			///or 'void ( :: *fun ) ( ExampleClass &c ) const ' 
			// or 'void ( * fun ) (ExampleClass &c )'
			SubscriberExample_R_void_A1_Class subscription1(59), subscription2(79), subscription3(89);
			I_SubscriberExample_R_void_A1_Class &isubscription2(subscription2);
			ClickExample_R_void_A1_Class click(98);
			click.Subscribe( * ( SubscriptionMaker( subscription1, &SubscriberExample_R_void_A1_Class::NotifyMe ) ) );
			click.Subscribe( * ( SubscriptionMaker( isubscription2, &I_SubscriberExample_R_void_A1_Class::NotifyMe ) ) );
			click.Subscribe( * ( SubscriptionMaker( subscription3, &SubscriberExample_R_void_A1_Class::NotifyMeC ) ) );
			{				
				typedef void (*TFun)( ExampleCompleClassWithId &o );
				TFun tfun = &NotifyMeV;
				click.Subscribe( *(  SubscriptionMaker( tfun ) ) );
			}
			click.ClickMe();
			click.ClickMe();
		}

		{
			//Observed class and subscribers whose subscriptions are of type
			// 'void ( :: *fun ) ( const ExampleClass &c )' 
			///or 'void ( :: *fun ) ( const ExampleClass &c ) const ' 
			// or 'void ( * fun ) ( const ExampleClass &c )'
			SubscriberExample_R_void_A1_Const_Class subscription1(60), subscription2(80), subscription3(36);
			I_SubscriberExample_R_void_A1_Const_Class &isubscription2(subscription2);
			ClickExample_R_void_A1_Const_Class click(98);
			click.Subscribe( * ( SubscriptionMaker( subscription1, &SubscriberExample_R_void_A1_Const_Class::NotifyMe ) ) );
			click.Subscribe( * ( SubscriptionMaker( isubscription2, &I_SubscriberExample_R_void_A1_Const_Class::NotifyMe ) ) );
			click.Subscribe( * ( SubscriptionMaker( subscription3, &SubscriberExample_R_void_A1_Const_Class::NotifyMeC ) ) );	
			{				
				typedef void (*TFun)( const ExampleCompleClassWithId &o );
				TFun tfun = &NotifyMeV;
				click.Subscribe( *(  SubscriptionMaker( tfun ) ) );
			}
			
			click.ClickMe();
		}
	
		{	
			//Observed class and subscribers whose subscriptions are of type
			// 'int ( :: *fun ) (  )' 
			///or 'int ( :: *fun ) (  ) const ' 
			// or 'int ( * fun ) (  )'
			SubscriberExample_R_int subscription1(58), subscription2(78), subscription3(90);
			I_SubscriberExample_R_int &isubscription2( subscription2 );
			ClickExample_R_int click(98);
			click.Subscribe( * ( SubscriptionMaker( subscription1, &SubscriberExample_R_int::NotifyMe  )  ) );
			click.Subscribe( * ( SubscriptionMaker( isubscription2, &I_SubscriberExample_R_int::NotifyMe )  ) );
			click.Subscribe( * ( SubscriptionMaker( subscription3, &SubscriberExample_R_int::NotifyMeC )  ) );		
			{				
				typedef int (*TFun)( );
				TFun tfun = &NotifyMe;
				click.Subscribe( *(  SubscriptionMaker( tfun ) ) );
			}
			click.ClickMe();
		}
		{
			//Observed class and subscribers whose subscriptions are of type
			// 'int ( :: *fun ) ( int )' 
			///or 'int ( :: *fun ) ( int ) const ' 
			// or 'int ( * fun ) ( int )'
			//But the 'Fire' function of the observed class
			//is a constant function
			SubscriberExample_R_int_A1_int subscription1(61), subscription2(81);
			I_SubscriberExample_R_int_A1_int &isubscription2( subscription2 );
			SubscriberExample_R_int_A1_int subscription3(89);
			ClickExample_Const_R_int_A1_int click(100);
			click.Subscribe( * ( SubscriptionMaker( subscription1, &SubscriberExample_R_int_A1_int::NotifyMe ) ) );
			click.Subscribe( * ( SubscriptionMaker( isubscription2, &I_SubscriberExample_R_int_A1_int::NotifyMe ) ) );
			click.Subscribe( * ( SubscriptionMaker( subscription3, &SubscriberExample_R_int_A1_int::NotifyMeC ) ) );	
			{				
				typedef int (*TFun)(int );
				TFun tfun = &NotifyMe;
				click.Subscribe( *(  SubscriptionMaker( tfun ) ) );
			}
			click.ClickMe();
			click.UnSubscribe(* ( SubscriptionMaker( subscription1, &SubscriberExample_R_int_A1_int::NotifyMe ) )  );
			click.ClickMe();
		}

		return true;
	}
}


int main( int argc, char * argv )
{

	int ret = test::test();

  return ret ;
}