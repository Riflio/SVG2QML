#ifndef INODE_H
#define INODE_H

class INodeInterface
{
public:
    virtual ~INodeInterface() {};
    virtual void reset() =0;
    virtual void addNext(INodeInterface * iNode) =0;
    virtual void addPrev(INodeInterface * iNode) =0;
    virtual INodeInterface * levelDown(INodeInterface * node) =0;
    virtual INodeInterface * levelUp() =0;
    virtual void removeFromLevel() =0;

    virtual INodeInterface * first() const =0;
    virtual INodeInterface * last() const =0;
    virtual INodeInterface * up() const =0;
    virtual INodeInterface * down() const =0;
    virtual INodeInterface * next() const =0;
    virtual INodeInterface * prev() const =0;
};

#endif // INODE_H
