#pragma opnce

#include <iostream>

namespace RAII {
template<typename Provider>
class Booking {
    Provider *m_provider;
    int m_counter;
public:
    Booking(Provider *provider, int counter) : m_provider(provider),
                                               m_counter(counter) 
    {

    }
    Booking(const Booking &other) = delete;
    Booking(Booking &&other) : m_provider(other.m_provider), m_counter(other.m_counter) {
        other.m_provider = nullptr;
        other.m_counter = 0;
    }
    
    Booking& operator =(const Booking &other) = delete;
    Booking& operator =(Booking &&other) {
        m_provider = other.m_provider;
        m_counter = other.m_counter;
        other.m_provider = nullptr;
        other.m_counter = 0;

        return *this;
    }

    ~Booking() {
        if (m_counter && m_provider) {
            m_provider->CancelOrComplete(*this);
        }
    }

};
}