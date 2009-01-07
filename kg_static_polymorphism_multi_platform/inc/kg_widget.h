#if !defined(KG_WIDGET_H_)
#define KG_WIDGET_H_

#if defined(PC_PLATFORM)
#define KGPLATFORM_PC  1
#define KGPLATFORM_PS2 0
#elif defined(PS2_PLATFORM)
#define KGPLATFORM_PC  0
#define KGPLATFORM_PS2 1
#endif

#include <string>
#include <iostream>


namespace  kg_static_polymorphism_multi_platform
{

	template< int iVal > int DoSomethingPlatformSpecific();

  /**

  You can capture the diferent platform based variants of widgets
    in a single class called 'PlarformWidget'  which is  parameterized by a plaform template integer.

	If the  build configuration is  PS2_Debug or PS2_Release, then KGPLATFORM_PS2 will be 1 and
	KGPLATFORM_PC will be zero. So the following typedef statement,
    synonimizes 'Widget' with 'PlatformWidget<KGPLATFORM_PS2>'.

	typedef PlatformWidget<KGPLATFORM_PS2> Widget


  */

	template < int T>
	class PlatformWidget
	{
	public:
		PlatformWidget();
		~PlatformWidget();

		void MethodWhichCanBeCommonForAllPlatforms()
		{
			std::cout << "Doing -MethodWhichCanBeCommonForAllPlatforms" << std::endl;
		}
		void MethodWhichRequirePlatformSpecificImplementation();
	};

	typedef PlatformWidget<1> Widget;
}//namespace  kg_static_polymorphism_multi_platform

#endif //KG_WIDGET_H_

