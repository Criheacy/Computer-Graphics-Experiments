### 实验二：直线与圆的描线法

##### 【说明】

- 用鼠标右键拖拽图形以平移，用鼠标中键拖拽图形以缩放
- 用鼠标左键拖拽以画出直线：填充为灰色的格子为直线经过的像素，黑色为直线参考线
- 可以在Settings.h中自行更改部分设置

##### 【演示】

![demo](demo.gif)

##### 【实现】

- Grid类：
  - public methods:
    - Translate(deltaPosition)
    - Scale(scale)
    - ScaleAt(scale, scalingPosition)
    - Reanchor(anchor)
  - interface for interactive:
    - bool InPoint(vec2 src)
    - SetStartPoint(vec2 point)
    - SetEndPoint(vec2 point)
    - SetHoverPoint(vec2 point)
  - interface for line drawing:
    - MarkPoint(int x, int y)
    - ClearPoints()
  - interface for rendering:
    - Render()
  - protected methods:
    - Transform(vec2 src) -> vec2 dist
    - InverseTransform(vec2 src) -> vec2 dist
    - ProjectToScreen(vec2 src) -> vec2 dist
  - private methods:
    - ConstainScale()
    - RenderPoints()
    - RenderGridLines()
    - RenderAuxiliaryLine()
- Algorithm类：
  - public methods:
    - DDALine(*grid, fromX, toX, fromY, toY)
    - BresenhamLine(*grid, fromX, toX, fromY, toY)
  - protected methods:
    (Use for converting point to main section by mapping function)
    - MarkPappedPoint(*grid, x, y, fromX, fromY, section)
    - (return section) ConvertToSection(&fromX, &toX, &fromY, &toY)

