#include "LogLevel.h"

namespace Greet {
	// Low level -> high priority
	LogLevel LogLevel::information(1 << 24);
	LogLevel LogLevel::warning(1 << 16);
	LogLevel LogLevel::error(1 << 8);

}