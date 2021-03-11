#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>


using namespace sf;
class FocusObject
{
public:
    virtual void setFocus() = 0;
    virtual void deleteFocus() = 0;
    virtual void event(const sf::Event&) = 0;
};



class TextBox : public Drawable, public FocusObject 
{
    void updateRect() {// ��������� ������� � ������� ����� ������������ �����
        FloatRect rect = m_text.getGlobalBounds(); //��� ������� ���������� ������� ������ � ���������� ������� ��������� 
        m_box.setPosition(rect.left - 5, rect.top - 5); //���������� �������
        m_box.setSize(Vector2f(rect.width + 10, rect.height + 10)); //������� �������
    }
    mutable RectangleShape m_box; //���������� �������� ������
    mutable Text m_text; //���������� ��������� � SFML ���������� �����
    mutable sf::String m_newText;//���������� ����������� �����
    mutable bool m_textChanged;//���������� ������� ����� ��������� ������ �� ����� 
public:
    TextBox(const Text& text) : m_text(text) {//����������� � �������  ����� ��������� ���������� ���� Text, ��� ���������� �������� ������������� ����� � ����
        m_newText = m_text.getString();//��������� ����� �������� �.�. ������� getString() ���������� ������
        updateRect();//��������� ������� �������
        m_box.setFillColor(sf::Color::Blue);//������ ���������� ���� �����
    }
    virtual void draw(RenderTarget& render, RenderStates states) const{//������� ��� ��������� ������(����������������)
        if (m_textChanged){//�������� ��� �� ������ �����
            const_cast<TextBox*>(this)->setText(m_newText);//����������������� �����
            m_textChanged = false;//������ �� "�� �������"
        }
        render.draw(m_box, states);//������ ������
        render.draw(m_text, states);//������ �����
    }
    virtual void setFocus(){//������� �������� ���� ��������� �������
        m_box.setFillColor(sf::Color::Red);//������ ���� �������
    }
    virtual void deleteFocus(){//������� ���������� ���� � �������� �� �����
        m_box.setFillColor(sf::Color::Blue);//������ ����� ����
    }
    virtual void event(const Event& event){//������� ��������� �������(� ������ ����� �����)
        if (event.type == Event::TextEntered){//������� �������
            m_textChanged = true;//������ �������� �� ����� "������"
            switch (event.text.unicode){//������������� �������� �������� � ���������� �� ������� ASCII
            case 0xD:m_newText; break; //��������� ������� ��� Return
            case 0x8://��������� ������� ��� "�������"
                if (!m_newText.isEmpty())
                    m_newText.erase(m_newText.getSize() - 1);break;
            default:{m_newText += static_cast<wchar_t>(event.text.unicode);}//� ������ ������� ���������� �������� � ���������� �������
            }
        }
    }
    void setText(const sf::String& str){//������� �������� ����� ������
        m_text.setString(str);//����� ������
        updateRect();//�������� ������� ������� ��� ����� ������
    }
    std::string GetText() { return m_newText; }//������� ������ ������
};



class FocusController{
    FocusObject* m_object;//�������� ���������� ���� ������������ ������
public:
    FocusController(FocusObject* obj = 0) : m_object(obj) { ; }//����������� � ����������
    void setFocusObject(FocusObject* new_obj){//������� ������� ����� ���������� ���� �������(�������� ��� �� ��������)
        if (m_object == new_obj)//���� ������������ ����� �� ��� ��������� �������
            return;//������ �� ����������
        if (m_object != 0)//������ ������ ������ �� ����������
            m_object->deleteFocus();//������ �� ����� ����
        if (new_obj != 0)//����� ������ ������ ���������� 
            new_obj->setFocus();//������������� � �������
        m_object = new_obj;//� ������ ������ �������� ����������� �����, ��� ������� ���������
    }
    FocusObject* getFocusObject(){//������� ����������� �������� ������
        return m_object;
    }
};
FocusController fc;



int menu(RenderWindow& window) { //������� ��� ��������� ����
    Texture menutexture1,menutexture2,menutexture3,menutexture4;//���������� ���������� ������� ����� ��������� �������� 
    menutexture1.loadFromFile("OpentBL.png");//�������� ����� "������� �������"
    menutexture2.loadFromFile("CLOSE.png");//�������� ����� "������� ���������"
    menutexture3.loadFromFile("Gausse.png");//�������� ����� "����� ������"
    menutexture4.loadFromFile("Jordan.png");//�������� ����� "����� ��������"
    Sprite menu1(menutexture1),menu2(menutexture2), menu3(menutexture3),menu4(menutexture4);//�������� �������� �� ����������� �������
    bool ismenu = true;//���� �������
    int menuvibor=0;//����� ������
    menu3.setPosition(100, 30);//��������� ���������� ������������ ������  "����� ������"
    menu4.setPosition(100, 90);//��������� ���������� ������������ ������ "����� ��������"
    menu1.setPosition(100, 150);//��������� ���������� ������������ ������ "������� �������"
    menu2.setPosition(100, 270);//��������� ���������� ������������ ������ "������� ���������"
    while (ismenu) {//���������� ���� ismenu=true
        Event event;//���������� ���������� "�������"
        while (window.pollEvent(event)){//��������� ����� �������
            if (event.type == Event::Closed)//���� ������������ ����� �� "�������" � ������ ������� ����
                window.close();//������� ����
        }
        menu1.setColor(Color::White);//������� � ������� �������� ��� �� ���� ������� ��� �����
        menu3.setColor(Color::White);//������� � ������� �������� ��� �� ���� ������� ��� �����
        menu4.setColor(Color::White);//������� � ������� �������� ��� �� ���� ������� ��� �����
        menu2.setColor(Color::White);//������� � ������� �������� ��� �� ���� ������� ��� �����
        window.clear(Color::Black);//���� �������� ���� ����
        if (IntRect(155, 100, 300, 40).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Blue); menuvibor = 3; };//���� ������� ������ �� ������, �� ������� ���� �� ����� � ����� ������ ����� "3"
        if (IntRect(155, 160, 300, 40).contains(Mouse::getPosition(window))) { menu4.setColor(Color::Blue); menuvibor = 4; };//���� ������� ������ �� ������, �� ������� ���� �� ����� � ����� ������ ����� "4"
        if (IntRect(155, 220, 300, 40).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menuvibor = 1; };//���� ������� ������ �� ������, �� ������� ���� �� ����� � ����� ������ ����� "1"
        if (IntRect(155, 330, 300, 40).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Blue); menuvibor = 2; };//���� ������� ������ �� ������, �� ������� ���� �� ����� � ����� ������ ����� "2"
        
        if (Mouse::isButtonPressed(Mouse::Left)) {//��������� ���
            if (menuvibor == 2) { ismenu = false; return 2; }//���� ����� ������ ����� ����, ���������� ����� 2 � ������� ���������
            if (menuvibor == 1) { ismenu = false; return 1; }//���� ����� ������ ����� �������, ���������� 1 � ������� �������
            if (menuvibor == 3) { ismenu = false; return 3; }//���� ����� ������ ����� ���, ���������� 3 � ������ ���������� ������ ������
            if (menuvibor == 4) { ismenu = false; return 4; }//���� ����� ������ ����� ������, ���������� 4 � ������ ���������� ������ ��������
        }
        window.draw(menu1);//��������� ������� � ����
        window.draw(menu3);//��������� ������� � ����
        window.draw(menu4);//��������� ������� � ����
        window.draw(menu2);//��������� ������� � ����
        window.display();//��������� ����
    }
}

template <typename RETURN_TYPE, typename STRING_TYPE>
RETURN_TYPE parse_string(const STRING_TYPE& str) {//������� ����������� ������ � ����� ������ ��� ������ 
    std::stringstream buf;//���������� ���������� ������ �� ��������� <sstream>
    buf << str;//������ �������� �������� �� ��������� ��������� ������������� 
    RETURN_TYPE val;//������������� ��� ������
    buf >> val;//�����������
    return val;//������ ���������� ��������
}


float* gauss(float** a, float* y, const int n)//������� ������ � ���������������� �����������
{
    float* x, max;//���������� ������� ����� ������� ������ �������, � ���������� ��� ������ ������������ ����� �� �������
    int k, index;//�������� ����������-������� �
    const double eps = 0.0001;  // ��������
    x = new float[n];//������������� ������������ ������ ���������� n
    k = 0;//������������� ����������-������� 
    while (k < n) { //���������� ���� ����������-������� ������ ������� �������
        max = abs(a[k][k]);//����� ����������� ����� �� ������ �� ������� ���������
        index = k;//��������� ������ ����������� �����
        for (int i = k + 1; i < n; i++) {//����� ������������� ����� �� �������
            if (abs(a[i][k]) > max) {//��������� ������ ����� � ������� 
                max = abs(a[i][k]);//���� ��� ������ ��������, �� ��� ���������� ������������
                index = i;//�������� ��� ������
            }
        }
        if (max < eps) {
            // ��� ��������� ������������ ���������
            std::cout << "������� �������� ���������� ��-�� �������� ������� ";
            std::cout << index << " ������� A" << std::endl;
            return 0;
        }
        for (int j = 0; j < n; j++) {//� ���� ����� �������� �����  ������ � ������������� ���������� �� �������
            float temp = a[k][j];
            a[k][j] = a[index][j];
            a[index][j] = temp;
        }
        float temp = y[k];//�������� ����� �������� ����� �����
        y[k] = y[index];
        y[index] = temp;
        // ������������ ��������� 
        for (int i = k; i < n; i++)
        {
            float temp = a[i][k];
            if (abs(temp) < eps) continue; // ��� �������� ������������ ����������
            for (int j = 0; j < n; j++)
                a[i][j] = a[i][j] / temp;
            y[i] = y[i] / temp;
            if (i == k)  continue; // ��������� �� �������� ���� �� ����
            for (int j = 0; j < n; j++)
                a[i][j] = a[i][j] - a[k][j];
            y[i] = y[i] - y[k];
        }
        k++;
    }
    // �������� �����������
    for (k = n - 1; k >= 0; k--)
    {
        x[k] = y[k];
        for (int i = 0; i < k; i++)
            y[i] = y[i] - a[i][k] * x[k];
    }
    return x;
}




std::string func_gauss(std::string str, int vib) {//������� ��� ����������� ������ � ������ ��������� ����� ������
    std::ofstream outputFile("matrix.txt");//�������� ����� � ������� ����� ������������� ������
    float** a, * y, * x;//����������: ������������� ���������� �������, ������������� �������
    int i, j;//���������� ����������
    std::string result;//���������� ���������� ������� ����� ������� �������� �� �����
    a = new float* [vib];//������������� �������� ������������� ���������� �������
    y = new float[vib];//�������������� ������������� ������� 
    for (i = 0; i < vib; i++)//���� ��� ������������� ����� ������������� ���������� �������
        a[i] = new float[vib]; //������������� ����� ������������� ���������� �������
    outputFile << str;//������ ���������� ������ ��� �������� � ����
    outputFile.close();//������� ����
    std::ifstream inputFile;//���������� ����� �� �������� ����� ����������� ������ �������
    inputFile.open("matrix.txt");//�������� ����� �� �������� ����� ����������� ������ �������
    i = 0;//������������� ���������
    j = 0;//������������� ���������
    while (!inputFile.eof() && i<vib) {//���������� ���� ����: �� ����� ����� � �������� ������ �������� �������������
        inputFile >> result;//������ �� �����  � ����������
        if ((result != ' ') && (j < vib)) { a[i][j] = parse_string<float>(result);}//���� ���������� �� ����� �������� ������� � ����� ������ �������� �������������
        else if (j==vib ){ y[i] = parse_string<float>(result); i++; j = -1; }//���� ����� ���� ���������� ��� � ���������� �������� ������������� - ����� ������
        j++;//��������� +1 � ��������� �����
    }
    inputFile.close();//������� ����� ����� ������
    x = gauss(a, y, vib);//������������� �������
    str.clear();//������� ������
    for (i = 0; i < vib; i++) {//���� ��� ������ � ��������� ������ ������
        str += "x[" + std::to_string(i + 1) + "]= " + std::to_string(x[i]) + '\n';//������ ������
    }  
    for (i = 0; i < vib; i++)//�������� ���������� �������
        delete[] a[i];//�������� ��� �����
    delete[] a,y;//�������� ����� �������
    return str;//����������� ������
}

std::string func_jordan(std::string str, int vib) {//������� ��� ����������� ������ � ������ ��������� ����� ��������
    std::ofstream outputFile("matrix.txt");//�������� ����� ������
    float** a,* x, ratio;//����������: ������������� ���������� �������, ������������� �������,
    int i, j,k;//���������� ����������
    std::string result;//���������� ���������� ������ ����� ������� �������� �� �����
    a = new float* [vib];//������������� �������� ���������� �������
    x = new float[vib];//������������� ������������� �������(�� ����� ������� ������)
    for (i = 0; i < vib; i++)//���� ��� ������������� ����� ������������� ���������� �������
        a[i] = new float[vib+1];//������������� ����� ������������� ���������� �������
    outputFile << str;//������ ���������� ������ ��� �������� � ����
    outputFile.close();//�������� �����
    std::ifstream inputFile;//���������� ���������� ����� ������
    inputFile.open("matrix.txt");//�������� ����� ������
    i = 0;//������������� ���������
    j = 0;//������������� ���������
    while (!inputFile.eof()) {//���������� ���� �� ����� �����
        inputFile >> result;//������ � ���������� �������� �� �����
        if ((result != ' ') && (j <= vib) && (i<vib)) { a[i][j] = parse_string<float>(result); }//��������������� ������ � ��� � ��������� ������
        else if (i<vib-1 && j>vib){ a[i + 1][0] = parse_string<float>(result); j = 0;i++; }//������ �������� � ������  �������� � ����� ������(��� �� �� ���������� �������� ��� �������� �� ����� ������)
        j++;//����������� ���������
    }
    inputFile.close();//�������� �����
    for (i = 0; i < vib; i++) {//������ ��������
        if (a[i][i] == 0.0) {//���� �����-�� �������� ����� ����� 0
            std::cout << "Mathematical Error!";//������
            exit(0);//�����
        }
        for (j = 0; j < vib; j++) {
            if (i != j) {//��� �� �� ��������� ������ (�������� ������ �� ������ ������)
                ratio = a[j][i] / a[i][i];//������� ������ ������ ������������ ����� �� ������� ��������� ��� ���������� �� ��� �������
                for (k = 0; k <= vib; k++) {
                    a[j][k] = a[j][k] - ratio * a[i][k];//������������� ����� ������ (���������� �������� ������ ������)
                }
            }
        }
    }
    for (i = 0; i < vib; i++) {
        x[i] = a[i][vib] / a[i][i];//������� ���������� �������� �� �������� ����� ����� �����
    }
    str.clear();//������� ������
    for (i = 0; i < vib; i++) {//���� ��� ������ ������
        str+="x[" + std::to_string(i+1) +"] = " + std::to_string(x[i])+'\n';//������ ������
    }
    for (i = 0; i < vib; i++)//�������� ���������� �������
        delete[] a[i];//�������� ��� �����
    delete[]  a,x;//�������� ����� �������
    return str;//����������� ������
}

void File_input(std::string str) {//������� ������ ��������� � ����
    std::ofstream outputFile("Resultat.txt");//�������� ����� ������
    outputFile << str;//������
    outputFile.close();//�������� �����
}

int main()
{
    int w;//���������� ���������� ��� ���������� ����
    RenderWindow app(VideoMode(800, 600), "Window");//���������� ���� � ��� �������� 
    Font font;//���������� ���������� ������
    Texture back;//���������� �������� 
    back.loadFromFile("Back.png");//�������� ����� ����� "��������� �����"
    Sprite Back(back);//������������� �������
    Back.setPosition(-160, 400);//����� ���������� ������� ����� "��������� �����"
    Text txt;//�������� ���������� ������� ����� ������� ������
    txt.setFont(font);//����� � �����
    txt.setPosition(50, 100);//����� ���������� ������
    TextBox tx(txt);//������� ������ � ����������� � ������ ��� �� ������
    int vib = 0;//���������� ������� ������ ���������� ����������� ������� �������������
    bool obrano = false;//���������� ������ ������� �������� "������ �� ����� ������������"
    Text txt1, txt2, txt3, txt4, txt5, txt6, eror, Resultat;//������� ���������� ��� ������ ����� 
    txt1.setFont(font);//����� �����
    txt1.setPosition(50, 100);//����� ����������
    txt2.setFont(font);//����� �����
    txt2.setPosition(50, 150);//����� ����������
    txt3.setFont(font);//����� �����
    txt3.setPosition(50, 200);//����� ����������
    txt4.setFont(font);//����� �����
    txt4.setPosition(50, 250);//����� ����������
    txt5.setFont(font);//����� �����
    txt5.setPosition(50, 300);//����� ����������
    txt6.setFont(font);//����� �����
    txt6.setPosition(50, 350);//����� ����������
    Resultat.setFont(font);//����� �����
    Resultat.setPosition(400, 100);//����� ����������
    eror.setFont(font);//����� �����
    eror.setPosition(100, 200);//����� ����������
    eror.setString("ERROR");//����� ��������� �����
    TextBox tx1(txt1), tx2(txt2), tx3(txt3), tx4(txt4), tx5(txt5), tx6(txt6);//������� ������ � ����������� � ������ ��� �� ������
   
    if (!font.loadFromFile("Roboto-Black.ttf")) {//�������� ������
        return EXIT_FAILURE;//� ������ �� ������ ��������, �������� ������
    }
    Back_menu:w = menu(app);//�������� ���� ����� �������������
    switch (w)//����� ������������
    {
    case 3: { //� ������ ���� ����� ������ ����� "3"
        fc.setFocusObject(&tx);
            while (!obrano) {//���������� ���� ������������ �� ������� �����
                Texture vvod;//���������� ��������
                vvod.loadFromFile("Vvod.png");//�������� ����� ����� "������� ����������� �����������"
                Sprite Vvod(vvod);//������������� �������
                Vvod.setPosition(80, -40);//����� ���������� ������� ����� "������� ����������� �����������"
                Event event;//�������� ���������� �������
                while (app.pollEvent(event)) {//��������� ����� �������
                    if (event.type == Event::Closed)//���� ������������ ����� �� "�������" � ������ ������� ����
                        app.close();//������� ����
                    Back.setColor(Color::White);//����� ���� ������� ��� ����� "��������� �����"
                    if (Mouse::isButtonPressed(Mouse::Left)) {//��������� ������� ������� ���
                        if (IntRect(-120, 470, 300, 40).contains(Mouse::getPosition(app))) { goto Back_menu; }//��������� ����� � ����
                    }
                    if (IntRect(-120, 470, 300, 40).contains(Mouse::getPosition(app))) { Back.setColor(Color::Blue); }//���� ����� �������� �� ����� "��������� �����", �������� ���� �� �����
                    if (Keyboard::isKeyPressed(Keyboard::Enter) && tx.GetText()!="") { vib = std::stoi(tx.GetText()); obrano = true; }//������������  ������ � ������������� ��� 
                    FocusObject* fo = fc.getFocusObject();//������� ��������� ����� ��������� ������
                    if (fo != 0) fo->event(event);//����������� ��� ��������� ������� ������� (������� TextBox::event())
                }
                app.clear();//����� ���� ����
                app.draw(tx);//��������� ������ � ������� ����� ������������� ������ �������� �������������
                app.draw(Back);//��������� ������ �� ������ "��������� �����"
                app.draw(Vvod);//��������� ������ �� ������ "������� ����������� �����������"
                app.display();//��������� ����
            }
            while (app.isOpen()) {//���� ���� �������
                Event event;//������� ���������� �������
                while (app.pollEvent(event)) {//��������� ����� �������
                    if (event.type == Event::Closed)//���� ������������ ����� �� "�������" � ������ ������� ����
                        app.close();//������� ����
                    Back.setColor(Color::White);//����� ���� ������� ��� ����� "��������� �����"
                    if (Mouse::isButtonPressed(Mouse::Left)) {//��������� ������� ������� ���
                        if (IntRect(50, 100, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx1); };//���� ����� ���� �� ���� �����������,����� ����� �� ���� ������
                        if (IntRect(50, 150, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx2); };//���� ����� ���� �� ���� �����������,����� ����� �� ���� ������
                        if (IntRect(50, 200, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx3); };//���� ����� ���� �� ���� �����������,����� ����� �� ���� ������
                        if (IntRect(50, 250, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx4); };//���� ����� ���� �� ���� �����������,����� ����� �� ���� ������
                        if (IntRect(50, 300, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx5); };//���� ����� ���� �� ���� �����������,����� ����� �� ���� ������
                        if (IntRect(50, 350, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx6); };//���� ����� ���� �� ���� �����������,����� ����� �� ���� ������
                        if (IntRect(-120, 470, 300, 40).contains(Mouse::getPosition(app))) { goto Back_menu; }//���� ����� ���� �� ���� �����������,����������� �����
                    }
                    if (IntRect(-120, 470, 300, 40).contains(Mouse::getPosition(app))) { Back.setColor(Color::Blue); }//���� ������ ������ �� ���� �����������, �������� ���� ������ �� �����
                    if (Keyboard::isKeyPressed(Keyboard::Enter) && (tx1.GetText()!="" && tx2.GetText()!="")) {//��������� ������� ������� ������� Enter
                        switch (vib) {
                        case 2: {
                            std::string str = tx1.GetText() + "\n" + tx2.GetText();//��������� � ������ ������(�������� �������������)
                            Resultat.setString(func_gauss(str, vib));//����������� ����� �������, ������� ���������� ����� ���� ������
                        }break;
                        case 3: {
                            if (tx3.GetText() != "") {//���������� ��� �������, ���� ��� ������� ���������
                                std::string str = tx1.GetText() + "\n" + tx2.GetText() + '\n' + tx3.GetText();//��������� � ������ ������(�������� �������������)
                                Resultat.setString(func_gauss(str, vib));//����������� ����� �������, ������� ���������� ����� ���� ������
                            }
                        }break;
                        case 4: {
                            if (tx3.GetText() != "" && tx4.GetText() != "") {//���������� ��� �������, ���� ��� ������� ���������
                                std::string str = tx1.GetText() + "\n" + tx2.GetText() + '\n' + tx3.GetText() + '\n' + tx4.GetText();//��������� � ������ ������(�������� �������������)
                                Resultat.setString(func_gauss(str, vib));//����������� ����� �������, ������� ���������� ����� ���� ������
                            }
                        }break;
                        case 5: {
                            if (tx3.GetText() != "" && tx4.GetText() != "" && tx5.GetText() != "") {//���������� ��� �������, ���� ��� ������� ���������
                                std::string str = tx1.GetText() + "\n" + tx2.GetText() + '\n' + tx3.GetText() + '\n' + tx4.GetText() + '\n' + tx5.GetText();//��������� � ������ ������(�������� �������������)
                                Resultat.setString(func_gauss(str, vib));//����������� ����� �������, ������� ���������� ����� ���� ������
                            }
                        }break;
                        case 6: {
                            if (tx3.GetText() != "" && tx4.GetText() != "" && tx5.GetText() != "" && tx6.GetText() != "") {//���������� ��� �������, ���� ��� ������� ���������
                                std::string str = tx1.GetText() + "\n" + tx2.GetText() + '\n' + tx3.GetText() + '\n' + tx4.GetText() + '\n' + tx5.GetText() + tx6.GetText();//��������� � ������ ������(�������� �������������)
                                Resultat.setString(func_gauss(str, vib));//����������� ����� �������, ������� ���������� ����� ���� ������
                            }
                        }break;
                        default: std::cout << "Error";
                        }
                        File_input(Resultat.getString());//�������� ����� � ����
                    }
                    FocusObject* fo = fc.getFocusObject();//������� ��������� ����� ��������� ������
                    if (fo != 0) fo->event(event);//����������� ��� ��������� ������� ������� (������� TextBox::event())
                }
                app.clear();//����� ���� ���� ����
                app.draw(Back);//����� ������ "��������� �����"
                switch (vib)//��������� ������� � ����������� �� ��������� ������������� ��������� 
                {
                case 2: {app.draw(tx1); app.draw(tx2); }break;
                case 3: {app.draw(tx1); app.draw(tx2); app.draw(tx3); }break;
                case 4: {app.draw(tx1); app.draw(tx2); app.draw(tx3); app.draw(tx4); }break;
                case 5: {app.draw(tx1); app.draw(tx2); app.draw(tx3); app.draw(tx4); app.draw(tx5); }break;
                case 6: {app.draw(tx1); app.draw(tx2); app.draw(tx3); app.draw(tx4); app.draw(tx5); app.draw(tx6); }break;
                default: app.draw(eror); break;
                }
                app.draw(Resultat);//����� ��������� ����������
                app.display();//��������� ����
            }

        }break;
    case 4: {//� ������ ���� ����� ������ ����� "4"
            fc.setFocusObject(&tx);
            while (!obrano) {//���������� ���� ������������ �� ������� �����
                Texture vvod;//���������� ��������
                vvod.loadFromFile("Vvod.png");//�������� ����� ����� "������� ����������� �����������"
                Sprite Vvod(vvod);//������������� �������
                Vvod.setPosition(80, -40);//����� ���������� ������� ����� "������� ����������� �����������"
                Event event;//������� ���������� �������
                while (app.pollEvent(event)) {//��������� ����� �������
                    if (event.type == Event::Closed)//���� ������������ ����� �� "�������" � ������ ������� ����
                        app.close();//������� ����
                    Back.setColor(Color::White);//����� ���� ������� ��� ����� "��������� �����"
                    if (Mouse::isButtonPressed(Mouse::Left)) {//��������� ������� ������� ���
                        if (IntRect(-120, 470, 300, 40).contains(Mouse::getPosition(app))) { goto Back_menu; }//��������� ����� � ����
                    }
                    if (IntRect(-120, 470, 300, 40).contains(Mouse::getPosition(app))) { Back.setColor(Color::Blue); }//���� ����� �������� �� ����� "��������� �����", �������� ���� �� �����
                    if (Keyboard::isKeyPressed(Keyboard::Enter) && tx.GetText()!="") { vib = std::stoi(tx.GetText()); obrano = true; }//������������  ������ � ������������� ��� 
                    FocusObject* fo = fc.getFocusObject();//������� ��������� ����� ��������� ������
                    if (fo != 0) fo->event(event);//����������� ��� ��������� ������� ������� (������� TextBox::event())
                }
                app.clear();//����� ���� ���� ����
                app.draw(tx);//��������� ������ � ������� ����� ������������� ������ �������� �������������
                app.draw(Back);//��������� ������ �� ������ "��������� �����"
                app.draw(Vvod);//��������� ������ �� ������ "������� ����������� �����������"
                app.display();//��������� ����
            }
            while (app.isOpen()) {//���� ���� �������
                Event event;//������� ���������� �������
                while (app.pollEvent(event)) {//��������� ����� �������
                    if (event.type == Event::Closed)//���� ������������ ����� �� "�������" � ������ ������� ����
                        app.close();//������� ����
                    Back.setColor(Color::White);//����� ���� ������� ��� ����� "��������� �����"
                    if (Mouse::isButtonPressed(Mouse::Left)) {//��������� ������� ������� ���
                        if (IntRect(50, 100, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx1); };//���� ����� ���� �� ���� �����������,����� ����� �� ���� ������
                        if (IntRect(50, 150, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx2); };//���� ����� ���� �� ���� �����������,����� ����� �� ���� ������
                        if (IntRect(50, 200, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx3); };//���� ����� ���� �� ���� �����������,����� ����� �� ���� ������
                        if (IntRect(50, 250, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx4); };//���� ����� ���� �� ���� �����������,����� ����� �� ���� ������
                        if (IntRect(50, 300, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx5); };//���� ����� ���� �� ���� �����������,����� ����� �� ���� ������
                        if (IntRect(50, 350, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx6); };//���� ����� ���� �� ���� �����������,����� ����� �� ���� ������
                        if (IntRect(-120, 470, 300, 40).contains(Mouse::getPosition(app))) { goto Back_menu; }//���� ����� ���� �� ���� �����������,����������� �����
                    }
                    if (IntRect(-120, 470, 300, 40).contains(Mouse::getPosition(app))) { Back.setColor(Color::Blue); }//���� ������ ������ �� ���� �����������, �������� ���� ������ �� �����
                    if (Keyboard::isKeyPressed(Keyboard::Enter) && (tx1.GetText() != "" && tx2.GetText() != "")) {//��������� ������� ������� ������� Enter
                        switch (vib) {
                        case 2: {
                            std::string str = tx1.GetText() + "\n" + tx2.GetText();//��������� � ������ ������(�������� �������������)
                            Resultat.setString(func_jordan(str, vib));//����������� ����� �������, ������� ���������� ����� ���� ������
                        }break;
                        case 3: {
                            if (tx3.GetText() != "") {//���������� ��� �������, ���� ��� ������� ���������
                                std::string str = tx1.GetText() + "\n" + tx2.GetText() + '\n' + tx3.GetText();//��������� � ������ ������(�������� �������������)
                                Resultat.setString(func_jordan(str, vib));//����������� ����� �������, ������� ���������� ����� ���� ������
                            }
                        }break;
                        case 4: {
                            if (tx3.GetText() != "" && tx4.GetText() != "") {//���������� ��� �������, ���� ��� ������� ���������
                                std::string str = tx1.GetText() + "\n" + tx2.GetText() + '\n' + tx3.GetText() + '\n' + tx4.GetText();//��������� � ������ ������(�������� �������������)
                                Resultat.setString(func_jordan(str, vib));//����������� ����� �������, ������� ���������� ����� ���� ������
                            }
                        }break;
                        case 5: {
                            if (tx3.GetText() != "" && tx4.GetText() != "" && tx5.GetText() != "") {//���������� ��� �������, ���� ��� ������� ���������
                                std::string str = tx1.GetText() + "\n" + tx2.GetText() + '\n' + tx3.GetText() + '\n' + tx4.GetText() + '\n' + tx5.GetText();//��������� � ������ ������(�������� �������������)
                                Resultat.setString(func_jordan(str, vib));//����������� ����� �������, ������� ���������� ����� ���� ������
                            }
                        }break;
                        case 6: {
                            if (tx3.GetText() != "" && tx4.GetText() != "" && tx5.GetText() != "" && tx6.GetText() != "") {//���������� ��� �������, ���� ��� ������� ���������
                                std::string str = tx1.GetText() + "\n" + tx2.GetText() + '\n' + tx3.GetText() + '\n' + tx4.GetText() + '\n' + tx5.GetText() + tx6.GetText();//��������� � ������ ������(�������� �������������)
                                Resultat.setString(func_jordan(str, vib));//����������� ����� �������, ������� ���������� ����� ���� ������
                            }
                        }break;
                        default: std::cout << "Error";
                        }
                        File_input(Resultat.getString());//�������� ����� � ����
                    }
                    FocusObject* fo = fc.getFocusObject();//������� ��������� ����� ��������� ������
                    if (fo != 0) fo->event(event);//����������� ��� ��������� ������� ������� (������� TextBox::event())

                }
                app.clear();//����� ���� ���� ����
                app.draw(Back);//����� ������ "��������� �����"
                switch (vib)//��������� ������� � ����������� �� ��������� ������������� ��������� 
                {
                case 2: {app.draw(tx1); app.draw(tx2); }break;
                case 3: {app.draw(tx1); app.draw(tx2); app.draw(tx3); }break;
                case 4: {app.draw(tx1); app.draw(tx2); app.draw(tx3); app.draw(tx4); }break;
                case 5: {app.draw(tx1); app.draw(tx2); app.draw(tx3); app.draw(tx4); app.draw(tx5); }break;
                case 6: {app.draw(tx1); app.draw(tx2); app.draw(tx3); app.draw(tx4); app.draw(tx5); app.draw(tx6); }break;
                default: app.draw(eror); break;
                }
                app.draw(Resultat);//����� ��������� ����������
                app.display();//��������� ����
            }
        }break;
    case 1: {system("Resultat.txt"); goto Back_menu; }break;//� ������ ���� ����� ������ ����� "1", ������� ������� � �������
    case 2: return 0; break;//������� ���������
    default:std::cout << "Error"; break;//������
    }
}