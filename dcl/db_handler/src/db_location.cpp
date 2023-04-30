#include <db_handler/db_location.hpp>

using namespace bsoncxx::builder::basic;
using namespace bsoncxx::types;

bsoncxx::document::value db_handler::DBLocationHandler::s_start_post = 
    make_document(
        kvp("capacity_upgrade",b_int32{0}),
        kvp("gen_second_upgrade",b_int32{0}),
        kvp("fortune_upgrade",b_int32{0}),
        kvp("resource_stored",b_int64{0})  
    );

bsoncxx::document::value db_handler::DBLocationHandler::s_start_zone_access =
    make_document(
        kvp("unlock_level",b_int32{0}) 
    );

bsoncxx::document::value db_handler::DBLocationHandler::s_start_dialog =
    make_document(
        kvp("current_dialog",b_int32{0}) 
    );