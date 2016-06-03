#include "Directives.h"
#include "typies.h"

Directives::Directives()
{
	active_command = true;
}

Directives::~Directives()
{
	active_command = false;
}
