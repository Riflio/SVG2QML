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

    /**
    * @brief Перебираем все итемы по очереди
    * @param item
    */
    static CNodeInterface * iterator(CNodeInterface * itm)
    {
        if ( itm->down!=nullptr ) {
            return itm->down;
        }  else if ( itm->next!=nullptr ) {
            return itm->next;
        } else if ( itm->up!=nullptr ) {
            while(true) {
                if ( itm->up==nullptr ) { return nullptr; }
                itm = itm->up;
                if ( itm->next!=nullptr ) { return itm->next; }
            }
        } else {
            return nullptr;
        }
     }
};

#endif // CNODE_H
