//
// Copyright (C) 2002 kgeorge
// 
// File: kgLocatorNode.cpp
//
// Dependency Graph Node: kgLocator
//
// Author: Maya SDK Wizard
//

#include <iostream>

#include "kgLocatorNode.h"

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MGlobal.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MDistance.h>
#include <maya/MPxManipContainer.h>
#include <maya/MFnDagNode.h>

#include <assert.h>


MTypeId     kgLocator::id( 0x00346 );
MString kgLocator::typeName( "kgLocator" );

//This is the attribute, which is going
//to be manipulated by the manipulator
MObject    kgLocator::height;

//A static array
//to hold the cross shape of the 
//locator. 
//This is static because, it is a constant 
MFloatPointArray kgLocator::pts;

//constructors destructors
kgLocator::kgLocator() {}
kgLocator::~kgLocator() {}

//dummy compute function
MStatus kgLocator::compute( const MPlug& plug, MDataBlock& data )
{
	MStatus returnStatus;
	return MS::kSuccess;
}

//Createor fn as registered in pluginMain.cpp
void* kgLocator::creator()
{

	return new kgLocator();
}

MStatus kgLocator::initialize()

{
	

	MFnUnitAttribute  unitFn;
	MStatus stat;
	
	//Let us inherit the attributes,
	//in case we need it
	MString parentClass("MPxLocatorNode");
	stat = inheritAttributesFrom( parentClass );
	
	//Create the height attribute
	height = unitFn.create( "height", "h", MFnUnitAttribute::kDistance );
	unitFn.setDefault( MDistance( 2.0,  MDistance::uiUnit() ) );
	unitFn.setMin( MDistance( 0.0,  MDistance::uiUnit() ) );	
	stat = addAttribute( height );
	if (!stat )
	{
		stat.perror( "Unable to add \"height\" attribute");
		return stat;
		
	}


	//generates points for drwing the locator
	getPoints( pts );
	
	
	// Notify Maya that there is an associated manipulator for this particular type of node
	MPxManipContainer::addToManipConnectTable( const_cast<MTypeId &>( id ) );
	
	return MS::kSuccess;
	
}




void kgLocator::draw( M3dView & view, const MDagPath & path, 
					 M3dView::DisplayStyle style, M3dView::DisplayStatus status )
{
	MStatus stat;

	//First get the value of the height attribute
	//of the assocuated locator
	MObject thisNode = thisMObject();
	MFnDagNode dagFn( thisNode  );  	
	MPlug heightPlug = dagFn.findPlug( height, &stat );
	float heightValue;
	heightPlug.getValue( heightValue );
		
	view.beginGL();
	glPushAttrib( GL_CURRENT_BIT );
	
	
	glBegin( GL_LINES );
	//Draw a cross
	for(unsigned int i=0; i < pts.length(); i+= 2 )
	{
		glVertex3f(pts[i].x, pts[i].y, pts[i].z );
		glVertex3f(pts[i+1].x, pts[i+1].y, pts[i+1].z );
	}
	//And a vertical spindle
	//of the same height as 
	//the height attribute
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, heightValue, 0.0f );
	
	glEnd();
	
	glPopAttrib();
	view.endGL();
}

bool kgLocator::isBounded() const
{
	return true;
}

MBoundingBox kgLocator::boundingBox() const
{
	MBoundingBox bbox;
	MPoint pt;
	MStatus stat;
	
	MObject thisNode = thisMObject();
	MFnDagNode dagFn( thisNode  );  
	
	MPlug heightPlug = dagFn.findPlug( height, &stat );
	float heightValue;
	heightPlug.getValue( heightValue );
	
	for(unsigned int i=0; i < pts.length(); ++i )
	{
		pt.x = pts[i].x; pt.y = pts[i].y; pt.z = pts[i].z;
		bbox.expand( pt );
	}
	//account for the vertical spindle
	pt.x=0.0; pt.y = 0.0f; pt.z = 0.0f;
	bbox.expand( pt );
	pt.x=0.0; pt.y = heightValue; pt.z = 0.0f;
	bbox.expand( pt );

	return bbox;
}


bool kgLocator::getPoints( MFloatPointArray &pts ) 
{
	
	MFloatPoint pt;
	
	pt.x = -0.5f; pt.y = 0.0f; pt.z = 0.0f;
	pts.append( pt );
	pt.x = 0.5f; pt.y = 0.0f; pt.z = 0.0f;
	pts.append( pt );
	pt.x = 0.0f; pt.y = 0.0f; pt.z = -0.5f;
	pts.append( pt );
	pt.x = 0.0f; pt.y = 0.0f; pt.z = 0.5f;
	pts.append( pt );
	assert( pts.length() == 4 );
	return true;
}