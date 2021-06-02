#include "graphics_component.h"

bool MapComponentComparer::operator()(const GraphicsComponent *a, const GraphicsComponent *b) {
	return (*a) < (*b);
}

Vertex::Vertex(int index, glm::vec3 position, Edge *headEdge) {
	this->index = index;
	this->position = position;
	this->headEdge = headEdge;
}

bool Vertex::operator==(const GraphicsComponent &rhs) const {
	if (typeid(rhs) == typeid(*this))
	{
		return this->index == dynamic_cast<const Vertex&>(rhs).index;
	}
	return false;
}

bool Vertex::operator<(const GraphicsComponent &rhs) const {
	if (typeid(rhs) == typeid(*this))
	{
		return this->index < dynamic_cast<const Vertex&>(rhs).index;
	}
	return false;
}

std::vector<GraphicsComponent*> Vertex::GetAdjacentComponent() {
	std::vector<GraphicsComponent*> res;
	for (Edge* edge = this->headEdge; edge != nullptr; edge = edge->next)
	{
		res.push_back(edge->to);
	}
	return res;
}

Edge::Edge(Vertex *from, Vertex *to, Edge *opposite, Edge *next) {
	this->from = from;
	this->to = to;
	this->opposite = opposite;
	this->next = next;
}

bool Edge::operator==(const GraphicsComponent &rhs) const {
	if (typeid(rhs) == typeid(*this))
	{
		return (this->from == dynamic_cast<const Edge&>(rhs).from)
		       && (this->to == dynamic_cast<const Edge&>(rhs).to);
	}
	return false;
}

bool Edge::operator<(const GraphicsComponent &rhs) const {
	if (typeid(rhs) == typeid(*this))
	{
		if (this->from != dynamic_cast<const Edge&>(rhs).from)
		{
			return this->from < dynamic_cast<const Edge&>(rhs).from;
		}
		else
		{
			return this->to < dynamic_cast<const Edge&>(rhs).to;
		}
	}
	return false;
}

std::vector<GraphicsComponent*> Edge::GetAdjacentComponent() {
	std::vector<GraphicsComponent*> res;
	res.push_back(this->next);
	res.push_back(this->opposite);
	return res;
}

std::vector<GraphicsComponent *> Face::GetAdjacentComponent() {
	std::vector<GraphicsComponent*> res;
	res.push_back(new Face(this->markedEdge->opposite));
	for (Edge* edge = this->markedEdge->next; edge != this->markedEdge; edge = edge->next)
	{
		res.push_back(new Face(edge->opposite));
	}
	return res;
}

Face::Face(Edge *markedEdge) {
	this->markedEdge = markedEdge;
}

bool Face::operator==(const GraphicsComponent &rhs) const {
	if (typeid(rhs) == typeid(*this))
	{
		const Face& faceRhs = dynamic_cast<const Face&>(rhs);
		if (*(this->markedEdge) == *(faceRhs.markedEdge))
		{
			return true;
		}
		for (Edge* edge = faceRhs.markedEdge->next; edge != faceRhs.markedEdge; edge = edge->next)
		{
			if (*(this->markedEdge) == *edge)
			{
				return true;
			}
		}
		return false;
	}
	return false;
}

bool Face::operator<(const GraphicsComponent &rhs) const {
	if (typeid(rhs) == typeid(*this))
	{
		if (this->markedEdge->from != dynamic_cast<const Face&>(rhs).markedEdge->from)
		{
			return this->markedEdge->from < dynamic_cast<const Face&>(rhs).markedEdge->from;
		}
		else
		{
			return this->markedEdge->to < dynamic_cast<const Face&>(rhs).markedEdge->to;
		}
	}
	return false;
}

/******************** Graphics Iterator Implementation ********************/

GraphicsIterator::GraphicsIterator(GraphicsComponent *beginComponent) {
	if (beginComponent != nullptr) {
		nextQueue.push(beginComponent);
		UpdateComponent(beginComponent);
	}
}

GraphicsIterator &GraphicsIterator::operator=(const GraphicsIterator &rhs) {
	this->inQueueMap = rhs.inQueueMap;
	this->nextQueue = rhs.nextQueue;    // deep copy the queue
}

bool GraphicsIterator::operator==(const GraphicsIterator &rhs) const {
	if (nextQueue.empty() && rhs.nextQueue.empty())
	{
		return true;
	}
	return false;
}

bool GraphicsIterator::operator!=(const GraphicsIterator &rhs) const {
	return !(*this == rhs);
}

GraphicsIterator::GraphicsIterator(const GraphicsIterator &rhs) {
	// Use assignment operator
	*this = rhs;
}

GraphicsComponent* GraphicsIterator::operator*() const {
	if (nextQueue.empty())
	{
		throw "End iterator cannot be dereferenced";
	}
	return nextQueue.front();
}

GraphicsIterator &GraphicsIterator::operator++() {
    UpdateComponent(nextQueue.front());
	nextQueue.pop();
	return *this;
}

const GraphicsIterator GraphicsIterator::end = GraphicsIterator(nullptr);

void GraphicsIterator::UpdateComponent(GraphicsComponent *component) {
	 std::vector<GraphicsComponent*> adjacentComponents = component->GetAdjacentComponent();
	 for (auto adjacentComponent : adjacentComponents)
	 {
		 if (inQueueMap[adjacentComponent])
		 {
			 inQueueMap[adjacentComponent] = true;
			 nextQueue.push(adjacentComponent);
		 }
	 }
}
