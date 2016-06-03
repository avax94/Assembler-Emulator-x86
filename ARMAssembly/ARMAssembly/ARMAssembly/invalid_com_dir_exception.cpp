#include "invalid_com_dir_exception.h"


invalid_com_dir_exception::invalid_com_dir_exception(char* c) : invalid_syntax_exception(c)
{
}


invalid_com_dir_exception::~invalid_com_dir_exception()
{
}
