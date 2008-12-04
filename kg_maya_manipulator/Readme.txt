//This maya plugin is an illustration 
//for creating manipulators

//This code is entirely mine
//But I have used "David Gould's,
//"Complete Maya Programming" as a reference.

//-keorge@serv.net , 2002

//To comiple the code, you need to use the appropriate
//.sln file frm the 'bld' subdirectory. You must have 'MAYA_LOCATION'
//pointing to the correct Maya installation directory. Eg:
//MAYA_LOCATION=C:\Program Files\Autodesk\Maya8.5

//1)I have tested only the Debug configuration

//2)To use this 
//Put the plugin
//in your maya plugins directory.

//3)Run Maya and load the plugin

//4)source kgGenerateSlabs.mel

//5)run "kgGenerateSlabs();" in the command line
//Select any kgLocator created
//Modify/Transformation Tools/Show Transformation Tools
//Pull the associated manipulator
//Watch the slab heights changing.

