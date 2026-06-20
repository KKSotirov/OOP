#include <iostream>
#include <utility>
#include <cstring>

class MediaResource
{
private:
    static int counter;
    const int id;
    char *title;

    void free()
    {
        delete[] this->title;
    }

    void copyObj(const MediaResource &other)
    {
        if (other.title == nullptr)
        {
            this->title = nullptr;
            return;
        }
        this->title = new char[strlen(other.title) + 1];
        strcpy(this->title, other.title);
    }

    void copyPar(const char *str)
    {
        if (str == nullptr)
        {
            this->title = nullptr;
        }
        else
        {
            this->title = new char[strlen(str) + 1];
            strcpy(this->title, str);
        }
    }

protected:
    MediaResource(const char *oldTitle, const int oldID) : id(oldID)
    {
        copyPar(oldTitle);
    }

    void swap(MediaResource &other) noexcept
    {
        std::swap(this->title, other.title);
    }

public:
    // parameterized constructor
    MediaResource(const char *name) : id(++counter)
    {
        copyPar(name);
    }
    // copy constructor
    MediaResource(const MediaResource &other) : id(++counter)
    {
        copyObj(other);
    }
    // operator =
    MediaResource &operator=(MediaResource other)
    {
        this->swap(other);
        return *this;

        // second variant :     MediaResource &operator=(const MediaResource& other)
        //  if(this!=&other) {
        //      free();
        //      copy(other);
        //      return *this;
        // }
    }
    // virtual destructor -> deletes dynamic array
    virtual ~MediaResource()
    {
        free();
    }

    // virtual clone -> deep copies of objects
    virtual MediaResource *clone() const = 0;
    // no idea what this does for now
    virtual void play(std::ostream &os) const = 0;
    // Метод, връщащ типа ("audio" или "video").
    virtual const char *type() const = 0;

    const int getID() const
    {
        return this->id;
    }
    void printTitle(std::ostream &os) const
    {
        if (this->title == nullptr)
            throw std::invalid_argument("Can't print an empty title!");

        os << this->title; // Само това! Нищо друго не ни трябва.
    }
};

int MediaResource::counter = 0;

class AudioResource : public MediaResource
{
private:
    int sample_rate;

public:
    // paramaterized constructor
    AudioResource(const char *name, const int sample) : MediaResource(name), sample_rate(sample) {}
    // protected constructor
    AudioResource(const char *oldTitle, const int oldID, const int sample) : MediaResource(oldTitle, oldID), sample_rate(sample) {}
    // NO DYNAMIC MEMORY --> NO UNIQUE RO3
    AudioResource *clone() const override
    {
        return new AudioResource(*this);
    }

    void play(std::ostream &os) const override
    {
        os << "ID:" << this->getID() << ", Frequency: " << this->sample_rate << ", Title: ";
        printTitle(os);
        os << std::endl;
    }

    const char *type() const override
    {
        return "audio";
    }
};

class VideoResource : public MediaResource
{
private:
    char *codec; // cannot be empty!!!

    bool hasValidCodec(const char *bs)
    {
        return (bs != nullptr);
    }

    void initCodec(const char *bs)
    {
        if (hasValidCodec(bs) == false)
            throw std::invalid_argument("codec cannot be empty!");
        else
        {
            codec = new char[strlen(bs) + 1];
            strcpy(codec, bs);
        }
    }

    void free()
    {
        delete[] codec;
    }

    // class-specific swap <3 Pesho
    void swap(VideoResource &other)
    {
        MediaResource::swap(other);
        std::swap(this->codec, other.codec);
    }

public:
    // parameterized constructor
    VideoResource(const char *name, const char *bs) : MediaResource(name)
    {
        initCodec(bs);
    }
    // protected constructor
    VideoResource(const char *oldTitle, const char *bs, const int oldID) : MediaResource(oldTitle, oldID)
    {
        initCodec(bs);
    }
    // DYNAMIC MEMORY --> RO3
    //  COPY CONSTRUCTOR
    VideoResource(const VideoResource &other) : MediaResource(other)
    {
        initCodec(other.codec);
    }
    // OPERATOR=
    VideoResource &operator=(VideoResource other)
    {
        this->swap(other);
        return *this;
    }

    ~VideoResource() override
    {
        free();
    }
    // overriding virtual functions in order to achieve polymorphism

    VideoResource *clone() const override
    {
        return new VideoResource(*this);
    }

    void play(std::ostream &os) const override
    {
        os << "Title: ";
        printTitle(os);
    }

    const char *type() const override
    {
        return "video";
    }
};

class StreamSession
{
private:
    MediaResource **resources; // array of objects Audio/Video
    const int started_at;
    // additional NEEDED parameters in order to control heterogenic container:
    size_t res_count;
    size_t res_cap;
    friend class MediaServer;
    // helper functions
    void copy(const StreamSession &other)
    {
        this->res_cap = other.res_cap;
        this->res_count = other.res_count;
        if (other.resources == nullptr)
        {
            this->resources = nullptr;
            return;
        }

        this->resources = new MediaResource *[res_cap];
        for (size_t i = 0; i < res_count; i++)
        {
            if (other.resources[i] == nullptr)
                this->resources[i] = nullptr;
            else
                this->resources[i] = other.resources[i]->clone();
        }
    }

    void free()
    {
        for (size_t i = 0; i < res_count; i++)
        {
            delete resources[i];
        }
        delete[] resources;
    }

    // default constructor
    StreamSession() : started_at(0), res_cap(0), res_count(0), resources(nullptr) {}
    // parameterized constructor
    StreamSession(const int started) : started_at(started), res_cap(0), res_count(0), resources(nullptr) {}
    // RO3
    // COPY CONSTR
    StreamSession(const StreamSession &other) : started_at(other.started_at)
    {
        copy(other);
    }
    // OPERATOR=
    StreamSession &operator=(const StreamSession &other) = delete;

    // DESTRUCTOR
    ~StreamSession()
    {
        free();
    }
    // IDEA -> All is private, only friend class MediaServer can work with StreamSession
    // method resize();
    void resize()
    {
        if (res_count == res_cap)
        {
            size_t newcap = res_cap == 0 ? 2 : res_cap * 2;
            MediaResource **resized = new MediaResource *[newcap];
            for (size_t i = 0; i < res_count; i++)
            {
                resized[i] = this->resources[i];
            }
            delete[] resources;
            this->resources = resized;
            this->res_cap = newcap;
        }
    }
    // method addResourse
    void addResource(const MediaResource &added)
    {
        if (this->res_cap > this->res_count)
        {
            this->resources[res_count] = added.clone();
            res_count++;
        }
        else
        {
            resize();
            addResource(added);
        }
    }
};

class MediaServer
{
private:
    StreamSession **sessions;
    unsigned capacity;
    unsigned count;
    unsigned max_duration; // ako se dostigne do max_duration -> ubiva se vruzkata
    unsigned current_time; // ЗАДЪЛЖИТЕЛНО: за да следим времето в сървъра!

    void copy(const MediaServer &other)
    {
        if (other.sessions == nullptr)
        {
            this->sessions = nullptr;
            return;
        }
        else
        {
            this->sessions = new StreamSession *[this->capacity];
            // 2. ЗАДЪЛЖИТЕЛНО зануляваме ВСИЧКО, за да няма боклуци в паметта
            for (size_t i = 0; i < this->capacity; i++)
            {
                this->sessions[i] = nullptr;
            }

            for (size_t i = 0; i < count; i++)
            {
                if (other.sessions[i] != nullptr)
                    this->sessions[i] = new StreamSession(*other.sessions[i]); // OPERATOR = IS DELETED ===> WE USE COPY CONSTRUCTOR!
            }
        }
    }

    void free()
    {
        for (size_t i = 0; i < count; i++)
        {
            delete this->sessions[i];
        }
        delete[] sessions;
    }

    void tighten(const unsigned index)
    {
        StreamSession **tight = new StreamSession *[count - 1];
        for (size_t i = 0; i < index; i++)
        {
            if (this->sessions[i] == nullptr)
                tight[i] = nullptr;
            else
            {
                tight[i] = new StreamSession(*this->sessions[i]);
            }
        }
        for (size_t i = index + 1; i < (count - 1); i++)
        {
            if (this->sessions[i] == nullptr)
                tight[i - 1] = nullptr;
            else
            {
                tight[i - 1] = new StreamSession(*this->sessions[i]);
            }
        }
        free();
        count--;
        this->sessions = new StreamSession *[count];
        this->sessions = tight;
    }

public:
    // Default constructor
    MediaServer() : sessions(nullptr), capacity(0), count(0), max_duration(0) {}
    // Parameterized constructor
    MediaServer(unsigned cap, unsigned max_dur) : sessions(nullptr), capacity(cap), max_duration(max_dur), count(0)
    {
        this->sessions = new StreamSession *[capacity];
        for (size_t i = 0; i < capacity; i++)
        {
            this->sessions[i] = nullptr;
        }
    }
    // DYNAMIC MEMORY -> RO3
    // Copy constructor
    MediaServer(const MediaServer &other) : capacity(other.capacity), count(other.count), max_duration(other.max_duration)
    {
        copy(other);
    }
    MediaServer &operator=(const MediaServer &other) = delete;

    ~MediaServer()
    {
        free();
    }

    // FINAL FUNCTIONS
    bool startStream(const MediaResource &resource)
    {
        // adds a new session
        if (this->capacity <= this->count)
            return false;
        this->sessions[count] = new StreamSession(current_time);
        this->sessions[count]->addResource(resource);
        this->count++;
        return true;
    }

    void stopStream(int resourceID)
    {
        // stops the stream with resourceID
        // tightens the array together
        tighten(resourceID);
    }

    void updateTime(unsigned seconds)
    {
        // GEMINI 100%
        // lengthens the current time
        //  stops inactive sessions
        this->current_time += seconds; // Напредваме времето на сървъра

        for (int i = 0; i < (int)count; i++)
        {
            // Изчисляваме колко време е минало от старта на сесията
            unsigned elapsed = this->current_time - sessions[i]->started_at;

            if (elapsed >= max_duration)
            {
                delete sessions[i]; // Изтриваме изтеклата сесия

                // Преместваме останалите указатели наляво
                for (unsigned k = i; k < count - 1; k++)
                {
                    sessions[k] = sessions[k + 1];
                }
                sessions[count - 1] = nullptr;
                count--;

                i--; // ВАЖНО: Връщаме индекса назад, защото следващият елемент дойде на тази позиция!
            }
        }
    }

    void saveMediaLog(const char *fileName)
    {
    }

    void loadMediaLog(const char *fileName)
    {
    }
};