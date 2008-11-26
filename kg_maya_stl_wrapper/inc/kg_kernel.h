#if !defined(KG_KERNEL_H_)
#define KG_KERNEL_H_

namespace kg
{

	//templatized base type
	//whose specialization yields the correct
	//functions and constants depending upon the type
	//Eg: BaseType<MVector>::zero would be a MVector initialized to all zeros.
	template< typename T >
	struct BaseType {	
		static  T zero(){ return static_cast< T > ( 0 ); }
		static T one() { return static_cast<T>(1); }
		static std::pointer_to_unary_function< T, T > fabsFun;
	};

} //namespace kg
#endif //KG_KERNEL_H_


