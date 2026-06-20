#include <iostream>
#include <string>
// Абстрактен базов клас FileNode:

// Има член-данни: String name (можеш да ползваш директно std::string за бързина сега).

// Чисто виртуални функции: size_t getSize() const = 0;, void print() const = 0;, и FileNode* clone() const = 0;.

// Правилен деструктор.

// Клас PlainFile (Наследник):

// Добавя член-данна: size_t size (големина на файла).

// Имплементира нужните функции.

// Клас Directory (Хетерогенен контейнер и Наследник едновременно!):

// Папка, която съдържа масив от други файлови обекти: FileNode* contents[10]; и size_t count;.

// getSize() на директорията трябва да връща сумата от размерите на всички елементи вътре в нея.

// Има метод void addNode(const FileNode& node);.

// Подсигури правилната памет (Деструктор, забрани копирането за бързина).

// Твоята първа стъпка:
// Напиши само кода за класовете FileNode и PlainFile. Когато си готов, ми ги прати тук, за да ги прегледам за секунди за грешки, и веднага ще преминем към голямата папка (Directory)!

class FileNode {
    protected:
        std::string name;

    public:
        virtual size_t getSize() const = 0;
        virtual void print() const = 0;
        virtual FileNode* clone() const = 0;
        virtual ~FileNode() = default;

        FileNode(const std::string& name) : name(name) {} 
        
};

class PlainFile : public FileNode {
    private:
        size_t size;

    public: 
        size_t getSize() const override {
            return this->size; // или просто size;
        }
        void print() const override {
            std::cout << "Name: " << this->name << ", size = " << this->size << std::endl;
        }
        FileNode* clone() const {
            return new PlainFile (*this);
        }

        PlainFile(const std::string& name, size_t otherSize) : FileNode(name), size(otherSize) {}

};

//heterogenen konteiner i naslednik ednovremenno
class Directory : public FileNode {
    private:
        FileNode* contents[10];
        size_t count = 0;
    
        //helper functions 
        void free() {
            for(size_t i = 0; i < count; i++) delete contents[i];

        }
    public: 
        size_t getSize() const {
            size_t size = 0;
            for(size_t i = 0; i < count; i++) {
                size += contents[i]->getSize();
            }
            return size;
        }

        void addnote(const FileNode& node) {
            if(count < 10) {
                contents[count] = node.clone();
                count ++;
            }
        }

        void print () const override {
            std::cout << "Directory: " << this->name << std::endl;
            for(size_t i = 0; i < count; i++) {
                contents[i]->print();    
            }
        }
        Directory (const std::string& name) : FileNode(name), count(0) {
            for(size_t i = 0; i < 10; i++) contents[i] = nullptr;
}
        Directory(Directory& other) = delete;
        
        ~Directory() {
            free();
        }
};
