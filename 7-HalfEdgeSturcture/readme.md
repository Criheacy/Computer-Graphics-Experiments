## Graphics: 使用半边数据结构实现的图形基类

### 特点：

- 严格封装，使用迭代器访问内容：

    - 提供`VertexBegin`、`VertexEnd`接口实现顶点的访问
    
    - 提供`FaceBegin`、`FaceEnd`接口实现面的访问
    
    - 迭代器返回`GraphicsIterator`类型：
    
        - 使用解引用运算获取内部数据
    
        - 使用前置自增运算符实现迭代器自增
    
    - 迭代器内部真正实现了“用到哪算到哪”，不会一次性将所有图形中元素放入容器，而是每次自增时更新运算
    
    - 访问会按照临近顺序，先返回头部节点相邻的节点，然后沿图形拓扑结构依次扩散，保证能每个点都被遍历到且仅遍历一次。
    
    - 如果在迭代器中部更新图形结构，原来的迭代器将失效，将不能保证遍历结果正确，需要重新获取

    - 使用举例：
    ```C++
    auto it = testGraphics.VertexBegin();
    while (it != testGraphics.VertexEnd())
    {
        printf("Vertex (%.1f,%.1f,%.1f)",
            *it.position.x, *it.position.y, *it.position.z);
        // do some thing on *it ...
        ++it;
    }
    ```