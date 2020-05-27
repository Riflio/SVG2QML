# SVG2QML
Convert SVG file to native QML file.
Преобразует SVG файл в готовый нативный QML файл.

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
- path ("d" commands: "mcslvhvazMCSLVHAZ")
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

### Как работает
Все SVG.g преобразуются в QML.Item, соблюдая вложенность.
Все SVG.circle, SVG.line, SVG.ellipse, SVG.rect преобразуются сначала в SVG.path, 
за тем, если указан SVG.transform, то он сразу применяется к SVG.path.
Все SVG.path преобразуются в QML.Shape { QML.ShapePath { QML.PathSvg { path: "path command" }}}. 
Все SVG.clipPath преобразуются сначала в SVG.path, а за тем 
преобразуются в QML.OpacityMask { maskSource: QML.Shape { ... }  }
Все SVG.linearGradient, SVG.radialGradient преобразуются в QML.LinearGradient и QML.RadialGradient для каждой QML.ShapePath соответственно. Если у градиентов есть gradientTransform, то будет две QML.Shape у одной заливка обычным цветом, у другой градиент и трансформация. Так же будет сделана QML.OpacityMask, что бы градиент не вылезал за пределы основной фигуры.

### Что не поддерживается
 - Всё остальное, что не указано, увы =(. 

Почему? Для наших дизайнов этого хватает.
Вы можете в pull request прислать примеры svg, чего бы хотелось.

### Зависимости
 - Qt >= 5.12, thats all =)

License
----
GPL V3
