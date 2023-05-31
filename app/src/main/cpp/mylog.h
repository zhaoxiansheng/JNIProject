//
// Created by zhaoyu on 2023/5/31.
//

#ifndef JNIPROJECT_MYLOG_H
#define JNIPROJECT_MYLOG_H

#include <iostream>

#define LOG_LEVEL_ERROR             (1)
#define LOG_LEVEL_WARNING           (2)
#define LOG_LEVEL_INFO              (3)
#define LOG_LEVEL_DEBUG             (4)

#define LOG_ERROR(...)          Logout(LOG_LEVEL_ERROR, __VA_ARGS__);
#define LOG_WARNING(...)        Logout(LOG_LEVEL_WARNING, __VA_ARGS__);
#define LOG_INFO(...)           Logout(LOG_LEVEL_INFO, __VA_ARGS__);
#define LOG_DEBUG(...)          Logout(LOG_LEVEL_DEBUG, __VA_ARGS__);

class mylog {
};


template<class ...Args>
void Logout(int _log_level, Args... args)
{
    auto print = [](auto i){std::cout<< i << " ";};
    std::initializer_list<int>{(print(args),0)...};
    std::cout<< std::endl;
}
#endif //JNIPROJECT_MYLOG_H
