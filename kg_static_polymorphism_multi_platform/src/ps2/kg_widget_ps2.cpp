#include "kg_widget_ps2.h"


using namespace std;

namespace  kg_static_polymorphism_multi_platform
{
	template<>int DoSomethingPlatformSpecific<KGPLATFORM_PS2>()
	{

		cout << "Doing something platform specific for ps2" << endl;
		return 0;
	}

	template < >PlatformWidget<KGPLATFORM_PS2>::PlatformWidget()
	{
		cout << "platform specific construction of Widget for ps2" << endl;
	}

	template < >PlatformWidget<KGPLATFORM_PS2>::~PlatformWidget()
	{
		cout << "platform specific destruction of Widget for ps2" << endl;
	}

    template < >
		void PlatformWidget<KGPLATFORM_PS2>::MethodWhichRequirePlatformSpecificImplementation()
	{
		cout << "Doing -MethodWhichRequirePlatformSpecificImplementation for ps2" << endl;
	}


} //namespace  kg_static_polymorphism_multi_platform