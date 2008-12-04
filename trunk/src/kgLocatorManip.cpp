//
// Copyright (C) 2002 kgeorge
// 
// File: kgLocatorManip.cpp
//
//
// Author: Maya SDK Wizard
//

#include <iostream>

#include "kgLocatorManip.h"

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MGlobal.h>
#include <maya/MFnDistanceManip.h>
#include <maya/MMatrix.h>

#include <math.h>
#include <assert.h>


const MTypeId     kgLocatorManip::typeId( 0x00347 );
const MString kgLocatorManip::typeName( "kgLocatorManip" );



//constructors, destructors
kgLocatorManip::kgLocatorManip() {}
kgLocatorManip::~kgLocatorManip() {}

//dummy compute function
MStatus kgLocatorManip::compute( const MPlug& plug, MDataBlock& data )
{
	
	return MS::kSuccess;
}

//creator fn, as registered in pluginMain.cpp
void* kgLocatorManip::creator()
{
	return new kgLocatorManip();
}


MStatus kgLocatorManip::initialize()
{
	//We dont need any new attributes
	MFnNumericAttribute nAttr;
	MStatus				stat;	
	stat = MPxManipContainer::initialize();
	return MS::kSuccess;
	
}


//create the only child manipulator.
//store its dagPath in heightDagPath.
MStatus kgLocatorManip::createChildren()
{
	MStatus stat = MStatus::kSuccess;	
	heightDagPath = addDistanceManip( "heightManip", "h");
	MFnDistanceManip heightFn( heightDagPath );	
	return stat;
}


//connect to the target object's manipulated attribute.
//After this step,
//manipulator can mainpulate the target attribute's value
MStatus kgLocatorManip::connectToDependNode( const MObject &node )
{
	MStatus stat;
	
	targetObj = node;
	MFnDependencyNode nodeFn(node); 	
	MFnDistanceManip heightFn( heightDagPath );
	MPlug heightPlug = nodeFn.findPlug( "height", &stat );
	if(!stat )
	{
		stat.perror( "Unable to find \"height\" plug");
		return stat;
	}
	heightFn.connectToDistancePlug( heightPlug );
	
	//Add callback in such a way that
	//manipulator location changes whenever the corresponding
	//target  object's location also changes
	addPlugToManipConversionCallback( heightFn.startPointIndex(), 
		(plugToManipConversionCallback)&kgLocatorManip::centerPointCallback );
	//Add callback in such a way that
	//manipulator direction changes whenever the corresponding
	//target  object's diretction also changes
	addPlugToManipConversionCallback( heightFn.directionIndex(), 
		(plugToManipConversionCallback)&kgLocatorManip::sideDirectionCallback ); 
	
	
	// Finish up by calling the base classes connectToDependNode method
	// This function must be called only once
	stat = finishAddingManips();
	//call the parent fn
	MPxManipContainer::connectToDependNode(node);
	
	return stat;
}


//mnaipulatorvwill be automatically
//drawn at the value returnsed by centerpointCall back
//added by the value retirned by sideDirectionalCallback

void kgLocatorManip::draw( M3dView & view, const MDagPath & path, 
						  M3dView::DisplayStyle style, M3dView::DisplayStatus status )
{
	
	MPxManipContainer::draw( view, path, style, status );
	//we dont need any custom drawing other than,
	//default drawing of the mainpulator
}

//utility fn that returns the target object's
//translation vector
MVector kgLocatorManip::nodeTranslation() const
{
	MDagPath path;
	MFnDagNode dagFn( targetObj ); 
	dagFn.getPath( path );
	path.pop();
	
	MFnTransform transformFn( path );
	return transformFn.translation( MSpace::kWorld );
}

//utility fn that transforms a given vector
//to the target object's inclusive matrix
MVector kgLocatorManip::worldOffset(MVector vec) const
{
	MDagPath path;
	MFnDagNode dagFn( targetObj ); 
	dagFn.getPath( path );
	MVector axis = vec * path.inclusiveMatrix();
	return axis;
}

//callback to make the manipulator
//follow any changs to the target's translation.
MManipData kgLocatorManip::centerPointCallback(unsigned index) const
{
	
	MFnNumericData numData;
	MObject numDataObj = numData.create( MFnNumericData::k3Double );
	MVector vec = nodeTranslation();
	numData.setData( vec.x, vec.y, vec.z );
	return MManipData( numDataObj);
	
}
//callback to make the manipulator
//follow any changs to the target's position or orintation
MManipData kgLocatorManip::sideDirectionCallback(unsigned index) const
{
	MFnNumericData numData;
	MObject numDataObj = numData.create( MFnNumericData::k3Double );
	MVector vec = worldOffset(MVector(0, 1, 0));
	numData.setData( vec.x, vec.y, vec.z );
	return MManipData( numDataObj);
}