## Graphics: 使用半边数据结构实现的图形基类

### 特点：

1. 使用迭代器访问图形元素：

    - 提供`VertexBegin`、`VertexEnd`接口实现顶点的访问；
    
    - 提供`EdgeBegin`、`EdgeEnd`接口实现面的访问；
    
    - 提供`FaceBegin`、`FaceEnd`接口实现面的访问；
    
    - 访问会按照临近顺序，先返回头部节点相邻的节点，然后沿图形拓扑结构依次扩散，保证能每个点都被遍历到且仅遍历一次。
    
    - 迭代器返回`GraphicsIterator`类型：
    
        - 使用解引用运算获取内部数据，解引用后得到`GraphicsComponent`类型，可以使用`dynamic_cast`转化为派生的`Vertex`、`Edge`或`Face`类型；
    	
        - 使用前置自增运算符实现迭代器自增；不提供后置自增运算符。

    - 使用举例：

	  ```C++
	  auto it = testGraphics.VertexBegin();
	  while (it != testGraphics.VertexEnd()) {
	      Vertex* current = dynamic_cast<Vertex*>(*it);
	      printf("Vertex (%.1f,%.1f,%.1f)",
	      	current.position.x, current.position.y, current.position.z);
	      ++it;
	  }
	  ```
	
	  ```C++
	  for (auto it = testGraphics.FaceBegin(); it = testGraphics.FaceEnd(); ++it) {
	      Face* current = dynamic_cast<Face*>(*it);
	      // do something on current face
	  }
	  ```

    - 实现细节：
    
        - 迭代器会在内部保存元素队列，每当一个元素被取出前会使用`GraphicsComponent`类型开放的`GetAdjacentComponent`接口获取相邻元素加入队列（该接口会被所有派生的子类实现），当队列为空无元素可取时迭代器与`End`相等；
        - 为防止元素被重复加入队列，迭代器内部使用`std::map`保存了`GraphicsComponent`到`bool`的映射集作为加入队列的标志；`std::map`需要在`GraphicsComponent`上实现偏序关系运算（严格反自反偏序），因此`GraphicsComponent`重载了“`<`”运算符供所有派生子类实现，并实现了其它诱导运算符；
        - 以此，迭代器内部真正实现了“用到哪算到哪”，不会一次性将所有图形中元素放入容器，而是每次自增时更新运算。如果在迭代器中部更新图形结构，原来的迭代器将失效，将不能保证遍历结果正确，需要重新获取。

2. 图形的编辑：
   - 提供`InsertVertexInEdge(Edge* edgeToInsert, Vertex* insertedVertex)`函数，该函数可以在图形的指定边中插入指定顶点（若顶点未指定，则默认插入边的中点）；插入后图形仍为三角面流形。
   - 提供`SubdivideFaces()`函数，该函数可以细分图形所有面（取每个面每条边的中点并连线，将面分割为四个子面）；细分后图形仍为三角面流形。

