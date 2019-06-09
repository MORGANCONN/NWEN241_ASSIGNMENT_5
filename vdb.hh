//
// Created by Connor on 20/05/2019.
//


#include "dbms2.hh"
#include <vector>

namespace dbms2{

    class VectorDbTable : public AbstractDbTable{

    public:
        int rows() const override;
        bool show(int index) const override;
        bool add(movie toAdd) override;
        bool remove(unsigned long int toRemove) override;
        const movie* get(int index) const;
        std::vector<movie> table;
    };

}



