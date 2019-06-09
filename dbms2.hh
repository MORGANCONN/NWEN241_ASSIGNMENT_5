

namespace dbms2 
{
    struct movie {
        unsigned long id;
        char title[50];
        unsigned short year;
        char director[50];
        char country[50];
        unsigned long numberOfOscars;
    };

    class AbstractDbTable{
    public:
    virtual int rows() const = 0;
    virtual bool show(int index) const = 0;
    virtual bool add(movie toAdd) = 0;
    virtual bool remove(unsigned long int toRemove) = 0;
    virtual const movie* get(int index) const = 0;
    bool loadCSV(const char *infn);
    bool saveCSV(const char *outfn);
    };
}
