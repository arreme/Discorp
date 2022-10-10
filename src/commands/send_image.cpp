#include <send_image.h>

SendImage::SendImage() 
{
    background1 = fopen("resources/Background1.png","rb");
    ptr1 = gdImageCreateFromPng(background1);
}

void SendImage::CreateCommand(dpp::commandhandler *handler) {
    this->handler = handler;
    handler->add_command
    (
        "image",
        {
            //{"name", dpp::param_info(dpp::pt_string, true, "Optional test parameter") }
        },
        [this](const std::string& command, const dpp::parameter_list_t& parameters, dpp::command_source src) {
            FILE *background2 = fopen("resources/Fruit1.png","rb");
            gdImagePtr ptr2 = gdImageCreateFromPng(background2);
            gdImagePtr baseImg = gdImageClone(ptr1);
            gdImageCopy(baseImg,ptr2,48,48,0,0,32,32);

            int size = -1;
            void *pngData = gdImagePngPtr(baseImg,&size);
            std::string s((char *) pngData,size);
            
            gdFree(ptr2);
            gdFree(baseImg);
            fclose(background2);
            
            dpp::message msg(src.channel_id, "");
 
            msg.add_file("img1out.png", s);
            gdFree(pngData);
            this->handler->reply(msg,src);
        },
        "Simple image"
    );
}