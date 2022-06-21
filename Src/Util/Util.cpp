#include "Util.hpp"

std::array<glm::vec2, 4> FUtil::ScreenQuad() noexcept
{
	std::array<glm::vec2, 4> Positions = {
		glm::vec2(-1.0,  1.0),
		glm::vec2( 1.0,  1.0),
		glm::vec2(-1.0, -1.0),
		glm::vec2( 1.0, -1.0),
	};
	return Positions;
}
