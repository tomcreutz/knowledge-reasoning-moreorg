#ifndef OWL_API_MODEL_AXIOM_HPP
#define OWL_API_MODEL_AXIOM_HPP

#include <owl_om/owlapi/model/OWLObject.hpp>
#include <owl_om/owlapi/model/HasAnnotations.hpp>

namespace owlapi {
namespace model {

/**
 *
 * \see http://www.w3.org/TR/owl2-syntax/#Axioms
 */
class OWLAxiom : public OWLObject, HasAnnotations
{
    enum Type { 
                Declaration = 0,            // TDLAxiomDeclaration
                ClassAxiom,                 // TDLConceptName
                ObjectPropertyAxiom,        // TDLObjectRoleName
                DataPropertyAxiom,          // TDLDataRoleName
                DatatypeDefinition,         // n/a
                Assertion,                  // n/a
                HasKey, 
                AnnotationAxiom,            // not part of the reasoner
                // ClassAxiom:
                SubClassOf,                 // TDLAxiomConceptInclusion
                EquivalentClasses,          // TDLAxiomEquivalentConcepts
                DisjointClasses,            // TDLAxiomDisjointConcepts
                DisjointUnion,              // TDLAxiomDisjointUnion
                // ObjectPropertyAxiom
                EquivalentObjectProperties, // TDLAxiomEquivalentORoles
                DisjointObjectProperties,   // TDLAxiomDisjointORoles
                SubObjectPropertyOf,        // TDLAxiomORoleSubsumption
                ObjectPropertyDomain,       // TDLAxiomORoleDomain
                ObjectPropertyRange,        // TDLAxiomDRoleRange
                InverseObjectProperties,    // TDLAxiomRoleInverse
                FunctionalObjectProperty,   // TDLAxiomORoleFunctional
                ReflexiveObjectProperty,    // TDLAxiomRoleReflexive
                InverseFunctionalObjectProperty, // TDLAxiomRoleInverseFunctional
                IrreflexiveObjectProperty,  // TDLAxiomRoleIrreflexive
                SymmetricObjectProperty,    // TDLAxiomRoleSymmetric
                TransitiveObjectProperty,   // TDLAxiomRoleTransitive
                AsymmetricObjectProperty,   // TDLAxiomRoleAsymmetric
                // DataPropertyAxiom
                SubDataPropertyOf,          // TDLDRoleSubsumption
                DisjointDataProperties,     // TDLAxiomDisjointDRoles
                EquivalentDataProperties,   // TDLAxiomEquivalentDRoles
                FunctionalDataProperties,   // TDLAxiomDRoleFunctional
                DataPropertyDomain,         // TDLAxiomDRoleDomain
                DataPropertyRange,          // TDLAxiomDRoleRange
                // Assertion
                SameIndividual,             // TDLAxiomSameIndividuals
                DifferentIndividuals,       // TDLAxiomDifferentIndividuals
                ClassAssertion,             // TDLAxiomInstanceOf
                ObjectPropertyAssertion,    // TDLAxiomRelatedTo
                NegativeObjectPropertyAssertion,  // TDLAxiomRelatedToNot
                DataPropertyAssertion,      // TDLAxiomValueOf
                NegativeDataPropertyAssertion,    // TDLAxiomValueOfNot
                // AnnotationAxiom
                SubAnnotationPropertyOf,    // n/a
                AnnotationPropertyDomain,   // n/a
                AnnotationPropertyRange,    // n/a
                AnnotationAssertion,        // n/a
                // Not explicitly stated in OWL 2 but for convenience
                SubPropertyChainOf
    };
public:
//
//    /**
//     * @param visitor
//     *        visitor to accept
//     */
//    void accept(OWLAxiomVisitor visitor);

    /**
     * Gets the annotations that annotate this axiom and whose annotation
     * property is equal to {@code annotationProperty}.
     * 
     * @param annotationProperty
     *        The annotation property that will be equal to the annotation
     *        property of each returned annotation.
     * @return A set of annotations that annotate this axiom, each of whose
     *         annotation properties is equals to {@code annotationProperty//     */
    //OWLAnnotationPtrList getAnnotations(const OWLAnnotationProperty& annotationProperty) const;
//
//    /**
//     * Gets an axiom that is structurally equivalent to this axiom without
//     * annotations. This essentially returns a version of this axiom stripped of
//     * any annotations
//     * 
//     * @return The annotationless version of this axiom
//     */
//    OWLAxiom getAxiomWithoutAnnotations() const;
//
//    /**
//     * Gets a copy of this axiom that is annotated with the specified
//     * annotations. If this axiom has any annotations on it they will be merged
//     * with the specified set of annotations. Note that this axiom will not be
//     * modified (or remove from any ontologies).
//     * 
//     * @param annotations
//     *        The annotations that will be added to existing annotations to
//     *        annotate the copy of this axiom
//     * @return A copy of this axiom that has the specified annotations plus any
//     *         existing annotations returned by the
//     *         {@code OWLAxiom#getAnnotations()} method.
//     */
//    OWLAxiom getAnnotatedAxiom(const OWLAnnotation::Set& annotations) const;
//
//    /**
//     * Determines if another axiom is equal to this axiom not taking into
//     * consideration the annotations on the axiom
//     * 
//     * @param axiom
//     *        The axiom to test if equal
//     * @return {@code true} if {@code axiom} without annotations is equal to
//     *         this axiom without annotations otherwise {@code false}.
//     */
//    bool equalsIgnoreAnnotations(const OWLAxiom& axiom) const;
//
//    /**
//     * Determines if this axiom is a logical axiom. Logical axioms are defined
//     * to be axioms other than both declaration axioms (including imports
//     * declarations) and annotation axioms.
//     * 
//     * @return {@code true} if the axiom is a logical axiom, {@code false} if
//     *         the axiom is not a logical axiom.
//     */
//    bool isLogicalAxiom() const;
//
//    /**
//     * Determines if this axioms in an annotation axiom (an instance of
//     * {@code OWLAnnotationAxiom})
//     * 
//     * @return {@code true} if this axiom is an instance of
//     *         {@code OWLAnnotationAxiom}, otherwise {@code false}.
//     */
//    bool isAnnotationAxiom() const;
//
//    /**
//     * Determines if this axiom has any annotations on it
//     * 
//     * @return {@code true} if this axiom has annotations on it, otherwise
//     *         {@code false}
//     */
//    bool isAnnotated() const;
//
//    /**
//     * Gets the axiom type for this axiom.
//     * @return The axiom type that corresponds to the type of this axiom.
//     */
//    AxiomType getAxiomType() const;
//
//    /**
//     * Determines if this axiom is one of the specified types
//     * 
//     * @param axiomTypes
//     *        The axiom types to check for
//     * @return {@code true} if this axiom is one of the specified types,
//     *         otherwise {@code false}
//     */
//    bool isOfType(AxiomType axiomTypes);
//
//    /**
//     * Determines if this axiom is one of the specified types
//     * 
//     * @param types
//     *        The axiom types to check for
//     * @return {@code true} if this axioms is one of the specified types,
//     *         otherwise {@code false}
//     */
//    bool isOfType(AxiomType::Set types);
//
//    /**
//     * Gets this axioms in negation normal form. i.e. any class expressions
//     * involved in this axiom are converted into negation normal form.
//     * 
//     * @return The axiom in negation normal form.
//     */
//    OWLAxiom getNNF();

};

} // end namespace model
} // end namespace owlapi
#endif // OWL_API_MODEL_AXIOM_HPP
