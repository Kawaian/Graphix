#include <GL/glew.h>

class GBuffer
{
public:
	enum GBUFFER_TEXTURE_TYPE
	{
		TEXTURE_TYPE_POSITION,
		TEXTURE_TYPE_DIFFUSE,
		TEXTURE_TYPE_NORMAL,
		TEXTURE_TYPE_TEXCOORD,
		GBUFFER_TEXTURE_COUNT
	};
	GBuffer(unsigned width, unsigned height);
	~GBuffer();

	void BindForOutput();
	void BindForInput();
	void SetReadBuffer(GBUFFER_TEXTURE_TYPE texType);

	GBuffer& operator=(const GBuffer& o) = delete;
	GBuffer(const GBuffer& o) = delete;
private:
	void Init(unsigned width, unsigned height);
};
