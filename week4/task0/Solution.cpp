#include "Common.h"

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`


class MyShape : public IShape {
    shared_ptr<ITexture> m_texture;
    ShapeType m_type;
    Size m_size;
    Point m_point;
public:
    MyShape(ShapeType shape_type) : m_type(shape_type),
                                    m_size({0, 0}),
                                    m_point({0, 0}) {
        
    }
    ~MyShape() = default;

    // Возвращает точную копию фигуры.
    // Если фигура содержит текстуру, то созданная копия содержит ту же самую
    // текстуру. Фигура и её копия совместно владеют этой текстурой.
    virtual std::unique_ptr<IShape> Clone() const {
        std::unique_ptr<IShape> clone = MakeShape(m_type);
        clone->SetPosition(GetPosition());
        clone->SetTexture(m_texture);
        clone->SetSize(GetSize());

        return clone;
    }

    virtual void SetPosition(Point point) {
        m_point = point;
    }
    virtual Point GetPosition() const {
        return m_point;
    }

    virtual void SetSize(Size size) {
        m_size = size;
    }
    virtual Size GetSize() const {
        return m_size;
    }

    virtual void SetTexture(std::shared_ptr<ITexture> texture) {
        m_texture = move(texture);
    }
    virtual ITexture* GetTexture() const {
        return m_texture.get();
    }

    // Рисует фигуру на указанном изображении
    virtual void Draw(Image&) const = 0;
};

class Rect : public MyShape {
public:
    Rect() : MyShape(ShapeType::Rectangle) {

    }
    ~Rect() {

    }

    virtual void Draw(Image& image) const {
        for (size_t i = 0; i < GetSize().height && i < image.size() - GetPosition().y; ++i) {
            for (size_t j = 0; j < GetSize().width && j < image[0].size() - GetPosition().x; ++j) {
                if (GetTexture() && i < GetTexture()->GetSize().height &&
                    j < GetTexture()->GetSize().width) {
                    image[i + GetPosition().y][j + GetPosition().x] = GetTexture()->GetImage()[i][j];
                }
                else {
                    image[i + GetPosition().y][j + GetPosition().x] = '.';
                }
            }
        }
    }
};

class Ellipce : public MyShape {
public:
    Ellipce() : MyShape(ShapeType::Ellipse) {

    }
    ~Ellipce() {

    }

    virtual void Draw(Image& image) const {
        for (size_t i = 0; i < GetSize().height && i < image.size() - GetPosition().y; ++i) {
            for (size_t j = 0; j < GetSize().width && j < image[0].size() - GetPosition().x; ++j) {
                if (!IsPointInEllipse({static_cast<int>(j), static_cast<int>(i)}, {GetSize().width, GetSize().height})) {
                    continue;
                }
                if (GetTexture() && i < GetTexture()->GetSize().height &&
                    j < GetTexture()->GetSize().width) {
                    image[i + GetPosition().y][j + GetPosition().x] = GetTexture()->GetImage()[i][j];
                }
                else {
                    image[i + GetPosition().y][j + GetPosition().x] = '.';
                }
            }
        }
    }
};

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
    if (shape_type == ShapeType::Rectangle) {
        return make_unique<Rect> ();
    }
    else {
        return make_unique<Ellipce> ();
    }
    
}

// #include <iostream>
// #include <memory>
// #include <string>
// #include <vector>

// using namespace std;

// class MyTest {
//     int m_num;
// public:
//     MyTest(int num) : m_num(num) {
//         cout << "Created" << m_num << endl;
//     }

//     ~MyTest() {
//         cout << "Deleted" << m_num << endl;
//     }

//     std::unique_ptr<MyTest> Clone() const {
//         return unique_ptr<MyTest> (new MyTest(11));
//     }
// };

// int main() {
//     auto ptr1 = make_shared<MyTest> (11);
//     auto ptr2 = make_shared<MyTest> (22);
//     ptr1 = ptr2;
//     cout << "aaaaa" << endl;
// }
