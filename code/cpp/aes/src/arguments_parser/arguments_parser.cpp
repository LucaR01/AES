//
// Created by Luca on 01/03/2023.
//

#include "arguments_parser/arguments_parser.hpp"

#include <iostream>
#include <map>

#include "docopt.h"

//#include "logger/logger.hpp" //TODO: remove
#include "version/version.hpp"
#include "graphics/gui/gui.hpp"
#include "graphics/console/console.hpp"

namespace aes::arg {

void parse_user_arguments(const int& argc, const char** argv)
{
    std::map< std::string, docopt::value > args = docopt::docopt(aes::arg::USAGE,{argv + 1, argv + argc}, true, aes::vrs::aes.version);

#ifndef RELEASE_MODE
    for(const auto& arg : args) {
        std::cout << arg.first << " : " << arg.second << std::endl;
        //AES_DEBUG("{} : {}", arg.first, arg.second) //TODO: doesn't work. remove.
    }
#endif

    if((args["-g"] && args["-g"].isBool() && args["-g"].asBool()) || (args["--gui"] && args["--gui"].isBool() && args["--gui"].asBool())) {
        aes::gui::show();
    } else {
        // Se non viene dato l'opzione -c o --console o -g o --gui allora mostrare la schermata di default.
        // O la console o la gui.
        aes::con::show_console();
    }

    /*if((args["-c"] && args["-c"].isBool() && args["-c"].asBool()) || (args["--console"] && args["--console"].isBool() && args["--console"].asBool())) {
       aes::con::show_console();
   } else {
        // Questo sia in caso di -g sia se non c'è la -g, quindi come default.
        aes::gui::show();
    }*/

    //TODO: forse sarebbe meglio mostrare la console come default. Perché la gui da problemi.

    /*if((args["-c"] && args["-c"].isBool() && args["-c"].asBool()) || (args["--console"] && args["--console"].isBool() && args["--console"].asBool())) {
        aes::con::show_console();
    }*/

    //TODO: se nè -c nè -g (e correlati) allora mostrare di default la gui oppure dovrebbe essere la console il default?

    /*if(!((args["-c"] && args["-c"].isBool() && args["-c"].asBool()) || (args["--console"] && args["--console"].isBool() && args["--console"].asBool()))
    && !((args["-g"] && args["-g"].isBool() && args["-g"].asBool()) || (args["--gui"] && args["--gui"].isBool() && args["--gui"].asBool()))) {
        // Se non viene dato l'opzione -c o --console o -g o --gui allora mostrare la schermata di default.
        // O la console o la gui.
        aes::gui::show();
    }*/
}

} // namespace aes::arg