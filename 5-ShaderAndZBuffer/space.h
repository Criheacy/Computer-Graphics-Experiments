#pragma once
#ifndef SPACE_H
#define SPACE_H

#include <vector>

#include "graphics.h"

struct GraphicsLinkNode {
	class Graphics* graphics;
	GraphicsLinkNode *next;
	GraphicsLinkNode *pre;
};

class Space final
{
public:
	static Space& GetInstance();

	// forbidden to use
	Space(Space const&) = delete;
	void operator=(Space const&) = delete;

	GraphicsLinkNode* AttachGraphics(class Graphics* graphics);
	void DetachGraphics(struct GraphicsLinkNode* graphicsIndex);

	unsigned int GetSerializedVerticesArraySize();
	float* GetSerializedVerticesArrayPtr();

	unsigned int GetSerializedIndicesArraySize();
	int* GetSerializedIndicesArrayPtr();

	void LogTest();

private:
	Space();

	void UpdateGraphicsVerticesArray();

	bool graphicsUpdateFlag;
	GraphicsLinkNode* head;

	unsigned int serializedVerticesArraySize;
	float* serializedVerticesArray;

	unsigned int serializedIndicesArraySize;
	int* serializedIndicesArray;
};


#endif  //SPACE_H
