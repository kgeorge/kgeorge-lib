#include "kg_widget_pc.h"


using namespace std;
namespace  kg_static_polymorphism_multi_platform
{
	template<>int DoSomethingPlatformSpecific<KGPLATFORM_PC>()
	{

		cout << "Doing something platform specific for pc" << endl;
		return 0;
	}

	template < >PlatformWidget<KGPLATFORM_PC>::PlatformWidget()
	{
		cout << "platform specific construction of Widget for pc" << endl;
	}

	template < >PlatformWidget<KGPLATFORM_PC>::~PlatformWidget()
	{
		cout << "platform specific destruction of Widget for pc" << endl;
	}

    template < >
		void PlatformWidget<KGPLATFORM_PC>::MethodWhichRequirePlatformSpecificImplementation()
	{
		cout << "Doing -MethodWhichRequirePlatformSpecificImplementation for pc" << endl;
	}


} //namespace  kg_static_polymorphism_multi_platform

