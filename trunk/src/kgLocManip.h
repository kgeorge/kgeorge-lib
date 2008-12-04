#ifndef _kgLocManip
#define _kgLocManip
//
// Copyright (C) 2003 Manipular Container for kgLocator
// 
// File: kgLocManip.h
//
//
// Author: Maya SDK Wizard
//

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 
#include <maya/MPxManipContainer.h>
#include <maya/MManipData.h>

 
class kgLocManip : public MPxManipContainer
{
public:
	kgLocManip();
	virtual				~kgLocManip(); 
	
	virtual MStatus createChildren();
	virtual MStatus connectToDependNode(const MObject & node);
    virtual void draw(M3dView & view, const MDagPath & path, M3dView::DisplayStyle style,M3dView::DisplayStatus status); 
	
	
	
	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
	
	static  void*		creator();
	static  MStatus		initialize();
	
    MManipData sideDirectionCallback(unsigned index) const;
	
    MVector nodeTranslation() const;
    MVector worldOffset(MVector vect) const;
	
    static const MTypeId typeId;
    static const MString typeName;
	
    MManipData centerPointCallback(unsigned index) const;
public:
	// Paths to child manipulators
    MDagPath heightDagPath;
	
	
    // Object that the manipulator will be operating on
    MObject targetObj;
	
	
};

#endif
