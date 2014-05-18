#include "../src/converter.h"

#include <map>
#include <string>
#include <iostream>
#include <algorithm>

namespace
{
    std::string tablesRoot;
    const char DIR_SEPARATOR =
    #ifdef WIN32
            '\\'
    #else
            '/'
    #endif
            ;

    std::string getTablePath(std::string const& tableName)
    {
        return tablesRoot + tableName + ".trans";
    }
}

bool test_CP895()
{
    // příšerně žluťoučký kůň úpěl ďábelské ódy in CP895
    const char IN[] =
            // příšerně
            "p\xa9\xa1\xa8" "ern\x88 "
            // žluťoucký
            "\x91lu\x9fou\x87k\x98 "
            // kůň
            "k\x96\xa4 "
            // úpěl
            "\xa3p\x88l "
            // ďábelské
            "\x83\xa0" "belsk\x82 "
            // ódy
            "\xa2" "dy";

    // příšerně žluťoučký kůň úpěl ďábelské ódy in UTF-8
    const char EXPECTED[] =
            // příšerně
            "p\xc5\x99\xc3\xad\xc5\xa1" "ern\xc4\x9b "
            // žluťoucký
            "\xc5\xbelu\xc5\xa5" "ou\xc4\x8dk\xc3\xbd "
            // kůň
            "k\xc5\xaf\xc5\x88 "
            // úpěl
            "\xc3\xbap\xc4\x9bl "
            // ďábelské
            "\xc4\x8f\xc3\xa1" "belsk\xc3\xa9 "
            // ódy
            "\xc3\xb3" "dy";


    cp2utf8::Converter converter(getTablePath("cp895"));

    const std::string out = converter.convert(IN);

    if (out != EXPECTED)
    {
        size_t idx = 0;
        while (idx < std::min(out.size(), sizeof(EXPECTED)-1) && out[idx] == EXPECTED[idx])
            ++idx;

        std::cerr << "failed at index " << idx << std::endl;
        return false;
    }

    std::cout << "passed" << std::endl;
    return true;
}

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        const char *progname = (argc > 0) ? argv[0] : "test";
        std::cerr << "Usage: " << progname << " path_to_translation_tables" << std::endl;
        return 1;
    }
    tablesRoot = argv[1] + std::string(1, DIR_SEPARATOR);

    return test_CP895() ? EXIT_SUCCESS : EXIT_FAILURE;
}
