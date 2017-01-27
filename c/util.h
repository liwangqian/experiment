//util.h

#include <utility>
#include <cstddef>


namespace snipts {


class tokenizer
{
public:
	typedef std::pair<const char*, const char*> word;
	static const word invalid_word;

public:
	~tokenizer();
	tokenizer(const char* content = NULL, const bool copy = true);

	void reset(const char* content, const bool copy = true);
	word next(const char delim = ' ');

	template<typename Delimiter>
	word next(const Delimiter& delim);

private:
	const char* _content;
	const char* _pc;
	std::size_t _size;
		  char* _inner_buffer;
};

class delimiter
{
public:
	~delimiter();
	delimiter(const char* content = NULL, const bool copy = true);

	void reset(const char* content, const bool copy = true);
	bool operator()(const char c) const;

private:
	const char* _content;
	std::size_t _size;
		  char* _inner_buffer;
};





/**
 *  implement for template functions
 *
 **/

template<typename Delimiter>
tokenizer::word tokenizer::next(const Delimiter& delim)
{
	const char* end = _content + _size;
	word aword = invalid_word;

	if ( _pc == end )
	{
		return aword;
	}

	aword.first = _pc;

	while( _pc != end )
	{
		if ( delim(*_pc) )
		{
			break;
		}

		++_pc;
	}

	aword.second = _pc;
	while ( _pc != end )
	{
		if ( !delim(*_pc) )
        {
            break;
        }

        ++_pc; //skip the delimiter.
	}

	return aword;
}


} //namespace snipts


