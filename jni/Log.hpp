#ifndef _LOG_HPP_
#define _LOG_HPP_

	/*
	 * 	Log - класс, реализующий вывод логов в LogCat
	 */

    class Log {
    public:
        static void error(const char* pMessage, ...);
        static void warn(const char* pMessage, ...);
        static void info(const char* pMessage, ...);
        static void debug(const char* pMessage, ...);
    };

#ifndef NDEBUG
    #define Log_debug(...) Log::debug(__VA_ARGS__)
#else
    #define Log_debug(...)
#endif

#endif

    /*
      Спецификаторы формата printf:
	%c   - Символ
	%s   - Строка символов
	%d,i - Целое десятичное со знаком
	%o   - Целое восьмеричное
	%u   - Целое десятичное без знака
	%x,X - Целое шестнадцатеричное
	%f   - Дробное число в фиксированном формате
	%e,E - Дробное число в научном формате
	%g,G - Дробное число в научном или фиксированном формате
	%p   - Указатель (в шестнадцатеричном виде)
     */
