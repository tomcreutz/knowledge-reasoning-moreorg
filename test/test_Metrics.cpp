#include <boost/test/unit_test.hpp>
#include "test_utils.hpp"

#include <organization_model/OrganizationModel.hpp>
#include <organization_model/metrics/Redundancy.hpp>
#include <owlapi/model/OWLOntologyAsk.hpp>
#include <owlapi/model/OWLOntologyTell.hpp>
#include <owlapi/model/OWLExactCardinalityRestriction.hpp>

using namespace organization_model;
using namespace owlapi;
using namespace owlapi::model;

BOOST_AUTO_TEST_SUITE(model_metrics)

BOOST_AUTO_TEST_CASE(redundancy)
{
    OrganizationModel om( getOMSchema() );

    OWLOntologyTell tell(om.ontology());

    OWLClass::Ptr a = tell.klass("http://klass/base");
    OWLClass::Ptr b = tell.klass("http://klass/base-derived");
    OWLClass::Ptr c = tell.klass("http://klass/base-derived-derived");
    OWLObjectProperty::Ptr property = tell.objectProperty("http://property/has");

    tell.subClassOf(c,b);
    tell.subClassOf(b,a);
    om.ontology()->refresh();

    std::vector<OWLCardinalityRestriction::Ptr> query, resourcePool;
    {
        OWLCardinalityRestriction::Ptr restriction(new OWLExactCardinalityRestriction(property, 2, a->getIRI()));
        query.push_back(restriction);
    }
    {
        OWLCardinalityRestriction::Ptr restriction(new OWLExactCardinalityRestriction(property, 2, c->getIRI()));
        query.push_back(restriction);
    }

    {
        OWLCardinalityRestriction::Ptr restriction(new OWLExactCardinalityRestriction(property, 2, b->getIRI()));
        resourcePool.push_back(restriction);
    }
    {
        OWLCardinalityRestriction::Ptr restriction(new OWLExactCardinalityRestriction(property, 2, c->getIRI()));
        resourcePool.push_back(restriction);
    }

    metrics::Redundancy redundancy(om);
    // Serial connection of two parallel a 0.5
    // [ [ 0.5 ] --- [ 0.5 ] ]  --- [ [ 0.5 ] --- [ 0.5 ] ]
    //
    // each serial system has a survivability of: 0.5*0.5 = 0.25
    // overall serial system: 0.25*0.25 = 0.0625
    double expected = 0.0625;
    double redundancyVal = redundancy.computeMetric(query, resourcePool);
    BOOST_REQUIRE_MESSAGE(redundancyVal == expected, "Redundancy expected: " << expected << " but got " << redundancyVal);
}

BOOST_AUTO_TEST_CASE(metric_map_computation)
{
    OrganizationModel om( getOMSchema() );
    om.ontology()->refresh();
    metrics::Redundancy redundancy(om);

    MetricMap survivability = redundancy.getMetricMap();

    //BOOST_TEST_MESSAGE("Survivability: \n" << metrics::Redundancy::toString(survivability));

    {
        IRI locationImageProvider("http://www.rock-robotics.org/2014/01/om-schema#ImageProvider");
        IRI sherpa("http://www.rock-robotics.org/2014/01/om-schema#Sherpa");

        ModelPool modelPool;
        for(int i = 1; i <= 3; ++i)
        {
            modelPool[sherpa] = i;
            double pSurvivability = redundancy.compute(locationImageProvider, modelPool);
            BOOST_TEST_MESSAGE("Survivability for #" << i << " " << sherpa << ": \n" << pSurvivability);
        }
    }
    {
        IRI locationImageProvider("http://www.rock-robotics.org/2014/01/om-schema#LocationImageProvider");
        IRI crex("http://www.rock-robotics.org/2014/01/om-schema#CREX");

        ModelPool modelPool;
        for(int i = 1; i <= 3; ++i)
        {
            modelPool[crex] = i;

            //    required: 
            //            ModelBound::List
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Camera' (1, 1000000)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Localization' (1, 1000000)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Locomotion' (1, 1000000)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Mapping' (1, 1000000)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Power' (1, 1000000)
            //
            //    available: 
            //            ModelBound::List
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Camera' (0, 3)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#EmiPassive' (0, 3)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#LaserScanner' (0, 3)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Localization' (0, 3)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Locomotion' (0, 3)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Mapping' (0, 3)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Power' (0, 3)

            double redundancyLevel = i;
            double parallel = 1 - pow((1-0.5),redundancyLevel);
            double expected = pow(parallel,5);
            double pSurvivability = redundancy.compute(locationImageProvider, modelPool);
            BOOST_REQUIRE_MESSAGE(pSurvivability == expected, "Survivability for #" << i << " " << crex << ": \n" << pSurvivability << " (expected: " << expected << ")");
        }
    }
    {
        IRI locationImageProvider("http://www.rock-robotics.org/2014/01/om-schema#LocationImageProvider");
        IRI stereoCamera("http://www.rock-robotics.org/2014/01/om-schema#StereoImageProvider");

        IRISet services;
        services.insert(locationImageProvider);
        services.insert(stereoCamera);

        IRI robot("http://www.rock-robotics.org/2014/01/om-schema#Sherpa");
        ModelPool modelPool;
        for(int i = 1; i <= 3; ++i)
        {
            modelPool[robot] = i;

            //    required: 
            //            ModelBound::List
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Camera' (1, 1000000)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Localization' (1, 1000000)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Locomotion' (1, 1000000)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Mapping' (1, 1000000)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Power' (1, 1000000)
            //
            //    available: 
            //            ModelBound::List
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Camera' (0, 3)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#EmiPassive' (0, 3)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#LaserScanner' (0, 3)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Localization' (0, 3)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Locomotion' (0, 3)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Mapping' (0, 3)
            //            ModelBound: 'http://www.rock-robotics.org/2014/01/om-schema#Power' (0, 3)

            double redundancyLevel = i;
            double parallel = 1 - pow((1-0.5),redundancyLevel);
            double expected = pow(parallel,5);

            double pSurvivability = redundancy.computeSharedUse(services, modelPool);
            BOOST_TEST_MESSAGE("Survivability for #" << i << " " << robot << ": \n" << pSurvivability);
            //BOOST_REQUIRE_MESSAGE(pSurvivability == expected, "Survivability for #" << i << " " << crex << ": \n" << pSurvivability << " (expected: " << expected << ")");
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()
