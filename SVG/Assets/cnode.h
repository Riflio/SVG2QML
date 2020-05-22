#ifndef CNODE_H
#define CNODE_H

/**
* @brief Базовый класс, что бы можно было по быстрому связать примитивы пути группы и тд
* у каждого элемента доно быть установлены указатели на
* next/prev - следующий/предыдущий элемент на одном уровне
* first/last - первый/последний элемент на одном уровне (для текущего уровня получать через верхний уровень (up) ),
* up - первый элемент из уровня выше
* down - первый элемент из уровня ниже
* если чего-то нету, то nullptr
*/

class CNodeInterface
{
public:
    CNodeInterface * first;
    CNodeInterface * last;
    CNodeInterface * up;
    CNodeInterface * down;
    CNodeInterface * next;
    CNodeInterface * prev;

    CNodeInterface() { reset(this); }
    virtual ~CNodeInterface() {}

    /**
    * @brief Сбрасываем состояние ноды
    * @param node
    */
    static void reset(CNodeInterface * node)
    {
        node->down = nullptr;
        node->next = nullptr;
        node->prev = nullptr;
        node->up = nullptr;
        node->last = nullptr;
        node->first = nullptr;
    }

    /**
    * @brief Добавляем на указанный уровень ещё один элемент в конец
    * @param level
    * @param node
    */
    static void addNext(CNodeInterface * level, CNodeInterface * node)
    {
        if ( level->first==nullptr ) level->first=node;

        if ( level->last!=nullptr ) {
            level->last->next=node;
            node->prev = level->last;
        }

        level->last = node;
        node->up = level;

        if ( level->down==nullptr ) level->down = node;
    }

    /**
    * @brief Добавляем на указанный уровень ещё один элемент в начало
    * @param level
    * @param node
    */
    static void addPrev(CNodeInterface * level, CNodeInterface * node)
    {
        if ( level->last==nullptr ) level->last = node;

        if ( level->first!=nullptr ) {
            level->first->prev = node;
            node->next = level->first;
        }

        level->first = node;
        node->up = level;
        level->down = node;
    }

    /**
    * @brief Добавляем на указанный уровень ещё элемент и заменяем указанный уровень на этот элемент
    * @param level
    * @param node
    */
    static CNodeInterface * levelDown(CNodeInterface * level, CNodeInterface * node )
    {
        addNext(level, node);
        return node;
    }

    /**
    * @brief Заменяем указанный уровень, на верхний уровень из указанного
    * @param level
    */
    static CNodeInterface * levelUp(CNodeInterface * level)
    {
       return level->up;
    }

    /**
    * @brief Убираем из уровня ноду
    * @param node
    */
    static void removeFromLevel(CNodeInterface * node)
    {
        if ( node->up!=nullptr ) {
            if ( node->up->last==node ) node->up->last = node->prev;
            if ( node->up->first==node ) node->up->first = node->next;
            if ( node->up->down==node ) node->up->down = node->next;
        }
        node->up = nullptr;

        if ( node->next!=nullptr ) node->next->prev = node->prev;
        if ( node->prev!=nullptr ) node->prev->next = node->next;

        node->next = nullptr;
        node->prev = nullptr;
    }

};

/**
* @brief Пробегаемся по всем итемам с учётом уровней
*/
class CNodeInterfaceIterator
{
public:
    CNodeInterfaceIterator(CNodeInterface * rootItm): _rootItm(rootItm), _curItm(rootItm), _curLevel(rootItm), _curType(IT_NONE) {
        if ( _curItm!=nullptr ) { _curType = IT_STARTELEMENT; }
    }

    //-- Устанавливаем при переходе к каждому элементу
    enum IterationType: int {
        IT_NONE = 2,
        IT_STARTELEMENT = 4,
        IT_STARTLEVEL = 8,
        IT_ENDLEVEL = 16
    };

    /**
    * @brief Переходим к следующему элементу
    * @return
    */
    bool next()
    {
        if ( _curItm==nullptr ) { return false; }

        if ( (_curType&IT_STARTELEMENT) && (_curItm->down!=nullptr) ) {
            _curLevel = _curItm;
            _curItm = _curItm->down;
            _curType = (IT_STARTELEMENT | IT_STARTLEVEL);
            return true;
        } else
        if ( _curItm->next!=nullptr ) {
            _curItm = _curItm->next;
            _curType = IT_STARTELEMENT;
            return true;
        } else
        if ( _curItm->up!=nullptr ) {
            _curItm = _curItm->up;
            _curLevel = _curItm;
            _curType = IT_ENDLEVEL;
            return true;
        }

        return false;
    }

    /**
    * @brief Переходим сразу на следующий уровень и не важно в каком месте сейчас
    * @return
    */
    bool nextLevel()
    {
        if ( _curItm->last!=nullptr ) { _curItm = _curItm->last; }
        if ( _curItm->up==nullptr ) { _curItm = nullptr; return false; }
        _curItm = _curItm->up;
        _curLevel = _curItm;
        _curType = IT_ENDLEVEL;
        return true;
    }

    template<class T=CNodeInterface*>
    T item() const { return static_cast<T>(_curItm); }

    template<class T=CNodeInterface*>
    T level() const { return static_cast<T>(_curLevel); }

    template<class T=CNodeInterface*>
    T rootItem() const { return static_cast<T>(_rootItm); }

    int type() const { return _curType; }

private:
    CNodeInterface * _rootItm;
    CNodeInterface * _curItm;
    CNodeInterface * _curLevel;
    int _curType;
};

#endif // CNODE_H
