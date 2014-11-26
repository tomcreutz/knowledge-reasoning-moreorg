#include "OWLEquivalentClassesAxiom.hpp"
#include "OWLAxiomVisitor.hpp"

namespace owlapi {
namespace model {

void OWLEquivalentClassesAxiom::accept(boost::shared_ptr<OWLAxiomVisitor> visitor)
{
    visitor->visit(*this);
}

} // end namespace model
} // end namespace owlapi
