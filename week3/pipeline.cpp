#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>

using namespace std;


struct Email {
    string from;
    string to;
    string body;
};

std::ostream& operator << (std::ostream& out, const Email& email) {
    out << email.from << endl << email.to << endl << email.body << endl;
    return out;
}

std::istream& operator >> (std::istream& in, Email& email) {
    string line;

    getline(in, email.from);
    getline(in, email.to);
    getline(in, email.body);
    return in;
}


class Worker {
    unique_ptr<Worker> m_next = nullptr;
public:
    virtual ~Worker() = default;
    virtual void Process(unique_ptr<Email> email) = 0;
    virtual void Run() {
        // только первому worker-у в пайплайне нужно это имплементировать
        throw logic_error("Unimplemented");
    }

protected:
    // реализации должны вызывать PassOn, чтобы передать объект дальше
    // по цепочке обработчиков
    void PassOn(unique_ptr<Email> email) const {
        if (m_next) {
            m_next->Process(move(email));
        }
    }

public:
    void SetNext(unique_ptr<Worker> next) {
        if (m_next) {
            m_next->SetNext(move(next));
        }
        else {
            m_next = move(next);
        }
    }
};


class Reader : public Worker {
    istream *m_stream;
public:
    Reader(istream& in) : Worker(), m_stream(&in) {
        //
    }

    void Run() {
        istream_iterator<Email> start(*m_stream);
        istream_iterator<Email> finish;

        while (start != finish) {
            PassOn(make_unique<Email>(*start++));
        }
    }

    void Process(unique_ptr<Email> email) {
        //
    }
};


class Filter : public Worker {
public:
    using Function = function<bool(const Email&)>;

    Function m_func;
public:
    Filter(Function func) : Worker(), m_func(func) {
        
    }

    void Process(unique_ptr<Email> email) {
        if (m_func(*email)) {
            PassOn(move(email));
        }
    }
};


class Copier : public Worker {
    const string m_address;
public:
    Copier(const string& address) : Worker(), m_address(address) {
        //
    }

    void Process(unique_ptr<Email> email) {
        if (email->to != m_address) {
            Email new_mail = {email->from, m_address, email->body};
            PassOn(move(email));
            PassOn(make_unique<Email>(new_mail));
        }
        else {
            PassOn(move(email));
        }
    }
    
};

class Sender : public Worker {
    ostream *m_stream;
public:
    Sender(ostream &out) : Worker(), m_stream(&out) {
        //
    }
    void Process(unique_ptr<Email> email) {
        *m_stream << *email;
        PassOn(move(email));
    }
};


// реализуйте класс
class PipelineBuilder {
    unique_ptr<Worker> chain = nullptr;
public:
    // добавляет в качестве первого обработчика Reader
    explicit PipelineBuilder(istream& in) {
        chain = make_unique<Reader>(in);
    }

    // добавляет новый обработчик Filter
    PipelineBuilder& FilterBy(Filter::Function filter) {
        chain->SetNext(make_unique<Filter>(filter));
        return *this;
    }

    // добавляет новый обработчик Copier
    PipelineBuilder& CopyTo(string recipient) {
        chain->SetNext(make_unique<Copier>(recipient));
        return *this;
    }

    // добавляет новый обработчик Sender
    PipelineBuilder& Send(ostream& out) {
        chain->SetNext(make_unique<Sender>(out));
        return *this;
    }

    // возвращает готовую цепочку обработчиков
    unique_ptr<Worker> Build() {
        return move(chain);
    }
};

// bool isone(const Email& mail) {
//     return mail.from == "1";
// }

// int main() {
    // stringstream input("Tiger Wolf Fox 1 2 3");
    // PipelineBuilder builder(input);

    // builder.FilterBy(isone);
    // builder.CopyTo("7");
    // builder.Send(cout);

    // auto runer = builder.Build();
    // runer->Run();
// }
