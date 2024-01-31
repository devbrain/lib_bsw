#if defined(_MSC_VER)
#   include <Windows.h>
#else
#   include <unistd.h>

#endif

#include <iostream>

#include <sstream>
#include <utility>

#include "bsw/logger/console.hh"
#include "bsw/logger/local_device.hh"
#include "bsw/logger/standard_layout.hh"
#include "bsw/logger/console_printer.hh"
#include "bsw/logger/formatter.hh"
#include "bsw/logger/record.hh"
#include "bsw/logger/log_record_layout.hh"

#include "bsw/warn/push.hh"
#include "bsw/warn/unused_macros"

#if defined(_MSC_VER)
#define GREEN console_output_c::green
#define RED console_output_c::red
#define NORMAL console_output_c::original
#define BLUE  console_output_c::blue
#define YELLOW  console_output_c::yellow
#define MAGENTA console_output_c::magenta
#else
#define RED   "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW   "\x1B[33m"
#define BLUE   "\x1B[34m"
#define MAGENTA   "\x1B[35m"
#define NORMAL "\x1B[0m"
#endif

#include "bsw/warn/pop.hh"

#if defined(_MSC_VER)
class console_output_c
{
public:
	enum concol
	{
		black = 0,
		dark_blue = 1,
		dark_green = 2,
		dark_aqua, dark_cyan = 3,
		dark_red = 4,
		dark_purple = 5, dark_pink = 5, dark_magenta = 5,
		dark_yellow = 6,
		dark_white = 7,
		gray = 8,
		blue = 9,
		green = 10,
		aqua = 11, cyan = 11,
		red = 12,
		purple = 13, pink = 13, magenta = 13,
		yellow = 14,
		white = 15,
		original = 16
	};

public:
	console_output_c();
	~console_output_c();

	int textcolor() const
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(m_console, &csbi);
		int a = csbi.wAttributes;
		return a % 16;
	}

	int backcolor() const
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(m_console, &csbi);
		int a = csbi.wAttributes;
		return (a / 16) % 16;
	}

	void setcolor(int textcol, int backcol)
	{
		if (m_protect)
		{
			if ((textcol % 16) == (backcol % 16))
			{
				textcol++;
			}
		}
		textcol %= 16; backcol %= 16;
		auto wAttributes = ((WORD)backcol << 4) | (WORD)textcol;
		SetConsoleTextAttribute(m_console, (WORD)wAttributes);
		return;
	}

	void set_original()
	{
		SetConsoleTextAttribute(m_console, m_attr);
	}

private:
	mutable HANDLE m_console;
	WORD    m_attr;
	bool    m_protect;
};

console_output_c::console_output_c()
{
	m_protect = true;
	m_console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(m_console, &info);
	m_attr = info.wAttributes;
}

console_output_c::~console_output_c()
{
	SetConsoleTextAttribute(m_console, m_attr);
}
#endif

namespace bsw::logger {
	class console : public local_device {
	 public:
		console (formatter_ptr_t formatter, predicate_ptr_t predicate);
	 private:
		bool _open_device () override;
		bool _do_print_string (const std::string& data) override;
		void _close_device () override;
		bool _do_print (record_ptr_t record) override;
#if defined(_MSC_VER)
		console_output_c s_console;
#endif
	};

	console::console (formatter_ptr_t formatter, predicate_ptr_t predicate)
		: local_device (std::move(formatter), std::move(predicate)) {
	}

	// -----------------------------------------------------------------
	bool console::_do_print (record_ptr_t record) {
		std::ostringstream ostream;
		_formatter ()->print (record, ostream);

		if (record) {
#if defined(_MSC_VER)
			console_output_c::concol c = console_output_c::original;
#else
			const char* c = nullptr;
#endif
			switch (record->level ()) {
				case eDEBUG:
				case eINFO:break;
				case eNOTICE:c = GREEN;
					break;
				case eWARNING:c = YELLOW;
					break;
				case eERROR:c = MAGENTA;
					break;
				case eFATAL:c = RED;
					break;
			}
#if defined(_MSC_VER)
			if (c != console_output_c::original)
			{
				s_console.setcolor(c, s_console.backcolor());
			}
			else
			{
				s_console.set_original();
			}
#else
			if (c) {
				_do_print_string (c);
			} else {
				_do_print_string (NORMAL);
			}
#endif
		}
		_do_print_string (ostream.str ());
		if (record) {
#if defined(_MSC_VER)
			s_console.set_original();
#else
			_do_print_string (NORMAL);
#endif
		}
		return true;
	}

	// -----------------------------------------------------------------
	bool console::_open_device () {
#if defined(_MSC_VER)
		return (GetConsoleWindow() != NULL);
#else
		return isatty (1) != 0;
#endif
	}

	// -----------------------------------------------------------------
	bool console::_do_print_string (const std::string& data) {

		if (!data.empty ()) {
			std::cout << data;
			auto ch = data[0];
			if (ch != 27) // do not line feed after control sequence
			{
				std::cout << std::endl;
			}
		}
		std::cout.flush ();

		return true;
	}

	// -----------------------------------------------------------------
	void console::_close_device () {
	}

	// -----------------------------------------------------------------
	device_ptr_t console_device () {
		return console_device (nullptr);
	}

	// ------------------------------------------------------------------
	device_ptr_t console_device (predicate_ptr_t predicate) {
		log_record_layout_ptr_t layout (standard_layout ());

		printer_ptr_t printer (new console_printer);

		formatter_ptr_t fmatter (new formatter);

		fmatter->init (layout, printer);

		return std::make_shared<console> (fmatter, std::move(predicate));
	}
} // ns logger

