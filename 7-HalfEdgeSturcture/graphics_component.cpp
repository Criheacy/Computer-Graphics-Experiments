#include "graphics_component.h"

GraphicsComponent::~GraphicsComponent() = default;

bool GraphicsComponent::operator==(const GraphicsComponent &rhs) const {
	return (!(*this < rhs)) && (!(rhs < *this));
}

bool GraphicsComponent::operator!=(const GraphicsComponent &rhs) const {
	return !(*this == rhs);
}

bool GraphicsComponent::operator<=(const GraphicsComponent &rhs) const {
	return (*this == rhs || *this < rhs);
}

bool GraphicsComponent::operator>=(const GraphicsComponent &rhs) const {
	return !(*this < rhs);
}

bool GraphicsComponent::operator>(const GraphicsComponent &rhs) const {
	return (*this >= rhs && *this != rhs);
}

bool MapComponentComparer::operator()(const GraphicsComponent *a, const GraphicsComponent *b) {
	return (*a) < (*b);
}

Vertex::Vertex(int index, glm::vec3 position, Edge *headEdge) {
	this->index = index;
	this->position = position;
	this->headEdge = headEdge;
}

Vertex::~Vertex() {
	// Delete all edges from vertex
	Edge* currentEdge = headEdge;
	Edge* nextEdge = nullptr;
	while (currentEdge != nullptr) {
		nextEdge = currentEdge->follow;
		// delete currentEdge;
		currentEdge = nextEdge;
	}
}

void Vertex::AddEdge(Edge *newEdge) {
	newEdge->follow = headEdge;
	headEdge = newEdge;
}

void Vertex::RemoveEdge(Edge *edgeToRemove) {
	if (edgeToRemove == headEdge) {
		headEdge = edgeToRemove->follow;
	} else {
		Edge *prevEdge = headEdge;
		for (Edge *edge = headEdge->follow; edge != nullptr; edge = edge->follow) {
			if (edge == edgeToRemove) {
				prevEdge->next = edge->next;
				break;
			}
			prevEdge = edge;
		}
	}
	edgeToRemove->next = nullptr;
}

Edge *Vertex::GetEdgeTo(Vertex *toVertex) const {
	if (headEdge == nullptr) {
		return nullptr;
	}
	if (headEdge->to == toVertex) {
		return headEdge;
	}
	for (Edge *edge = headEdge->follow; edge != nullptr; edge = edge->follow) {
		if (edge->to == toVertex) {
			return edge;
		}
	}
	return nullptr;
}

bool Vertex::operator<(const GraphicsComponent &rhs) const {
	if (typeid(rhs) == typeid(*this)) {
		return this->index < dynamic_cast<const Vertex &>(rhs).index;
	}
	return false;
}

std::vector<GraphicsComponent *> Vertex::GetAdjacentComponent() {
	std::vector<GraphicsComponent *> res;
	for (Edge *edge = this->headEdge; edge != nullptr; edge = edge->follow) {
		res.push_back(edge->to);
	}
	return res;
}

Edge::Edge(Vertex *from, Vertex *to, Edge *opposite, Edge *next, Edge *follow) {
	this->from = from;
	this->to = to;
	this->opposite = opposite;
	this->next = next;
	this->follow = follow;
}

Edge::~Edge() = default;

bool Edge::operator<(const GraphicsComponent &rhs) const {
	if (typeid(rhs) == typeid(*this)) {
		if (*(this->from) != *(dynamic_cast<const Edge &>(rhs).from)) {
			return *(this->from) < *(dynamic_cast<const Edge &>(rhs).from);
		} else {
			return *(this->to) < *(dynamic_cast<const Edge &>(rhs).to);
		}
	}
	return false;
}

std::vector<GraphicsComponent *> Edge::GetAdjacentComponent() {
	std::vector<GraphicsComponent *> res;
	res.push_back(this->next);
	res.push_back(this->opposite);
	return res;
}

std::vector<GraphicsComponent *> Face::GetAdjacentComponent() {
	std::vector<GraphicsComponent *> res;
	res.push_back(new Face(this->markedEdge->opposite));
	for (Edge *edge = this->markedEdge->next; edge != this->markedEdge; edge = edge->next) {
		res.push_back(new Face(edge->opposite));
	}
	return res;
}

Face::Face(Edge *markedEdge) {
	this->markedEdge = markedEdge;
	MinimizeMarkedEdge();
}

Face::~Face() = default;

bool Face::operator<(const GraphicsComponent &rhs) const {
	if (typeid(rhs) == typeid(*this)) {
		return *(this->markedEdge) < *(dynamic_cast<const Face &>(rhs).markedEdge);
	}
	return false;
}

void Face::MinimizeMarkedEdge() {
	Edge *edgeHead = markedEdge;
	Edge *minimizedMarkedEdge = edgeHead;

	for (Edge *currentEdge = edgeHead->next; currentEdge != edgeHead; currentEdge = currentEdge->next) {
		if (currentEdge->from->index < minimizedMarkedEdge->from->index) {
			minimizedMarkedEdge = currentEdge;
		}
	}
	markedEdge = minimizedMarkedEdge;
}

/******************** Graphics Iterator Implementation ********************/

GraphicsIterator::GraphicsIterator(GraphicsComponent *beginComponent) {
	if (beginComponent != nullptr) {
		nextQueue.push(beginComponent);
		inQueueMap[beginComponent] = true;
		UpdateComponent(beginComponent);
	}
}

GraphicsIterator &GraphicsIterator::operator=(const GraphicsIterator &rhs) {
	this->inQueueMap = rhs.inQueueMap;
	this->nextQueue = rhs.nextQueue;// deep copy the queue
	return *this;
}

bool GraphicsIterator::operator==(const GraphicsIterator &rhs) const {
	if (nextQueue.empty() && rhs.nextQueue.empty()) {
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

GraphicsComponent *GraphicsIterator::operator*() const {
	if (nextQueue.empty()) {
		throw "End iterator cannot be dereferenced";
	}
	return nextQueue.front();
}

GraphicsIterator &GraphicsIterator::operator++() {
	UpdateComponent(nextQueue.front());
	if (nextQueue.empty()) {
		throw "End iterator cannot be increased";
	}
	nextQueue.pop();
	return *this;
}

const GraphicsIterator GraphicsIterator::end = GraphicsIterator(nullptr);

void GraphicsIterator::UpdateComponent(GraphicsComponent *component) {
	std::vector<GraphicsComponent *> adjacentComponents = component->GetAdjacentComponent();
	for (auto adjacentComponent : adjacentComponents) {
		if (!inQueueMap[adjacentComponent]) {
			inQueueMap[adjacentComponent] = true;
			nextQueue.push(adjacentComponent);
		}
	}
}
