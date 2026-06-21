#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

// --- Помощни функции за обработка на низове ---
inline bool isSpace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

inline bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

inline std::string trim(const std::string &s)
{
    size_t a = 0;
    while (a < s.size() && isSpace(s[a]))
        ++a;
    size_t b = s.size();
    while (b > a && isSpace(s[b - 1]))
        --b;
    return s.substr(a, b - a);
}

inline std::vector<std::string> splitSpaces(const std::string &str)
{
    std::vector<std::string> tokens;
    std::string current = "";
    for (char c : str)
    {
        if (isSpace(c))
        {
            if (!current.empty())
            {
                tokens.push_back(current);
                current = "";
            }
        }
        else
        {
            current += c;
        }
    }
    if (!current.empty())
    {
        tokens.push_back(current);
    }
    return tokens;
}

// --- Структури за грешки и статистика ---
struct ParseError
{
    int line_number = 0;
    std::string message;
};

struct DocumentStats
{
    int total_lines = 0;
    int h1 = 0, h2 = 0, h3 = 0, h4 = 0, h5 = 0, h6 = 0;
    int paragraphs = 0;
    int hr = 0;
    int list_items = 0;
    int bold = 0;
    int italic = 0;
    int code = 0;
    int strike = 0;
};

class TreePrinter
{
private:
    int lineCount = 0;
    const int maxLines = 24;

public:
    void print(const std::string &text)
    {
        std::cout << text << "\n";
        lineCount++;
        if (lineCount >= maxLines)
        {
            std::cout << "[ Натиснете Enter за следваща страница... ]";
            std::string dummy;
            std::getline(std::cin, dummy);
            lineCount = 0;
        }
    }
};

// --- Йерархия на Inline елементите (Вътрешни стилове) ---
class InlineNode
{
public:
    virtual ~InlineNode() = default;
    virtual InlineNode *clone() const = 0;
    virtual std::string toHTML() const = 0;
    virtual void printTree(int depth, TreePrinter &p) const = 0;
    virtual void collectStats(DocumentStats &stats) const = 0;
};

class InlineSequence
{
private:
    std::vector<InlineNode *> parts;

    void clear()
    {
        for (auto *node : parts)
            delete node;
        parts.clear();
    }

    void copyFrom(const InlineSequence &other)
    {
        parts.reserve(other.parts.size());
        for (auto *node : other.parts)
        {
            parts.push_back(node ? node->clone() : nullptr);
        }
    }

public:
    InlineSequence() = default;

    ~InlineSequence()
    {
        clear();
    }

    InlineSequence(const InlineSequence &other)
    {
        copyFrom(other);
    }

    // Идиом Copy-and-Swap за силна гаранция за изключения
    InlineSequence &operator=(InlineSequence other)
    {
        std::swap(parts, other.parts);
        return *this;
    }

    void add(InlineNode *node)
    {
        if (node)
            parts.push_back(node);
    }

    std::string toHTML() const
    {
        std::string res = "";
        for (auto *node : parts)
            if (node)
                res += node->toHTML();
        return res;
    }

    void printTree(int depth, TreePrinter &p) const
    {
        for (auto *node : parts)
            if (node)
                node->printTree(depth, p);
    }

    void collectStats(DocumentStats &stats) const
    {
        for (auto *node : parts)
            if (node)
                node->collectStats(stats);
    }
};

class TextNode : public InlineNode
{
private:
    std::string text;

public:
    TextNode(const std::string &t) : text(t) {}
    InlineNode *clone() const override { return new TextNode(text); }
    std::string toHTML() const override { return text; }
    void printTree(int depth, TreePrinter &p) const override
    {
        p.print(std::string(depth, ' ') + "Text: \"" + text + "\"");
    }
    void collectStats(DocumentStats &stats) const override {}
};

class BoldNode : public InlineNode
{
private:
    InlineSequence content;

public:
    BoldNode(const InlineSequence &seq) : content(seq) {}
    InlineNode *clone() const override { return new BoldNode(content); }
    std::string toHTML() const override { return "<b>" + content.toHTML() + "</b>"; }
    void printTree(int depth, TreePrinter &p) const override
    {
        p.print(std::string(depth, ' ') + "Bold:");
        content.printTree(depth + 2, p);
    }
    void collectStats(DocumentStats &stats) const override
    {
        stats.bold++;
        content.collectStats(stats);
    }
};

class ItalicNode : public InlineNode
{
private:
    InlineSequence content;

public:
    ItalicNode(const InlineSequence &seq) : content(seq) {}
    InlineNode *clone() const override { return new ItalicNode(content); }
    std::string toHTML() const override { return "<i>" + content.toHTML() + "</i>"; }
    void printTree(int depth, TreePrinter &p) const override
    {
        p.print(std::string(depth, ' ') + "Italic:");
        content.printTree(depth + 2, p);
    }
    void collectStats(DocumentStats &stats) const override
    {
        stats.italic++;
        content.collectStats(stats);
    }
};

class CodeNode : public InlineNode
{
private:
    InlineSequence content;

public:
    CodeNode(const InlineSequence &seq) : content(seq) {}
    InlineNode *clone() const override { return new CodeNode(content); }
    std::string toHTML() const override { return "<code>" + content.toHTML() + "</code>"; }
    void printTree(int depth, TreePrinter &p) const override
    {
        p.print(std::string(depth, ' ') + "Code inline:");
        content.printTree(depth + 2, p);
    }
    void collectStats(DocumentStats &stats) const override
    {
        stats.code++;
        content.collectStats(stats);
    }
};

class StrikeNode : public InlineNode
{
private:
    InlineSequence content;

public:
    StrikeNode(const InlineSequence &seq) : content(seq) {}
    InlineNode *clone() const override { return new StrikeNode(content); }
    std::string toHTML() const override { return "<s>" + content.toHTML() + "</s>"; }
    void printTree(int depth, TreePrinter &p) const override
    {
        p.print(std::string(depth, ' ') + "Strike:");
        content.printTree(depth + 2, p);
    }
    void collectStats(DocumentStats &stats) const override
    {
        stats.strike++;
        content.collectStats(stats);
    }
};

// --- Парсер за вътрешни стилове (Коректна Markdown семантика) ---
class InlineParser
{
public:
    static InlineSequence parse(const std::string &text, int line_num, std::vector<ParseError> &errors)
    {
        InlineSequence seq;
        size_t i = 0;
        size_t n = text.size();
        std::string current_text = "";

        while (i < n)
        {
            // БОЛД: съвпадение на "**" според стандартния Markdown спецификатор
            if (i + 1 < n && text[i] == '*' && text[i + 1] == '*')
            {
                if (!current_text.empty())
                {
                    seq.add(new TextNode(current_text));
                    current_text = "";
                }
                size_t close = std::string::npos;
                size_t j = i + 2;
                while (j + 1 < n)
                {
                    if (text[j] == '*' && text[j + 1] == '*')
                    {
                        close = j;
                        break;
                    }
                    ++j;
                }
                if (close != std::string::npos)
                {
                    std::string inner = text.substr(i + 2, close - (i + 2));
                    seq.add(new BoldNode(parse(inner, line_num, errors)));
                    i = close + 2;
                }
                else
                {
                    errors.push_back({line_num, "Незатворен блок за болд (**)"});
                    current_text += "**";
                    i += 2;
                }
            }
            // ИТАЛИК: съвпадение на единична звезда "*" (пропускайки болд двойки)
            else if (text[i] == '*')
            {
                if (!current_text.empty())
                {
                    seq.add(new TextNode(current_text));
                    current_text = "";
                }
                size_t close = std::string::npos;
                size_t j = i + 1;
                while (j < n)
                {
                    if (j + 1 < n && text[j] == '*' && text[j + 1] == '*')
                    {
                        j += 2;
                    }
                    else if (text[j] == '*')
                    {
                        close = j;
                        break;
                    }
                    else
                    {
                        ++j;
                    }
                }
                if (close != std::string::npos)
                {
                    std::string inner = text.substr(i + 1, close - (i + 1));
                    seq.add(new ItalicNode(parse(inner, line_num, errors)));
                    i = close + 1;
                }
                else
                {
                    errors.push_back({line_num, "Незатворен блок за италик (*)"});
                    current_text += "*";
                    i += 1;
                }
            }
            // КОД СТИЛ: "`"
            else if (text[i] == '`')
            {
                if (!current_text.empty())
                {
                    seq.add(new TextNode(current_text));
                    current_text = "";
                }
                size_t close = text.find('`', i + 1);
                if (close != std::string::npos)
                {
                    std::string inner = text.substr(i + 1, close - (i + 1));
                    seq.add(new CodeNode(parse(inner, line_num, errors)));
                    i = close + 1;
                }
                else
                {
                    errors.push_back({line_num, "Незатворен блок за код (`)"});
                    current_text += "`";
                    i += 1;
                }
            }
            // ЗАЧЕРКНАТ TEXT: "~"
            else if (text[i] == '~')
            {
                if (!current_text.empty())
                {
                    seq.add(new TextNode(current_text));
                    current_text = "";
                }
                size_t close = text.find('~', i + 1);
                if (close != std::string::npos)
                {
                    std::string inner = text.substr(i + 1, close - (i + 1));
                    seq.add(new StrikeNode(parse(inner, line_num, errors)));
                    i = close + 1;
                }
                else
                {
                    errors.push_back({line_num, "Незатворен блок за зачеркнат текст (~)"});
                    current_text += "~";
                    i += 1;
                }
            }
            else
            {
                current_text += text[i];
                i++;
            }
        }

        if (!current_text.empty())
        {
            seq.add(new TextNode(current_text));
        }
        return seq;
    }
};

// --- Йерархия на Структурните блокове ---
class Block
{
public:
    virtual ~Block() = default;
    virtual Block *clone() const = 0; // Задължителен за дълбоко копиране на документа
    virtual std::string toHTML() const = 0;
    virtual void printTree(int depth, TreePrinter &p) const = 0;
    virtual void collectStats(DocumentStats &stats) const = 0;
};

class HeadingBlock : public Block
{
private:
    int level;
    InlineSequence content;

public:
    HeadingBlock(int lvl, const InlineSequence &seq) : level(lvl), content(seq) {}
    Block *clone() const override { return new HeadingBlock(*this); }

    std::string toHTML() const override
    {
        std::string lvlStr = std::to_string(level);
        return "<h" + lvlStr + ">" + content.toHTML() + "</h" + lvlStr + ">\n";
    }

    void printTree(int depth, TreePrinter &p) const override
    {
        p.print(std::string(depth, ' ') + "Heading " + std::to_string(level) + ":");
        content.printTree(depth + 2, p);
    }

    void collectStats(DocumentStats &stats) const override
    {
        if (level == 1)
            stats.h1++;
        else if (level == 2)
            stats.h2++;
        else if (level == 3)
            stats.h3++;
        else if (level == 4)
            stats.h4++;
        else if (level == 5)
            stats.h5++;
        else if (level == 6)
            stats.h6++;
        content.collectStats(stats);
    }
};

class ParagraphBlock : public Block
{
private:
    InlineSequence content;

public:
    ParagraphBlock(const InlineSequence &seq) : content(seq) {}
    Block *clone() const override { return new ParagraphBlock(*this); }

    std::string toHTML() const override
    {
        return "<p>" + content.toHTML() + "</p>\n";
    }

    void printTree(int depth, TreePrinter &p) const override
    {
        p.print(std::string(depth, ' ') + "Paragraph:");
        content.printTree(depth + 2, p);
    }

    void collectStats(DocumentStats &stats) const override
    {
        stats.paragraphs++;
        content.collectStats(stats);
    }
};

class HRBlock : public Block
{
public:
    Block *clone() const override { return new HRBlock(*this); }
    std::string toHTML() const override { return "<hr />\n"; }
    void printTree(int depth, TreePrinter &p) const override
    {
        p.print(std::string(depth, ' ') + "Horizontal Rule (HR)");
    }
    void collectStats(DocumentStats &stats) const override { stats.hr++; }
};

class ListBlock : public Block
{
private:
    bool ordered;
    std::vector<InlineSequence> items;

public:
    ListBlock(bool ord) : ordered(ord) {}
    Block *clone() const override { return new ListBlock(*this); }
    bool isOrdered() const { return ordered; }
    void addItem(const InlineSequence &seq) { items.push_back(seq); }

    std::string toHTML() const override
    {
        std::string tag = ordered ? "ol" : "ul";
        std::string res = "<" + tag + ">\n";
        for (const auto &item : items)
        {
            res += "  <li>" + item.toHTML() + "</li>\n";
        }
        res += "</" + tag + ">\n";
        return res;
    }

    void printTree(int depth, TreePrinter &p) const override
    {
        p.print(std::string(depth, ' ') + (ordered ? "Ordered List:" : "Unordered List:"));
        for (const auto &item : items)
        {
            p.print(std::string(depth + 2, ' ') + "ListItem:");
            item.printTree(depth + 4, p);
        }
    }

    void collectStats(DocumentStats &stats) const override
    {
        stats.list_items += items.size();
        for (const auto &item : items)
        {
            item.collectStats(stats);
        }
    }
};

// --- Документ Мениджър (Вече съобразен с Правилото на трите) ---
class MarkdownDocument
{
private:
    std::vector<Block *> blocks;
    std::vector<ParseError> errors;
    int total_lines_read = 0;

    std::string ltrim(const std::string &s) const
    {
        size_t a = 0;
        while (a < s.size() && isSpace(s[a]))
            ++a;
        return s.substr(a);
    }

    void clear()
    {
        for (auto *b : blocks)
            delete b;
        blocks.clear();
        errors.clear();
        total_lines_read = 0;
    }

    void copyFrom(const MarkdownDocument &other)
    {
        blocks.reserve(other.blocks.size());
        for (auto *b : other.blocks)
        {
            blocks.push_back(b ? b->clone() : nullptr);
        }
        errors = other.errors;
        total_lines_read = other.total_lines_read;
    }

public:
    MarkdownDocument() = default;

    ~MarkdownDocument()
    {
        clear();
    }

    MarkdownDocument(const MarkdownDocument &other)
    {
        copyFrom(other);
    }

    // Идиом Copy-and-Swap за пълна Exception Safety при управлението на ресурсния вектор
    MarkdownDocument &operator=(MarkdownDocument other)
    {
        std::swap(blocks, other.blocks);
        std::swap(errors, other.errors);
        std::swap(total_lines_read, other.total_lines_read);
        return *this;
    }

    bool parseFromFile(const std::string &filename)
    {
        clear();
        std::ifstream in(filename);
        if (!in)
            return false;

        std::string line;
        ListBlock *current_list = nullptr;

        try
        {
            while (std::getline(in, line))
            {
                total_lines_read++;
                if (!line.empty() && line.back() == '\r')
                {
                    line.pop_back();
                }

                bool is_empty = true;
                for (char c : line)
                {
                    if (!isSpace(c))
                    {
                        is_empty = false;
                        break;
                    }
                }

                if (is_empty)
                {
                    if (current_list)
                    {
                        blocks.push_back(current_list);
                        current_list = nullptr;
                    }
                    continue;
                }

                std::string lead_trimmed = ltrim(line);

                // 1. Хоризонтална линия
                if (lead_trimmed == "---")
                {
                    if (current_list)
                    {
                        blocks.push_back(current_list);
                        current_list = nullptr;
                    }
                    blocks.push_back(new HRBlock());
                    continue;
                }

                // 2. Заглавия (Heading)
                if (lead_trimmed[0] == '#')
                {
                    size_t h_count = 0;
                    while (h_count < lead_trimmed.size() && lead_trimmed[h_count] == '#')
                    {
                        h_count++;
                    }
                    if (h_count <= 6 && h_count < lead_trimmed.size() && isSpace(lead_trimmed[h_count]))
                    {
                        if (current_list)
                        {
                            blocks.push_back(current_list);
                            current_list = nullptr;
                        }
                        std::string content = trim(lead_trimmed.substr(h_count));
                        blocks.push_back(new HeadingBlock(h_count, InlineParser::parse(content, total_lines_read, errors)));
                        continue;
                    }
                    else if (h_count > 6)
                    {
                        errors.push_back({total_lines_read, "Прекалено дълбоко ниво на заглавието (над h6)"});
                    }
                }

                // 3. Неподреден Списък
                if ((lead_trimmed[0] == '-' || lead_trimmed[0] == '*') && lead_trimmed.size() > 1 && isSpace(lead_trimmed[1]))
                {
                    std::string content = trim(lead_trimmed.substr(2));
                    InlineSequence item_seq = InlineParser::parse(content, total_lines_read, errors);

                    if (current_list && !current_list->isOrdered())
                    {
                        current_list->addItem(item_seq);
                    }
                    else
                    {
                        if (current_list)
                            blocks.push_back(current_list);
                        current_list = new ListBlock(false);
                        current_list->addItem(item_seq);
                    }
                    continue;
                }

                // 4. Подреден Списък
                size_t dig_count = 0;
                while (dig_count < lead_trimmed.size() && isDigit(lead_trimmed[dig_count]))
                {
                    dig_count++;
                }
                if (dig_count > 0 && dig_count < lead_trimmed.size() && lead_trimmed[dig_count] == '.')
                {
                    size_t next_idx = dig_count + 1;
                    if (next_idx < lead_trimmed.size() && isSpace(lead_trimmed[next_idx]))
                    {
                        std::string content = trim(lead_trimmed.substr(next_idx));
                        InlineSequence item_seq = InlineParser::parse(content, total_lines_read, errors);

                        if (current_list && current_list->isOrdered())
                        {
                            current_list->addItem(item_seq);
                        }
                        else
                        {
                            if (current_list)
                                blocks.push_back(current_list);
                            current_list = new ListBlock(true);
                            current_list->addItem(item_seq);
                        }
                        continue;
                    }
                }

                // 5. По подразбиране: Обикновен Параграф
                if (current_list)
                {
                    blocks.push_back(current_list);
                    current_list = nullptr;
                }
                std::string content = trim(lead_trimmed);
                blocks.push_back(new ParagraphBlock(InlineParser::parse(content, total_lines_read, errors)));
            }

            if (current_list)
            {
                blocks.push_back(current_list);
                current_list = nullptr;
            }
        }
        catch (...)
        {
            delete current_list;
            clear();
            throw; // Препредаваме изключението след почистване на динамичната памет
        }

        in.close();
        return true;
    }

    bool saveToHTML(const std::string &filename) const
    {
        std::ofstream out(filename);
        if (!out)
            return false;

        out << "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n";
        out << "<title>Converted Document</title>\n</head>\n<body>\n";
        for (auto *b : blocks)
        {
            if (b)
                out << b->toHTML();
        }
        out << "</body>\n</html>\n";
        out.close();
        return true;
    }

    void printErrors() const
    {
        if (errors.empty())
        {
            std::cout << "Документът е валиден. Няма намерени грешки.\n";
        }
        else
        {
            std::cout << "Намерени грешки при парсване:\n";
            for (const auto &err : errors)
            {
                std::cout << "Ред " << err.line_number << ": " << err.message << "\n";
            }
        }
    }

    void printStats() const
    {
        DocumentStats stats;
        stats.total_lines = total_lines_read;
        for (auto *b : blocks)
        {
            if (b)
                b->collectStats(stats);
        }
        std::cout << "Статистика на документа:\n";
        std::cout << "  Брой прочетени редове: " << stats.total_lines << "\n";
        std::cout << "  Заглавия по нива (H1-H6): " << stats.h1 << " " << stats.h2 << " " << stats.h3 << " "
                  << stats.h4 << " " << stats.h5 << " " << stats.h6 << "\n";
        std::cout << "  Параграфи: " << stats.paragraphs << "\n";
        std::cout << "  Хоризонтални линии (HR): " << stats.hr << "\n";
        std::cout << "  Елементи на списъци (LI): " << stats.list_items << "\n";
        std::cout << "  Използвани вътрешни стилове:\n";
        std::cout << "    Bold (**): " << stats.bold << " | Italic (*): " << stats.italic
                  << " | Code (`): " << stats.code << " | Strike (~): " << stats.strike << "\n";
    }

    void printTreeStructure() const
    {
        TreePrinter p;
        p.print("--- СИНТАКТИЧНО ДЪРВО НА ДОКУМЕНТА ---");
        for (auto *b : blocks)
        {
            if (b)
                b->printTree(2, p);
        }
    }
};

// --- Потребителски Интерфейс (CLI) ---
class CommandLineInterface
{
private:
    MarkdownDocument doc;

    bool fileExists(const std::string &name) const
    {
        std::ifstream f(name.c_str());
        return f.good();
    }

public:
    void run()
    {
        std::string inputLine;
        std::cout << "Markdown to HTML Converter CLI зареден.\n";
        std::cout << "Възможни команди: translate, validate, info, print, exit\n";

        while (true)
        {
            std::cout << "> ";
            if (!std::getline(std::cin, inputLine))
                break;

            std::vector<std::string> args = splitSpaces(inputLine);
            if (args.empty())
                continue;

            std::string command = args[0];

            if (command == "exit")
            {
                std::cout << "Прекратяване на програмата. Довиждане!\n";
                break;
            }
            else if (command == "translate")
            {
                if (args.size() < 3)
                {
                    std::cout << "Грешка: Невалиден синтаксис. Използвайте: translate <input_file> <output_file>\n";
                    continue;
                }
                std::string infile = args[1];
                std::string outfile = args[2];

                if (!doc.parseFromFile(infile))
                {
                    std::cout << "Грешка: Неуспешен опит за отваряне или четене на: " << infile << "\n";
                    continue;
                }

                if (fileExists(outfile))
                {
                    std::cout << "Файлът '" << outfile << "' вече съществува. Презаписване? (y/n): ";
                    std::string choice;
                    std::getline(std::cin, choice);
                    choice = trim(choice);
                    if (choice != "y" && choice != "Y")
                    {
                        std::cout << "Операцията е отказана от потребителя.\n";
                        continue;
                    }
                }

                if (doc.saveToHTML(outfile))
                {
                    std::cout << "Успешно генериран HTML файл: " << outfile << "\n";
                }
                else
                {
                    std::cout << "Грешка при запис в: " << outfile << "\n";
                }
            }
            else if (command == "validate")
            {
                if (args.size() < 2)
                {
                    std::cout << "Грешка: Липсва входен файл. Използвайте: validate <input_file>\n";
                    continue;
                }
                if (!doc.parseFromFile(args[1]))
                {
                    std::cout << "Грешка при зареждане на файла.\n";
                    continue;
                }
                doc.printErrors();
            }
            else if (command == "info")
            {
                if (args.size() < 2)
                {
                    std::cout << "Грешка: Липсва входен файл. Използвайте: info <input_file>\n";
                    continue;
                }
                if (!doc.parseFromFile(args[1]))
                {
                    std::cout << "Грешка при зареждане на файла.\n";
                    continue;
                }
                doc.printStats();
            }
            else if (command == "print")
            {
                if (args.size() < 2)
                {
                    std::cout << "Грешка: Липсва входен файл. Използвайте: print <input_file>\n";
                    continue;
                }
                if (!doc.parseFromFile(args[1]))
                {
                    std::cout << "Грешка при зареждане на файла.\n";
                    continue;
                }
                doc.printTreeStructure();
            }
            else
            {
                std::cout << "Невалидна команда. Опитайте пак.\n";
            }
        }
    }
};

int main()
{
    CommandLineInterface cli;
    cli.run();
    return 0;
}