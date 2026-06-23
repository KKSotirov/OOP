#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

// ==========================================
// 1. ПОМОЩНИ УТИЛИТИ ЗА ОБРАБОТКА НА СТРИНГОВЕ
// ==========================================
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

// ==========================================
// 2. СТРУКТУРИ ЗА ДАННИ И СТРАНИЦИРАНЕ
// ==========================================
struct ParseError
{
    int line_number = 0;
    std::string message;
};

struct DocumentStats
{
    int total_lines = 0;
    int headings[7] = {0}; // Индекси от 1 до 6 съответстват на h1...h6. Без if-else!
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
    const int maxLines = 22;

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

// ==========================================
// 3. ПОЛИМОРФНА ЙЕРАРХИЯ ЗА INLINE ЕЛЕМЕНТИ
// ==========================================
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
    ~InlineSequence() { clear(); }
    InlineSequence(const InlineSequence &other) { copyFrom(other); }

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
        p.print(std::string(depth, ' ') + "Bold (*):");
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
        p.print(std::string(depth, ' ') + "Italic (**):");
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
        p.print(std::string(depth, ' ') + "Code span (`):");
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
        p.print(std::string(depth, ' ') + "Strike (~):");
        content.printTree(depth + 2, p);
    }
    void collectStats(DocumentStats &stats) const override
    {
        stats.strike++;
        content.collectStats(stats);
    }
};

// ==========================================
// 4. ПАТЪРН МАЧИНГ ЗА ИНЛАЙН ЕЛЕМЕНТИ (HASKELL STYLE В ООП)
// ==========================================
class InlineRule
{
public:
    virtual ~InlineRule() = default;
    virtual std::string getDelimiter() const = 0;
    virtual std::string getError() const = 0;
    virtual InlineNode *createNode(const InlineSequence &inner) const = 0;
    virtual bool isLeaf() const { return false; }
};

class ItalicRule : public InlineRule
{
public:
    std::string getDelimiter() const override { return "**"; }
    std::string getError() const override { return "Незатворен маркер за италик (**)"; }
    InlineNode *createNode(const InlineSequence &inner) const override { return new ItalicNode(inner); }
};

class BoldRule : public InlineRule
{
public:
    std::string getDelimiter() const override { return "*"; }
    std::string getError() const override { return "Незатворен маркер за болд (*)"; }
    InlineNode *createNode(const InlineSequence &inner) const override { return new BoldNode(inner); }
};

class CodeRule : public InlineRule
{
public:
    std::string getDelimiter() const override { return "`"; }
    std::string getError() const override { return "Незатворен маркер за код (`)"; }
    InlineNode *createNode(const InlineSequence &inner) const override { return new CodeNode(inner); }
    bool isLeaf() const override { return true; }
};

class StrikeRule : public InlineRule
{
public:
    std::string getDelimiter() const override { return "~"; }
    std::string getError() const override { return "Незатворен маркер за зачеркнат текст (~)"; }
    InlineNode *createNode(const InlineSequence &inner) const override { return new StrikeNode(inner); }
};

class InlineParser
{
private:
    std::vector<InlineRule *> rules;

    InlineParser()
    {
        rules.push_back(new ItalicRule());
        rules.push_back(new BoldRule());
        rules.push_back(new CodeRule());
        rules.push_back(new StrikeRule());
    }

    ~InlineParser()
    {
        for (auto *r : rules)
            delete r;
    }

public:
    static InlineSequence parse(const std::string &text, int line_num, std::vector<ParseError> &errors)
    {
        static InlineParser instance; // Избягваме повторно заделяне на памет за правилата

        InlineSequence seq;
        size_t i = 0;
        size_t n = text.size();
        std::string current_text = "";

        while (i < n)
        {
            InlineRule *matchedRule = nullptr;
            for (auto *rule : instance.rules)
            {
                std::string delim = rule->getDelimiter();
                if (i + delim.size() <= n && text.compare(i, delim.size(), delim) == 0)
                {
                    matchedRule = rule;
                    break;
                }
            }

            if (matchedRule)
            {
                if (!current_text.empty())
                {
                    seq.add(new TextNode(current_text));
                    current_text = "";
                }

                std::string delim = matchedRule->getDelimiter();
                size_t close = text.find(delim, i + delim.size());

                if (close != std::string::npos)
                {
                    std::string inner = text.substr(i + delim.size(), close - (i + delim.size()));
                    if (matchedRule->isLeaf())
                    {
                        InlineSequence leafSeq;
                        leafSeq.add(new TextNode(inner));
                        seq.add(matchedRule->createNode(leafSeq));
                    }
                    else
                    {
                        seq.add(matchedRule->createNode(parse(inner, line_num, errors)));
                    }
                    i = close + delim.size();
                }
                else
                {
                    errors.push_back({line_num, matchedRule->getError()});
                    current_text += delim;
                    i += delim.size();
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

// ==========================================
// 5. ПОЛИМОРФНА ЙЕРАРХИЯ ЗА БЛОКОВИ ЕЛЕМЕНТИ
// ==========================================
class Block
{
public:
    virtual ~Block() = default;
    virtual Block *clone() const = 0;
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
        return "<h" + std::to_string(level) + ">" + content.toHTML() + "</h" + std::to_string(level) + ">\n";
    }

    void printTree(int depth, TreePrinter &p) const override
    {
        p.print(std::string(depth, ' ') + "Heading (H" + std::to_string(level) + "):");
        content.printTree(depth + 2, p);
    }

    void collectStats(DocumentStats &stats) const override
    {
        // КРАЙ НА IF-ELSE: Директно добавяме стойността в масива според нивото
        stats.headings[level]++;
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
    void printTree(int depth, TreePrinter &p) const override { p.print(std::string(depth, ' ') + "Horizontal Rule (HR)"); }
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
        p.print(std::string(depth, ' ') + (ordered ? "Ordered List (<ol>):" : "Unordered List (<ul>):"));
        for (const auto &item : items)
        {
            p.print(std::string(depth + 2, ' ') + "List Item (<li>):");
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

// ==========================================
// 6. ПАТЪРН МАЧИНГ ЗА БЛОКОВЕ (CHAIN OF RESPONSIBILITY)
// ==========================================
class BlockParserRule
{
public:
    virtual ~BlockParserRule() = default;
    virtual bool tryParse(const std::string &line, int line_num, std::vector<Block *> &blocks, std::vector<ParseError> &errors, ListBlock *&current_list) = 0;
};

class HRRule : public BlockParserRule
{
public:
    bool tryParse(const std::string &line, int line_num, std::vector<Block *> &blocks, std::vector<ParseError> &, ListBlock *&current_list) override
    {
        if (line != "---")
            return false;
        if (current_list)
        {
            blocks.push_back(current_list);
            current_list = nullptr;
        }
        blocks.push_back(new HRBlock());
        return true;
    }
};

class HeadingRule : public BlockParserRule
{
public:
    bool tryParse(const std::string &line, int line_num, std::vector<Block *> &blocks, std::vector<ParseError> &errors, ListBlock *&current_list) override
    {
        if (line.empty() || line[0] != '#')
            return false;

        size_t h_count = 0;
        while (h_count < line.size() && line[h_count] == '#')
            h_count++;

        if (h_count > 6)
        {
            errors.push_back({line_num, "Невалидно ниво на заглавие (повече от 6 нива на #)"});
            return false;
        }

        if (h_count < line.size() && line[h_count] == ' ')
        {
            if (current_list)
            {
                blocks.push_back(current_list);
                current_list = nullptr;
            }
            std::string content_str = trim(line.substr(h_count));
            InlineSequence seq = InlineParser::parse(content_str, line_num, errors);
            blocks.push_back(new HeadingBlock(static_cast<int>(h_count), seq));
            return true;
        }
        return false;
    }
};

class ListItemRule : public BlockParserRule
{
public:
    bool tryParse(const std::string &line, int line_num, std::vector<Block *> &blocks, std::vector<ParseError> &errors, ListBlock *&current_list) override
    {
        bool is_unordered = (line.size() >= 2 && (line[0] == '-' || line[0] == '*') && line[1] == ' ');
        bool is_ordered = false;
        size_t dot_pos = 0;

        if (!line.empty() && isDigit(line[0]))
        {
            while (dot_pos < line.size() && isDigit(line[dot_pos]))
                dot_pos++;
            if (dot_pos < line.size() && line[dot_pos] == '.' && dot_pos + 1 < line.size() && line[dot_pos + 1] == ' ')
                is_ordered = true;
        }

        if (!is_unordered && !is_ordered)
            return false;

        size_t skip = is_unordered ? 2 : (dot_pos + 2);
        std::string content_str = trim(line.substr(skip));
        InlineSequence seq = InlineParser::parse(content_str, line_num, errors);

        if (current_list && current_list->isOrdered() != is_ordered)
        {
            blocks.push_back(current_list);
            current_list = nullptr;
        }
        if (!current_list)
        {
            current_list = new ListBlock(is_ordered);
        }
        current_list->addItem(seq);
        return true;
    }
};

// ==========================================
// 7. КЛАС ДОКУМЕНТ (МАНИПУЛАЦИЯ И ПАРСЕНЕ)
// ==========================================
class MarkdownDocument
{
private:
    std::vector<Block *> blocks;
    std::vector<ParseError> errors;
    int total_lines_read = 0;
    std::vector<BlockParserRule *> blockRules;

    void initializeRules()
    {
        blockRules.push_back(new HRRule());
        blockRules.push_back(new HeadingRule());
        blockRules.push_back(new ListItemRule());
    }

    void clearRules()
    {
        for (auto *r : blockRules)
            delete r;
        blockRules.clear();
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
    MarkdownDocument() { initializeRules(); }
    ~MarkdownDocument()
    {
        clear();
        clearRules();
    }

    MarkdownDocument(const MarkdownDocument &other)
    {
        initializeRules();
        copyFrom(other);
    }

    MarkdownDocument &operator=(MarkdownDocument other)
    {
        std::swap(blocks, other.blocks);
        std::swap(errors, other.errors);
        std::swap(total_lines_read, other.total_lines_read);
        return *this;
    }

    // КРАЙ НА IF-ELSE ВЕРИГИТЕ: Използваме чист полиморфизъм за редовете
    bool parseFromFile(const std::string &filename)
    {
        std::ifstream in(filename);
        if (!in.is_open())
            return false;

        clear();
        std::string line;
        ListBlock *current_list = nullptr;

        while (std::getline(in, line))
        {
            total_lines_read++;
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

            std::string lead_trimmed = trim(line);
            bool matched = false;

            // Изпълнение на Pattern Matching веригата от правила
            for (auto *rule : blockRules)
            {
                if (rule->tryParse(lead_trimmed, total_lines_read, blocks, errors, current_list))
                {
                    matched = true;
                    break;
                }
            }

            // Ако нито едно правило не е мачнало, по подразбиране е свободен параграф
            if (!matched)
            {
                if (current_list)
                {
                    blocks.push_back(current_list);
                    current_list = nullptr;
                }
                InlineSequence seq = InlineParser::parse(lead_trimmed, total_lines_read, errors);
                blocks.push_back(new ParagraphBlock(seq));
            }
        }
        if (current_list)
        {
            blocks.push_back(current_list);
            current_list = nullptr;
        }
        return true;
    }

    void printErrors() const
    {
        if (errors.empty())
        {
            std::cout << "Документът е напълно валиден съгласно спецификацията!\n";
            return;
        }
        std::cout << "Списък на грешките по редове:\n";
        for (const auto &err : errors)
        {
            std::cout << "  Ред " << err.line_number << ": " << err.message << "\n";
        }
    }

    void printStats() const
    {
        DocumentStats stats;
        stats.total_lines = total_lines_read;
        for (auto *b : blocks)
            if (b)
                b->collectStats(stats);

        std::cout << "Статистика на файла:\n";
        std::cout << "  Брой редове: " << stats.total_lines << "\n";
        std::cout << "  Разпределение по тип блокове:\n";
        std::cout << "    h1: " << stats.headings[1] << ", h2: " << stats.headings[2] << ", h3: " << stats.headings[3]
                  << ", h4: " << stats.headings[4] << ", h5: " << stats.headings[5] << ", h6: " << stats.headings[6] << "\n";
        std::cout << "    p: " << stats.paragraphs << ", hr: " << stats.hr << ", li: " << stats.list_items << "\n";
        std::cout << "  Брой форматиращи елементи:\n";
        std::cout << "    <i>: " << stats.italic << ", <b>: " << stats.bold
                  << ", <code>: " << stats.code << ", <s>: " << stats.strike << "\n";
    }

    void printTreeStructure() const
    {
        TreePrinter p;
        p.print("--- СИНТАКТИЧНО ДЪРВО ---");
        for (auto *b : blocks)
            if (b)
                b->printTree(0, p);
    }

    void translateToHTML(const std::string &outfile) const
    {
        std::ifstream check(outfile);
        if (check.good())
        {
            check.close();
            std::cout << "Внимание: Файлът '" << outfile << "' вече съществува. Презаписване? (y/n): ";
            char ans;
            std::cin >> ans;
            std::cin.ignore(10000, '\n');
            if (ans != 'y' && ans != 'Y')
            {
                std::cout << "Преводът е прекратен от потребителя.\n";
                return;
            }
        }

        std::ofstream out(outfile);
        if (!out.is_open())
        {
            std::cout << "Грешка при запис на изходния файл.\n";
            return;
        }

        out << "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n";
        out << "<title>Преведен Документ</title>\n</head>\n<body>\n";
        for (auto *b : blocks)
            if (b)
                out << b->toHTML();
        out << "</body>\n</html>\n";
        std::cout << "Успешно генериран HTML: " << outfile << "\n";
    }
};

// ==========================================
// 8. ШАБЛОН "КОМАНДА" (COMMAND DESIGN PATTERN)
// ==========================================
class Command
{
public:
    virtual ~Command() = default;
    virtual void execute(MarkdownDocument &doc, const std::vector<std::string> &args) = 0;
    virtual std::string getName() const = 0;
};

class DocumentCommand : public Command
{
protected:
    virtual void executeWithFile(MarkdownDocument &doc, const std::vector<std::string> &args) = 0;
    virtual size_t requiredArgs() const { return 2; }
    virtual std::string getUsage() const = 0;

public:
    void execute(MarkdownDocument &doc, const std::vector<std::string> &args) override
    {
        if (args.size() < requiredArgs())
        {
            std::cout << "Грешка: Липсват аргументи. Използвайте: " << getUsage() << "\n";
            return;
        }
        if (!doc.parseFromFile(args[1]))
        {
            std::cout << "Грешка при зареждане на файла: " << args[1] << "\n";
            return;
        }
        executeWithFile(doc, args);
    }
};

class PrintCommand : public DocumentCommand
{
public:
    std::string getName() const override { return "print"; }

protected:
    std::string getUsage() const override { return "print <input>"; }
    void executeWithFile(MarkdownDocument &doc, const std::vector<std::string> &) override { doc.printTreeStructure(); }
};

class ValidateCommand : public DocumentCommand
{
public:
    std::string getName() const override { return "validate"; }

protected:
    std::string getUsage() const override { return "validate <input>"; }
    void executeWithFile(MarkdownDocument &doc, const std::vector<std::string> &) override { doc.printErrors(); }
};

class InfoCommand : public DocumentCommand
{
public:
    std::string getName() const override { return "info"; }

protected:
    std::string getUsage() const override { return "info <input>"; }
    void executeWithFile(MarkdownDocument &doc, const std::vector<std::string> &) override { doc.printStats(); }
};

class TranslateCommand : public DocumentCommand
{
public:
    std::string getName() const override { return "translate"; }

protected:
    size_t requiredArgs() const override { return 3; }
    std::string getUsage() const override { return "translate <input> <output>"; }
    void executeWithFile(MarkdownDocument &doc, const std::vector<std::string> &args) override { doc.translateToHTML(args[2]); }
};

class ExitCommand : public Command
{
private:
    bool &m_running;

public:
    ExitCommand(bool &running) : m_running(running) {}
    std::string getName() const override { return "exit"; }
    void execute(MarkdownDocument &, const std::vector<std::string> &) override { m_running = false; }
};

// ==========================================
// 9. REPL ИНТЕРФЕЙСЕН КЛАС БЕЗ IF-ELSE СТРУКТУРИ
// ==========================================
class CommandLine
{
private:
    std::vector<Command *> registry;

    void initialize(bool &running)
    {
        registry.push_back(new PrintCommand());
        registry.push_back(new ValidateCommand());
        registry.push_back(new InfoCommand());
        registry.push_back(new TranslateCommand());
        registry.push_back(new ExitCommand(running));
    }

    void clean()
    {
        for (auto *cmd : registry)
            delete cmd;
        registry.clear();
    }

public:
    void run()
    {
        MarkdownDocument doc;
        bool running = true;
        initialize(running);

        std::cout << "=== Markdown -> HTML Редактор (ФМИ ООП Проект) ===\n";
        std::cout << "Налични команди: print, validate, info, translate, exit\n";

        std::string inputLine;
        while (running && std::cout << "> " && std::getline(std::cin, inputLine))
        {
            std::string cleaned = trim(inputLine);
            if (cleaned.empty())
                continue;

            std::vector<std::string> args = splitSpaces(cleaned);
            if (args.empty())
                continue;

            bool found = false;
            for (auto *cmd : registry)
            {
                if (cmd->getName() == args[0])
                {
                    cmd->execute(doc, args);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                std::cout << "Невалидна команда. Опитайте пак.\n";
            }
        }
        clean();
    }
};

int main()
{
    CommandLine cli;
    cli.run();
    return 0;
}