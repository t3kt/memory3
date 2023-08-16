/* Shared Use License: This file is owned by Derivative Inc. (Derivative)
* and can only be used, and/or modified for use, in conjunction with
* Derivative's TouchDesigner software, and only if you are a licensee who has
* accepted Derivative's TouchDesigner license or assignment agreement
* (which also govern the use of this file). You may share or redistribute
* a modified version of this file provided the following conditions are met:
*
* 1. The shared file or redistribution must retain the information set out
* above and this list of conditions.
* 2. Derivative's name (Derivative Inc.) or its trademarks may not be used
* to endorse or promote products derived from this file without specific
* prior written permission from Derivative.
*/

/*
* Produced by:
*
* 				Derivative Inc
*				401 Richmond Street West, Unit 386
*				Toronto, Ontario
*				Canada   M5V 3A8
*				416-591-3555
*
* NAME:				SOP_CPlusPlusBase.h
*
*
*	Do not edit this file directly!
*	Make a subclass of SOP_CPlusPlusBase instead, and add your own
*	data/functions.

*	Derivative Developers:: Make sure the virtual function order
*	stays the same, otherwise changes won't be backwards compatible
*/
//#pragma once

#ifndef __SOP_CPlusPlusBase__
#define __SOP_CPlusPlusBase__

#include <assert.h>
#include "CPlusPlus_Common.h"

namespace TD
{

class SOP_CPlusPlusBase;

#pragma pack(push, 8)

// Define for the current API version that this sample code is made for.
// To upgrade to a newer version, replace the files
// SOP_CPlusPlusBase.h
// CPlusPlus_Common.h
// from the samples folder in a newer TouchDesigner installation.
// You may need to upgrade your plugin code in that case, to match
// the new API requirements
const int SOPCPlusPlusAPIVersion = 3;

class SOP_PluginInfo
{
public:
	int32_t			apiVersion = 0;

	int32_t			reserved[100];

	// Information used to describe this plugin as a custom OP.
	OP_CustomOPInfo	customOPInfo;

	int32_t			reserved2[20];
};

enum class SOP_Winding : int32_t
{
	// Clockwise vertex winding. Don't use, for legacy plugins.
	LegacyCW = 0,

	// Counter-clockwise vertex winding. All new plugins should work this way.
	CCW,
};

class SOP_GeneralInfo
{
public:
	// Set this to true if you want the SOP to cook every frame, even
	// if none of it's inputs/parameters are changing.
	// This is generally useful for cases where the node is outputting to
	// something external to TouchDesigner, such as a network socket or device.
	// It ensures the node cooks every if nothing inside the network is using/viewing
	// the output of this node.
	// Important:
	// If the node may not be viewed/used by other nodes in the file,
	// such as a TCP network output node that isn't viewed in perform mode,
	// you should set cookOnStart = true in OP_CustomOPInfo.
	// That will ensure cooking is kick-started for this node.
	// Note that this fix only works for Custom Operators, not
	// cases where the .dll is loaded into CPlusPlus SOP.
	// DEFAULT: false
	bool cookEveryFrame;

	// Set this to true if you want the SOP to cook every frame, but only
	// if someone asks for it to cook. So if nobody is using the output from
	// the SOP, it won't cook. This is difereent from 'cookEveryFrame'
	// since that will cause it to cook every frame no matter what.
	// Defaults to false.
	// DEFAULT: false
	bool	cookEveryFrameIfAsked;


	// Set this flag to true to load the geometry to the GPU for faster updating
	// Note that if you set this flag to 'true', then the function executeVBO() is being called
	// instead of execute() function. 
	// If this flag is set to false, then execute() function is called.
	// When directToGPU is true, the data is not available to be used in SOPs,
	// except to be rendered with a Render TOP.
	// DEFAULT: false.
	bool	directToGPU;


	// Legacy code used clockwise vertex winding, but counter-clockwise is more consistent
	// with TouchDesigner. Older plugins will have this set to LegacyCW,
	// but newer ones should set it to CCW, as the sample code does.
	SOP_Winding			winding;

private:
	int32_t	reserved[19];
};



// The buffer object mode. 
// This helps the graphics driver determine where to keep the data
// for beter performance.
enum class VBOBufferMode : int32_t
{
	// The data will be modified once or rarely and used many times.
	Static = 0,

	// The data will be modified repeatedly and used many times.
	Dynamic,
};

// an enumerator to specify the group type
enum class SOP_GroupType
{
	Point = 0,
	Primitive,
};


// This class is used to create geometry on the CPU, to be used in SOP networks.
// NOTE: set 'directToGPU' flag from SOP_GeneralInfo class to false.
class SOP_Output
{
public:

	SOP_Output()
	{
	}

	~SOP_Output()
	{
	}

	// Add a single point at the given position.
	// Returns the point's index.
	virtual int32_t	addPoint(const Position& pos) = 0;

	// Add multiple points at specified positions.
	// 'numPoints' is the number of points to be added. 
	virtual bool	addPoints(const Position* pos, int32_t numPoints) = 0;

	// Returns the number of added points at the time of query
	virtual	int32_t	getNumPoints() = 0;

	// Set the normal vector for the point with the 'pointIdx'.
	// The point must already exist by via calling addPoints() or addPoint().
	virtual bool	setNormal(const Vector& n, int32_t pointIdx) = 0;

	// Set the normal vectors for existing points.
	// Note that has been the points must be already added by calling addPoints() or addPoint().
	// The startPointIdx indicates the start index of the points to set normals for.
	virtual bool	setNormals(const Vector* n, int32_t numPoints, int32_t startPointIdx) = 0;

	// Returns true if the normal has been set for this geometry.
	virtual bool	hasNormal() = 0;

	// Set the color value with Color (i.e. r,g,b,a) for the point with 'pointIdx' index.
	// The point must already exist by via calling addPoints() or addPoint().
	virtual bool	setColor(const Color& c, int32_t pointIdx) = 0;

	// Set the colors for points that are already added.
	// The startPointIdx indicates the start index of the points to set colors for.
	virtual bool	setColors(const Color* colors, int32_t numPoints, int32_t startPointIdx) = 0;

	// Returns true if the color has been set for this geometry.
	virtual bool	hasColor() = 0;

	// Set texture coordinate data for existing points.
	// The numLayers is the texcoord size and can be from 1 up to 8 for texture layers
	// The first numLayers used will be used as the max number of layers for all future calls for this cook.
	// The pointIdx specifies the point index with the texture coords
	virtual bool	setTexCoord(const TexCoord* tex, int32_t numLayers, int32_t pointIdx) = 0;

	// Set texture coordinate data for existing points.
	// The numLayers is the texCoord size and can be from 1 up to 8 for texCoord layers.
	// The first numLayers used will be used as the max number of layers for all future calls for this cook.
	// The startPointIdx indicates the start index of the points to set texCoord for.
	virtual	bool	setTexCoords(const TexCoord* t, int32_t numPoints, int32_t numLayers, int32_t startPointIdx) = 0;

	// Returns true if the texCoord/textures has been set for this geometry.
	virtual bool	hasTexCoord() = 0;

	// Returns the number of texcoord layers
	virtual int32_t getNumTexCoordLayers() = 0;

	// Set the custom attribute with SOP_CustomAttribData (must have set its name, number of components, and its type)
	// The data param must hold the data for the custom attribute.
	// E.g a custom atrrib with 4 components for each point should holds 4*numPoints values for its data.
	virtual bool	setCustomAttribute(const SOP_CustomAttribData* cu, int32_t numPoints) = 0;

	// Returns true if the custom attributes has been set for this geometry.
	virtual bool	hasCustomAttibutes() = 0;

	// Add a triangle using the points at the given 3 indices.
	virtual bool	addTriangle(int32_t ptIdx1, int32_t ptIdx2, int32_t ptIdx3) = 0;

	// Add multiple triangles using an array of point's indices.
	// The size param represents the number of triangles to be added.
	// 'indices' must contain at least 3 * size elements.
	virtual bool	addTriangles(const int32_t *indices, int32_t size) = 0;

	// add particle systems from the points that has been already added. The points can have colors, normals and custom attribs.
	// the startIndex param is the staring index of the points from particle system.
	virtual bool	addParticleSystem(int32_t numParticles, int32_t startIndex) = 0;

	// Add line strip from the points that has been already added. The points can have colors, normals and custom attribs.
	// the 'indices' contains the indices of vertices, and 'size' is the number of indices for the line strip
	virtual bool	addLine(const int32_t *indices, int32_t size) = 0;

	// Add line strips from the points that has been already added.The points can have colors, normals and custom attribs.
	// the 'indices' contains the indices of vertices, 'sizeOfEachLine' contains the number of vertices for each line,
	// 'numOfLines' specifies the number of lines to be drawn.
	// Note that the number of elements in sizeOfEachLine must be equal to numOfLines.
	virtual bool	addLines(const int32_t *indices, int32_t* sizeOfEachLine, int32_t numOfLines) = 0;

	// Returns the number of added primitives at the time of query. Currently it is either the number of triangles or particles.
	virtual int32_t	getNumPrimitives() = 0;

	// Set the bounding box for the whole geometry.
	// Setting the bounding box helps to have exact homing on the viewer.
	// You may set this value at each frame for non static geometries that are translating constantly.
	virtual bool	setBoundingBox(const BoundingBox& bbox) = 0;

	// Add a group with input type and name.
	// Returns false if a group with this name already exists.
	virtual bool	addGroup(const SOP_GroupType& type, const char* name) = 0;

	// Destroy a group with input type and name.
	// Returns false if a group with this name for the specified type does not exists.
	virtual bool	destroyGroup(const SOP_GroupType& type, const char* name) = 0;

	// Add a point with its index to an already existing group with SOP_GroupType::Point type.
	// Returns false if a point group with this name does not exists Or
	// if a point with that index does not exists.
	virtual bool	addPointToGroup(int index, const char* name) = 0;

	// Add a primitive with its index to an already existing group with SOP_GroupType::Primitive type.
	// Returns false if a primitive group with this name does not exists Or
	// if a pritimive with that index does not exists.
	virtual bool	addPrimToGroup(int index, const char* name) = 0;

	// Add a point/prim index to an already defined group.
	// Returns false if a primitive group with this name does not exists Or
	// if a pritimive/point with that index does not exists.
	virtual bool	addToGroup(int index, const SOP_GroupType& type, const char* name) = 0;

	// Add a point with its index to an already existing group with SOP_GroupType::Point type.
	// Returns false if a point group with this name does not exists Or
	// if a point with that index does not exists.
	virtual bool	discardFromPointGroup(int index, const char* name) = 0;

	// Add a primitive with its index to an already existing group with SOP_GroupType::Primitive type.
	// Returns false if a primitive group with this name does not exists Or
	// if a pritimive with that index does not exists.
	virtual bool	discardFromPrimGroup(int index, const char* name) = 0;

	// Remove a point/prim index from an already defined group.
	// Returns false if a primitive group with this name does not exists Or
	// if a pritimive/point with that index does not exists.
	virtual bool	discardFromGroup(int index, const SOP_GroupType& type, const char* name) = 0;

private:

	int32_t		reserved[20];
};


// This class is used to load geometry directly onto the GPU. The geometry can be used for rendering, but not SOP networks.
// NOTE: set 'directToGPU' flag from SOP_GeneralInfo class to true.
class SOP_VBOOutput
{

public:

	SOP_VBOOutput()
	{
	}

	~SOP_VBOOutput()
	{
	}

	// enable/set the normal, color, texcoord, if the geometry contains this information
	virtual void		enableNormal() = 0;

	virtual void		enableColor() = 0;

	virtual void		enableTexCoord(int32_t numLayers = 0) = 0;

	// Returns true if the normal, color, texcoord, or custom attributes has been set for this geometry.
	virtual bool		hasNormal() = 0;

	virtual bool		hasColor() = 0;

	virtual bool		hasTexCoord() = 0;

	virtual bool		hasCustomAttibutes() = 0;

	// Add the custom attribute with SOP_CustomAttribInfo (must have set its name, number of components, and its type)
	virtual bool		addCustomAttribute(const SOP_CustomAttribInfo& cu) = 0;

	// Allocates and setup VBO buffers. 
	// Call this fucntion before adding any points, colors or normals,
	// but after enableNormal(), enableColor(), addCustomAttribute().
	// 'numVertices' is how much memory to allocate for positions/normals etc.
	// 'numIndices' is how much memory to allocate for indices that are used
	// to build primitives.
	virtual void		allocVBO(int32_t numVertices, int32_t numIndices, VBOBufferMode mode) = 0;

	// Returns the start of an array of Positions that should be filled.
	// The length of this array is numVertices.
	virtual Position*	getPos() = 0;

	// Returns the start of an array of Vectors that should be filled.
	// The length of this array is numVertices.
	virtual Vector*		getNormals() = 0;

	// Returns the start of an array of Colors that should be filled.
	// The length of this array is numVertices.
	virtual Color*		getColors() = 0;

	// Returns the start of an array of TexCoords that should be filled.
	// The length of this array is numVertices.
	virtual TexCoord*	getTexCoords() = 0;

	// Returns the number of texcoord layers which has been already set
	// by enableTexCoord() call.
	virtual int32_t		getNumTexCoordLayers() = 0;

	// Returns an array of indices (of vertices) that should be filled. The indices
	// for all the triangles must be set as their initial values is undefined.
	// It is used to create 'numTriangles' triangles.
	// Length of the returned array is numTriangles * 3.
	virtual int32_t*	addTriangles(int32_t numTriangles) = 0;

	// Returns an array of indices (of vertices) that should be filled. The indices
	// for particles must be set as their initial values is undefined.
	// It is used to create 'numParticles' particles.
	// Length of the returned array is numParticles.
	virtual int32_t*	addParticleSystem(int32_t numParticles) = 0;

	// Returns an array of indices (of vertices) that should be filled.
	// It is used to create line strip with numIndices.
	// Length of the returned array is numIndices.
	virtual int32_t*	addLines(int32_t numIndices) = 0;

	// Fills SOP_CustomAttribData with data for the given attribute 'name'.
	// The intData or floatData member contains a pointer to the member that should
	// be filled.
	// Returns false is case of null arguments, or invalid name.
	virtual bool		getCustomAttribute(SOP_CustomAttribData* cu, const char* name) = 0;

	// Finish updating the VBO buffers.
	// After you are done with the VBO buffers, make sure to call this function
	// Note: this function must be the last function to be called 
	virtual void		updateComplete() = 0;

	// Set the bounding box for the whole geometry.
	// We recommned to set the bounding box in GPU direct mode for exact homing.
	// You may set this value at each frame for non static geometries that are translating constantly.
	virtual bool		setBoundingBox(const BoundingBox& bbox) = 0;

private:

};



/*** DO NOT EDIT THIS CLASS, MAKE A SUBCLASS OF IT INSTEAD ***/
class SOP_CPlusPlusBase
{

protected:

	SOP_CPlusPlusBase()
	{
	}

public:

	virtual
	~SOP_CPlusPlusBase()
	{
	}

	// BEGIN PUBLIC INTERFACE

	// Some general settings can be assigned here (if you ovierride it)
	// The OP_Inputs* provides the access to the custom parameters
	// before the call to the execute/VBO() functions.
	virtual void
	getGeneralInfo(SOP_GeneralInfo*, const OP_Inputs*, void* reserved1)
	{
	}


	// Add geometry data such as points, normals, colors, and triangles
	// or particles and etc. obtained from your desired algorithm or external files.
	// If the "directToGPU" flag is set to false, this function is being called
	// instead of executeVBO().
	// See the OP_Inputs class definition for more details on it's contents
	virtual void	execute(SOP_Output*, const OP_Inputs*, void* reserved1) = 0;

	// For direct GPU loading (i.e. "directToGPU" is set to true) this function is being called
	// instead of execute().
	// Fill the VBO buffers with the geometry data, obtained from your desired algorithm or files,
	// such as points, normals, colors, texcoord, triangles, and etc.
	virtual void	executeVBO(SOP_VBOOutput*, const OP_Inputs*, void* reserved1) = 0;


	// Override these methods if you want to output values to the Info CHOP/DAT
	// returning 0 means you dont plan to output any Info CHOP channels
	virtual int32_t
	getNumInfoCHOPChans(void *reserved1)
	{
		return 0;
	}

	// Specify the name and value for CHOP 'index',
	// by assigning something to 'name' and 'value' members of the
	// OP_InfoCHOPChan class pointer that is passed (it points
	// to a valid instance of the class already.
	// the 'name' pointer will initially point to nullptr
	// you must allocate memory or assign a constant string
	// to it.
	virtual void
	getInfoCHOPChan(int32_t index, OP_InfoCHOPChan* chan, void *reserved1)
	{
	}


	// Return false if you arn't returning data for an Info DAT
	// Return true if you are.
	// Set the members of the CHOP_InfoDATSize class to specify
	// the dimensions of the Info DAT
	virtual bool
	getInfoDATSize(OP_InfoDATSize* infoSize, void* reserved1)
	{
		return false;
	}


	// You are asked to assign values to the Info DAT 1 row or column at a time
	// The 'byColumn' variable in 'getInfoDATSize' is how you specify
	// if it is by column or by row.
	// 'index' is the row/column index
	// 'nEntries' is the number of entries in the row/column
	// Strings should be UTF-8 encoded.
	virtual void
	getInfoDATEntries(int32_t index, int32_t nEntries, 
						OP_InfoDATEntries* entries, void* reserved1)
	{
	}


	// You can use this function to put the node into a warning state
	// with the returned string as the message.
	virtual void
	getWarningString(OP_String *warning, void *reserved1)
	{
	}

	// You can use this function to put the node into a error state
	// with the returned string as the message.
	virtual void
	getErrorString(OP_String *error, void *reserved1)
	{
	}

	// Use this function to return some text that will show up in the
	// info popup (when you middle click on a node)
	virtual void
	getInfoPopupString(OP_String *info, void *reserved1)
	{
	}


	// Override these methods if you want to define specfic parameters
	virtual void
	setupParameters(OP_ParameterManager* manager, void* reserved1)
	{
	}


	// This is called whenever a pulse parameter is pressed
	virtual void
	pulsePressed(const char* name, void* reserved1)
	{
	}

	// END PUBLIC INTERFACE


private:

	// Reserved for future features
	virtual int32_t	reservedFunc6() { return 0; }
	virtual int32_t	reservedFunc7() { return 0; }
	virtual int32_t	reservedFunc8() { return 0; }
	virtual int32_t	reservedFunc9() { return 0; }
	virtual int32_t	reservedFunc10() { return 0; }
	virtual int32_t	reservedFunc11() { return 0; }
	virtual int32_t	reservedFunc12() { return 0; }
	virtual int32_t	reservedFunc13() { return 0; }
	virtual int32_t	reservedFunc14() { return 0; }
	virtual int32_t	reservedFunc15() { return 0; }
	virtual int32_t	reservedFunc16() { return 0; }
	virtual int32_t	reservedFunc17() { return 0; }
	virtual int32_t	reservedFunc18() { return 0; }
	virtual int32_t	reservedFunc19() { return 0; }
	virtual int32_t	reservedFunc20() { return 0; }

	int32_t			reserved[400];

};

#pragma pack(pop)

static_assert(offsetof(SOP_PluginInfo, apiVersion) == 0, "Incorrect Alignment");
static_assert(offsetof(SOP_PluginInfo, customOPInfo) == 408, "Incorrect Alignment");
static_assert(sizeof(SOP_PluginInfo) == 944, "Incorrect Size");

static_assert(offsetof(SOP_GeneralInfo, cookEveryFrame) == 0, "Incorrect Alignment");
static_assert(offsetof(SOP_GeneralInfo, cookEveryFrameIfAsked) == 1, "Incorrect Alignment");
static_assert(offsetof(SOP_GeneralInfo, directToGPU) == 2, "Incorrect Alignment");
static_assert(offsetof(SOP_GeneralInfo, winding) == 4, "Incorrect Alignment");
static_assert(sizeof(SOP_GeneralInfo) == 84, "Incorrect Size");

};	// namespace TD

#endif