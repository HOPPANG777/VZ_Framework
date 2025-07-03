#pragma once

enum  EFileStream : UINT
{
    FILE_STREAM_READ = 1U << 0,
    FILE_STREAM_WRITE = 1U << 1
};

class Tile;
class FileStream
{
public:
    FileStream(std::string path, UINT flags);
    ~FileStream();

    template<typename T, typename = typename std::enable_if <
        std::is_same<T, bool>::value ||
        std::is_same<T, char>::value ||
        std::is_same<T, int >::value ||
        std::is_same<T, float>::value ||
        std::is_same<T, UINT>::value ||
        std::is_same<T, Vector2>::value ||
        std::is_same<T, Vector3>::value ||
        std::is_same<T, Matrix>::value ||
        std::is_same<T, Color>::value ||
        std::is_same<T, Tile>::value
    >::type>
    void Write(T value)
    {
        out.write(reinterpret_cast<const char*>(&value),
            sizeof(T));
    }

    template<typename T, typename = typename std::enable_if <
        std::is_same<T, bool>::value ||
        std::is_same<T, char>::value ||
        std::is_same<T, int >::value ||
        std::is_same<T, float>::value ||
        std::is_same<T, UINT>::value ||
        std::is_same<T, Vector2>::value ||
        std::is_same<T, Vector3>::value ||
        std::is_same<T, Matrix>::value ||
        std::is_same<T, Color>::value ||
        std::is_same<T, Tile>::value
    >::type>
    T Read()
    {
        T value;

        in.read(reinterpret_cast<char*>(&value),
            sizeof(T));

        return value;
    }

private:
    std::ofstream out;
    std::ifstream in;

    UINT flag;

};
