//
// Copyright (C) 2002 kgeorge
// 
// File: pluginMain.cpp
//
// Author: Maya SDK Wizard
//

#include <iostream>

#include "kgLocatorNode.h"
#include "kgLocatorManip.h"


#include <maya/MFnPlugin.h>

MStatus initializePlugin( MObject obj )
//
//	Description:
//		this method is called when the plug-in is loaded into Maya.  It 
//		registers all of the services that this plug-in provides with 
//		Maya.
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "kgLocator", "4.5", "Any");

	status = plugin.registerNode( kgLocator::typeName, kgLocator::id, &kgLocator::creator,
		&kgLocator::initialize, MPxNode::kLocatorNode );
	if (!status) {
		status.perror("registerNode");
		return status;
	}
	status = plugin.registerNode( kgLocatorManip::typeName, kgLocatorManip::typeId, &kgLocatorManip::creator,
		&kgLocatorManip::initialize, MPxNode::kManipContainer );
	if (!status) {
		status.perror("registerNode");
		return status;
	}
	return status;
}

MStatus uninitializePlugin( MObject obj)
//
//	Description:
//		this method is called when the plug-in is unloaded from Maya. It 
//		deregisters all of the services that it was providing.
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status = plugin.deregisterNode( kgLocator::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	status = plugin.deregisterNode( kgLocatorManip::typeId );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
