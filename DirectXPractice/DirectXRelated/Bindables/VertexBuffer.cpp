#include "VertexBuffer.h"

void UniChili::VertexBuffer::bind(Graphics& graphics) noexcept
{
	const UINT offset = 0u;

	getContext(graphics)->IASetVertexBuffers(
		0u, // start slot
		1u, // buffer count
		pVertexBuffer.GetAddressOf(),
		&stride,
		&offset);
}
