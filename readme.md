# SVG2QML

Convert SVG file to native QML file.
Преобразует SVG файл в готовый нативный QML файл.

![Original SVG file](/examples/buttons1_original.png)
![Converted QML file](/examples/buttons1.png)

See [Original SVG file](/examples/buttons1.svg) and [Converted QML file](/examples/buttons1.qml)

## Changelog
Version 1.3
- Shape x,y,width,height as content
- Fix parent styles
- Fix gradients in negative values

Version 1.2
- Reworked the creation of gradients and gradientTransform
- Separately output transform matrix
- Defs order

Version 1.1
- Migrated to Qt 6 (6.2.0)
- Support quadratic Bezier (path qQ)
- Added Drag&Drop area for files to convert
- Fix bug with transform

## Поддерживаемые элементы:

В скобках указаны поддерживаемые индивидуальные параметры. 
Список общих дальше.
- svg
- defs
- title
- desc
- g
- line (x1, y1, x2, y2)
- rect (x, y, width, height, rx, ry)
- circle (cx, cy, r)
- polyline (points)
- polygon (points)
- ellipse (cx, cy, rx, ry)
- path ("d" commands: "mcsqlvhvazMCSQLVHAZ")
- clipPath (g, rect, circle, ellipse, path)
- linearGradient (x1, y1, x2, y2, gradientTransform)
 - stop (offset, stop-color, stop-opacity) 
- radialGradient (cx, cy, r, fx, fy, gradientTransform)
 - stop (offset, stop-color, stop-opacity)
- style (base selector "#id", ".class")

## Поддерживаемые общие параметры

 - opacity
 - fill (color, opacity, gradient)
 - stroke (color, opacity, width, linejoin: bevel/miter/roud, linecap: butt/round/square, miterlimit, dasharray)
 - style (only upper parameters)
 - transform (translate, scale, rotate, matrix3x2)
 - id
 - class

## В планах

 - text
 - image

## Problems



## Tips
 - In Inkscape save as "simple svg".
 - In Adobe Illustrator save as "SVG Tiny 1.1".

### Как работает

Все SVG.g преобразуются в QML.Item, соблюдая вложенность.<br/>
Все SVG.circle, SVG.line, SVG.ellipse, SVG.rect преобразуются сначала в SVG.path,<br/>
за тем, если указан SVG.transform, то он сразу применяется к SVG.path.<br/>
Все SVG.path преобразуются в QML.Shape { QML.ShapePath { QML.PathSvg { path: "path command" }}}.<br/>
Все SVG.clipPath преобразуются сначала в SVG.path, а за тем<br/>
преобразуются в QML.OpacityMask { maskSource: QML.Shape { ... }  }<br/>
Все SVG.linearGradient, SVG.radialGradient преобразуются в QML.LinearGradient и QML.RadialGradient для каждой QML.ShapePath соответственно.<br/>
Если у градиентов есть gradientTransform, то будет две QML.Shape у одной заливка обычным цветом, у другой градиент и трансформация.<br/>
Так же будет сделана QML.OpacityMask, что бы градиент не вылезал за пределы основной фигуры.<br/>

### Что не поддерживается

 - Всё остальное, что не указано, увы =(. 

Почему? Для наших дизайнов этого хватает.
Вы можете в pull request прислать примеры svg, чего бы хотелось.

### Зависимости

 - Qt >= 6.2.0, thats all =)

License
----
GPL V3
