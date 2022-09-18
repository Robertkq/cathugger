#ifndef KQIMAGERANDOMIZER
#define KQIMAGERANDOMIZER

#include <vector>
#include <string>
#include <ctime>

struct ImageRandomizer
{
public:

    static ImageRandomizer* getInstance(size_t nCats = 0, size_t nDogs = 0)
    {
        if (!m_instance)
            m_instance = new ImageRandomizer(nCats, nDogs);
        return m_instance;
    }

    std::string getCat() { return getRandomPhoto(m_Cats, m_nCats); }

    std::string getDog() { return getRandomPhoto(m_Dogs, m_nDogs); }

    std::string getRandomPhoto(std::vector<int>& positions, size_t size)
    {
        if (positions.empty())
        {
            for (auto i = 1; i <= size; ++i)
            {
                positions.push_back(i);
            }
        }
        size_t index = rand() % positions.size();
        std::string filename = std::to_string(positions[index]) + ".jpg";
        positions.erase(positions.begin() + index);

        return filename;
    }

    

private:

    ImageRandomizer(size_t nCats, size_t nDogs)
        : m_nCats(nCats), m_nDogs(nDogs)
    {
        srand(static_cast<int>(time(NULL)));
        for (auto i = 1; i <= nCats; ++i)
            m_Cats.push_back(i);
        for (auto i = 1; i <= nDogs; ++i)
            m_Dogs.push_back(i);
    }

    static ImageRandomizer* m_instance;

    std::vector<int> m_Cats;
    std::vector<int> m_Dogs;

    size_t m_nCats;
    size_t m_nDogs;

};

ImageRandomizer* ImageRandomizer::m_instance = nullptr;



#endif