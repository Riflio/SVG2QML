#ifndef CEMPTY_H
#define CEMPTY_H

/**
* @brief
*/
class CEmpty {
public:
    CEmpty() {};

    class CEmptyPriv {
        public:
        bool operator==(const float &f) const { return (f!=f); }
    };

    const static CEmptyPriv CEMPTY;

    virtual ~CEmpty() {};
    virtual bool operator==(const CEmptyPriv &) const = 0;

private:

};

const CEmpty::CEmptyPriv CEMPTY;


#endif // CEMPTY_H
