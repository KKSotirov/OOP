#include <iostream>
#include <cstring>
#include <stdexcept>
class Subject
{
private:
	char *subj;
	char *anot;
	int credits = 0;

	void free(Subject &subject)
	{
		delete[] subject.subj;
		subject.subj = nullptr;
		delete[] subject.anot;
		subject.anot = nullptr;
		subject.credits = 0;
	}

	void initialize(const Subject &other)
	{
		if (isNullPtr(other.subj) == false)
		{
			this->subj = new char[strlen(other.subj) + 1];
			strcpy(this->subj, other.subj);
		}
		else
			this->subj = nullptr;

		if (isNullPtr(other.anot) == false)
		{
			this->anot = new char[strlen(other.anot) + 1];
			strcpy(this->anot, other.anot);
		}
		else
			this->anot = nullptr;

		this->credits = other.credits;
	}

	bool isSmaller(const Subject &other) const
	{
		if (this->credits == other.credits)
			return strlen(this->anot) < strlen(other.anot) ? true : false;
		return this->credits < other.credits ? true : false;
	}

	bool isNullPtr(const char *arr) const
	{
		return (arr == nullptr);
	}

public:
	// default constructor
	Subject() : subj(nullptr), anot(nullptr), credits(0) {}

	// copy constructor
	Subject(const Subject &other)
	{
		initialize(other);
	}

	Subject &operator=(const Subject &other)
	{
		if (this != &other)
		{
			free(*this);
			initialize(other);
		}
		return *this;
	}

	bool operator==(const Subject &other) const
	{
		if (isNullPtr(this->subj) && isNullPtr(other.subj) &&
			isNullPtr(this->anot) && isNullPtr(other.anot))
			return true;

		if (isNullPtr(this->subj) || isNullPtr(other.subj) ||
			isNullPtr(this->anot) || isNullPtr(other.anot))
			return false;

		return (strcmp(this->subj, other.subj) == 0 && strcmp(this->anot, other.anot) == 0 && this->credits == other.credits);
	}

	bool operator<(const Subject &other) const
	{
		return isSmaller(other);
	}

	bool operator>(const Subject &other) const
	{
		return other < *this;
	}

	~Subject()
	{
		free(*this);
	}

	const char *getSubj() const
	{
		return this->subj;
	}

	int getCredits() const
	{
		return this->credits;
	}

	void printSubject()
	{
		if (this->subj == nullptr)
			return;

		const char *ptr = this->subj;
		while (*ptr != '\0')
		{
			std::cout << *ptr;
			ptr++;
		}
	}
};

class Plan
{
private:
	Subject *subjects;
	int *exams;
	const int maxCredits;

	size_t getSize(const Plan &plan) const
	{
		if (plan.subjects == nullptr)
			return 0;
		size_t size = 0;
		const Subject *currentSubject = plan.subjects; // valid, we have operator= for subject objects
		while (currentSubject->getCredits() != 0)
		{
			size++;
			currentSubject++;
		}
		return size;
	}

	void free(Plan &plan)
	{
		delete[] plan.subjects;
		delete[] plan.exams;
		plan.subjects = nullptr;
		plan.exams = nullptr;
	}

public:
	// default constructor
	Plan() : subjects(nullptr), exams(nullptr), maxCredits(73) {}

	// operator= -> delete
	Plan &operator=(const Plan &other) = delete;
	Plan(const Plan &otherPlan) = delete;

	// operator += -> adds a &subject
	void operator+=(const Subject &other)
	{
		size_t size = getSize(*this);
		bool found = false;
		for (size_t i = 0; i < size; i++)
			if (this->subjects[i] == other)
				found = true;
		if (found == false)
		{
			Subject *tmpSubj = new Subject[size + 2];
			int *tmpExams = new int[size + 1];
			for (size_t i = 0; i < size; i++)
			{
				tmpSubj[i] = this->subjects[i];
				tmpExams[i] = this->exams[i];
			}

			tmpSubj[size] = other;
			tmpExams[size] = 2;			   // grades range from 2 to 6, so logically default will be 2
			tmpSubj[size + 1] = Subject(); // using the default constructor which initializes with nullptr values and 0 credits

			delete[] this->subjects;
			delete[] this->exams;

			this->subjects = tmpSubj; // pointer swap
			this->exams = tmpExams;
		}
	}

	void operator-=(const Subject &other)
	{
		size_t oldSize = getSize(*this);
		bool isFound = false;
		size_t indexKey = 0;
		// обхождаме масив от предмети, търсим this->subjects[i] == other, markirame tova i, copirame vsichko do pos i v nov masiv [size - 1], i vsichko sled pos i
		for (size_t i = 0; i < oldSize; i++)
		{
			if (this->subjects[i] == other)
			{
				isFound = true;
				indexKey = i;
				break;
			}
		}
		if (isFound == false)
			throw std::invalid_argument("The subject you want to remove is not part of the Plan.");
		// if isFound == true
		Subject *tmpSubj = new Subject[oldSize];
		int *tmpExams = new int[oldSize - 1];
		for (size_t i = 0; i < indexKey; i++)
		{
			tmpSubj[i] = this->subjects[i];
			tmpExams[i] = this->exams[i];
		}
		// currently at pos indexKey

		for (size_t i = indexKey + 1; i < oldSize; i++)
		{
			tmpSubj[(i - 1)] = this->subjects[i];
			tmpExams[(i - 1)] = this->exams[i];
		}
		tmpSubj[oldSize - 1] = Subject();
		delete[] this->subjects;
		delete[] this->exams;

		this->subjects = tmpSubj;
		this->exams = tmpExams;
	}

	Subject &operator[](int index) const
	{
		return subjects[index];
	}

	void print()
	{
		std::cout << "The study plan is the following: " << std::endl;
		int size = getSize(*this);
		for (size_t i = 0; i < size; i++)
		{
			std::cout << "Subject: ";
			this->subjects[i].printSubject();									// prints "Геометрия"
			std::cout << ", which has these grades: " << exams[i] << std::endl; // "Геометрия, which has these grades: 3"
		}
		std::cout << "The maximum credits of the whole plan are: " << maxCredits << std::endl;
	}

	// destructor
	~Plan()
	{
		free(*this);
	}
};

int main()
{
	Plan p;
	// use setters to enter data for plan
	return 0;
}
