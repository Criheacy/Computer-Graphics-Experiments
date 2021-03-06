#include "space.h"
#include "utils/progress_bar.h"

Space &Space::GetInstance() {
	static Space instance;
	return instance;
}

Space::Space() {
	head = nullptr;
	graphicsUpdateFlag = true;
	serializedVertexArraySize = 0;
	serializedVertexArray = nullptr;

	view = glm::mat4(1.0);
}

GraphicsLinkNode *Space::AttachGraphics(Graphics *graphics) {
	// Graphics list has changed
	graphicsUpdateFlag = true;

	if (head == nullptr) {
		head = new GraphicsLinkNode;
		head->graphics = graphics;
		head->next = head;
		head->pre = head;
		return head;
	} else {
		GraphicsLinkNode *newNode = new GraphicsLinkNode;
		newNode->graphics = graphics;
		newNode->next = head->next;
		newNode->pre = head;
		head->next->pre = newNode;
		head->next = newNode;
		return newNode;
	}
}

void Space::DetachGraphics(GraphicsLinkNode *graphicsIndex) {
	graphicsIndex->next->pre = graphicsIndex->pre;
	graphicsIndex->pre->next = graphicsIndex->next;
	if (graphicsIndex == head) head = nullptr;
	delete graphicsIndex;

	// Graphics list has changed
	graphicsUpdateFlag = true;
}

// Update serialized-vertices-array and serialized-indices-array
// Called when vertices data are required, after graphics list is changed
void Space::UpdateGraphicsVerticesArray() {
	if (head == nullptr) {
		serializedVertexArraySize = 0;
		serializedVertexArray = nullptr;
		return;
	}
	delete[] serializedVertexArray;
	delete[] serializedIndexArray;

	serializedVertexArraySize = 0;
	serializedIndexArraySize = 0;
	GraphicsLinkNode *nowNode;

	// Accumulate number of vertices in all attached graphics
	int graphicsCount = 0;
	nowNode = head;
	do {
		serializedVertexArraySize += nowNode->graphics->GetVertexCount();
		serializedIndexArraySize += nowNode->graphics->GetFaceCount();
		++graphicsCount;
		nowNode = nowNode->next;
	} while (nowNode != head);

	serializedVertexArraySize *= 3;
	serializedIndexArraySize *= 3;
	// Reallocate space for new arrays
	serializedVertexArray = new float[serializedVertexArraySize];
	serializedIndexArray = new int[serializedIndexArraySize];

	int currentGraphics = 0;
	// Enumerate each graphics and add vertex info to serialized vertices array
	nowNode = head;
	int vertexOffset = 0;
	int indexOffset = 0;
	do {
		++currentGraphics;

		ProgressBar indexProcessBar(nowNode->graphics->GetFaceCount());
		indexProcessBar.SetInterceptInterval(200);
		indexProcessBar.SetProgressTitle("Upload graphics to space ["
		                                    + std::to_string(currentGraphics) + "/"
											+ std::to_string(graphicsCount) + "]"
		                                    + " - " + "serialize indices");
		// Serialize indices first
		for (auto it = nowNode->graphics->FaceBegin(); it != nowNode->graphics->FaceEnd(); ++it) {
			// Indices number needs to add vertex-cnt to match origin vertex in space group
			auto currentFace = dynamic_cast<Face *>(*it);
			serializedIndexArray[indexOffset * 3] = currentFace->markedEdge->to->index + vertexOffset;
			serializedIndexArray[indexOffset * 3 + 1] = currentFace->markedEdge->next->to->index + vertexOffset;
			serializedIndexArray[indexOffset * 3 + 2] = currentFace->markedEdge->next->next->to->index + vertexOffset;
			// Discard face object
			// delete currentFace;
			indexOffset++;
			indexProcessBar.UpdateProgressValue(1);
			indexProcessBar.Log();
		}

		// Build vertexArray for storing vertex from iterators temporarily
		// Since vertex accessed from iterator are not order by its index, but in near-by-order instead,
		// so a sort function to reorder them by index before inserting to the serialized array is necessary
		printf("%s", std::string("START Upload graphics to space ["
		                         + std::to_string(currentGraphics) + "/"
		                         + std::to_string(graphicsCount) + "]" + " - vertex converting").c_str());
		std::cout << std::flush;
		Vertex *vertexArray = new Vertex[nowNode->graphics->GetVertexCount()];
		int vertexArrayCnt = 0;
		for (auto it = nowNode->graphics->VertexBegin(); it != nowNode->graphics->VertexEnd(); ++it) {
			vertexArray[vertexArrayCnt++] = *(dynamic_cast<Vertex *>(*it));
		}

		printf(" - COMPLETE\n");
		std::cout << std::flush;

		// Use stable sort to insure time complexity
		// Sort by index of vertex
		printf("%s", std::string("START Upload graphics to space ["
		 + std::to_string(currentGraphics) + "/"
		 + std::to_string(graphicsCount) + "]" + " - sorting").c_str());
		std::cout << std::flush;

		std::sort(vertexArray, vertexArray + vertexArrayCnt);

		printf(" - COMPLETE\n");
		std::cout << std::flush;


		ProgressBar vertexProcessBar(vertexArrayCnt);
		vertexProcessBar.SetInterceptInterval(200);
		vertexProcessBar.SetProgressTitle("Upload graphics to space ["
		                                 + std::to_string(currentGraphics) + "/"
		                                 + std::to_string(graphicsCount) + "]"
		                                 + " - " + "serialize vertices");

		for (int i = 0; i < vertexArrayCnt; i++) {
			// Serialize vector data into 3 floats
			serializedVertexArray[vertexOffset * 3] = vertexArray[i].position.x;
			serializedVertexArray[vertexOffset * 3 + 1] = vertexArray[i].position.y;
			serializedVertexArray[vertexOffset * 3 + 2] = vertexArray[i].position.z;
			vertexOffset++;
			vertexProcessBar.SetProgressValue(i + 1);
			vertexProcessBar.Log();
		}
		nowNode = nowNode->next;
	} while (nowNode != head);

	// Update complete, clear state flag
	graphicsUpdateFlag = false;
}

unsigned int Space::GetSerializedVerticesArraySize() {
	if (graphicsUpdateFlag) UpdateGraphicsVerticesArray();
	return serializedVertexArraySize;
}

float *Space::GetSerializedVerticesArrayPtr() {
	if (graphicsUpdateFlag) UpdateGraphicsVerticesArray();
	return serializedVertexArray;
}

unsigned int Space::GetSerializedIndicesArraySize() {
	if (graphicsUpdateFlag) UpdateGraphicsVerticesArray();
	return serializedIndexArraySize;
}

int *Space::GetSerializedIndicesArrayPtr() {
	if (graphicsUpdateFlag) UpdateGraphicsVerticesArray();
	return serializedIndexArray;
}

void Space::LogTest() {
	if (graphicsUpdateFlag) UpdateGraphicsVerticesArray();

	printf("========== Space Info ==========\n");
	printf(" -> vertex: %d\n", serializedVertexArraySize);
	printf(" -> vertex list:\n");
	for (unsigned int i = 0; i < serializedVertexArraySize; i += 3) {
		printf("\t%.1f, %.1f, %.1f,\n",
		       *(serializedVertexArray + i),
		       *(serializedVertexArray + i + 1),
		       *(serializedVertexArray + i + 2));
	}

	printf(" -> index: %d\n", serializedIndexArraySize);
	for (unsigned int i = 0; i < serializedIndexArraySize; i += 3) {
		printf("\t%d, %d, %d\n",
		       *(serializedIndexArray + i),
		       *(serializedIndexArray + i + 1),
		       *(serializedIndexArray + i + 2));
	}
	printf("===================================\n\n");
}
