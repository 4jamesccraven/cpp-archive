#ifndef CPP_ARCHIVE_COMMON
#define CPP_ARCHIVE_COMMON

#include <string>
#include <unordered_map>

using MainType = int(*)();

struct ProjectDispatch
{

    static std::unordered_map<std::string, MainType>& get() {
        static std::unordered_map<std::string, MainType> map;
        return map;
    }

    ProjectDispatch(const std::string& name, MainType f)
    {
        get()[name] = f;
    }
};

#endif
