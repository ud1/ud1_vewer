# ud1 viewer
 
## Краткое описание
Вьювер для отображения и отладки сцен для конкурсов типа Russian AI Cup.
Может работать в одном из двух режимов, 2d или 3d, выбор режима определяется типом отправляемого сообщения fieldSize или field3d.

Может сохранять переданные команды в файл, и открывать его для просмотра старых игр.

Запускает tcp сервер, клиентское приложение подключается к нему и отсылает комманды. Используется бинарный протокол.
Формат сообщений можно посмотреть в d_format.hpp. Основной тип var_t это std::variant, может хранить строку, вещественное число, беззнаковое целое число (используется также для передачи цветов в формате 0xrrggbbaa), двумерный вектор тип P, трехмерный вектор типа V3, и матрица M4. Для V3 и M4 используется библиотека glm.
Тип SObj (Simple Object) это просто std::map<string, var_t> - набор свойств с именами в качестве ключа и значениями var_t.
Тип Obj состоит из строки типа type, набора аттрибутов SObj и набора подобъектов subObjs. Например это может быть игровой объект (например машина), в props которого написаны произвольные свойства, а в subObjs составные части этого объекта, которые в частности могут использоваться для отрисовки (колеса в виде кругов, кузов в виде многоугольника и т.д.).
Клиентсвое приложение отправляет эти произвольные объекты Obj. Они будут отображаться в дереве объектов. Содержимое может быть произвольным, однако есть особые типы Obj и SObj, которые имеют особую обработку во вьювере, и влияют на отрисовку.


Особые типы Obj.type (Можно посмотреть в mainwindow.cpp в методе void RefsView::process(const Obj &obj)):

"tick" - создает новый тик, вьювер при этом переключается на отображение предыдущего тика.

"fieldSize" - размер 2d поля с атрибутами w/h. Вьювер переключается в режим рисование 2d.

"field3d" - размер и положение 3d поля. Вьювер переключается в режим рисование 3d. Параметры minP/maxP типа двумерного вектора P, hMin/hMax - максимальная/минимальная высота.

"static" - статические объекты, задает набор объектов для рисования в каждом тике, чтоб не передавать их каждый тик, а только один раз в самом начале.


Особые типы SObj["type"] (Можно посмотреть в view.cpp в методах renderObj2d и renderObj3d):

"circle" - круг с центром в "p", цветом "c" и радиусом "r".

"circumference" - окружность с центром в "p", цветом "c" и радиусом "r".

"line" - линия с цветом "c" и набором точек "p1", "p2", "p3"...

"poly" - выпуклый многоугольник с цветом "c" и набором точек "p1", "p2", "p3"...

"sphere" - сфера с центром в "p", цветом "c" и радиусом "r" и источником света в lp.

"disk" - диск с центром в "p", цветом "c" и радиусом "r" и нормалью "n".

"rect" - прямоугольник с центром в "p", цветом "c", касательными (два перпендикулярных вектора задающий плоскость и размеры) ht1/ht2, источником света в lp, сдвигом сетки gridShift. 

"line3d" - линия с цветом "c" и набором точек "p1", "p2", "p3"...

"cyl_inner_025" - одна четверть внутренней поверхности цилиндра, с цветом "c", источником света в lp, матрицей модели "transf".


## Управление
В 2d режиме можно левой кнопкой мыши двигать поле, колесиком менять масштаб, правой кнопкой измерять расстояния.
В 3d режиме можно перемещаться в пространстве кнопками W/A/S/D/Space/Ctrl, увеличеная скорость Shift. Вращение камеры левой кнопкой мыши.
Клиентской программе также приходят события о нажатиях клавиш, тем самым можно влиять на поведение стратегии, например можно запрограммировать паузу и потиковый расчет. Или ручное управление объектами в стратегии.

Пример клиентской программы в testClient2d.cpp и testClient3d.cpp


## Сборка
Вьювер можно собрать командами qmake && make. Он использует Qt glm glew nanovg boost.

nanovg надо скачать с гитхаба и положить исходный код в каталог nanovg (рядом с каталогом shaders).

Клиенту нужны glm и boost.

## Детали отрисовки
Рисуются объекты Obj::subObjs. Сферы, диски и четверть цилиндры рисуются рейтрейсом. Сферы, прямоугольники и четверть цилиндры окрашиваются дополнительно сеткой в местах пересечения с плоскостями с целыми координатами x/y/z, чтоб прямоугольники с координатами целиком в целых числах не окрашивались целиком в темный цвет, можно передавать параметр gridShift. Четверть цилиндр можно трансформировать через transf матрицу, например:

```
obj.subObjs["mycyl"]["transf"] = glm::translate(V3(3, 2 * L - 3, 5))
    * glm::rotate((float) (- M_PI * 0.5), V3(0, 0, 1))
        * glm::scale(V3(6, 6, 10));
```

