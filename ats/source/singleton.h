//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_SINGLETON_H
#define ATS_SINGLETON_H

namespace ats {

    template<class T>
    class Singleton {
    public:
        static T &instance() {
            static T _instance;
            return _instance;
        }

    public:
        Singleton(const Singleton &) = delete;

        Singleton &operator=(const Singleton &) = delete;

    protected:
        Singleton() = default;

        virtual ~Singleton() = default;
    };

}

#endif //ATS_SINGLETON_H
