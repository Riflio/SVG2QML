#ifndef CNODE_H
#define CNODE_H

#include "inode.h"

/**
* @brief Базовый класс, что бы можно было по быстрому связать примитивы пути группы и тд
* у каждого элемента доно быть установлены указатели на
* next/prev - следующий/предыдущий элемент на одном уровне
* first/last - первый/последний элемент на одном уровне (для текущего уровня получать через верхний уровень (up) ),
* up - первый элемент из уровня выше
* down - первый элемент из уровня ниже
* если чего-то нету, то nullptr
*/
class CNodeInterface: public virtual INodeInterface
{
public:

    CNodeInterface(): _first(nullptr), _last(nullptr), _up(nullptr), _down(nullptr), _next(nullptr), _prev(nullptr) {  }
    virtual ~CNodeInterface() {}

    /**
    * @brief Сбрасываем состояние ноды
    * @param node
    */
    void reset() override
    {
        CNodeInterface * node = dynamic_cast<CNodeInterface*>(this);
        node->_down = nullptr;
        node->_next = nullptr;
        node->_prev = nullptr;
        node->_up = nullptr;
        node->_last = nullptr;
        node->_first = nullptr;
    }

    /**
    * @brief Добавляем на текущий уровень ещё один элемент в конец
    * @param node
    */
    void addNext(INodeInterface * iNode) override
    {
        CNodeInterface * level = dynamic_cast<CNodeInterface*>(this);
        CNodeInterface * node = dynamic_cast<CNodeInterface*>(iNode);

        if ( level->_first==nullptr ) { level->_first = node; }

        if ( level->_last!=nullptr ) {
            level->_last->_next=node;
            node->_prev = level->_last;
        }

        level->_last = node;
        node->_up = level;

        if ( level->_down==nullptr ) { level->_down = node; }
    }

    /**
    * @brief Добавляем на текущий уровень ещё один элемент в начало
    * @param node
    */
    void addPrev(INodeInterface * iNode) override
    {
        CNodeInterface * level = dynamic_cast<CNodeInterface*>(this);
        CNodeInterface * node = dynamic_cast<CNodeInterface*>(iNode);

        if ( level->_last==nullptr ) { level->_last = node; }

        if ( level->_first!=nullptr ) {
            level->_first->_prev = node;
            node->_next = level->_first;
        }

        level->_first = node;
        node->_up = level;
        level->_down = node;
    }

    /**
    * @brief Создаём уровень ниже
    * @param node
    */
    INodeInterface * levelDown(INodeInterface * node) override
    {
        addNext(node);
        return node;
    }

    /**
    * @brief Переходим на уровень выше
    */
    INodeInterface * levelUp() override
    {
       return up();
    }

    /**
    * @brief Убираем из уровня ноду
    */
    void removeFromLevel() override
    {
        CNodeInterface * node = this;
        if ( node->_up!=nullptr ) {
            if ( node->_up->_last==node ) { node->_up->_last = node->_prev; }
            if ( node->_up->_first==node ) { node->_up->_first = node->_next; }
            if ( node->_up->_down==node ) { node->_up->_down = node->_next; }
        }
        node->_up = nullptr;

        if ( node->_next!=nullptr ) { node->_next->_prev = node->_prev; }
        if ( node->_prev!=nullptr ) { node->_prev->_next = node->_next; }

        node->_next = nullptr;
        node->_prev = nullptr;
    }

    INodeInterface * first() const override { return _first; }
    INodeInterface * last() const override { return _last; }
    INodeInterface * up() const override { return _up; }
    INodeInterface * down() const override { return _down; }
    INodeInterface * next() const override { return _next; }
    INodeInterface * prev() const override { return _prev; }

private:
    CNodeInterface * _first;
    CNodeInterface * _last;
    CNodeInterface * _up;
    CNodeInterface * _down;
    CNodeInterface * _next;
    CNodeInterface * _prev;

};

/**
* @brief Пробегаемся по всем итемам с учётом уровней
*/
class CNodeInterfaceIterator
{
public:
    CNodeInterfaceIterator(INodeInterface * rootItm): _rootItm(rootItm), _curItm(rootItm), _curLevel(rootItm), _curType(IT_NONE) {
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

        if ( (_curType&IT_STARTELEMENT) && (_curItm->down()!=nullptr) ) {
            _curLevel = _curItm;
            _curItm = _curItm->down();
            _curType = (IT_STARTELEMENT | IT_STARTLEVEL);
            return true;
        } else
        if ( _curItm->next()!=nullptr ) {
            _curItm = _curItm->next();
            _curType = IT_STARTELEMENT;
            return true;
        } else
        if ( _curItm->up()!=_rootItm->up() ) {
            _curItm = _curItm->up();
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
        if ( _curItm->last()!=nullptr ) { _curItm = _curItm->last(); }
        if ( _curItm->up()==_rootItm->up() ) { _curItm = nullptr; return false; }
        _curItm = _curItm->up();
        _curLevel = _curItm;
        _curType = IT_ENDLEVEL;
        return true;
    }

    template<class T=CNodeInterface*>
    T item() const { return dynamic_cast<T>(_curItm); }

    template<class T=CNodeInterface*>
    T level() const { return dynamic_cast<T>(_curLevel); }

    template<class T=CNodeInterface*>
    T rootItem() const { return dynamic_cast<T>(_rootItm); }

    int type() const { return _curType; }

private:
    INodeInterface * _rootItm;
    INodeInterface * _curItm;
    INodeInterface * _curLevel;
    int _curType;
};

#endif // CNODE_H
