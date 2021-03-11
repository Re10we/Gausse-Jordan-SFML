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
    void updateRect() {// Рисование спрайта в котором будет отображаться текст
        FloatRect rect = m_text.getGlobalBounds(); //эта функция возвращает границы текста в глобальной системе координат 
        m_box.setPosition(rect.left - 5, rect.top - 5); //координаты спрайта
        m_box.setSize(Vector2f(rect.width + 10, rect.height + 10)); //размеры спрайта
    }
    mutable RectangleShape m_box; //переменная хранящая спрайт
    mutable Text m_text; //переменная выводящая в SFML полученный текст
    mutable sf::String m_newText;//переменная сохраняющая текст
    mutable bool m_textChanged;//переменная которая будет указывать изменён ли текст 
public:
    TextBox(const Text& text) : m_text(text) {//конструктор в который  будем вводиться переменная типа Text, что отображает набраный пользователем текст в окне
        m_newText = m_text.getString();//задаеться новое значение т.к. функция getString() возвращает строку
        updateRect();//обновляем границы спрайта
        m_box.setFillColor(sf::Color::Blue);//задаем стандарный цвет Синий
    }
    virtual void draw(RenderTarget& render, RenderStates states) const{//Функция для изменения текста(переопределенная)
        if (m_textChanged){//проверка был ли изменён текст
            const_cast<TextBox*>(this)->setText(m_newText);//переопределяеться тескт
            m_textChanged = false;//меняем на "не изменен"
        }
        render.draw(m_box, states);//рисуем спрайт
        render.draw(m_text, states);//рисуем текст
    }
    virtual void setFocus(){//Функция задающая цвет поточнуму спрайту
        m_box.setFillColor(sf::Color::Red);//задаем цвет красный
    }
    virtual void deleteFocus(){//Функция изменяющая цвет с красного на синий
        m_box.setFillColor(sf::Color::Blue);//задаем синий цвет
    }
    virtual void event(const Event& event){//функция обработни события(в данном случе ввода)
        if (event.type == Event::TextEntered){//условие события
            m_textChanged = true;//меняем значение на текст "Изменён"
            switch (event.text.unicode){//просматриваем вводимые значения с клавиатуры по таблице ASCII
            case 0xD:m_newText; break; //отдельная команда для Return
            case 0x8://отдельная команда для "Пробела"
                if (!m_newText.isEmpty())
                    m_newText.erase(m_newText.getSize() - 1);break;
            default:{m_newText += static_cast<wchar_t>(event.text.unicode);}//в других случаях записываем вводимые с клавиатуры символы
            }
        }
    }
    void setText(const sf::String& str){//функция задающая новую строку
        m_text.setString(str);//новая строка
        updateRect();//изменяем размеры спрайта для новой строки
    }
    std::string GetText() { return m_newText; }//функция вывода текста
};



class FocusController{
    FocusObject* m_object;//создание переменной типа виртуального класса
public:
    FocusController(FocusObject* obj = 0) : m_object(obj) { ; }//конструктор с параметром
    void setFocusObject(FocusObject* new_obj){//функция которая будет определять цвет спрайта(актывный или не активный)
        if (m_object == new_obj)//если пользователь нажал по уже поточнуму спрайту
            return;//ничего не происходит
        if (m_object != 0)//старый обьект делаем не выделяемым
            m_object->deleteFocus();//меняем на синий цвет
        if (new_obj != 0)//новый обьект делаем выделяемым 
            new_obj->setFocus();//окрашиваеться в красный
        m_object = new_obj;//и теперь старым обьектом становиться новый, для будущих сравнений
    }
    FocusObject* getFocusObject(){//функция возращающая поточный обьект
        return m_object;
    }
};
FocusController fc;



int menu(RenderWindow& window) { //функция для отрисовки меню
    Texture menutexture1,menutexture2,menutexture3,menutexture4;//обьявление переменных которые будут загружать текстуру 
    menutexture1.loadFromFile("OpentBL.png");//текстура слова "Открыть блокнот"
    menutexture2.loadFromFile("CLOSE.png");//текстура слова "Закрыть программу"
    menutexture3.loadFromFile("Gausse.png");//текстура слова "Метод Гаусса"
    menutexture4.loadFromFile("Jordan.png");//текстура слова "Метод Джардана"
    Sprite menu1(menutexture1),menu2(menutexture2), menu3(menutexture3),menu4(menutexture4);//создание спрайтов из загруженных текстур
    bool ismenu = true;//меню открыто
    int menuvibor=0;//номер выбора
    menu3.setPosition(100, 30);//задаються координаты расположения кнопки  "Метод Гаусса"
    menu4.setPosition(100, 90);//задаються координаты расположения кнопки "Метод Джордана"
    menu1.setPosition(100, 150);//задаються координаты расположения кнопки "Открыть блокнот"
    menu2.setPosition(100, 270);//задаються координаты расположения кнопки "Закрыть программу"
    while (ismenu) {//продолжать пока ismenu=true
        Event event;//обьявления переменной "события"
        while (window.pollEvent(event)){//обработка окном события
            if (event.type == Event::Closed)//если пользователь нажал на "Закрыть" в правом верхнем углу
                window.close();//закрыть окно
        }
        menu1.setColor(Color::White);//спрайту с текстом задаёться тот же цвет который она имела
        menu3.setColor(Color::White);//спрайту с текстом задаёться тот же цвет который она имела
        menu4.setColor(Color::White);//спрайту с текстом задаёться тот же цвет который она имела
        menu2.setColor(Color::White);//спрайту с текстом задаёться тот же цвет который она имела
        window.clear(Color::Black);//окну задаёться цвет фона
        if (IntRect(155, 100, 300, 40).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Blue); menuvibor = 3; };//если навести мышкой на спрайт, он пеменят цвет на синий и номер выбора будет "3"
        if (IntRect(155, 160, 300, 40).contains(Mouse::getPosition(window))) { menu4.setColor(Color::Blue); menuvibor = 4; };//если навести мышкой на спрайт, он пеменят цвет на синий и номер выбора будет "4"
        if (IntRect(155, 220, 300, 40).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menuvibor = 1; };//если навести мышкой на спрайт, он пеменят цвет на синий и номер выбора будет "1"
        if (IntRect(155, 330, 300, 40).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Blue); menuvibor = 2; };//если навести мышкой на спрайт, он пеменят цвет на синий и номер выбора будет "2"
        
        if (Mouse::isButtonPressed(Mouse::Left)) {//обработка ЛКМ
            if (menuvibor == 2) { ismenu = false; return 2; }//если номер выбора равен двум, возвратить число 2 и закрыть программу
            if (menuvibor == 1) { ismenu = false; return 1; }//если номер выбора равен еденици, возвратить 1 и открыть блокнот
            if (menuvibor == 3) { ismenu = false; return 3; }//если номер выбора равен трём, возвратить 3 и начать выполнения метода Гаусса
            if (menuvibor == 4) { ismenu = false; return 4; }//если номер выбора равен четырём, возвратить 4 и начать выполнения метода Джордана
        }
        window.draw(menu1);//отрисовка спрайта в окне
        window.draw(menu3);//отрисовка спрайта в окне
        window.draw(menu4);//отрисовка спрайта в окне
        window.draw(menu2);//отрисовка спрайта в окне
        window.display();//обработка окна
    }
}

template <typename RETURN_TYPE, typename STRING_TYPE>
RETURN_TYPE parse_string(const STRING_TYPE& str) {//функция коевертации строки в любой другой тип данных 
    std::stringstream buf;//обьявления переменной строки из директывы <sstream>
    buf << str;//строка получает значения из параметра введённого пользователем 
    RETURN_TYPE val;//возвращаеммый тип данных
    buf >> val;//конвертация
    return val;//возват полученого значения
}


float* gauss(float** a, float* y, const int n)//функция Гаусса с последовательным исключением
{
    float* x, max;//переменная которая будет хранить массив ответов, и переменная что хранит максимальное число со столбца
    int k, index;//обьявляю переменную-счетчик и
    const double eps = 0.0001;  // точность
    x = new float[n];//инициализирую динамический массив размерносю n
    k = 0;//инициализирую переменную-счетчик 
    while (k < n) { //продолжать пока переменная-счетчик меньше размера массива
        max = abs(a[k][k]);//нашел максимально число со строки по главной диагонале
        index = k;//сохраняем индекс максимально числа
        for (int i = k + 1; i < n; i++) {//поиск максимального числа со столбца
            if (abs(a[i][k]) > max) {//сравниваю нижнее число с верхним 
                max = abs(a[i][k]);//если оно больше верхнего, то оно сановиться максимальным
                index = i;//сохраняю его индекс
            }
        }
        if (max < eps) {
            // нет ненулевых диагональных элементов
            std::cout << "Решение получить невозможно из-за нулевого столбца ";
            std::cout << index << " матрицы A" << std::endl;
            return 0;
        }
        for (int j = 0; j < n; j++) {//в этом цикле поднимаю вверх  строки с максимальными значениями по столбцу
            float temp = a[k][j];
            a[k][j] = a[index][j];
            a[index][j] = temp;
        }
        float temp = y[k];//поднимаю вверх ответные числа вверх
        y[k] = y[index];
        y[index] = temp;
        // Нормализация уравнений 
        for (int i = k; i < n; i++)
        {
            float temp = a[i][k];
            if (abs(temp) < eps) continue; // для нулевого коэффициента пропустить
            for (int j = 0; j < n; j++)
                a[i][j] = a[i][j] / temp;
            y[i] = y[i] / temp;
            if (i == k)  continue; // уравнение не вычитать само из себя
            for (int j = 0; j < n; j++)
                a[i][j] = a[i][j] - a[k][j];
            y[i] = y[i] - y[k];
        }
        k++;
    }
    // обратная подстановка
    for (k = n - 1; k >= 0; k--)
    {
        x[k] = y[k];
        for (int i = 0; i < k; i++)
            y[i] = y[i] - a[i][k] * x[k];
    }
    return x;
}




std::string func_gauss(std::string str, int vib) {//функция для возвращения ответа в спрайт используя метод Гаусса
    std::ofstream outputFile("matrix.txt");//открытие файла в который будет производиться запись
    float** a, * y, * x;//обьявление: динамического двумерного массива, динамического массива
    int i, j;//обьявление итераторов
    std::string result;//обьявление переменной которая будет хранить значения из файла
    a = new float* [vib];//инициализация столбцов динамического двумерного массива
    y = new float[vib];//инициаализация динамического массива 
    for (i = 0; i < vib; i++)//цикл для инициализации строк динамического двумерного массива
        a[i] = new float[vib]; //инициализация строк динамического двумерного массива
    outputFile << str;//запись полученной строки как параметр в файл
    outputFile.close();//закрыть файл
    std::ifstream inputFile;//обьявление файла из которого будет происходить считка массива
    inputFile.open("matrix.txt");//открытие файла из которого будет происходить считка массива
    i = 0;//инициализация итератора
    j = 0;//инициализация итератора
    while (!inputFile.eof() && i<vib) {//продолжать цикл пока: не конец файла и столбцов меньше заданных пользователем
        inputFile >> result;//запись из файла  в переменную
        if ((result != ' ') && (j < vib)) { a[i][j] = parse_string<float>(result);}//если переменная не имеет значения пробела и строк меньше заданных пользователем
        else if (j==vib ){ y[i] = parse_string<float>(result); i++; j = -1; }//если строк тоже количество что и количество заданных пользователем - сбить счётчк
        j++;//прибавить +1 к итератору строк
    }
    inputFile.close();//закрыть файла после считки
    x = gauss(a, y, vib);//инициализация массива
    str.clear();//очистка строки
    for (i = 0; i < vib; i++) {//цикл для записи в очищенную строку ответа
        str += "x[" + std::to_string(i + 1) + "]= " + std::to_string(x[i]) + '\n';//запись ответа
    }  
    for (i = 0; i < vib; i++)//удаления двумерного массива
        delete[] a[i];//удаление его строк
    delete[] a,y;//удаления всего массива
    return str;//возвращение ответа
}

std::string func_jordan(std::string str, int vib) {//функция для возвращения ответа в спрайт используя метод Джардана
    std::ofstream outputFile("matrix.txt");//открытие файла записи
    float** a,* x, ratio;//обьявление: динамического двумерного массива, динамического массива,
    int i, j,k;//обьявление итераторов
    std::string result;//обьявление переменной котрая будет хранить значения из файла
    a = new float* [vib];//инициализация столбцов двумерного массива
    x = new float[vib];//инициализация динамического массива(он будет хранить ответы)
    for (i = 0; i < vib; i++)//цикл для инициализации строк динамического двумерного массива
        a[i] = new float[vib+1];//инициализация строк динамического двумерного массива
    outputFile << str;//запись полученной строки как параметр в файл
    outputFile.close();//закрытие файла
    std::ifstream inputFile;//обьявления переменной файла считки
    inputFile.open("matrix.txt");//открытие файла считки
    i = 0;//инициализация итератора
    j = 0;//инициализация итератора
    while (!inputFile.eof()) {//продолжать пока не конец файла
        inputFile >> result;//запись в переменную значения из файла
        if ((result != ' ') && (j <= vib) && (i<vib)) { a[i][j] = parse_string<float>(result); }//конвертирование строки в тип с плавающей точкой
        else if (i<vib-1 && j>vib){ a[i + 1][0] = parse_string<float>(result); j = 0;i++; }//сбитие счётчика и запись  значения в новую строку(что бы не пропустить значения при переходе на новую строку)
        j++;//прибавление итератора
    }
    inputFile.close();//закрытие файла
    for (i = 0; i < vib; i++) {//Методо Джордана
        if (a[i][i] == 0.0) {//если какое-то значение будет равно 0
            std::cout << "Mathematical Error!";//ошибка
            exit(0);//выход
        }
        for (j = 0; j < vib; j++) {
            if (i != j) {//что бы не совпадали строки (начинать всегда со второй строки)
                ratio = a[j][i] / a[i][i];//деление нижней строки относительно числа по главной диагонали что находиться на его столбце
                for (k = 0; k <= vib; k++) {
                    a[j][k] = a[j][k] - ratio * a[i][k];//инициализация целой строки (неключевые значения станут нулями)
                }
            }
        }
    }
    for (i = 0; i < vib; i++) {
        x[i] = a[i][vib] / a[i][i];//деление оставшихся значения на значения после знака равно
    }
    str.clear();//очистка строки
    for (i = 0; i < vib; i++) {//цикл для записи ответа
        str+="x[" + std::to_string(i+1) +"] = " + std::to_string(x[i])+'\n';//запись ответа
    }
    for (i = 0; i < vib; i++)//удаления двумерного массива
        delete[] a[i];//удаление его строк
    delete[]  a,x;//удаления всего массива
    return str;//возвращение ответа
}

void File_input(std::string str) {//функция записи результат в файл
    std::ofstream outputFile("Resultat.txt");//открытие файла записи
    outputFile << str;//запись
    outputFile.close();//закрытие файла
}

int main()
{
    int w;//обьявление переменной для упрвлением меню
    RenderWindow app(VideoMode(800, 600), "Window");//обьявление окна и его размеров 
    Font font;//обьявление переменной шрифта
    Texture back;//обьявление текстуры 
    back.loadFromFile("Back.png");//загрузка файла слова "Вернуться назад"
    Sprite Back(back);//инициализация спрайта
    Back.setPosition(-160, 400);//задаю координаты спрайта слова "Верунться назад"
    Text txt;//обьявляю переменную которая будет хранить строку
    txt.setFont(font);//задаю её шрифт
    txt.setPosition(50, 100);//задаю координаты текста
    TextBox tx(txt);//передаю строку в конструктор и создаю для неё спрайт
    int vib = 0;//переменная которая хранит количество неизвестных заданых пользователем
    bool obrano = false;//переменная хранит булевое значение "сделал ли выбор пользователь"
    Text txt1, txt2, txt3, txt4, txt5, txt6, eror, Resultat;//обявляю переменные что хрянят текст 
    txt1.setFont(font);//задаю шрифт
    txt1.setPosition(50, 100);//задаю координаты
    txt2.setFont(font);//задаю шрифт
    txt2.setPosition(50, 150);//задаю координаты
    txt3.setFont(font);//задаю шрифт
    txt3.setPosition(50, 200);//задаю координаты
    txt4.setFont(font);//задаю шрифт
    txt4.setPosition(50, 250);//задаю координаты
    txt5.setFont(font);//задаю шрифт
    txt5.setPosition(50, 300);//задаю координаты
    txt6.setFont(font);//задаю шрифт
    txt6.setPosition(50, 350);//задаю координаты
    Resultat.setFont(font);//задаю шрифт
    Resultat.setPosition(400, 100);//задаю координаты
    eror.setFont(font);//задаю шрифт
    eror.setPosition(100, 200);//задаю координаты
    eror.setString("ERROR");//задаю начальный текст
    TextBox tx1(txt1), tx2(txt2), tx3(txt3), tx4(txt4), tx5(txt5), tx6(txt6);//передаю строку в конструктор и создаю для неё спрайт
   
    if (!font.loadFromFile("Roboto-Black.ttf")) {//загрузка шрифта
        return EXIT_FAILURE;//в случае не успеха загрузки, написать ошибку
    }
    Back_menu:w = menu(app);//открытие меню перед пользователем
    switch (w)//выбор пользователя
    {
    case 3: { //в случае если номер выбора равен "3"
        fc.setFocusObject(&tx);
            while (!obrano) {//продолжать пока пользователь не сделает выбор
                Texture vvod;//обьявление текстуры
                vvod.loadFromFile("Vvod.png");//загрузка файла слова "Введите количеством неизвестных"
                Sprite Vvod(vvod);//инициализация спрайта
                Vvod.setPosition(80, -40);//задаю координаты спрайта слова "Введите количеством неизвестных"
                Event event;//обьявляю переменную события
                while (app.pollEvent(event)) {//обработка окном события
                    if (event.type == Event::Closed)//если пользователь нажал на "Закрыть" в правом верхнем углу
                        app.close();//закрыть окно
                    Back.setColor(Color::White);//задаю цвет спрайта для слова "Вернуться назад"
                    if (Mouse::isButtonPressed(Mouse::Left)) {//обработка события нажатия ЛКМ
                        if (IntRect(-120, 470, 300, 40).contains(Mouse::getPosition(app))) { goto Back_menu; }//вернуться назад в меню
                    }
                    if (IntRect(-120, 470, 300, 40).contains(Mouse::getPosition(app))) { Back.setColor(Color::Blue); }//если мышка навелась на слово "Верунться назад", поменять цвет на синий
                    if (Keyboard::isKeyPressed(Keyboard::Enter) && tx.GetText()!="") { vib = std::stoi(tx.GetText()); obrano = true; }//конвертирует  строку в целочисленный тип 
                    FocusObject* fo = fc.getFocusObject();//передаю указателю адрес поточного фокуса
                    if (fo != 0) fo->event(event);//обрабатываю для поточного спрайта событие (функция TextBox::event())
                }
                app.clear();//задаю цвет окна
                app.draw(tx);//отрисовую спрайт в который будет производиться запись значения пользователем
                app.draw(Back);//отрисовую спрайт со словом "Вернуться назад"
                app.draw(Vvod);//отрисовую спрайт со словом "Введите количеством неизвестных"
                app.display();//обработка окна
            }
            while (app.isOpen()) {//пока окно открыто
                Event event;//обьвляю переменную события
                while (app.pollEvent(event)) {//обработка окном события
                    if (event.type == Event::Closed)//если пользователь нажал на "Закрыть" в правом верхнем углу
                        app.close();//закрыть окно
                    Back.setColor(Color::White);//задаю цвет спрайта для слова "Вернуться назад"
                    if (Mouse::isButtonPressed(Mouse::Left)) {//обработка события нажатия ЛКМ
                        if (IntRect(50, 100, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx1); };//если левый клик по этим координатам,задаю фокус на этот спрайт
                        if (IntRect(50, 150, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx2); };//если левый клик по этим координатам,задаю фокус на этот спрайт
                        if (IntRect(50, 200, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx3); };//если левый клик по этим координатам,задаю фокус на этот спрайт
                        if (IntRect(50, 250, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx4); };//если левый клик по этим координатам,задаю фокус на этот спрайт
                        if (IntRect(50, 300, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx5); };//если левый клик по этим координатам,задаю фокус на этот спрайт
                        if (IntRect(50, 350, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx6); };//если левый клик по этим координатам,задаю фокус на этот спрайт
                        if (IntRect(-120, 470, 300, 40).contains(Mouse::getPosition(app))) { goto Back_menu; }//если левый клик по этим координатам,возвращаюсь назад
                    }
                    if (IntRect(-120, 470, 300, 40).contains(Mouse::getPosition(app))) { Back.setColor(Color::Blue); }//если курсор навёлся по этим координатам, изменить цвет кнопки на синий
                    if (Keyboard::isKeyPressed(Keyboard::Enter) && (tx1.GetText()!="" && tx2.GetText()!="")) {//обработка события нажатия клавиши Enter
                        switch (vib) {
                        case 2: {
                            std::string str = tx1.GetText() + "\n" + tx2.GetText();//запихиваю в строку массив(введённый пользователем)
                            Resultat.setString(func_gauss(str, vib));//обрабатываю через функцию, которая возвращает ответ типа строки
                        }break;
                        case 3: {
                            if (tx3.GetText() != "") {//выпольнять при условии, если все спрайты заполнены
                                std::string str = tx1.GetText() + "\n" + tx2.GetText() + '\n' + tx3.GetText();//запихиваю в строку массив(введённый пользователем)
                                Resultat.setString(func_gauss(str, vib));//обрабатываю через функцию, которая возвращает ответ типа строки
                            }
                        }break;
                        case 4: {
                            if (tx3.GetText() != "" && tx4.GetText() != "") {//выпольнять при условии, если все спрайты заполнены
                                std::string str = tx1.GetText() + "\n" + tx2.GetText() + '\n' + tx3.GetText() + '\n' + tx4.GetText();//запихиваю в строку массив(введённый пользователем)
                                Resultat.setString(func_gauss(str, vib));//обрабатываю через функцию, которая возвращает ответ типа строки
                            }
                        }break;
                        case 5: {
                            if (tx3.GetText() != "" && tx4.GetText() != "" && tx5.GetText() != "") {//выпольнять при условии, если все спрайты заполнены
                                std::string str = tx1.GetText() + "\n" + tx2.GetText() + '\n' + tx3.GetText() + '\n' + tx4.GetText() + '\n' + tx5.GetText();//запихиваю в строку массив(введённый пользователем)
                                Resultat.setString(func_gauss(str, vib));//обрабатываю через функцию, которая возвращает ответ типа строки
                            }
                        }break;
                        case 6: {
                            if (tx3.GetText() != "" && tx4.GetText() != "" && tx5.GetText() != "" && tx6.GetText() != "") {//выпольнять при условии, если все спрайты заполнены
                                std::string str = tx1.GetText() + "\n" + tx2.GetText() + '\n' + tx3.GetText() + '\n' + tx4.GetText() + '\n' + tx5.GetText() + tx6.GetText();//запихиваю в строку массив(введённый пользователем)
                                Resultat.setString(func_gauss(str, vib));//обрабатываю через функцию, которая возвращает ответ типа строки
                            }
                        }break;
                        default: std::cout << "Error";
                        }
                        File_input(Resultat.getString());//запихнул ответ в файл
                    }
                    FocusObject* fo = fc.getFocusObject();//передаю указателю адрес поточного фокуса
                    if (fo != 0) fo->event(event);//обрабатываю для поточного спрайта событие (функция TextBox::event())
                }
                app.clear();//задаю цвет фона окна
                app.draw(Back);//рисую кнопки "Вернуться назад"
                switch (vib)//отрисовую спрайты в зависимости от введённого пользователем значением 
                {
                case 2: {app.draw(tx1); app.draw(tx2); }break;
                case 3: {app.draw(tx1); app.draw(tx2); app.draw(tx3); }break;
                case 4: {app.draw(tx1); app.draw(tx2); app.draw(tx3); app.draw(tx4); }break;
                case 5: {app.draw(tx1); app.draw(tx2); app.draw(tx3); app.draw(tx4); app.draw(tx5); }break;
                case 6: {app.draw(tx1); app.draw(tx2); app.draw(tx3); app.draw(tx4); app.draw(tx5); app.draw(tx6); }break;
                default: app.draw(eror); break;
                }
                app.draw(Resultat);//рисую результат вычисления
                app.display();//обработка окна
            }

        }break;
    case 4: {//в случае если номер выбора равен "4"
            fc.setFocusObject(&tx);
            while (!obrano) {//продолжать пока пользователь не сделает выбор
                Texture vvod;//обьявление текстуры
                vvod.loadFromFile("Vvod.png");//загрузка файла слова "Введите количеством неизвестных"
                Sprite Vvod(vvod);//инициализация спрайта
                Vvod.setPosition(80, -40);//задаю координаты спрайта слова "Введите количеством неизвестных"
                Event event;//обьвляю переменную события
                while (app.pollEvent(event)) {//обработка окном события
                    if (event.type == Event::Closed)//если пользователь нажал на "Закрыть" в правом верхнем углу
                        app.close();//закрыть окно
                    Back.setColor(Color::White);//задаю цвет спрайта для слова "Вернуться назад"
                    if (Mouse::isButtonPressed(Mouse::Left)) {//обработка события нажатия ЛКМ
                        if (IntRect(-120, 470, 300, 40).contains(Mouse::getPosition(app))) { goto Back_menu; }//вернуться назад в меню
                    }
                    if (IntRect(-120, 470, 300, 40).contains(Mouse::getPosition(app))) { Back.setColor(Color::Blue); }//если мышка навелась на слово "Верунться назад", поменять цвет на синий
                    if (Keyboard::isKeyPressed(Keyboard::Enter) && tx.GetText()!="") { vib = std::stoi(tx.GetText()); obrano = true; }//конвертирует  строку в целочисленный тип 
                    FocusObject* fo = fc.getFocusObject();//передаю указателю адрес поточного фокуса
                    if (fo != 0) fo->event(event);//обрабатываю для поточного спрайта событие (функция TextBox::event())
                }
                app.clear();//задаю цвет фона окна
                app.draw(tx);//отрисовую спрайт в который будет производиться запись значения пользователем
                app.draw(Back);//отрисовую спрайт со словом "Вернуться назад"
                app.draw(Vvod);//отрисовую спрайт со словом "Введите количеством неизвестных"
                app.display();//обработка окна
            }
            while (app.isOpen()) {//пока окно открыто
                Event event;//обьвляю переменную события
                while (app.pollEvent(event)) {//обработка окном события
                    if (event.type == Event::Closed)//если пользователь нажал на "Закрыть" в правом верхнем углу
                        app.close();//закрыть окно
                    Back.setColor(Color::White);//задаю цвет спрайта для слова "Вернуться назад"
                    if (Mouse::isButtonPressed(Mouse::Left)) {//обработка события нажатия ЛКМ
                        if (IntRect(50, 100, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx1); };//если левый клик по этим координатам,задаю фокус на этот спрайт
                        if (IntRect(50, 150, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx2); };//если левый клик по этим координатам,задаю фокус на этот спрайт
                        if (IntRect(50, 200, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx3); };//если левый клик по этим координатам,задаю фокус на этот спрайт
                        if (IntRect(50, 250, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx4); };//если левый клик по этим координатам,задаю фокус на этот спрайт
                        if (IntRect(50, 300, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx5); };//если левый клик по этим координатам,задаю фокус на этот спрайт
                        if (IntRect(50, 350, 300, 40).contains(Mouse::getPosition(app))) { fc.setFocusObject(&tx6); };//если левый клик по этим координатам,задаю фокус на этот спрайт
                        if (IntRect(-120, 470, 300, 40).contains(Mouse::getPosition(app))) { goto Back_menu; }//если левый клик по этим координатам,возвращаюсь назад
                    }
                    if (IntRect(-120, 470, 300, 40).contains(Mouse::getPosition(app))) { Back.setColor(Color::Blue); }//если курсор навёлся по этим координатам, изменить цвет кнопки на синий
                    if (Keyboard::isKeyPressed(Keyboard::Enter) && (tx1.GetText() != "" && tx2.GetText() != "")) {//обработка события нажатия клавиши Enter
                        switch (vib) {
                        case 2: {
                            std::string str = tx1.GetText() + "\n" + tx2.GetText();//запихиваю в строку массив(введённый пользователем)
                            Resultat.setString(func_jordan(str, vib));//обрабатываю через функцию, которая возвращает ответ типа строки
                        }break;
                        case 3: {
                            if (tx3.GetText() != "") {//выпольнять при условии, если все спрайты заполнены
                                std::string str = tx1.GetText() + "\n" + tx2.GetText() + '\n' + tx3.GetText();//запихиваю в строку массив(введённый пользователем)
                                Resultat.setString(func_jordan(str, vib));//обрабатываю через функцию, которая возвращает ответ типа строки
                            }
                        }break;
                        case 4: {
                            if (tx3.GetText() != "" && tx4.GetText() != "") {//выпольнять при условии, если все спрайты заполнены
                                std::string str = tx1.GetText() + "\n" + tx2.GetText() + '\n' + tx3.GetText() + '\n' + tx4.GetText();//запихиваю в строку массив(введённый пользователем)
                                Resultat.setString(func_jordan(str, vib));//обрабатываю через функцию, которая возвращает ответ типа строки
                            }
                        }break;
                        case 5: {
                            if (tx3.GetText() != "" && tx4.GetText() != "" && tx5.GetText() != "") {//выпольнять при условии, если все спрайты заполнены
                                std::string str = tx1.GetText() + "\n" + tx2.GetText() + '\n' + tx3.GetText() + '\n' + tx4.GetText() + '\n' + tx5.GetText();//запихиваю в строку массив(введённый пользователем)
                                Resultat.setString(func_jordan(str, vib));//обрабатываю через функцию, которая возвращает ответ типа строки
                            }
                        }break;
                        case 6: {
                            if (tx3.GetText() != "" && tx4.GetText() != "" && tx5.GetText() != "" && tx6.GetText() != "") {//выпольнять при условии, если все спрайты заполнены
                                std::string str = tx1.GetText() + "\n" + tx2.GetText() + '\n' + tx3.GetText() + '\n' + tx4.GetText() + '\n' + tx5.GetText() + tx6.GetText();//запихиваю в строку массив(введённый пользователем)
                                Resultat.setString(func_jordan(str, vib));//обрабатываю через функцию, которая возвращает ответ типа строки
                            }
                        }break;
                        default: std::cout << "Error";
                        }
                        File_input(Resultat.getString());//запихнул ответ в файл
                    }
                    FocusObject* fo = fc.getFocusObject();//передаю указателю адрес поточного фокуса
                    if (fo != 0) fo->event(event);//обрабатываю для поточного спрайта событие (функция TextBox::event())

                }
                app.clear();//задаю цвет фона окна
                app.draw(Back);//рисую кнопки "Вернуться назад"
                switch (vib)//отрисовую спрайты в зависимости от введённого пользователем значением 
                {
                case 2: {app.draw(tx1); app.draw(tx2); }break;
                case 3: {app.draw(tx1); app.draw(tx2); app.draw(tx3); }break;
                case 4: {app.draw(tx1); app.draw(tx2); app.draw(tx3); app.draw(tx4); }break;
                case 5: {app.draw(tx1); app.draw(tx2); app.draw(tx3); app.draw(tx4); app.draw(tx5); }break;
                case 6: {app.draw(tx1); app.draw(tx2); app.draw(tx3); app.draw(tx4); app.draw(tx5); app.draw(tx6); }break;
                default: app.draw(eror); break;
                }
                app.draw(Resultat);//рисую результат вычисления
                app.display();//обработка окна
            }
        }break;
    case 1: {system("Resultat.txt"); goto Back_menu; }break;//в случае если номер выбора равен "1", открыть блокнот с ответом
    case 2: return 0; break;//закрыть программу
    default:std::cout << "Error"; break;//ошибка
    }
}