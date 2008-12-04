//
// Copyright (C) 2003 kgLocManip 
// 
// File: kgLocManip.cpp
//
//
// Author: Maya SDK Wizard
//
#include <iostream>

#include "kgLocManip.h"

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MGlobal.h>
#include <maya/MFnDistanceManip.h>
#include <maya/MMatrix.h>

#include <math.h>

// You MUST change this to a unique value!!!  The id is a 32bit value used
// to identify this ype of node in the binary file format.  
//
//#error change the following to a unique value and then erase this line 
const MTypeId     kgLocManip::typeId( 0x00347 );
const MString kgLocManip::typeName( "kgLocManip" );




kgLocManip::kgLocManip() {}
kgLocManip::~kgLocManip() {}

MStatus kgLocManip::compute( const MPlug& plug, MDataBlock& data )
//
//	Description:
//		This method computes the value of the given output plug based
//		on the values of the input attributes.
//
//	Arguments:
//		plug - the plug to compute
//		data - object that provides access to the attributes for this node
//
{
	MStatus returnStatus;


	return MS::kSuccess;
}

void* kgLocManip::creator()
//
//	Description:
//		this method exists to give Maya a way to create new objects
//      of this type. 
//
//	Return Value:
//		a new object of this type
//
{
	return new kgLocManip();
}

MStatus kgLocManip::initialize()
//
//	Description:
//		This method is called to create and initialize all of the attributes
//      and attribute dependencies for this node type.  This is only called 
//		once when the node type is registered with Maya.
//
//	Return Values:
//		MS::kSuccess
//		MS::kFailure
//		
{
	// This sample creates a single input float attribute and a single
	// output float attribute.
	//
	MFnNumericAttribute nAttr;
	MStatus				stat;

	stat = MPxManipContainer::initialize();
	return MS::kSuccess;

}


MStatus kgLocManip::createChildren()
// 
// Create the base manipulators that you can interactively change
//
{
MStatus stat = MStatus::kSuccess;

heightDagPath = addDistanceManip( "heightManip", "h");
MFnDistanceManip heightFn( heightDagPath );
heightFn.setStartPoint( MVector(0.0, 0.0, 0.0) );
heightFn.setDirection( MVector(0.0, 1.0, 0.0) );

return stat;
}


MStatus kgLocManip::connectToDependNode( const MObject &node )
//
// Connect the manipulators to the node's plugs. Also setup any
// callbacks for placing and displaying the manipulator components.
// 
{
MStatus stat;

targetObj = node;
MFnDependencyNode nodeFn(node); 
    
MFnDistanceManip heightFn( heightDagPath );
MPlug heightPlug = nodeFn.findPlug( "translateY", &stat );
if(!stat )
{
		stat.perror( "Unable to find \"translateY\" plug");
		return stat;
}
heightFn.connectToDistancePlug( heightPlug );

addPlugToManipConversionCallback( heightFn.startPointIndex(), 
													(plugToManipConversionCallback)&kgLocManip::centerPointCallback );
addPlugToManipConversionCallback( heightFn.directionIndex(), 
													(plugToManipConversionCallback)&kgLocManip::sideDirectionCallback ); 


// Finish up by calling the base classes connectToDependNode method
// This function must be called only once
stat = finishAddingManips();
MPxManipContainer::connectToDependNode(node);

return stat;
}



void kgLocManip::draw( M3dView & view, const MDagPath & path, 
		M3dView::DisplayStyle style, M3dView::DisplayStatus status )
{

	MPxManipContainer::draw( view, path, style, status );

	MVector nodePos = nodeTranslation();
	int nSegments = 10;
	view.beginGL();
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glTranslated( nodePos.x, nodePos.y, nodePos.z );
	glRotatef( 90.0f, 1.0f, 0.0f, 0.0f );
	glBegin(GL_LINE_LOOP);
	
	for(int i=0; i < nSegments; ++i  )
	{
		double angle = (2 * M_PI) / nSegments  * i;
		GLdouble x = cos (angle );
		GLdouble y = sin ( angle );
		glVertex2d ( x, y );
	}


	glEnd();
	glPopMatrix();
	view.endGL();


}

MVector kgLocManip::nodeTranslation() const
//
// Returns a vector that contains the node's world position
//
{
MDagPath path;
MFnDagNode dagFn( targetObj ); 
dagFn.getPath( path );
path.pop();

MFnTransform transformFn( path );
return transformFn.translation( MSpace::kWorld );
}


MVector kgLocManip::worldOffset(MVector vec) const
//
// Returns a vector that contains the node's world position
//
{
MDagPath path;
MFnDagNode dagFn( targetObj ); 
dagFn.getPath( path );
MVector axis = vec * path.inclusiveMatrix();
return axis;
}

    MManipData kgLocManip::centerPointCallback(unsigned index) const
	{

		MFnNumericData numData;
		MObject numDataObj = numData.create( MFnNumericData::k3Double );
		MVector vec = nodeTranslation();
		numData.setData( vec.x, vec.y, vec.z );
		return MManipData( numDataObj);

	}

 MManipData kgLocManip::sideDirectionCallback(unsigned index) const
 {
MFnNumericData numData;
		MObject numDataObj = numData.create( MFnNumericData::k3Double );
		MVector vec = worldOffset(MVector(0, 1, 0));
		numData.setData( vec.x, vec.y, vec.z );
		return MManipData( numDataObj);
 }