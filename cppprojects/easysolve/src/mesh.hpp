#include "field.hpp"

class Mesh {
protected:
    Index* index;
    Field x;

public:
    Mesh(Index& meshIndex) : x(meshIndex) {
        index = &meshIndex;
    }
};

class UniformMesh : public Mesh {
    double lx;
    double dx;

public:
    UniformMesh(Index& meshIndex, double lx) : Mesh(meshIndex), lx(lx) {
        dx = lx / double(index->getNumCell(0));
        for (auto zone = index->getInteriorZone(0); zone.next(); ) {
            int i = zone.getCurrent();
            x.element({ i }) = (i-index->getBegin(0)) * dx;
        }
    }
    
};
