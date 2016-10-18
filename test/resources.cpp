/**
 * @author J. Santos <jamillo@gmail.com>
 * @date October 13, 2016
 */

#include "resources.h"

boost::filesystem::path Resources::resources()
{
	return boost::filesystem::path(TEST_RESOURCE_DIR);
}
