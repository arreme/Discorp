#include <iostream>

namespace pm 
{
    typedef enum PM_ERR_CODE
    {
        SUCCESS = 0,
        NO_PLANT = -1,
        ALREADY_OCCUPED = -2,
    } pm_err_code;

    pm_err_code PlantThere(std::string plant_name, uint16_t plot_n);

    
}