#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <iostream>
#include <db_instance.hpp>

MongoDBInstance *instance = MongoDBInstance::GetInstance();

class testRunListener : public Catch::EventListenerBase {
public:
    using Catch::EventListenerBase::EventListenerBase;

    void testRunStarting(Catch::TestRunInfo const&) override {
        instance->createPool("mongodb://192.168.1.33:27017");
    }
};

CATCH_REGISTER_LISTENER(testRunListener)