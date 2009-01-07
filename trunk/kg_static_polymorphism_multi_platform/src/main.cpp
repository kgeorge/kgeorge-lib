#include "kg_widget.h"


using namespace  kg_static_polymorphism_multi_platform;
using namespace std;




int main( int argc, char *argv )
{
	DoSomethingPlatformSpecific<1>();
	{   
		Widget thisWidget;
		thisWidget.MethodWhichCanBeCommonForAllPlatforms();
		thisWidget.MethodWhichRequirePlatformSpecificImplementation();
	}
	cout << flush;
	return 0;

}